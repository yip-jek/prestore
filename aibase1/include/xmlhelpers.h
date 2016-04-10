#ifndef __XMLHELPERS_H__
#define __XMLHELPERS_H__

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

BEGIN_SUNRISE_NAMESPACE



namespace xml {

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/
/** \file xmlhelpers.h
 *	\brief xml辅助类 */
 
/** \class xmllocation
 * \brief xml流定位类 */
 /** \}*/
class xmllocation {
public:
   /** \brief 构造函数*/
   xmllocation() { reset(); }

   /** \brief 获得当前行号
    *  \return 返回当前行号 */
   int get_line() const { return line; }

   /** \brief 获得当前位置
    *  \return 返回当前位置 */
   int get_pos() const { return pos; }

   /** \brief 当前位置向后移动指定步长
    *  \param chars 增长步长, 缺省步长为1
    *  \return 返回移动后的位置 */
   void step( int chars = 1 ) { pos+=chars; }

   /** \brief 标志进入新的一行,位置在新行的第一位 */
   void newline() { line++; pos=1; }

   /** \brief 重置位置值,回到第一行第一位 */
   void reset() { line=pos=1; }

protected:
   int line; /**< 当前位置所在行号 */
   int pos;  /**< 但前位置所在位置 */
};

}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
