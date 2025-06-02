#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#if 0

template < typename T > class CSingleton
{
	static T* m_singleton;
public:
    CSingleton()
    {
    	long offset = (long)(T*)1l - (long)( CSingleton  <T*>)( T* )1l;
        m_singleton = ( T* )((long)this + offset );
    }
    ~CSingleton()
    {
    	m_singleton = 0;
    }
    static T& GetSingleton()
    {
    	return *m_singleton;
        
    }
};

template < typename T > T* CSingleton < T >::m_singleton = 0;

#else

template< class T > class CSingleton
{
public:
    CSingleton(){};
    ~CSingleton(){};
    static T& GetSingleton()
    {
    	static T m_singleton;
        return m_singleton;
    }
};

#endif


#endif