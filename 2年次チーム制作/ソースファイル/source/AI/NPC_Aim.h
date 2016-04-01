#ifndef __NPC_AIM_H__
#define __NPC_AIM_H__

class NPC_Brain;
class Character;
class NPC_FuzzyTool;

struct FuzzySetVal
{
	float left;
	float midlle;
	float right;
	FuzzySetVal(const float v) : left(v), midlle(v), right(v){}
};

struct Desirable
{
	FuzzySetVal	m_not_Desirable;	//�]�܂����Ȃ�
	FuzzySetVal	m_desirable;			//�̂��܂���
	FuzzySetVal	m_very_Desirable;	//���Ȃ�]�܂���
	Desirable() : m_not_Desirable(.0f), m_desirable(.0f), m_very_Desirable(.0f)
	{}
};

class NPC_Aim
{
protected :
	int						m_progress_Frame;//�o�ߎ���
	float						m_max_AimSpeed;
	int						m_update_Interval;	//Aim�̍X�V�Ԋu
	Vector3				m_current_AimPos;	//���݂�Aim�^�[�Q�b�g

protected :
	virtual void	Aim(NPC_Brain* brain, Character* character) = 0;
public :
	NPC_Aim();
	virtual ~NPC_Aim(){}
	void	Update(NPC_Brain* brain, Character* character);
	void	Activate();

};


class NPC_Aim_Weak : public NPC_Aim
{
private :
private:
	static const char*	m_fuzzy_key_Dist;
	static const char* m_fuzzy_key_speed;	//�������̑��ΓI�ȑ��x�̑傫��
	static const char* m_fuzzy_key_DesirableTriangle;		//�ǂꂭ�炢�C�����ׂ���(��,��,��,��)

private :
	FuzzyModule*		m_aim_Fuzzy;
	Desirable				m_pos_Angle;	
	Desirable				m_target_Velocity;
	Desirable				m_desirable_TriangleParam;

private :
	void	LoadParam(char* filename);
	void	Initialize_FuzzyTool();

	Vector3 Calculate_AimPos(NPC_Brain* brain, Character* character, const Vector3& target_Pos);
	void	Aim(NPC_Brain* brain, Character* character);

public :
	NPC_Aim_Weak(char* param_Filename);
	~NPC_Aim_Weak();
	void	Exit();
};

class NPC_Aim_Normal : public NPC_Aim
{
private :
	void	Aim(NPC_Brain* brain, Character* character);
	Vector3 Calculate_AimPos(NPC_Brain* brain, Character* character, 
											const Vector3& target_Pos, const Vector3& target_Velocity);
public :
	NPC_Aim_Normal(char* param_Filename);
};


class NPC_Aim_Strong : public NPC_Aim
{
private:
	void	Aim(NPC_Brain* brain, Character* character);
	Vector3 Calculate_AimPos(NPC_Brain* brain, Character* character,
		const Vector3& target_Pos, const Vector3& target_Velocity);
public:
	NPC_Aim_Strong(char* param_Filename);
};

#endif
