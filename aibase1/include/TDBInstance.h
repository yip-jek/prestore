/***************************************************
 FileName : TDBInstance.h
 Author : ̷����
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
	/** \brief ��ʼ��
	 */
	static void Init(TOraCon* con);

	/** \brief ȡ��Informix����ʵ��ָ��
	 *
	 */
    static TOraCon* ConInstance(void);

	/** \breif ȡ��Informix��ѯʵ��ָ��
	 *
	 */
    static TQueryOra* QryInstance(void);

	/** \brief �ͷ���Դ�����������ݿ�����
	 */
    static void Destory();

  private:
	/** \brief ȱʡ���캯��
	 *
	 * ����Ȩ����private��ȡ���û����ܶ�̬����ʵ��������Singletonģʽ�ĵ���������
	 *
	 */
    TDBInstance() {}

  private:
	/** \brief Informix������ʵ��ָ�� */
    static TOraCon* m_pConInst;
  };
#else
#include "TInformixEx.h"
#include "TQueryEx.h"

/** \defgroup libdb   ���ݿ��̿�(libdb) */
/** \{*/
/** \file TDBInstance.h
 *  \brief ʹ��Singletonģʽʵ�ֵ�ȡ��Infomix��Connection��Queryʵ���ĵ�����.*/

/** \class TDBInstance
 *  \brief ʹ��Singletonģʽʵ�ֵ�ȡ��Infomix��Connection��Queryʵ���ĵ�����.*/
/** \}*/

/** \example exDB.cpp 
 *  \example exInf.ecpp
 *  \brief TDBInstance TQueryEx TQueryOra TInformixEx TOraCon ��ʾ������ 
 */

class TDBInstance
  {
  public:
	/** \brief ��ʼ��
	 */
	static void Init(TInformixEx* con);

	/** \brief ȡ��Informix����ʵ��ָ��
	 *
	 */
    static TInformixEx* ConInstance(void);

	/** \breif ȡ��Informix��ѯʵ��ָ��
	 *
	 */
    static TQueryEx* QryInstance(void);

	/** \brief �ͷ���Դ�����������ݿ�����
	 */
    static void Destory();

  private:
	/** \brief ȱʡ���캯��
	 *
	 * ����Ȩ����private��ȡ���û����ܶ�̬����ʵ��������Singletonģʽ�ĵ���������
	 *
	 */
    TDBInstance() {}

  private:
	/** \brief Informix������ʵ��ָ�� */
    static TInformixEx* m_pConInst;
  };

#endif
#endif // End of "__TDBINSTANCE_H__"
