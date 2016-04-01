#ifndef __PARENT_AND_CHILD_H__
#define __PARENT_AND_CHILD_H__

#include <d3dx9math.h>
#include <set>

typedef D3DXMATRIX MATRIX;
typedef MATRIX* LPMATRIX;
typedef const MATRIX* LPCMATRIX;

class PARENT;
typedef PARENT* LPPARENT;

class CHILD;
typedef CHILD* LPCHILD;

class PARENT
{
private:
	typedef std::set<LPCHILD> CHILDDATA;
public:
	PARENT(){}
	virtual ~PARENT(){}
private:
	CHILDDATA m_Child_data;
public:
	int		GetChildCount	()const;
	void	AddChild		(LPCHILD child);
	void	ReleaseChild	(LPCHILD child);
	bool	isChild			(LPCHILD child)const;
public:
	void UpdateAllChild(LPCMATRIX pParentWorldMat, LPCMATRIX pParentWorldMatInv, LPCMATRIX pParentMoveMat);
};

class CHILD
{
	friend class PARENT;
public:
	CHILD() :m_pParent(nullptr){}
	virtual ~CHILD(){}
private:
	LPPARENT	m_pParent;
private:
	virtual void ParentMove(LPCMATRIX pParentWorldMat, LPCMATRIX pParentWorldMatInv, LPCMATRIX pParentMoveMat, LPCMATRIX AffineMat) = 0;
protected:
	LPPARENT GetParent()
	{
		return m_pParent;
	}
};

class PARENTANDCHILD :public PARENT, public CHILD
{
public:
	PARENTANDCHILD(){}
	virtual ~PARENTANDCHILD(){}
public:
};

#endif