#include "TimeRender.h"

TimeRender::TimeRender(
	LPIEX2DOBJ	pNumberTex,
	LPIEX2DOBJ	pClockTex,
	bool		TexDelete) :
m_Max_time(1000),
m_Time_count(0),
m_pNumberTex(pNumberTex),
m_TexDelete(TexDelete),
m_pClockTex(pClockTex),
GameBaseEntity(game_id::id_timer)
{

}

TimeRender::~TimeRender()
{
	if (m_TexDelete)
	{
		delete m_pNumberTex;
		delete m_pClockTex;
	}
}

void TimeRender::RenderWaku(const TextureRenderRect& trr, COLOR color)
{
	TextureRenderRect ltrr = trr;

	ltrr.render_rect.width = (int)((float)ltrr.render_rect.width*1.30f);
	ltrr.render_rect.height = (int)((float)ltrr.render_rect.height*1.65f);

	ltrr.render_rect.left -= (ltrr.render_rect.width - trr.render_rect.width) / 2;
	ltrr.render_rect.top -= (ltrr.render_rect.height - trr.render_rect.height) / 2;

	ltrr.render_rect.left -= trr.render_rect.width / 16;

	m_pClockTex->Render(
		ltrr,
		RS_COPY,
		color
		);
}

int	 TimeRender::GetRemainingFrame()const
{
	return m_Time_count;
}

void TimeRender::Reset()
{
	m_Time_count = m_Max_time;
}

void TimeRender::SetTime(int frame)
{
	m_Time_count = frame;
}

void TimeRender::SetMaxTime(int frame)
{
	m_Max_time = frame;
}

void TimeRender::Update(int speed)
{
	m_Time_count -= speed;
	if (m_Time_count < 0)m_Time_count = 0;
}

void TimeRender::Render(CRTextureRect rc,COLOR color)
{
	const int number_size = rc.width / 5;
	const int sellsize = 64;

	const int time[2] = 
	{
		(m_Time_count / 3600) % 60, 
		(m_Time_count / 60) % 60,
	};

	TextureRenderRect rpara;

	rpara.render_rect = rc;
	rpara.read_tex_rect.left = 0;
	rpara.read_tex_rect.top = 0;
	rpara.read_tex_rect.width = 512;
	rpara.read_tex_rect.height = 256;

	RenderWaku(rpara,color);

	
	rpara.render_rect.width = number_size * 15 / 20;

	rpara.read_tex_rect.width = 64;
	rpara.read_tex_rect.height = 64;
	rpara.read_tex_rect.top = 0;

	rpara.read_tex_rect.left = sellsize*(10);
	rpara.render_rect.left += rc.width / 2 - number_size * 15 / 20;
	m_pNumberTex->Render(rpara);

	rpara.render_rect.left = rc.left;

	for (int i = 0; i < 2; i++)
	{
		rpara.read_tex_rect.left = sellsize*(time[i] / 10);
		m_pNumberTex->Render(rpara);

		rpara.render_rect.left += number_size;

		rpara.read_tex_rect.left = sellsize*(time[i] % 10);
		m_pNumberTex->Render(rpara);

		rpara.render_rect.left += (int)((float)number_size*1.5f);
	}

}


bool TimeRender::HandleMessage(LPGameMessage msg)
{
	switch (msg->type)
	{
	case msg_t::timer_render_msg:
		TimerRenderMsgInfo* info = (TimerRenderMsgInfo*)msg->exinfo;
		Render(info->tex_rc, info->color);
		return true;
	}

	return false;
}