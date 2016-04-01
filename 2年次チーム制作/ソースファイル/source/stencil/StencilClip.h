#ifndef __STENCILCLIP_H__
#define __STENCILCLIP_H__

#include <d3d9.h>

class StencilClip 
{
public:
	// マスク色
	enum MaskColor {
		MaskColor_Trans = 0x00,	//! 透明色
		MaskColor_Fill = 0x01,	//! 塗りつぶし
		MaskColor_None = 0xff  //! 無効カラー
	};

	// モード
	enum Mode {
		Mode_Masking,	//! マスク書き込みモード中
		Mode_Draw		//! 描画モード中
	};
protected:
	Mode				mode_;			//!< モード
	MaskColor			writeColor_;	//!< マスク書き込み時の色
	MaskColor			refColor_;		//!< 描画時のマスキング色
	IDirect3DDevice9*	device_;		//!< 描画デバイス
	DWORD				curZTest_;		//!< 既存のZテストの有無 
	DWORD				curZFunc_;		//!< 既存のZテスト比較関数

public:
	StencilClip();
	virtual ~StencilClip();

	//! クリッピング領域作成開始
	virtual bool regionBegin(IDirect3DDevice9* device, MaskColor clearColor = MaskColor_Trans);

	//! クリッピング領域作成終了
	virtual bool regionEnd();

	//! クリッピング描画開始
	virtual bool drawBegin();

	//! クリッピング描画終了
	virtual bool drawEnd(MaskColor clearColor = MaskColor_Trans);

	//! 書き込み時マスクカラーの設定
	void setWriteMaskColor(MaskColor color);

	//! 描画時のマスクカラーの指定
	void setRefMaskColor(MaskColor color);
};

#endif