#ifndef __COLA_BULLET_H__
#define __COLA_BULLET_H__

#include "iextreme.h"
#include "GameSystem\GameBase.h"

//**************************************************************
//	�Q�[����ɑ��݂���e�̃x�[�X�N���X
//**************************************************************
class BulletBase
{
public:
	virtual ~BulletBase(){}
	virtual bool isDelete() = 0;	//�������邩�ǂ���
	virtual void Update() = 0;		//�X�V
	virtual void Render() = 0;		//�`��
};

typedef BulletBase* LPBULLETBASE;

//**************************************************************
//	�e�}�l�[�W���N���X
//**************************************************************

//�e�Ǘ��ő吔
static const int BULLETMAX = 600;


class BulletManager:public GameBaseEntity
{
private:
	int				m_BulletSetCount;			//�e���Z�b�g����J�E���g
	LPBULLETBASE	m_pBulletArray[BULLETMAX];	//�e�̃|�C���^�f�[�^
public:
	BulletManager();
	~BulletManager();
public:
	bool HandleMessage(LPGameMessage msg);		//���b�Z�[�W��M
	bool AddBullet(LPBULLETBASE bullet);		//�e���}�l�[�W���ɃZ�b�g(new�����|�C���^��n������)
	void Update();	
	void Render();
};

#endif