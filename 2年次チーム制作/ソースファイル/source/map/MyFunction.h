#ifndef __MY_FUNCTION_H__
#define __MY_FUNCTION_H__

//�������inline�W�J�����́H�H�H

//�`�F�b�N�t��delete
template <typename T>inline void safe_delete ( T*& p )
{
	typedef char type_must_be_complete [sizeof( T ) ? 1 : -1];
	( void ) sizeof( type_must_be_complete );
	if ( p != nullptr ) { delete ( p ); ( p ) = nullptr; }
}

//�`�F�b�N�t��delete(�z��p)
template <typename T>inline void safe_array_delete ( T*& p )
{
	typedef char type_must_be_complete [sizeof( T ) ? 1 : -1];
	( void ) sizeof( type_must_be_complete );
	if ( p != nullptr ) { delete[] ( p ); ( p ) = nullptr; }
}

//�`�F�b�N�t��delete�֐��I�u�W�F�N�g
template <typename T> struct safe_deleter
{
	void operator()( T* p )const
	{
		safe_delete ( p );
	}
};

//�`�F�b�N�t��delete�֐��I�u�W�F�N�g(�z��p)
template <typename T> struct  safe_array_deleter
{
	void operator()( T* p )
	{
		safe_array_delete ( p );
	}
};


#endif //__MY_FUNCTION_H__

//EOF