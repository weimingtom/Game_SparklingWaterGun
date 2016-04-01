#pragma once

template< class T >
class SingletonTemplate{
public:
	static T& getInstance(void){
		static T singleton;
		return singleton;
	}
protected:
	SingletonTemplate();
	virtual ~SingletonTemplate();
private:
	SingletonTemplate(const SingletonTemplate& obj);
	SingletonTemplate& operator=(const SingletonTemplate& obj);
	static T* singleton;
};

template< class T >
T* SingletonTemplate< T >::singleton = 0;

template< class T >
SingletonTemplate< T >::SingletonTemplate(){

}

template< class T >
SingletonTemplate< T >::~SingletonTemplate(){

}