#ifndef __PP_SPATIAL_PARTITIONNING_H__
#define __PP_SPATIAL_PARTITIONNING_H__

#include "iextreme.h"
#include <vector>

//************************************************************
//	空間分割ヘッダ
//************************************************************


//型変換たち
typedef Vector3				sp_vector;
typedef const sp_vector*	lpc_sp_vector;

class sp_cell;
class sp_field;
class sp_object;

typedef sp_cell		*lp_sp_cell;
typedef sp_field	*lp_sp_field;
typedef sp_object	*lp_sp_object;
typedef void		*lp_client;	//ホントはテンプレートが良かった…


//************************************************************
//	空間に登録するオブジェクトクラス
//************************************************************
class sp_object
{
	friend class sp_cell;
	friend class sp_field;
private:
	lp_sp_field		m_pMyfield;				//フィールドへのポインタ
	lp_sp_cell		m_pMycell;				//自分の空間へのポインタ
	lp_sp_object	m_pNext, m_pPre;		//自分の次と自分の前のオブジェクトへのポインタ
	lp_client		m_client;				//このクラスのクライアントへのポインタ
public:
	inline sp_object(lp_client client_ptr);	//登録する物をvoid*として引数にとる
	inline ~sp_object();					//デストラクタで自動的に空間から切り離される
public:
	inline bool isEntry()const;
	inline void EntryToField(lp_sp_field pField, lpc_sp_vector position_ptr);	//フィールドクラスに自分を登録する
	inline void PositionUpdate(lpc_sp_vector new_position_ptr);					//登録する空間を更新する(先にEntryToField関数を実行しておかないといけない)
	inline void ExitCell();														//現在登録されている空間から切り離す
};

//************************************************************
//	空間クラス
//************************************************************
class sp_cell
{
	friend class sp_object;
	friend class sp_field;
private:
	lp_sp_object m_pObjectlist;	//登録しているオブジェクトのリスト
public:
	inline sp_cell();
	inline ~sp_cell();
public:
	inline void add_object(lp_sp_object add_obj);	//空間に登録する
};

//************************************************************
//	空間をひとまとめにしたフィールドクラス
//************************************************************
class sp_field
{
	friend class sp_object;
	friend class sp_cell;
public:
	typedef std::vector<lp_client> OBJVECTOR;			//オブジェクト配列(listかvectorか… 悩むところ…)
private:
	lp_sp_cell const	m_pCell_array;					//空間の配列
	const sp_vector		m_field_center;					//フィールド中央の座標(ｙ座標いらないような…)
	const float			m_field_size, m_field_size_2;	//フィールドサイズ
	const float			m_cell_size, m_cell_size_2;		//空間一つあたりのサイズ
	const int			m_deep;							//分割の深さ
	const DWORD			m_cell_max;						//空間数
public:
	inline sp_field(
		lpc_sp_vector	pFieldCenter,	//フィールド中央の位置
		float			field_size,		//フィールドサイズ
		int				deep			//分割の深さ(4のdeep乗の個数の空間ができる)
		);
	inline ~sp_field();
public:
	inline DWORD	SeparateBit(DWORD n);					//DWORD型のビットを一つ一つ0で区切る
	inline DWORD	GetMortonNumber(float x, float y);		//座標からこのフィールド上での空間番号を得る
public:
	inline lp_sp_cell	GetPositionCell(float x,float z);	//座標から空間へのポインタを得る
	inline OBJVECTOR	SearchObject(lpc_sp_vector pos,float size);	//指定した場所からsize以内(だいたい)の登録されているオブジェクトを得る
};





//************************************************************
//	ここから下は↑の関数の中身
//************************************************************

inline sp_object::sp_object(lp_client client_ptr) :
m_pMyfield(nullptr),
m_pMycell(nullptr),
m_pNext(nullptr),
m_pPre(nullptr),
m_client(client_ptr)
{
}

inline sp_object::~sp_object()
{
	ExitCell();
}

inline bool sp_object::isEntry()const
{
	return (m_pMycell != nullptr);
}

inline void sp_object::EntryToField(
	lp_sp_field pField,
	lpc_sp_vector position)
{
	ExitCell();
	m_pMyfield = pField;
	m_pMycell = m_pMyfield->GetPositionCell(position->x, position->z);
	if (m_pMycell)m_pMycell->add_object(this);
}

inline void sp_object::PositionUpdate(lpc_sp_vector new_position_ptr)
{
	if (!m_pMyfield)return;
	lp_sp_cell new_cell = m_pMyfield->GetPositionCell(new_position_ptr->x, new_position_ptr->z);

	if(new_cell != m_pMycell)
	{
		ExitCell();
		m_pMycell = new_cell;
		if (m_pMycell)
			m_pMycell->add_object(this);
	}
}

