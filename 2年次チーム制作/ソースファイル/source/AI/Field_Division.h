#ifndef __FIELD_DIVISION_H__
#define __FIELD_DIVISION_H__
#include	"iextreme.h"
#include	<vector>
#include	<math.h>

template<class T>
class Cell;
template<class T>
class  FieldDivision_Manager;

template<class T>
class Tree_Object 
{
	friend class Cell<T>;
public :
	Cell<T>*							registar_Cell;		//登録されている空間
	T*								client_Object;	//オブジェクトへのポインタ
	Tree_Object<T>*		before_Object;
	Tree_Object<T>*		next_Object;
public :
	Tree_Object(T* object)
	{
		registar_Cell = nullptr;
		before_Object = nullptr;
		next_Object = nullptr;
		client_Object = object; 
	}
	/**
	*@brief 空間に登録
	*/
	bool	Registar_Field(float pos_X, float pos_Z, FieldDivision_Manager<T>* field)
	{
		//登録している空間があれば登録を外す
		if (registar_Cell)
			Remove_Cell();

		//所属空間を設定する
		registar_Cell = field->Get_Cell(pos_X, pos_Z);
		if (!registar_Cell)
		{
			MessageBoxPlus("所属空間が特定できませんでした posX %f posZ %f", pos_X, pos_Z);
			return false;
		}
		if (!registar_Cell->Add_Object(this))
		{
			MessageBoxPlus("空間にオブジェクトを登録できません", pos_X, pos_Z);
			return false;
		}
		return true;
	}
	/**
	*@brief 現在所属している空間から切り離す
	*/
	bool	Remove_Cell()
	{
		//登録されていない場合はそのままreturn
		if (!registar_Cell)
			return false;
		//自身が先頭だった場合
		if (this == registar_Cell->object_List)
		{
			if (next_Object)
			{
				registar_Cell->object_List = next_Object;
				next_Object = nullptr;
				before_Object = nullptr;
				return true;
			}
			else
			{
				registar_Cell->object_List = nullptr;
				before_Object = nullptr;
				return true;
			}
		}

		//自身が最後だった場合
		if (next_Object == nullptr)
		{
			before_Object->next_Object = nullptr;
			before_Object = nullptr;
			return true;
		}

		//それ以外
		before_Object->next_Object = next_Object;
		next_Object->before_Object = before_Object;
		next_Object = nullptr;
		before_Object = nullptr;
		return true;
	}

};

//空間クラス
template<class T>
class Cell
{
	friend class Tree_Object<T>;
protected :
	Tree_Object<T>* object_List;	//ツリーの先頭ポインタ(リスト構造)

public :
	Cell() : object_List(nullptr){}
	//void	Reset_Link()
	bool	Add_Object(Tree_Object<T>* add_Object)
	{
		if (add_Object == nullptr) return false;
		//二重登録は避ける
		//if (add_Object->registar_Cell == this)
			//return false;
		//新規登録
		if (object_List == nullptr)
		{
			object_List = add_Object;
			//add_Object->registar_Cell = this;
			return true;
		}
		//オブジェクトリストを更新(先頭に挿入)
		add_Object->next_Object = object_List;
		object_List->before_Object = add_Object;
		object_List = add_Object;
		//add_Object->registar_Cell = this;
		return true;
	}
	Tree_Object<T>* Get_First()	{ return object_List; }
	template<class C>
	bool	Get_AllObject(C*  ret_List)
	{
		if (object_List == nullptr)
		{
			console::Print("object_Listが空でした\n");
			return false;
		}

		Tree_Object<T>* work = object_List;
		for (; work != nullptr;)
		{
			ret_List->push_back(work->client_Object);
			work = work->next_Object;
		}
		return true;
	}

	//空間のサイズ取得
	int	Size()
	{
		int ret = 0;
		Tree_Object<T>* work = object_List;

		for (; work != nullptr;)
		{
			ret++;
			work = work->next_Object;
		}
		return ret;
	}
};

