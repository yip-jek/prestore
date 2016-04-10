
#if !defined(__PRE_H__)
#define      __PRE_H__

#ifdef __cplusplus
extern "C"{
#endif 

////////////////////////////////////////////////////////////////////////////////////////
// Marco information
#define M_MAX_FILEPATH	 512
#define M_MAX_PATH       256
#define M_MAX_NAME        64
#define M_TIMEBUF_LEN    128
#define M_SHORTBUF_LEN   256
#define M_LONGBUF_LEN   1024

////////////////////////////////////////////////////////////////////////////////////////
// data type information
#define VOID    void

#undef BOOL
#undef FALSE
#undef TRUE
typedef bool BOOL;
#define FALSE false
#define TRUE true

#if 0
#if !defined(FALSE)
	#define FALSE  false
#endif
#if !defined(TRUE)
	#define TRUE   !FALSE
#endif
#if !defined(BOOL)
	typedef bool BOOL;
#endif
#endif

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef int INT;
typedef long LONG;
typedef unsigned long DWORD;
typedef char CHAR;
typedef CHAR *LPSTR;
typedef const CHAR *LPCSTR;
typedef unsigned char UCHAR;
typedef short SHORT;
typedef unsigned short USHORT;
typedef DWORD ULONG;
typedef double DOUBLE;

////////////////////////////////////////////////////////////////////////////////////////
// marco information
#ifndef __linux__

#if !defined(max) 
	#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#if !defined(MAX)
	#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#if !defined(min)
	#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#if !defined(MIN)
	#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif

#define _countof(array) (sizeof(array)/sizeof(array[0]))

////////////////////////////////////////////////////////////////////////////////////////
// debug information
#ifdef _DEBUG
	#define ASSERT(bf) \
	{ \
		if(!(bf)) \
		{ \
			printf("Assert failed at line %d of file %s\n", \
				   __LINE__,__FILE__); \
			exit(0); \
		} \
	} 
	#define ASSERT_VALID(p)    ASSERT(p!=NULL)
	#define VERIFY(f)          ASSERT(f)
	#define TRACEIT              printf
#else
	inline void ASSERT(BOOL/* bf*/) {}
	#define ASSERT_VALID(p)    ASSERT(p!=NULL)
	#define VERIFY(f)          ASSERT(f)
	#define TRACEIT(f)              
#endif

// 用于对象释放
#define DELETE_OBJECT(f) \
	{	\
		delete f;	\
		f = NULL;	\
	}

#ifdef __cplusplus
}
#endif

#endif

#ifndef ABS
#define ABS 0.000001
#endif



