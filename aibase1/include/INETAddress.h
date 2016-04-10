#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H

#include <vector>
using std::vector;

#include "Prelude.h"
#include "Address.h"

BEGIN_SUNRISE_NAMESPACE

/*! \file INETAddress.h 

	An incapsulation of TCP/UDP Internet Protocol socket address structure.
*/

/*! \class INETAddress
 * \brief inet地址类
 * 封装inet地址
 */
class INETAddress : public Address {
public:
	/*! \enum Protocol
	 * \brief 协议类型枚举
	 */
	enum Protocol { 
		TCP,					/*!< TCP */
		UDP					/*!< UDP */
	};

public:
	/*! \brief 缺省构造函数
	 */
	INETAddress ();

	/*! \brief 构造函数
	 * 用in_addr地址结构体和端口号构建对象, 客户端使用.
	 *
	 *  @param haddr_ 服务器地址
	 *  @param port_ 服务器监听的端口
	 */
	INETAddress (struct in_addr * haddr_, int port_);

	/*! \brief 构造函数
	 * 用主机名和端口号构建对象, 客户端使用.
	 *
	 *  @param host_ 服务器主机名
	 *  @param port_ 服务器监听的端口
	 */
	INETAddress(const char* host_, int port_);

	/*! \brief 构造函数
	 * 用主机名, 服务名和指定的协议类型构建对象
	 * 其中服务名必需在/etc/services文件中定义
	 *
	 *  @param host_ 服务器主机名
	 *  @param service_ 服务名
	 *  @param protocol_ 协议类型: TCP (缺省) 或者 UDP
	 */
	INETAddress (const char* host_, const char* service_, 
				 Protocol protocol_ = TCP);

	/*! \brief 构造函数
	 * 用端口号构建对象
	 *
	 *  @param port_ 服务器端口
	 */
	INETAddress (int port_);

	/*! \brief 构造函数
	 * 客户端和服务器端INET地址类构造函数
		格式:
	       - [host:]service 
	       - service[@host]

	    如果没有写host, 则用INADDR_ANY地址.
		 service必需要在/etc/services文件里有相应内容.

	    @param address_ 地址串
	    @param protocol_ 协议类型: TCP (缺省) 或 UDP.
	 */
	INETAddress (const char* address_, Protocol protocol_ = TCP);

	/*! \brief 拷贝构造函数
	 */
	INETAddress (SA_IN* address_);

	/*! \brief 拷贝构造函数
	 */
	INETAddress (SA* address_);

	/*! \brief 析构函数
	 */
	~INETAddress () { 
		trace_with_mask("INETAddress::~INETAddress",SOCKTRACE);
	}

	/*! \brief 获取地址实例的尺寸
	 */
	const int getLength () const { return sizeof (m_address); }

	/*! \brief 获取具体地址实例指针
	 */
	SA* getAddress () const { return (SA*) &m_address; }

	/*! \brief 获取主机名
	 */
	string getHostName ();

	/*! \brief 获取端口号
	 */
	int getPort () const { return ntohs (m_address.sin_port); }

	/*! \brief 把地址状态写入日志文件
	 */
	void dump ();

	/*! \brief 获取完整主机名, 如果主机别名，则存放在vector里面返回。

		@param aliases_ 主机别名列表
		@return 返回完整主机名.
	*/
	static string 
	get_fully_qualified_domain_name (vector<string>& aliases_);

private:
	/*! \brief  使主机名转换为点数字格式，端口号转换为网络独立的字节序.

	    如果转换失败，设定对象状态为bad. 同时还设定errno.
	 */
	void createHostPort (const char* host_, int port_);

	/*! \brief 从/etc/services文件查找出端口号，并转换为网络独立的
	 * 字节序.
	 *
	   @param serv_ 服务名
	   @param prot_ 协议类型: tcp (缺省) 或 udp.
	   @return 返回独立于网络的字节序端口号, 如果失败则返回0.
	 */
	int getServiceByName (string serv_, Protocol prot_ = TCP);

	/*! \brief 公共初始化函数, 各构造函数需要调用
	 */
	void init ();

private:
	/*! \brief 完整域名的缓冲.
	 */
	static string m_fqdn_cache;

private:
	/*! \brief 网络地址结构体sockaddr_in
	 */
	SA_IN m_address;
};

END_SUNRISE_NAMESPACE

#endif /* INET_ADDRESS_H */
