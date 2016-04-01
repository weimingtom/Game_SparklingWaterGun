#include "ParentAndChild.h"

int	PARENT::GetChildCount()const
{
	return (int)m_Child_data.size();
}

void PARENT::AddChild(LPCHILD child)
{
	child->m_pParent = this;
	m_Child_data.insert(child);
}

void PARENT::ReleaseChild(LPCHILD child)
{
	CHILDDATA::iterator it=m_Child_data.find(child);
	if (it != m_Child_data.end())m_Child_data.erase(it);
}

bool PARENT::isChild(LPCHILD child)const
{
	return m_Child_data.count(child) > 0;
}

void PARENT::UpdateAllChild(
	LPCMATRIX pParentWorldMat,
	LPCMATRIX pParentWorldMatInv,
	LPCMATRIX pParentMoveMat)
{
	CHILDDATA::iterator it = m_Child_data.begin();
	CHILDDATA::iterator end = m_Child_data.end();

	MATRIX m = *pParentWorldMatInv**pParentMoveMat**pParentWorldMat;

	while (it != end)
	{
		(*it)->ParentMove(pParentWorldMat, pParentWorldMatInv, pParentMoveMat,&m);
		++it;
	}
}