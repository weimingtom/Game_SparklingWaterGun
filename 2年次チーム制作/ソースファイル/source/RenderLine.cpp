#include"iextreme.h"
#include	"RenderLine.h"

void DrawLine::Render(Vector3 from, Vector3 to, D3DCOLOR color)
{
	//ƒxƒNƒgƒ‹•ÏŠ·
	D3DXVECTOR3 p0(from.x, from.y, from.z);
	D3DXVECTOR3 p1(to.x, to.y, to.z);
	D3DMATERIAL9 m = { 0 };
	m.Diffuse = m.Ambient = D3DXCOLOR(color);
	iexSystem::Device->SetMaterial(&m);
	
	static _VB vb(iexSystem::Device, sizeof(D3DXVECTOR3)* 2);

	D3DXVECTOR3 *p;
	vb->Lock(0, 0, (void**)&p, 0);
	p[0] = p0;
	p[1] = p1;
	vb->Unlock();

	iexSystem::Device->SetFVF(D3DFVF_XYZ);
	iexSystem::Device->SetStreamSource(0, vb, 0, sizeof(D3DXVECTOR3));
	iexSystem::Device->SetTransform(D3DTS_WORLD, &D3DXMATRIX(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
	iexSystem::Device->DrawPrimitive(D3DPT_LINELIST, 0, 1);

}