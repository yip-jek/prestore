#ifndef __XMLHELPERS_H__
#define __XMLHELPERS_H__

#if !defined(__PRELUDE_H__)
#include "Prelude.h"
#endif

BEGIN_SUNRISE_NAMESPACE



namespace xml {

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/
/** \file xmlhelpers.h
 *	\brief xml������ */
 
/** \class xmllocation
 * \brief xml����λ�� */
 /** \}*/
class xmllocation {
public:
   /** \brief ���캯��*/
   xmllocation() { reset(); }

   /** \brief ��õ�ǰ�к�
    *  \return ���ص�ǰ�к� */
   int get_line() const { return line; }

   /** \brief ��õ�ǰλ��
    *  \return ���ص�ǰλ�� */
   int get_pos() const { return pos; }

   /** \brief ��ǰλ������ƶ�ָ������
    *  \param chars ��������, ȱʡ����Ϊ1
    *  \return �����ƶ����λ�� */
   void step( int chars = 1 ) { pos+=chars; }

   /** \brief ��־�����µ�һ��,λ�������еĵ�һλ */
   void newline() { line++; pos=1; }

   /** \brief ����λ��ֵ,�ص���һ�е�һλ */
   void reset() { line=pos=1; }

protected:
   int line; /**< ��ǰλ�������к� */
   int pos;  /**< ��ǰλ������λ�� */
};

}

END_SUNRISE_NAMESPACE

#endif
/* vi: set ts=3: */
