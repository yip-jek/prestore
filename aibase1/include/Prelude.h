#ifndef __PRELUDE_H__
#define __PRELUDE_H__

#ifdef NEED_INLINE
#define INLINE inline
#else
#define INLINE
#endif

/** \file Prelude.h
  * \brief 定义了一些共用的宏
  */

//定义是否使用sunrise命名空间
#ifdef NEED_NAMESPACE
#define BEGIN_SUNRISE_NAMESPACE namespace sunrise {
#define END_SUNRISE_NAMESPACE }
#define USING_SUNRISE using namespace sunrise
#define SR_SUNRISE_NAMESPACE sunrise::
#else
#define BEGIN_SUNRISE_NAMESPACE
#define END_SUNRISE_NAMESPACE
#define USING_SUNRISE
#define SR_SUNRISE_NAMESPACE
#endif

#ifdef USING_OLD_IOSTREAM
#include <iostream.h>
#define COUT cout
#define CERR cerr
#define CIN  cin
#define ENDL endl
#define OSTREAM ostream
#define FLUSH flush
#define ENDS ends
//#define OSTRINGSTREAM ostringstream
#else
#if !(defined(__TSTRING_H__) && defined(__linux__))
#include <iostream>
#endif
//using namespace std;
#define COUT std::cout
#define CERR std::cerr
#define CIN  std::cin
#define ENDL std::endl
#define OSTREAM std::ostream
#define FLUSH std::flush
#define ENDS std::ends
//#define OSTRINGSTREAM std::ostringstream
#endif

#endif
