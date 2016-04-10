#ifndef SINGLETON_INC_
#define SINGLETON_INC_

#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <cstdlib>
#include <new>

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif
#include "Threads.h"

/** \example Singleton_test.cpp
 *  \brief Singleton类和简单版本的Singleton类用法演示程序
 */

/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
/** \file Singleton.h 
 *  \brief 定义了通过模板支持多种创建策略,生命期控制策略和线程的单体类
 */
/** \}*/
 

BEGIN_SUNRISE_NAMESPACE 

    namespace Private
    {
/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
/** \class LifetimeTracker Singleton.h
 * \brief 生命期跟踪器
 * 函数模板SetLongevity的协助类
 */                 
/** \}*/
        class LifetimeTracker
        {
        public:
            LifetimeTracker(unsigned int x) : longevity_(x) 
            {}
            
            virtual ~LifetimeTracker() = 0;
            
            static bool Compare(const LifetimeTracker* lhs,
                const LifetimeTracker* rhs)
            {
                return lhs->longevity_ > rhs->longevity_;
            }
            
        private:
            unsigned int longevity_;
        };
        
        // Definition required
        inline LifetimeTracker::~LifetimeTracker() {} 
        
        // Helper data
        typedef LifetimeTracker** TrackerArray;
        extern TrackerArray pTrackerArray;
        extern unsigned int elements;

        /** 单件摧毁函数
			*/
        template <typename T>
        struct Deleter
        {
            static void Delete(T* pObj)
            { delete pObj; }
        };
/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
        /** \class ConcreteLifetimeTracker
			* 具体生命期跟踪器
			*/
/** \}*/
        template <typename T, typename Destroyer>
        class ConcreteLifetimeTracker : public LifetimeTracker
        {
        public:
            ConcreteLifetimeTracker(T* p,unsigned int longevity, Destroyer d)
                : LifetimeTracker(longevity)
                , pTracked_(p)
                , destroyer_(d)
            {}
            
            ~ConcreteLifetimeTracker()
            { destroyer_(pTracked_); }
            
        private:
            T* pTracked_;
            Destroyer destroyer_;
        };

        void AtExitFn(); // declaration needed below
    
    } // namespace Private

	/** \brief 模板函数SetLongevity
	 * 为每个单件实例分配一个整形值，确保系统中的每个单件按照顺序释放
	 */
    template <typename T, typename Destroyer>
    void SetLongevity(T* pDynObject, unsigned int longevity,
        Destroyer d = Private::Deleter<T>::Delete)
    {
        using namespace Private;
        
        TrackerArray pNewArray = static_cast<TrackerArray>(
                std::realloc(pTrackerArray, 
                    sizeof(*pTrackerArray) * (elements + 1)));
        if (!pNewArray) throw std::bad_alloc();
        
        // Delayed assignment for exception safety
        pTrackerArray = pNewArray;
        
        LifetimeTracker* p = new ConcreteLifetimeTracker<T, Destroyer>(
            pDynObject, longevity, d);
        
        // Insert a pointer to the object into the queue
        TrackerArray pos = std::upper_bound(
            pTrackerArray, 
            pTrackerArray + elements, 
            p, 
            LifetimeTracker::Compare);
        std::copy_backward(
            pos, 
            pTrackerArray + elements,
            pTrackerArray + elements + 1);
        *pos = p;
        ++elements;
        
        // Register a call to AtExitFn
        std::atexit(Private::AtExitFn);
    }
/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
	/** \breif 使用::operator new作为单件的创建策略
	 * 直接使用new操作符创建单件对象
	 */
/** \}*/
    template <class T> struct CreateUsingNew
    {
        static T* Create()
        { return new T; }
        
        static void Destroy(T* p)
        { delete p; }
    };
/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
	/** \brief 使用placement new作为创建策略
	 * 就是用malloc和placement new创建单件对象
	 */
/** \}*/
    template <class T> struct CreateUsingMalloc
    {
        static T* Create()
        {
            void* p = std::malloc(sizeof(T));
            if (!p) return 0;
            return new(p) T;
        }
        
        static void Destroy(T* p)
        {
            p->~T();
            std::free(p);
        }
    };
    
/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
	/** \brief 使用静态内存作为创建策略
	 * 就是使用静态内存创建单件对象(对于不同的机器，由于存在内存对整(memory alignment)问题,可能
	 * 不可移植.
	 */
/** \}*/
    template <class T> struct CreateStatic
    {
        union MaxAlign
        {
            char t_[sizeof(T)];
            short int shortInt_;
            int int_;
            long int longInt_;
            float float_;
            double double_;
            long double longDouble_;
            struct Test;
            int Test::* pMember_;
            int (Test::*pMemberFn_)(int);
        };
        
        static T* Create()
        {
            static MaxAlign staticMemory_;
            return new(&staticMemory_) T;
        }
        
        static void Destroy(T* p)
        {
            p->~T();
        }
    };
    
/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
	/** \brief 缺省生命期控制策略
	 * 直接使用编译器产生的atexit();
	 */
