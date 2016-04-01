#pragma once
#include	<vector>

namespace debug_render
{
	class Number
	{
	private:
		iex2DObj*m_number_Tex;
		std::vector<int>	m_number_vector;
		const float texture_size = 512;
		const float number_pixel = 64;

	private:
		void	Render_Number(const Vector3& center_Pos, const float width,
			const float height, const int number);

	public:
		Number(char* texture_Filename);
		~Number();

	public:
		/**
		*@brief	描画
		*@param	描画したい数字（何桁でもOK）
		*@param 描画したい座標
		*@param	描画サイズ（幅）
		*@param 描画サイズ（高さ）
		*/
		void	Draw(int draw_Number,const Vector3& centor_Pos, float width, float height);


		//コーラ用
		void	Draw(int draw_Number, const Vector3& centor_Pos, float width, float height,LPCAMERA camera);
		void	Render_Number(const Vector3& center_Pos, const float width,
			const float height, const int number,LPCAMERA camera);
	};
}
