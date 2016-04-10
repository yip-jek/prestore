#if !defined(__TAPP_H__)
#define __TAPP_H__

#if !defined (__PRELUDE_H__)
#include "Prelude.h"
#endif

#include "dynso_helpers.h"
//#include "TInformixEx.h"

/** \defgroup libcd  �������ģ��(libcd) */
/** \{*/

/** \file TApp.h
 * \brief TApp��ͷ�ļ�
 */

/** \class TApp
 * \brief ���嶯̬�����,��BaseComponent�ļ̳к�����
 * ��һ��������
 */

/** \}*/

class  TApp : public DynSO::BaseComponent {
	public:
		//virtual int run(TInformixEx* con, int argc, char* argv[]) = 0;
		virtual int run(int argc, char* argv[]) = 0;
};

/** \brief ˵��c�⺯�� */
extern "C" {
	TApp* CreateObject(void);
}

#else 
class TApp;
#endif

