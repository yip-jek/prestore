#if !defined(__TAPP_H__)
#define __TAPP_H__

#if !defined (__PRELUDE_H__)
#include "Prelude.h"
#endif

#include "dynso_helpers.h"
//#include "TInformixEx.h"

/** \defgroup libcd  编码解码模块(libcd) */
/** \{*/

/** \file TApp.h
 * \brief TApp类头文件
 */

/** \class TApp
 * \brief 定义动态组件类,对BaseComponent的继承和扩充
 * 是一个纯虚类
 */

/** \}*/

class  TApp : public DynSO::BaseComponent {
	public:
		//virtual int run(TInformixEx* con, int argc, char* argv[]) = 0;
		virtual int run(int argc, char* argv[]) = 0;
};

/** \brief 说明c库函数 */
extern "C" {
	TApp* CreateObject(void);
}

#else 
class TApp;
#endif

