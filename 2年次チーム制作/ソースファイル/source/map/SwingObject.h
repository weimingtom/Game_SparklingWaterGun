#ifndef __SWING_OBJECT_H__
#define __SWING_OBJECT_H__

#include "iextreme.h"
#include <array>

#include "MyFunction.h"

// �h����̃N���X
template<size_t NumObjects>
class SwingObject
{
public:
	enum{ num_objects = NumObjects };
private:
	Vector3 original_position_;                                 // ���_(�x�_)
	Vector3 resultant_;                                             // �^����ꂽ�e����
	std::array<Vector3, num_objects> positions_; // �h����̂̍��W
	std::array<Vector3, num_objects> velocities_; // �h����̂̑��x
	std::array<float, num_objects> distances_;      // �h����̓��m�̋���
	float coefficient_of_drag_;                                  // ��R��
public:
	SwingObject() :
		original_position_(0, 0, 0),
		resultant_(0, 0, 0), 
		positions_(),
		velocities_(),
		distances_(),
		coefficient_of_drag_(0.0f)
	{
		positions_.fill(original_position_);
		velocities_.fill(original_position_);
		distances_.fill(0.0f);
	}

	~SwingObject()
	{

	}
	
public:
	void Set_OriginalPosition(const Vector3& pos)
	{
		original_position_ = pos;
	}

	void Set_ObjectPosition(size_t n, const Vector3 &pos)
	{
		if (n < 0 || n >= num_objects)return;

		if (n == 0)
		{
			positions_[0] = pos;
			distances_[0] = (positions_[0] - original_position_).Length();
		}
		else
		{
			positions_[n] = pos;
			distances_[n] = (positions_[n] - positions_[n - 1]).Length();
		}
	}

	void AddForce(const Vector3 &force)
	{
		resultant_ += force;
	}

	void Set_CoefficientOfDrag(const float drag/*= 0.0f ~ 1.0f*/)
	{
		coefficient_of_drag_ = drag;
	}

	void Update()
	{
		Vector3 prev_pos;
		Vector3 from_parent_vec;
		float distance;
		for (size_t i = 0; i < num_objects; ++i)
		{// ���W�ۑ�
			prev_pos = positions_[i];

			// ���x�E���W�X�V
			velocities_[i] += resultant_;
			positions_[i] += velocities_[i];

			// ��R�͍�p
			velocities_[i] -= velocities_[i] * coefficient_of_drag_;

			// ���W�␳
			if (i == 0)
			{
				from_parent_vec = positions_[i] - original_position_;
			}
			else
			{
				from_parent_vec = positions_[i] - positions_[i - 1];
			}

			distance = from_parent_vec.Length();

			if (distance > distances_[i])
			{
				from_parent_vec.Normalize();

				if (i == 0)
				{
					positions_[i] = original_position_ + from_parent_vec*distances_[i];
				}
				else
				{
					positions_[i] = positions_[i - 1] + from_parent_vec*distances_[i];

					// �e�֕␳�l�̉e����^����
					float revision_distance = distance - distances_[i];
					velocities_[i - 1] += from_parent_vec*revision_distance;
				}
				// ���x�Z�o
				velocities_[i] = positions_[i] - prev_pos;
			}
		}
		resultant_ = Vector3(0, 0, 0);
	}

	Vector3 Get_OriginalPosition()const
	{
		return original_position_;
	}

	Vector3 Get_Position(size_t n)const
	{
		if (n < 0 || n >= num_objects)return Vector3(0, 0, 0);
		return positions_[n];
	}

};

// �v�f�����P�̂Ƃ�
template<>
class SwingObject<1>
{
public:
	enum{ num_objects = 1 };
private:
	Vector3 original_position_;
	Vector3 resultant_;

	Vector3 position_;
	Vector3 velocity_;
	float distance_;

	float coefficient_of_drag_;

public:
	SwingObject() :
		original_position_(0, 0, 0),
		resultant_(0, 0, 0),
		position_(0, 0, 0),
		velocity_(0, 0, 0),
		distance_(0.0f),
		coefficient_of_drag_(0.0f)
	{
	}

	~SwingObject()
	{
	}

public:
	void Set_OriginalPosition(const Vector3& pos)
	{
		original_position_ = pos;
	}

