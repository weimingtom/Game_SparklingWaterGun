#include "itTextLoader.h"
#include <cstdio>
#include <cstdlib>

#define isEof(i) (i>=m_file_size-1)
#define isSpace(c) (c==' ')
#define isNewline(c) (c=='\n')
#define intc_size 10
#define floatc_size 10

//通常コンストラクタ
itTextLoader::itTextLoader():
m_pBuf(nullptr),
m_index(0),
m_file_size(0)
{

}


itTextLoader::itTextLoader(const char *file_name):
m_index(0)
{//初期化と同時にファイルを開く
	fileOpen(file_name);
}

//デストラクタ
itTextLoader::~itTextLoader()
{
	//バッファ解放
	bufferRelease();
}

//ファイル保存領域を解放
void itTextLoader::bufferRelease()
{
	if (m_pBuf)
	{
		delete[]m_pBuf;
		m_pBuf = nullptr;
		m_index = 0;
		m_file_size = 0;
	}
}

//空白読み飛ばし
bool itTextLoader::skipSpace()
{
	while (!isEof(m_index))
	{
		if (m_pBuf[m_index] != ' '	//空白もしくは改行をスキップ
			&&m_pBuf[m_index] != '\n')return true;
		++m_index;
	}
	return false;
}

//ファイルオープン(開けなかった場合はfalseを返す)
bool itTextLoader::fileOpen(const char *file_name)
{
	FILE* fp;

	if (fopen_s(&fp, file_name, "r"))
		return false;//エラーの場合

	//一応解放しておく
	bufferRelease();

	//ファイルサイズを取得
	fseek(fp, (long)0, SEEK_END);
	m_file_size = ftell(fp);

	//シークを先頭に
	fseek(fp, 0, SEEK_SET);

	//バッファ作成
	m_pBuf = new char[m_file_size];

	//ファイル読み込み
	fread_s(
		(void*)m_pBuf,
		(size_t)m_file_size,
		sizeof(char),
		(size_t)m_file_size,
		fp);

	//ファイルポインタを閉じる
	fclose(fp);

	return true;
}

//現在ファイルを開いているかどうか(開いていた場合はtrueを返す)
bool itTextLoader::isOpen()
{
	return m_pBuf != nullptr;
}

//現在のindex位置がファイルの終端の場合trueを返す(開いていなかった場合もtrueを返す)
bool itTextLoader::isFileEnd()
{
	return isEof(m_index);
}


//現在のindex位置から引数が見つかるまでindexを移動(ファイル終端まで関数がtrueを返さなかった場合はこの関数はfalseを返す)
bool itTextLoader::findStr(const char *str)
{
	int stri, bufi, savei = m_index;
	bool flg;

	while (!isEof(m_index))
	{
		flg = true;
		stri = 0;
		bufi = m_index;

		while (str[stri] != '\0')	//検索文字列の終端までループ
		{
			if (m_pBuf[bufi++] != str[stri++]||	//もし一致しなかったらループを抜ける
				isEof(bufi))
			{
				flg = false;
				break;
			}
		}
		if (flg)	//フラグがtrueなら検索成功なのでindexを移動
		{
			m_index = bufi;
			return true;
		}
		++m_index;
	}

	//検索失敗ならもとのインデックスにもどす
	m_index = savei;

	return false;
}


void itTextLoader::loadInt(int& out)		
{
	//int型読み込み
	char t[intc_size];	
	loadStr(t, intc_size);
	out = atoi(t);
}

void itTextLoader::loadFloat(float& out)
{
	//float型読み込み
	char t[floatc_size];
	loadStr(t, floatc_size);
	out = (float)atof(t);
}

void itTextLoader::loadStr(char* out, int out_size)
{
	//char型文字列読み込み(サイズが収まりきらない場合は途中で打ち切って\0を終端にセット)
	skipSpace();	//空白を飛ばす
	int i;
	--out_size;

	for (i = 0; i < out_size; i++)
	{
		out[i] = m_pBuf[m_index++];

		char temp = m_pBuf[m_index];

		if (isSpace(m_pBuf[m_index]) ||
			isNewline(m_pBuf[m_index]) ||
			isEof(m_index))
		{
			break;
		}
	}

	out[i + 1] = '\0';
}