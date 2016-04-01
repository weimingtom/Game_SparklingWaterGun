#ifndef __MANAGER_BASE_H__
#define __MANAGER_BASE_H__

#include <cassert>

template <class _object_type,int _data_size>
class ManagerBase
{
public:
	typedef _object_type OBJECT,* LPOBJECT;
	typedef int INDEX;
protected:
	LPOBJECT	m_pObject_data[_data_size];
	INDEX		m_Index_count;
	const int	m_DataSize;
public:
	enum
	{
		ERR_INDEX = -1
	};
public:
	ManagerBase():
	m_DataSize(_data_size),
	m_Index_count(0)
	{
		assert(_data_size>0&&"_data_size‚ª‚OˆÈ‰º‚Å‚·");
		for (int i = 0; i < _data_size; ++i)
			m_pObject_data[i] = nullptr;
	}
	virtual ~ManagerBase(){}
public:
	INDEX RegistObject(LPOBJECT object_ptr)
	{
		for (int i = 0; i < _data_size; ++i)
		{
			++m_Index_count;
			m_Index_count %= _data_size;

			if (m_pObject_data[m_Index_count])continue;

			m_pObject_data[m_Index_count] = object_ptr;
			return m_Index_count;
		}
		return ERR_INDEX;
	}
	bool GetObjectPtr(LPOBJECT* pOut, INDEX index)
	{
		if (index >= _data_size || index < 0)return false;
		if (m_pObject_data[index])
		{
			*pOut = m_pObject_data[index];
			return true;
		}
		return false;
	}
	bool EraceObject(INDEX index)
	{
		if (index >= _data_size || index < 0)return false;
		if (m_pObject_data[index])
		{
			m_pObject_data[index] = nullptr;
			return true;
		}
		return false;
	}
};

#endif