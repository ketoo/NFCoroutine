// -------------------------------------------------------------------------
//    @FileName         :    NFSingleton.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2011-01-21
//    @Module           :    NFSingleton
//
// -------------------------------------------------------------------------

#ifndef NF_SINGLETON_H
#define NF_SINGLETON_H

#include <iostream>

template <class T>
class NFSingleton
{

public:

    NFSingleton()
    {
        //assert( !m_instance );
        m_pInstance = static_cast< T* >(this);
    }

    ~NFSingleton()
    {
        //assert( m_instance );
        m_pInstance = NULL;
    }

public:

    static T* GetSingletonPtr()
    {
        if (NULL == m_pInstance)
        {
            m_pInstance = new T;
        }

        return m_pInstance;
    }

    static T& GetSingletonRef()
    {
        return *GetSingletonPtr();
    }

    static T* Instance()
    {
        return GetSingletonPtr();
    }

    static T& InstanceRef()
    {
        return GetSingletonRef();
    }

    static void ReleaseInstance()
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }

private:
    static T* m_pInstance;
};

template <class T>
T* NFSingleton<T>::m_pInstance = NULL;

#endif