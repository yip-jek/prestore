#ifndef SIMPLE_SINGLETON_HOLDER_H
#define SIMPLE_SINGLETON_HOLDER_H

#include <cstdlib>


/** \defgroup libsr 基础工具库(libsr)
   *  \{*/
/** \file SSingleton.h 
   *  \brief 相对于\ref Singleton.h中定义的单体类实现的简单单体类,
             由于Singleton.h使用的一些模板在sun编译器上编译不过,
             所以使用这个简单的Singleton来实现单体功能 
    */
/** \class SingletonHolder SSingleton.h
  * \brief 相对于 \ref Singleton.h 里面实现的单体功能更简单的Singleton 
 */
/** \}*/

template<class T>
class SingletonHolder
{
public:
	/** \brief 获取单体对象的引用
	  * \return 返回单体对象的引用*/
	static T& Instance();
private:
	/** \brief 构造单体对象*/
	static void MakeInstance();
	/** \brief 销毁单体对象*/
	static void DestroySingleton();

	/** \brief 构造函数*/
	SingletonHolder();
	/** \brief 单体对象的指针*/
	static T* pInstance_;
	/** \brief 单体对象是否被销毁的标志*/
	static bool destroyed_;
};
template<class T>
T* SingletonHolder<T>::pInstance_ = NULL;

template<class T> 
bool SingletonHolder<T>::destroyed_ = true;

template<class T>
inline T& SingletonHolder<T>::Instance()
{
	if (!pInstance_)
	{
		MakeInstance();
	}
	return *pInstance_;
}

template<class T>
void SingletonHolder<T>::MakeInstance()
{
	if (!pInstance_)
	{
		if (destroyed_)
		{
			destroyed_ = false;
		}
		pInstance_ = new T;
		std::atexit(&DestroySingleton);
	}
}

template<class T>
void SingletonHolder<T>::DestroySingleton()
{
	//assert(!destroyed_);
	delete (pInstance_);
	pInstance_ = NULL;
	destroyed_ = true;
}

#endif
