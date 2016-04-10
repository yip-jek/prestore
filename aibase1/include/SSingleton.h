#ifndef SIMPLE_SINGLETON_HOLDER_H
#define SIMPLE_SINGLETON_HOLDER_H

#include <cstdlib>


/** \defgroup libsr �������߿�(libsr)
   *  \{*/
/** \file SSingleton.h 
   *  \brief �����\ref Singleton.h�ж���ĵ�����ʵ�ֵļ򵥵�����,
             ����Singleton.hʹ�õ�һЩģ����sun�������ϱ��벻��,
             ����ʹ������򵥵�Singleton��ʵ�ֵ��幦�� 
    */
/** \class SingletonHolder SSingleton.h
  * \brief ����� \ref Singleton.h ����ʵ�ֵĵ��幦�ܸ��򵥵�Singleton 
 */
/** \}*/

template<class T>
class SingletonHolder
{
public:
	/** \brief ��ȡ������������
	  * \return ���ص�����������*/
	static T& Instance();
private:
	/** \brief ���쵥�����*/
	static void MakeInstance();
	/** \brief ���ٵ������*/
	static void DestroySingleton();

	/** \brief ���캯��*/
	SingletonHolder();
	/** \brief ��������ָ��*/
	static T* pInstance_;
	/** \brief ��������Ƿ����ٵı�־*/
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