template<class T>
class  FieldDivision_Manager
{
protected:
	Cell<T>*		 cell_Array;	//空間配列
	
	Vector3	center_Pos;		//空間の中心座標
	float		field_Size;		//空間領域の大きさ（ルート空間）
	float		min_Cellsize;		//最少空間サイズ
	int			cell_Deep;				//4^deep
	DWORD num_Cell;				//空間数

protected :
	template<typename C>
	bool	SearchObject_AllDirection(const float& pos_X, const float& pos_Z,
							const float& search_Dist, C* container)
	{
		static Vector3 search_Vec[4] =
		{
			Vector3(sinf(Direction::north),.0f,cosf(Direction::north)),
			Vector3 (sinf(Direction::east), .0f, cosf(Direction::east) ),
			Vector3(sinf(Direction::south), .0f, cosf(Direction::south)),
			Vector3(sinf(Direction::west), .0f, cosf(Direction::west)),
		};
		const int search_Level = 1 + (int)(search_Dist / min_Cellsize);
		for (int i = 0; i < search_Level; i++)
		{
			for (int direction = 0; direction < 4; direction++)
			{
				float work_X = pos_X + search_Vec[direction].x * (min_Cellsize * i);
				float work_Z = pos_Z + search_Vec[direction].z * (min_Cellsize * i);
				Cell<T>* work_cell = Get_Cell(work_X, work_Z);
				work_cell->Get_AllObject(container);
				if (i == 0)
					break;
			}
		}
		return true;
	}


public :
	FieldDivision_Manager()
	{
		field_Size = .0f;
		min_Cellsize = .0f;
		cell_Deep = 0;
		num_Cell = 0;
		cell_Array = nullptr;
	}
	virtual ~FieldDivision_Manager()
	{
		if (cell_Array)
		{
			delete[] cell_Array;
			cell_Array = nullptr;
		}			
	}

	void	Initialize(const Vector3& center, const float& f_Size,
		const int& deep)
	{
		center_Pos = center;
		field_Size = f_Size;
		cell_Deep = deep;
		num_Cell = (DWORD)(pow(4.0, (double)(cell_Deep)));
		cell_Array = new Cell<T>[num_Cell];
		cell_Array[48];
		float work = pow(2.0f, (float)(cell_Deep)); 
		min_Cellsize = field_Size / work;
	}

	void	Release()
	{
		if (cell_Array)
		{
			delete[] cell_Array;
			cell_Array = nullptr;
		}
	}
	
	DWORD Get_MotonNumber(float x, float z)
	{
		//引数座標が負だとおかしくなるので正の数に持っていく
		x -= center_Pos.x;
		z -= center_Pos.z;

		float work = field_Size / 2;
		x += work;
		z += work;

		z = field_Size - z;

		//最少空間単位で割る
		x = x / min_Cellsize;
		z = z / min_Cellsize;
		
		return(SeparateBit(DWORD(x)) | (SeparateBit(DWORD(z)) << 1));
	}

	//ビット分割
	DWORD SeparateBit(DWORD in)
	{
		in = (in | (in << 8)) & 0x00FF00FF;
		in = (in | (in << 4)) & 0x0F0F0F0F;
		in = (in | (in << 2)) & 0x33333333;
		in = (in | (in << 1)) & 0x55555555;
		return in;
	}

