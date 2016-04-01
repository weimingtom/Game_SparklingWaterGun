#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__


//********************************************************
//		データ管理クラステンプレート
//********************************************************

template<class data_type,size_t data_max,class key_type>
class DataManager
{
	typedef data_type MYDATA, *LPMYDATA;
	typedef key_type DATAKEY;
private:
	LPMYDATA m_pDataArray[data_max];
public:
	inline DataManager()
	{
		for (size_t i = 0; i < data_max; ++i)
		{
			m_pDataArray[i] = nullptr;
		}
	}
	inline ~DataManager()
	{
		Release();
	}
public:
	inline void Release()
	{
		for (size_t i = 0; i < data_max; ++i)
		{
			delete m_pDataArray[i];
			m_pDataArray[i] = nullptr;
		}
	}
	inline bool SetData(DATAKEY set_key, LPMYDATA pData)
	{
		if (m_pDataArray[set_key])return false;
		m_pDataArray[set_key] = pData;
		return true;
	}
	inline bool GetData(DATAKEY key, LPMYDATA* pOut)
	{
		if (m_pDataArray[key])
		{
			*pOut = m_pDataArray[key];
			return true;
		}
		return false;
	}
	inline LPMYDATA GetData(DATAKEY key)
	{
		return m_pDataArray[key];
	}
};

#endif