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
 *  \brief Singleton��ͼ򵥰汾��Singleton���÷���ʾ����
 */

/** \defgroup libsr �������߿�(libsr)
 *  \{*/
/** \file Singleton.h 
 *  \brief ������ͨ��ģ��֧�ֶ��ִ�������,�����ڿ��Ʋ��Ժ��̵߳ĵ�����
 */
/** \}*/
 

BEGIN_SUNRISE_NAMESPACE 

    namespace Private
    {
/** \defgroup libsr �������߿�(libsr)
 *  \{*/
/** \class LifetimeTracker Singleton.h
 * \brief �����ڸ�����
 * ����ģ��SetLongevity��Э����
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

        /** �����ݻٺ���
			*/
        template <typename T>
        struct Deleter
        {
            static void Delete(T* pObj)
            { delete pObj; }
        };
/** \defgroup libsr �������߿�(libsr)
 *  \{*/
        /** \class ConcreteLifetimeTracker
			* ���������ڸ�����
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

	/** \brief ģ�庯��SetLongevity
	 * Ϊÿ������ʵ������һ������ֵ��ȷ��ϵͳ�е�ÿ����������˳���ͷ�
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
/** \defgroup libsr �������߿�(libsr)
 *  \{*/
	/** \breif ʹ��::operator new��Ϊ�����Ĵ�������
	 * ֱ��ʹ��new������������������
	 */
/** \}*/
    template <class T> struct CreateUsingNew
    {
        static T* Create()
        { return new T; }
        
        static void Destroy(T* p)
        { delete p; }
    };
/** \defgroup libsr �������߿�(libsr)
 *  \{*/
	/** \brief ʹ��placement new��Ϊ��������
	 * ������malloc��placement new������������
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
    
/** \defgroup libsr �������߿�(libsr)
 *  \{*/
	/** \brief ʹ�þ�̬�ڴ���Ϊ��������
	 * ����ʹ�þ�̬�ڴ洴����������(���ڲ�ͬ�Ļ��������ڴ����ڴ����(memory alignment)����,����
	 * ������ֲ.
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
    
/** \defgroup libsr �������߿�(libsr)
 *  \{*/
	/** \brief ȱʡ�����ڿ��Ʋ���
	 * ֱ��ʹ�ñ�����������atexit();
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

	/** \brief '����'�����ڲ���(Phoenix)
	 * ���ٵ����Ƿ��Ѿ����ݻ٣����������ʹ���������ʱ�ٴ���һ��.
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
		 /** \brief �������ڿ��Ƶĵ�����������(Adapter Pattern)
		  */
        template <class T>
        struct Adapter
        {
            void operator()(T*) { return pFun_(); }
            void (*pFun_)();
        };
    }

/** \defgroup libsr �������߿�(libsr)
 *  \{*/
	/** \brief �������ڿ��Ʋ��Ե�Singleton
	 * ϵͳ�е�ÿ�������������ṩһ����ΪGetLongevity(T*)��public��������������������
	 * ֵ(��ֵԽ��Խ���ں��ͷţ���ֵԽСԽ�类�ͷ�).
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

/** \defgroup libsr �������߿�(libsr)
 *  \{*/
	/** \brief ���ͷŵ����������ڿ��Ʋ���
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

/** \defgroup libsr �������߿�(libsr)
 *  \{*/
	/** \class SingletonHolder Singleton.h
	 *  \brief ����������
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

	/** \brief ���ص���ʵ��
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

	/** \brief MakeInstance - Instance�����ĸ�������
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
