#ifndef __MY_FUNCTION_H__
#define __MY_FUNCTION_H__

//これってinline展開されるの？？？

//チェック付きdelete
template <typename T>inline void safe_delete ( T*& p )
{
	typedef char type_must_be_complete [sizeof( T ) ? 1 : -1];
	( void ) sizeof( type_must_be_complete );
	if ( p != nullptr ) { delete ( p ); ( p ) = nullptr; }
}

//チェック付きdelete(配列用)
template <typename T>inline void safe_array_delete ( T*& p )
{
	typedef char type_must_be_complete [sizeof( T ) ? 1 : -1];
	( void ) sizeof( type_must_be_complete );
	if ( p != nullptr ) { delete[] ( p ); ( p ) = nullptr; }
}

//チェック付きdelete関数オブジェクト
template <typename T> struct safe_deleter
{
	void operator()( T* p )const
	{
		safe_delete ( p );
	}
};

//チェック付きdelete関数オブジェクト(配列用)
template <typename T> struct  safe_array_deleter
{
	void operator()( T* p )
	{
		safe_array_delete ( p );
	}
};


#endif //__MY_FUNCTION_H__

//EOF