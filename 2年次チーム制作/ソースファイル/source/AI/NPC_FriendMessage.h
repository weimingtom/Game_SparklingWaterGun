#pragma once

class NPC_Brain;

class NPCFriendMessage
{
	static const int m_message_interval = 180;
	static const int	m_responce_Time = 45;
	struct
	{
		bool				active;							//(message_DelayFrame >0 || wait_Frame >0 )? true : false
		int				message_DelayFrame;	//���b�Z�[�W��\������܂ł̃t���[����
		int				wait_Frame;					//���b�Z�[�W�\����̑ҋ@�t���[��
		int				message_Type;
	} m_param;

private :
	NPC_Brain* m_holder;

public :
	NPCFriendMessage(NPC_Brain* holder);
	~NPCFriendMessage();
	
public :
	void	Avtive(const int message_type);
	void	Update(Character*character);
};