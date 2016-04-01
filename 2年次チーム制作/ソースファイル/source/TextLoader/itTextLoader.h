#ifndef __IT_TEXT_LOADER_H__
#define __IT_TEXT_LOADER_H__

//******************************************************************
//		テキストローダークラス
//******************************************************************

class itTextLoader
{
private:
	char*			m_pBuf;					//ファイル保存領域
	int				m_index;				//現在の要素番号
	int				m_file_size;			//ファイルサイズ
public:
	itTextLoader();							//通常コンストラクタ
	itTextLoader(const char *file_name);	//初期化と同時にファイルを開く

	~itTextLoader();						//デストラクタ
private:
	void bufferRelease();					//ファイル保存領域を解放
	bool skipSpace();						//空白読み飛ばし
public:
	bool fileOpen(const char *file_name);	//ファイルオープン(開けなかった場合はfalseを返す)
public:
	bool isOpen();							//現在ファイルを開いているかどうか(開いていた場合はtrueを返す)
	bool isFileEnd();						//現在のindex位置がファイルの終端の場合trueを返す(開いていなかった場合もtrueを返す)
public:
	bool findStr(const char *str);			//現在のindex位置から引数の文字列の終わりまでindexを移動させる(ファイル終端まで関数がtrueを返さなかった場合はこの関数はfalseを返す)
public:
	void loadInt(int& out);					//int型読み込み
	void loadFloat(float& out);				//float型読み込み
	void loadStr(char* out, int out_size);	//char型文字列読み込み(サイズが収まりきらない場合は途中で打ち切って\0を終端にセット)
};


#endif