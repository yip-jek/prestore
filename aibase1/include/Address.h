//------------------------------------------------------------------------------
//                           Address.h
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef ADDRESS_H
#define ADDRESS_H

#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>		// addresses handling
#include <sys/un.h>
#include <string.h>
#include <errno.h>

#include "Prelude.h"
#include "Logger.h"
#include "Assert.h"

BEGIN_SUNRISE_NAMESPACE 

typedef struct sockaddr SA;	// stolen from R.Stevens
typedef struct sockaddr_in SA_IN;	
typedef struct sockaddr_un SA_UN;	

/*! \file Address.h 
 * \brief INET或UNIX-domain地址数据类型的抽象类.
*/

/*! \class Address
 * \brief 地址类. 定义了抽象的地址(包括INET和UNIX-domain)处理方法, 包括地址管理.
 */
class Address {
public:
	/*! \enum addr_state_t
	 * \brief 地址状态(合法，非法)枚举
	 */
	enum addr_state_t { 
		goodbit=0,				/*!< 合法地址 */
		badbit=1				/*!< 非法地址 */
	};
	typedef int addrstate;

private:
	unsigned char m_state;

public:
	/*! \brief 构造函数
	 */
	Address ();

	/*! \brief 析构函数
	 */
	virtual ~Address();

	/*! \brief 检查地址的合法性
	    @return 如果地址合法，则返回true. 否则返回false.
	 */
	bool good() const;

	/*! \brief 获取在地址构造过程中是否产生了错误(例如主机名/端口查找失败或者格式不正确等)
	    @return 如果地址无误，返回true, 否则返回true.
	 */
	bool bad() const;

	operator void* () const;

	/*! \brief bad()函数的非函数.
	    @return 如果地址非法，返回true, 否则返回true.
	*/
	bool operator! () const;
	
	/*! \brief 获取地址实例的size
	 */
	virtual const int getLength() const = 0;

	/*! \brief 获取地址实例指针
	 */
	virtual SA* getAddress() const = 0;
	
	/*! \brief 把对象状态写入日志文件
	 */
	virtual void dump () ;

protected:
	/*! \brief 设置状态
	 *  @param flag_ 状态值
	 */
	void setstate (addrstate flag_) { m_state |= flag_; }
};

END_SUNRISE_NAMESPACE

#endif /* ADDRESS_H */
