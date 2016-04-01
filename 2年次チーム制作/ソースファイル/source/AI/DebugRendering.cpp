#include	"iextreme.h"
#include	"..\BillBoard.h"

//コーラ用
#include "Camera.h"
#include "CameraMove.h"

#include	"DebugRendering.h"

using namespace debug_render;

#pragma region Number

Number::Number(char* texture_Filename)
{
	m_number_Tex = new iex2DObj(texture_Filename);
	m_number_vector.reserve(7);	//とりあえず7けたくらい
}

Number::~Number()
{
	if (m_number_Tex)
	{
		delete m_number_Tex;
		m_number_Tex = nullptr;
	}
}

void	Number::Render_Number(const Vector3& center_Pos, const float width,
													const float height, int number)
{
	float tu[2];
	float tv[2];
	tu[0] = (number_pixel  * (float)(number % 8 ))/  texture_size;
	tu[1] = tu[0] + (number_pixel / texture_size);

	tv[0] = (float)(number / 8) * 0.125f;
	tv[1] = tv[0] + 0.125f;
	Billboard::Draw3D(center_Pos, m_number_Tex, width, height, tu, tv, RS_COPY);
}

void	Number::Draw(int draw_Number,const Vector3& centor_Pos, float billboard_width, float billboard_height)
{
	//数字を桁ごとに分解する
	while (draw_Number > 0)
	{
		int surplus = draw_Number % 10;
		m_number_vector.emplace_back(surplus);
		draw_Number /= 10;
	}
	int	number_of_digits = m_number_vector.size();
	float number_width = billboard_width / (float)number_of_digits;

	Vector3 right(matView._11, matView._21, matView._31);
	Vector3 up(matView._12, matView._22, matView._32);
	right.Normalize();
	up.Normalize();

	int harf_size = number_of_digits / 2;
	float centor_index = -harf_size;

	for (int i = number_of_digits - 1; i >= 0; i--)
	{
		Vector3 draw_Pos = centor_Pos + right * (number_width *  centor_index);
		Render_Number(draw_Pos, number_width, billboard_height, m_number_vector[i]);
		centor_index += 1.0f;
	}
	m_number_vector.clear();
}


//コーラ用
void	Number::Draw(int draw_Number, const Vector3& centor_Pos, float  billboard_width, float billboard_height, LPCAMERA camera)
{
	//数字を桁ごとに分解する
	while (draw_Number > 0)
	{
		int surplus = draw_Number % 10;
		m_number_vector.emplace_back(surplus);
		draw_Number /= 10;
	}
	int	number_of_digits = m_number_vector.size();
	float number_width = billboard_width / (float)number_of_digits;

	Vector3 right(CameraFunction::GetRight(camera));
	Vector3 up(CameraFunction::GetUp(camera));
	right.Normalize();
	up.Normalize();

	int harf_size = number_of_digits / 2;
	float centor_index = -harf_size;

	for (int i = number_of_digits - 1; i >= 0; i--)
	{
		Vector3 draw_Pos = centor_Pos + right * (number_width *  centor_index);
		Render_Number(draw_Pos, number_width, billboard_height, m_number_vector[i],camera);
		centor_index += 1.0f;
	}
	m_number_vector.clear();
}

void	Number::Render_Number(const Vector3& center_Pos, const float width,
	const float height, int number,LPCAMERA camera)
{
	float tu[2];
	float tv[2];
	tu[0] = (number_pixel  * (float)(number % 8)) / texture_size;
	tu[1] = tu[0] + (number_pixel / texture_size);

	tv[0] = (float)(number / 8) * 0.125f;
	tv[1] = tv[0] + 0.125f;
	Billboard::Draw3D(center_Pos, m_number_Tex, width, height, tu, tv, RS_COPY,camera);
}

#pragma endregion
