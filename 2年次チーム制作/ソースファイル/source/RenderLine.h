#ifndef __LINE_H__
#define __LINE_H__

//頂点バッファ構造体
struct _VB
{
	LPDIRECT3DVERTEXBUFFER9 p;
	_VB(LPDIRECT3DDEVICE9 d3dd, UINT length) : p(0) { d3dd->CreateVertexBuffer(length, 0, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &p, NULL); }
	virtual ~_VB() { if (p) p->Release(); }
	operator LPDIRECT3DVERTEXBUFFER9() { return p; }
	LPDIRECT3DVERTEXBUFFER9 operator ->() { return p; }
};

enum { BLACK = 0x00000000, RED = 0xFFFF0000, GREEN = 0xFF00FF00, BLUE = 0xFF0000FF, YELLOW = 0xFFFFFF00, MAGENTA = 0xFFFF00FF, CYAN = 0xFF00FFFF, WHITE = 0xFFFFFFFF };

class DrawLine
{
public :
	static void Render(Vector3 from, Vector3 to, D3DCOLOR color = WHITE);
};

#endif