inline void sp_object::ExitCell()
{
	if (!m_pMycell)	//登録されていなかった場合はnullにしてreturn
	{
		m_pNext = m_pPre = nullptr;
		m_pMycell = nullptr;
		return;
	}

	if (this == m_pMycell->m_pObjectlist)	//自分がリスト先頭だった場合
	{
		if (m_pNext)	//次があれば
		{
			m_pMycell->m_pObjectlist = m_pNext;	//リストの先頭を自分の次に
			m_pNext->m_pPre = nullptr;
		}
		else
			m_pMycell->m_pObjectlist = nullptr;
		m_pNext = m_pPre = nullptr;
		m_pMycell = nullptr;
		return;
	}

	if (m_pPre)	//自分の前があるなら
	{
		if (m_pNext)	//自分の次があるなら自分の前と連結
		{
			m_pPre->m_pNext = m_pNext;
		}
		else{
			m_pPre->m_pNext = nullptr;//ないなら自分が終端なので前の次をnullに
		}
	}

	if (m_pNext)	//自分の次があるなら
	{
		if (m_pPre)	//前があるなら次のやつの前に設定
		{
			m_pNext->m_pPre = m_pPre;
		}
		else{
			m_pNext->m_pPre
				= 0;//前がないなら自分がリストの初めなので自分の次を先頭に
		}
	}
	m_pNext = m_pPre = nullptr;
	m_pMycell = nullptr;
}


inline sp_cell::sp_cell() :
m_pObjectlist(nullptr)
{

}

inline sp_cell::~sp_cell()
{
	lp_sp_object pNext;
	while (m_pObjectlist)
	{
		if (m_pObjectlist->m_pNext)pNext = m_pObjectlist->m_pNext;
		else pNext = nullptr;

		m_pObjectlist->ExitCell();
		m_pObjectlist = pNext;
	}
}

inline void sp_cell::add_object(lp_sp_object add_obj)
{
	if (m_pObjectlist)
	{
		m_pObjectlist->m_pPre = add_obj;
	}
	add_obj->m_pNext = m_pObjectlist;
	add_obj->m_pPre=nullptr;
	m_pObjectlist = add_obj;
}

inline sp_field::sp_field(
	lpc_sp_vector	pFieldCenter,
	float			field_size,
	int				deep
	):
	m_field_center					(*pFieldCenter),
	m_field_size					(field_size),
	m_field_size_2					(field_size/2),
	m_deep							(deep),
	m_cell_max						((DWORD)pow(4, (double)deep)),
	m_pCell_array					(new sp_cell[(DWORD)pow(4, (double)deep)]),
	m_cell_size						(m_field_size/powf(2.0f,(float)deep)),
	m_cell_size_2					(m_cell_size/2)
{
#ifdef _DEBUG
	char t[1000];
	sprintf_s<1000>(t,
		"\n空間分割情報 \n フィールドサイズ= %f \n 空間数 = %d \n 空間一つあたりのサイズ = %f \n",
		m_field_size, (int)m_cell_max, m_cell_size
		);
	OutputDebugString(t);
#endif
}

inline sp_field::~sp_field()
{
	delete []m_pCell_array;
}

inline DWORD	sp_field::SeparateBit(DWORD n)
{
	n = (n | (n << 8)) & 0x00FF00FF;
	n = (n | (n << 4)) & 0x0F0F0F0F;
	n = (n | (n << 2)) & 0x33333333;
	n = (n | (n << 1)) & 0x55555555;

	return n;
}

inline DWORD	sp_field::GetMortonNumber(float x, float z)
{
	x -= m_field_center.x;
	z -= m_field_center.z;

	x += m_field_size_2;
	z += m_field_size_2;

	z = m_field_size - z;

	x /= m_cell_size;
	z /= m_cell_size;

	return (SeparateBit(DWORD(x)) | (SeparateBit(DWORD(z)) << 1));

}

inline lp_sp_cell	sp_field::GetPositionCell(float x,float z)
{
	DWORD morton = GetMortonNumber(x, z);
	if (morton < m_cell_max)return &m_pCell_array[morton];
	return nullptr;
}

inline sp_field::OBJVECTOR	sp_field::SearchObject(lpc_sp_vector pos, float size)
{
	static const float search_pos_vec[4][2]=
	{
		{-1,1},
		{1,1},
		{1,-1},
		{-1,-1}
	};
	const float search_vec[4][2]=
	{
		{ m_cell_size, 0 },
		{ 0, -m_cell_size },
		{ -m_cell_size, 0 },
		{ 0, m_cell_size }
	};
	OBJVECTOR ret;
	ret.reserve(10);
	float	search_x, search_z;
	float	now_search_len = m_cell_size_2;
	int		now_cell_num = 1;
	const int search_lvl = 1 + (int)(size / m_cell_size);
	lp_sp_object	temp=nullptr;
	lp_sp_cell		search_cell;

	for (int lvl_cnt = 0; lvl_cnt < search_lvl; lvl_cnt++)
	{
		for (int i = 0; i < 4; i++)
		{
			search_x = pos->x + search_pos_vec[i][0] * now_search_len;
			search_z = pos->z + search_pos_vec[i][1] * now_search_len;
			
			for (int j = 0; j < now_cell_num; j++)
			{
				search_cell = GetPositionCell(search_x, search_z);
				if (search_cell)temp = search_cell->m_pObjectlist;

				while (temp)
				{
					ret.push_back(temp->m_client);
					temp = temp->m_pNext;
				}
				search_x += search_vec[i][0];
				search_z += search_vec[i][1];
			}
		}
		now_cell_num += 2;
		now_search_len += m_cell_size;
	}
	return ret;
}

#endif