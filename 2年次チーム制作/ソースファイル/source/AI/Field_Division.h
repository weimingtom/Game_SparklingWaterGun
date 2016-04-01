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
	Cell<T>*							registar_Cell;		//�o�^����Ă�����
	T*								client_Object;	//�I�u�W�F�N�g�ւ̃|�C���^
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
	*@brief ��Ԃɓo�^
	*/
	bool	Registar_Field(float pos_X, float pos_Z, FieldDivision_Manager<T>* field)
	{
		//�o�^���Ă����Ԃ�����Γo�^���O��
		if (registar_Cell)
			Remove_Cell();

		//������Ԃ�ݒ肷��
		registar_Cell = field->Get_Cell(pos_X, pos_Z);
		if (!registar_Cell)
		{
			MessageBoxPlus("������Ԃ�����ł��܂���ł��� posX %f posZ %f", pos_X, pos_Z);
			return false;
		}
		if (!registar_Cell->Add_Object(this))
		{
			MessageBoxPlus("��ԂɃI�u�W�F�N�g��o�^�ł��܂���", pos_X, pos_Z);
			return false;
		}
		return true;
	}
	/**
	*@brief ���ݏ������Ă����Ԃ���؂藣��
	*/
	bool	Remove_Cell()
	{
		//�o�^����Ă��Ȃ��ꍇ�͂��̂܂�return
		if (!registar_Cell)
			return false;
		//���g���擪�������ꍇ
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

		//���g���Ōゾ�����ꍇ
		if (next_Object == nullptr)
		{
			before_Object->next_Object = nullptr;
			before_Object = nullptr;
			return true;
		}

		//����ȊO
		before_Object->next_Object = next_Object;
		next_Object->before_Object = before_Object;
		next_Object = nullptr;
		before_Object = nullptr;
		return true;
	}

};

//��ԃN���X
template<class T>
class Cell
{
	friend class Tree_Object<T>;
protected :
	Tree_Object<T>* object_List;	//�c���[�̐擪�|�C���^(���X�g�\��)

public :
	Cell() : object_List(nullptr){}
	//void	Reset_Link()
	bool	Add_Object(Tree_Object<T>* add_Object)
	{
		if (add_Object == nullptr) return false;
		//��d�o�^�͔�����
		//if (add_Object->registar_Cell == this)
			//return false;
		//�V�K�o�^
		if (object_List == nullptr)
		{
			object_List = add_Object;
			//add_Object->registar_Cell = this;
			return true;
		}
		//�I�u�W�F�N�g���X�g���X�V(�擪�ɑ}��)
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
			console::Print("object_List����ł���\n");
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

	//��Ԃ̃T�C�Y�擾
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
	Cell<T>*		 cell_Array;	//��Ԕz��
	
	Vector3	center_Pos;		//��Ԃ̒��S���W
	float		field_Size;		//��ԗ̈�̑傫���i���[�g��ԁj
	float		min_Cellsize;		//�ŏ���ԃT�C�Y
	int			cell_Deep;				//4^deep
	DWORD num_Cell;				//��Ԑ�

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
		//�������W�������Ƃ��������Ȃ�̂Ő��̐��Ɏ����Ă���
		x -= center_Pos.x;
		z -= center_Pos.z;

		float work = field_Size / 2;
		x += work;
		z += work;

		z = field_Size - z;

		//�ŏ���ԒP�ʂŊ���
		x = x / min_Cellsize;
		z = z / min_Cellsize;
		
		return(SeparateBit(DWORD(x)) | (SeparateBit(DWORD(z)) << 1));
	}

	//�r�b�g����
	DWORD SeparateBit(DWORD in)
	{
		in = (in | (in << 8)) & 0x00FF00FF;
		in = (in | (in << 4)) & 0x0F0F0F0F;
		in = (in | (in << 2)) & 0x33333333;
		in = (in | (in << 1)) & 0x55555555;
		return in;
	}

	/**
	*@brief �w�肵�����W�̋�Ԃ𓾂�
	*@param x��z���W
	*/
	Cell<T>* Get_Cell(float x, float z)
	{
		DWORD morton = Get_MotonNumber(x, z);
		if (morton > num_Cell)
		{
			MessageBoxPlus("���[�g���ԍ����������� morton%d ,num_Cell %d", morton, num_Cell);
			return nullptr;
		}
		return &cell_Array[morton];
	}
	template<typename C>
	/**
	*@brief	�w�肵�����W�ɋ߂��I�u�W�F�N�g��Ԃ�
	`@param1�E2 x���W��z���W
	*@param5 �ԋp����R���e�i�ւ̃|�C���^(���͕K��vector�ŁA�m�[�h�̃^�C�v�����낦��)
	*@param	true�ɂ���ƈ������W�����������Ԃ͔�΂�
	*@return	�T��������true
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
	*@brief	�w�肵�����W�̋�ԃZ���̃I�u�W�F�N�g��Ԃ�
	`@param1�E2 x���W��z���W
	*@param3 ���������������̃|�C���^(���K�����邱��)
	*@param4 ��������������
	*@param5 �ԋp����R���e�i�ւ̃|�C���^(���͕K��vector�ŁA�m�[�h�̃^�C�v�����낦��)
	*@param	true�ɂ���ƈ������W�����������Ԃ͔�΂�
	*@return	�T��������true
	*@note		�S���ʂ�T���������ꍇ��search_Vec��nullptr�ɂ���
	*@note		dist��0��param1.2�̍��W����������m�[�h�݂̂�ԋp����(���p�͂Ȃ�ł�OK)
	*/
	bool	Search_Object( float pos_X, float pos_Z,const Vector3* search_Vec,
											const float& search_Dist, C* container,bool skip_F = false )
	{
		//�S�����T���̏ꍇ
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
