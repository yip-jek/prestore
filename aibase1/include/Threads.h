#ifndef THREADS_H_
#define THREADS_H_

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

#ifndef DEFAULT_THREADING
#ifdef NEED_NAMESPACE
#define DEFAULT_THREADING /**/ sunrise::SingleThreaded
#else
#define DEFAULT_THREADING /**/ SingleThreaded
#endif
#endif

/** \addtogroup libsr 基础工具库(libsr) 
@{ 
*/

/** @file Threads.h
 * \brief SRThread::Thread模板类
**/

BEGIN_SUNRISE_NAMESPACE

////////////////////////////////////////////////////////////////////////////////
// class template SingleThreaded
// Implementation of the ThreadingModel policy used by various classes
// Implements a single-threaded model; no synchronization
////////////////////////////////////////////////////////////////////////////////

    template <class Host>
    class SingleThreaded
    {
    public:
        struct Lock
        {
            Lock() {}
            Lock(const Host&) {}
        };
        
        typedef Host VolatileType;

        typedef int IntType; 

        static IntType AtomicAdd(volatile IntType& lval, IntType val)
        { return lval += val; }
        
        static IntType AtomicSubtract(volatile IntType& lval, IntType val)
        { return lval -= val; }

        static IntType AtomicMultiply(volatile IntType& lval, IntType val)
        { return lval *= val; }
        
        static IntType AtomicDivide(volatile IntType& lval, IntType val)
        { return lval /= val; }
        
        static IntType AtomicIncrement(volatile IntType& lval)
        { return ++lval; }
        
        static IntType AtomicDecrement(volatile IntType& lval)
        { return --lval; }
        
        static void AtomicAssign(volatile IntType & lval, IntType val)
        { lval = val; }
        
        static void AtomicAssign(IntType & lval, volatile IntType & val)
        { lval = val; }
    };
    
#ifdef _WINDOWS_

////////////////////////////////////////////////////////////////////////////////
// class template ObjectLevelLockable
// Implementation of the ThreadingModel policy used by various classes
// Implements a object-level locking scheme
////////////////////////////////////////////////////////////////////////////////

    template <class Host>
    class ObjectLevelLockable
    {
        CRITICAL_SECTION mtx_;

    public:
        ObjectLevelLockable()
        {
            ::InitializeCriticalSection(&mtx_);
        }

        ~ObjectLevelLockable()
        {
            ::DeleteCriticalSection(&mtx_);
        }

        class Lock;
        friend class Lock;
        
        class Lock
        {
            ObjectLevelLockable& host_;
            
            Lock(const Lock&);
            Lock& operator=(const Lock&);
        public:
            Lock(Host& host) : host_(host)
            {
                ::EnterCriticalSection(&host_.mtx_);
            }
            ~Lock()
            {
                ::LeaveCriticalSection(&host_.mtx_);
            }
        };

        typedef volatile Host VolatileType;

        typedef LONG IntType; 

        static IntType AtomicIncrement(volatile IntType& lval)
        { return InterlockedIncrement(&const_cast<IntType&>(lval)); }
        
        static IntType AtomicDecrement(volatile IntType& lval)
        { return InterlockedDecrement(&const_cast<IntType&>(lval)); }
        
        static void AtomicAssign(volatile IntType& lval, IntType val)
        { InterlockedExchange(&const_cast<IntType&>(lval), val); }
        
        static void AtomicAssign(IntType& lval, volatile IntType& val)
        { InterlockedExchange(&lval, val); }
    };
    
    template <class Host>
    class ClassLevelLockable
    {
        static CRITICAL_SECTION mtx_;

        struct Initializer;
        friend struct Initializer;
        struct Initializer
        {
            Initializer()
            {
                ::InitializeCriticalSection(&mtx_);
            }
            ~Initializer()
            {
                ::DeleteCriticalSection(&mtx_);
            }
        };
        
        static Initializer initializer_;

    public:
        class Lock;
        friend class Lock;
        
        class Lock
        {
            Lock(const Lock&);
            Lock& operator=(const Lock&);
        public:
            Lock()
            {
                ::EnterCriticalSection(&mtx_);
            }
            Lock(Host&)
            {
                ::EnterCriticalSection(&mtx_);
            }
            ~Lock()
            {
                ::LeaveCriticalSection(&mtx_);
            }
        };

        typedef volatile Host VolatileType;

        typedef LONG IntType; 

        static IntType AtomicIncrement(volatile IntType& lval)
        { return InterlockedIncrement(&const_cast<IntType&>(lval)); }
        
        static IntType AtomicDecrement(volatile IntType& lval)
        { return InterlockedDecrement(&const_cast<IntType&>(lval)); }
        
        static void AtomicAssign(volatile IntType& lval, IntType val)
        { InterlockedExchange(&const_cast<IntType&>(lval), val); }
        
        static void AtomicAssign(IntType& lval, volatile IntType& val)
        { InterlockedExchange(&lval, val); }
    };
    
    template <class Host>
    CRITICAL_SECTION ClassLevelLockable<Host>::mtx_;
    
    template <class Host>
    typename ClassLevelLockable<Host>::Initializer 
    ClassLevelLockable<Host>::initializer_;
    
#endif  

/** @} */

END_SUNRISE_NAMESPACE
	 
////////////////////////////////////////////////////////////////////////////////
// Change log:
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
// January 10, 2002: Fixed bug in AtomicDivide - credit due to Jordi Guerrero
// August 14, 2002: Changed some AtomicDivide's to AtomicDecrement's MKH
////////////////////////////////////////////////////////////////////////////////

#endif
