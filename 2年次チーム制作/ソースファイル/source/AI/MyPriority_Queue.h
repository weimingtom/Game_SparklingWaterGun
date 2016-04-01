#ifndef __MYPRIORIT_QUEUE_H__
#define __MYPRIORIT_QUEUE_H__

#include	<vector>
#include	<algorithm>

template<class insert_class, class pred>
class MyPriority_Queue
{
private:
	std::vector<insert_class*> m_heap;
	pred*									m_pred;
	int										m_max_Size;
public:
	MyPriority_Queue<insert_class, pred>(pred* sort_Alg, const int& max_Size) :
		m_pred(sort_Alg), m_max_Size(max_Size)
	{
		m_heap.reserve(m_max_Size);
	}
	~MyPriority_Queue()
	{
		m_heap.clear();
		delete m_pred;
		m_pred = nullptr;
	}

	void	Release()
	{
		delete m_pred;
		m_pred = nullptr;
		
	}

	void	Push(insert_class* insert)
	{
		m_heap.push_back(insert);
	
		//std::push_heap(m_heap.begin(), m_heap.end(), *m_pred());
		//std::sort(m_heap.begin(), m_heap.end(), *m_pred);
	}

	insert_class*	Pop()
	{
		insert_class* ret = m_heap.back();
	//	std::pop_heap(m_heap.begin(), m_heap.end(), *m_pred());
		m_heap.pop_back();
		//m_heap.pop_back();
		return ret;
	}
	void		Pop(insert_class* erase)
	{
		auto erase_i = std::find(m_heap.begin(), m_heap.end(), erase);
		m_heap.erase(erase_i);
	}
	insert_class*	Top()
	{
		insert_class* ret = nullptr;
		float	min_score = FLT_MAX;
		for (auto it : m_heap)
		{
			if (it->score < min_score)
			{
				min_score = it->score;
				ret = it;
			}
		}
		return ret;
	}

	insert_class*	Back()
	{
		return m_heap.front();
	}

	

	bool	Empty(){ return m_heap.empty(); }
	void	Clear(){ m_heap.clear(); }



};


#endif