	void Set_ObjectPosition(const Vector3 &pos)
	{
		position_ = pos;
		distance_ = (position_ - original_position_).Length();
	}

	void AddForce(const Vector3 &force)
	{
		resultant_ += force;
	}

	void Set_CoefficientOfDrag(const float drag/*= 0.0f ~ 1.0f*/)
	{
		coefficient_of_drag_ = drag;
	}

	void Update()
	{
		Vector3 prev_pos;
		Vector3 from_parent_vec;
		float dist;

		{// ���W�ۑ�
			prev_pos = position_;

			// ��R�͍�p
			velocity_ -= velocity_ * coefficient_of_drag_;

			// ���x�E���W�X�V
			velocity_ += resultant_;
			position_ += velocity_;

			// ���W�␳
			from_parent_vec = position_ - original_position_;

			dist = from_parent_vec.Length();

			if (dist > distance_)
			{
				from_parent_vec.Normalize();
				position_ = original_position_ + from_parent_vec*distance_;
			}
			// ���x�Z�o
			velocity_ = position_ - prev_pos;
		}
		resultant_ = Vector3(0, 0, 0);
	}

	Vector3 Get_OriginalPosition()const
	{
		return original_position_;
	}

	Vector3 Get_ObjectPosition()const
	{
		return position_;
	}

};

// �v�f�����O�̂Ƃ�
template<>
class SwingObject<0>
{
	const size_t num_objects_;

	Vector3 original_position_;
	Vector3 resultant_;
	Vector3 *positions_;
	Vector3 *velocities_;
	float *distances_;

	float coefficient_of_drag_;
public:
	SwingObject(size_t num_objects) :
		num_objects_(num_objects),
		original_position_(0, 0, 0),
		resultant_(0, 0, 0),
		positions_(),
		velocities_(),
		distances_(),
		coefficient_of_drag_(0.0f)
	{
		positions_ = new Vector3[num_objects_];
		velocities_ = new Vector3[num_objects_];
		distances_ = new float[num_objects_];
	}

	~SwingObject()
	{
		safe_array_delete(positions_);
		safe_array_delete(velocities_);
		safe_array_delete(distances_);
	}

public:
	void Set_OriginalPosition(const Vector3& pos)
	{
		original_position_ = pos;
	}

	void AddForce(const Vector3 &force)
	{
		resultant_ += force;
	}

	void Set_CoefficientOfDrag(const float drag/*= 0.0f ~ 1.0f*/)
	{
		coefficient_of_drag_ = drag;
	}

	void Update()
	{
		Vector3 prev_pos;
		Vector3 from_parent_vec;
		float distance;
		for (size_t i = 0; i < num_objects_; ++i)
		{// ���W�ۑ�
			prev_pos = positions_[i];

			// ���x�E���W�X�V
			velocities_[i] += resultant_;
			positions_[i] += velocities_[i];

			// ��R�͍�p
			velocities_[i] -= velocities_[i]*coefficient_of_drag_;

			// ���W�␳
			if (i == 0)
			{
				from_parent_vec = positions_[i] - original_position_;
			}
			else
			{
				from_parent_vec = positions_[i] - positions_[i - 1];
			}

			distance = from_parent_vec.Length();

			if (distance > distances_[i])
			{
				from_parent_vec.Normalize();

				if (i == 0)
				{
					positions_[i] = original_position_ + from_parent_vec*distances_[i];
				}
				else
				{
					positions_[i] = positions_[i - 1] + from_parent_vec*distances_[i];

					// �e�֕␳�l�̉e����^����
					float revision_distance = distance - distances_[i];
					velocities_[i - 1] += from_parent_vec*revision_distance;
				}
				// ���x�Z�o
				velocities_[i] = positions_[i] - prev_pos;
			}
		}
		resultant_ = Vector3(0, 0, 0);
	}

	Vector3 Get_OriginalPosition()const
	{
		return original_position_;
	}

	Vector3 Get_Position(size_t n)const
	{
		if (n < 0 || n >= num_objects_)return Vector3(0, 0, 0);
		return positions_[n];
	}
};

#endif // __SWING_OBJECT_H__

// EOF