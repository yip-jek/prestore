/***************************************************
 FileName : TDBInstance.h
 Author : 谭立辉
 Version : 0.1.001
 Date : 2002/12/6 7:17:39
 Description : 
 Function List : 
 History : 
 1. Date : 
    Author : 
    Modification : 
 2. 
 ******************************************************/


#ifndef __TDBINSTANCE_H__
#define __TDBINSTANCE_H__


#ifdef __ORACLE
#include "TQueryOra.h"
#include "TOraCon.h"
class TDBInstance
  {
  public:
	/** \brief 初始化
	 */
	static void Init(TOraCon* con);

	/** \brief 取回Informix连接实例指针
	 *
	 */
    static TOraCon* ConInstance(void);

	/** \breif 取回Informix查询实例指针
	 *
	 */
    static TQueryOra* QryInstance(void);

	/** \brief 释放资源，比如是数据库连接
	 */
    static void Destory();

  private:
	/** \brief 缺省构造函数
	 *
	 * 访问权限是private，取保用户不能动态创建实例，这是Singleton模式的典型做法。
	 *
	 */
    TDBInstance() {}

  private:
	/** \brief Informix连接类实例指针 */
    static TOraCon* m_pConInst;
  };
#else
#include "TInformixEx.h"
#include "TQueryEx.h"

/** \defgroup libdb   数据库编程库(libdb) */
/** \{*/
/** \file TDBInstance.h
 *  \brief 使用Singleton模式实现的取用Infomix的Connection或Query实例的单件类.*/

/** \class TDBInstance
 *  \brief 使用Singleton模式实现的取用Infomix的Connection或Query实例的单件类.*/
/** \}*/

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon 类示例程序 
 */

class TDBInstance
  {
  public:
	/** \brief 初始化
	 */
	static void Init(TInformixEx* con);

	/** \brief 取回Informix连接实例指针
	 *
	 */
    static TInformixEx* ConInstance(void);

	/** \breif 取回Informix查询实例指针
	 *
	 */
    static TQueryEx* QryInstance(void);

	/** \brief 释放资源，比如是数据库连接
	 */
    static void Destory();

  private:
	/** \brief 缺省构造函数
	 *
	 * 访问权限是private，取保用户不能动态创建实例，这是Singleton模式的典型做法。
	 *
	 */
    TDBInstance() {}

  private:
	/** \brief Informix连接类实例指针 */
    static TInformixEx* m_pConInst;
  };

#endif
#endif // End of "__TDBINSTANCE_H__"