/** \}*/
    template <class T>
    struct DefaultLifetime
    {
        static void ScheduleDestruction(T*, void (*pFun)())
        { std::atexit(pFun); }
        
        static void OnDeadReference()
        { throw std::logic_error("Dead Reference Detected"); }
    };

	/** \brief '不死'生命期策略(Phoenix)
	 * 跟踪单件是否已经被摧毁，如果是则在使用这个单件时再创建一个.
	 */
    template <class T>
    class PhoenixSingleton
    {
    public:
        static void ScheduleDestruction(T*, void (*pFun)())
        {
#ifndef ATEXIT_FIXED
            if (!destroyedOnce_)
#endif
                std::atexit(pFun);
        }
        
        static void OnDeadReference()
        {
#ifndef ATEXIT_FIXED
            destroyedOnce_ = true;
#endif
        }
        
    private:
#ifndef ATEXIT_FIXED
        static bool destroyedOnce_;
#endif
    };
    
#ifndef ATEXIT_FIXED
    template <class T> bool PhoenixSingleton<T>::destroyedOnce_ = false;
#endif
        
    namespace Private
    {
		 /** \brief 带生命期控制的单件的适配器(Adapter Pattern)
		  */
        template <class T>
        struct Adapter
        {
            void operator()(T*) { return pFun_(); }
            void (*pFun_)();
        };
    }

/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
	/** \brief 带生命期控制策略的Singleton
	 * 系统中的每个单件都必需提供一个名为GetLongevity(T*)的public函数，返回生命期整形
	 * 值(数值越大越排在后被释放，数值越小越早被释放).
	 */
/** \}*/
    template <class T>
    class SingletonWithLongevity
    {
    public:
        static void ScheduleDestruction(T* pObj, void (*pFun)())
        {
            Private::Adapter<T> adapter = { pFun };
            SetLongevity(pObj, GetLongevity(pObj), adapter);
        }
        
        static void OnDeadReference()
        { throw std::logic_error("Dead Reference Detected"); }
    };

/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
	/** \brief 不释放单件的生命期控制策略
	 */
/** \}*/
    template <class T>
    struct NoDestroy
    {
        static void ScheduleDestruction(T*, void (*)())
        {}
        
        static void OnDeadReference()
        {}
    };

/** \defgroup libsr 基础工具库(libsr)
 *  \{*/
	/** \class SingletonHolder Singleton.h
	 *  \brief 单件包裹类
	 */
/** \}*/
    template
    <
        typename T,
        template <class> class CreationPolicy = CreateUsingNew,
        template <class> class LifetimePolicy = DefaultLifetime,
        template <class> class ThreadingModel = SingleThreaded
    >
    class SingletonHolder
    {
    public:
        static T& Instance();
        
    private:
        // Helpers
        static void MakeInstance();
        static void DestroySingleton();
        
        // Protection
        SingletonHolder();
        
        // Data
        typedef typename ThreadingModel<T*>::VolatileType PtrInstanceType;
        static PtrInstanceType pInstance_;
        static bool destroyed_;
    };
    
////////////////////////////////////////////////////////////////////////////////
// SingletonHolder's data
////////////////////////////////////////////////////////////////////////////////

    template
    <
        class T,
        template <class> class C,
        template <class> class L,
        template <class> class M
    >
    typename SingletonHolder<T, C, L, M>::PtrInstanceType
        SingletonHolder<T, C, L, M>::pInstance_;

    template
    <
        class T,
        template <class> class C,
        template <class> class L,
        template <class> class M
    >
    bool SingletonHolder<T, C, L, M>::destroyed_;

	/** \brief 返回单件实例
	 */
    template
    <
        class T,
        template <class> class CreationPolicy,
        template <class> class LifetimePolicy,
        template <class> class ThreadingModel
    >
    inline T& SingletonHolder<T, CreationPolicy, 
        LifetimePolicy, ThreadingModel>::Instance()
    {
        if (!pInstance_)
        {
            MakeInstance();
        }
        return *pInstance_;
    }

	/** \brief MakeInstance - Instance函数的辅助函数
	 */
    template
    <
        class T,
        template <class> class CreationPolicy,
        template <class> class LifetimePolicy,
        template <class> class ThreadingModel
    >
    void SingletonHolder<T, CreationPolicy, 
        LifetimePolicy, ThreadingModel>::MakeInstance()
    {
        typename ThreadingModel<T>::Lock guard;
        (void)guard;
        
        if (!pInstance_)
        {
            if (destroyed_)
            {
                LifetimePolicy<T>::OnDeadReference();
                destroyed_ = false;
            }
            pInstance_ = CreationPolicy<T>::Create();
            LifetimePolicy<T>::ScheduleDestruction(pInstance_, 
                &DestroySingleton);
        }
    }

    template
    <
        class T,
        template <class> class CreationPolicy,
        template <class> class L,
        template <class> class M
    >
    void SingletonHolder<T, CreationPolicy, L, M>::DestroySingleton()
    {
        assert(!destroyed_);
        CreationPolicy<T>::Destroy(pInstance_);
        pInstance_ = 0;
        destroyed_ = true;
    }

END_SUNRISE_NAMESPACE

////////////////////////////////////////////////////////////////////////////////
// Change log:
// May 21, 2001: Correct the volatile qualifier - credit due to Darin Adler
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
// January 08, 2002: Fixed bug in call to realloc - credit due to Nigel Gent and
//      Eike Petersen
// March 08, 2002: moved the assignment to pTrackerArray in SetLongevity to fix
//      exception safety issue. Credit due to Kari Hoijarvi
// May 09, 2002: Fixed bug in Compare that caused longevities to act backwards.
//      Credit due to Scott McDonald.
////////////////////////////////////////////////////////////////////////////////

#endif // SINGLETON_INC_
