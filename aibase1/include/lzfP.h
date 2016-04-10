#ifndef LZFP_h
#define LZFP_h

#define STANDALONE /* at the moment, this is ok. */

#ifndef STANDALONE
# include "lzf.h"
#endif

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file lzfP.h
 *  \brief 定义了一些lzf压缩解压缩类需要的一些宏 */
/** \}*/


/*
 * size of hashtable is (1 << HLOG) * sizeof (char *)
 * decompression is independent of the hash table size
 * the difference between 15 and 14 is very small
 * for small blocks (and 14 is also faster).
 * For a low-memory configuration, use HLOG == 13;
 * For best compression, use 15 or 16.
 */
#ifndef HLOG
# define HLOG 14
#endif

/*
 * sacrifice some compression quality in favour of compression speed.
 * (roughly 1-2% worse compression for large blocks and
 * 9-10% for small, redundant, blocks and >>20% better speed in both cases)
 * In short: enable this for binary data, disable this for text data.
 */
#ifndef ULTRA_FAST
# define ULTRA_FAST 1
#endif

/*
 * unconditionally aligning does not cost very much, so do it if unsure
 */
#ifndef STRICT_ALIGN
# define STRICT_ALIGN !defined(__i386)
#endif

/*
 * use string functions to copy memory.
 * this is usually a loss, even with glibc's optimized memcpy
 */
#ifndef USE_MEMCPY
# define USE_MEMCPY 0
#endif

/*
 * you may choose to pre-set the hash table (might be faster on modern cpus
 * and large (>>64k) blocks)
 */
#ifndef INIT_HTAB
# define INIT_HTAB 0
#endif

/*****************************************************************************/
/* nothing should be changed below */

typedef unsigned char u8;

#if !STRICT_ALIGN
/* for unaligned accesses we need a 16 bit datatype. */
# include <limits.h>
# if USHRT_MAX == 65535
    typedef unsigned short u16;
# elif UINT_MAX == 65535
    typedef unsigned int u16;
# else
#  warn need 16 bit datatype when STRICT_ALIGN == 0, this is non-fatal
#  undef STRICT_ALIGN
#  define STRICT_ALIGN 1
# endif
#endif

#if USE_MEMCPY || INIT_HTAB
# include <string.h>
#endif

#endif