	/**
	*@brief 指定した座標の空間を得る
	*@param xとz座標
	*/
	Cell<T>* Get_Cell(float x, float z)
	{
		DWORD morton = Get_MotonNumber(x, z);
		if (morton > num_Cell)
		{
			MessageBoxPlus("モートン番号がおかしい morton%d ,num_Cell %d", morton, num_Cell);
			return nullptr;
		}
		return &cell_Array[morton];
	}
	template<typename C>
	/**
	*@brief	指定した座標に近いオブジェクトを返す
	`@param1・2 x座標とz座標
	*@param5 返却するコンテナへのポインタ(今は必ずvectorで、ノードのタイプもそろえる)
	*@param	trueにすると引数座標が所属する空間は飛ばす
	*@return	探索成功でtrue
	*/
	bool Search_Object_Near(float pos_X, float pos_Z,
		 C* container,float search_Dist = 0)
	{
		static Vector2 search_cellVec[4] =
		{
			{ -1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, -1.0f, },
			{ -1.0f, -1.0f }
		};

		static Vector2 search_Vec[4] =
		{
			{ min_Cellsize, .0f, },
			{ .0f, min_Cellsize, },
			{ -min_Cellsize, .0f, },
			{ .0f, min_Cellsize, },
		};
		float search_pos_x, search_pos_z;
		float current_Length = min_Cellsize / 2.0f;
		int current_CellNum = 1;
		int search_level = 1 + (int)(search_Dist / min_Cellsize);
		Tree_Object<T>* work = nullptr;
		Cell<T>* search_Cell = nullptr;
		for (int cur_level = 0; cur_level < search_level; cur_level++)
		{
			for (int i = 0; i < 4; i++)
			{
				search_pos_x = pos_X + search_cellVec[i].x * current_Length;
				search_pos_z = pos_Z + search_cellVec[i].y * current_Length;

				for (int j = 0; j < current_CellNum; j++)
				{
					search_Cell = Get_Cell(search_pos_x, search_pos_z);
					if (search_Cell)
						work = search_Cell->Get_First();
					while (work)
					{
						container->push_back(work->client_Object);
						work = work->next_Object;
					}
					search_pos_x += search_Vec[i].x;
					search_pos_z += search_Vec[i].y;
				}
			}
			current_CellNum += 2;
			current_Length += min_Cellsize;
		}
		if (!container->empty())
			return true;
		return false;
	}

	template<typename C>
	/**
	*@brief	指定した座標の空間セルのオブジェクトを返す
	`@param1・2 x座標とz座標
	*@param3 検索したい方向のポインタ(正規化すること)
	*@param4 検索したい距離
	*@param5 返却するコンテナへのポインタ(今は必ずvectorで、ノードのタイプもそろえる)
	*@param	trueにすると引数座標が所属する空間は飛ばす
	*@return	探索成功でtrue
	*@note		全方位を探索したい場合はsearch_Vecをnullptrにする
	*@note		distが0でparam1.2の座標が所属するノードのみを返却する(方角はなんでもOK)
	*/
	bool	Search_Object( float pos_X, float pos_Z,const Vector3* search_Vec,
											const float& search_Dist, C* container,bool skip_F = false )
	{
		//全方向探索の場合
		if (!search_Vec)
		{
			if (search_Dist > .0f)
			{
				return SearchObject_AllDirection(pos_X, pos_Z, search_Dist, container);
			}
			Cell<T>* work_cell = Get_Cell(pos_X, pos_Z);
			return work_cell->Get_AllObject(container);
		}

		if (skip_F)
		{
			pos_X += search_Vec->x * min_Cellsize * 1.1f;
			pos_Z += search_Vec->z * min_Cellsize * 1.1f;
		}
	


		const int search_Level = 1 + (int)(search_Dist / min_Cellsize);

		for (int i = 0; i < search_Level; i++)
		{
			float work_X = pos_X + search_Vec->x * (min_Cellsize * i);
			float work_Z = pos_Z + search_Vec->z * (min_Cellsize * i);
			Cell<T>* work_cell = Get_Cell(work_X, work_Z);
			work_cell->Get_AllObject(container);
		}
		return true;
	}

	int	AllObjectSize()
	{
		int ret = 0;
		for (int i = 0; i < (int)num_Cell; i++)
		{
			ret += cell_Array[i].Size();
		}
		return ret;
	}

};

#endif
