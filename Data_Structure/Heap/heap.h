//*************************************************//
//			This version uses 0-indexing		   //
// To make this a max-heap, in the main function,  //
// pass in a greater-than-or-equal comparator, or  //
// vice-versa for min-heap					   	   //
//*************************************************//

#ifndef HEAP_H
#define HEAP_H

#include<vector>
#include<iostream>

template<typename T, typename C>
class Heap
{
public:
	Heap(const unsigned short);
	~Heap();
	int size();
	void clear();
	void insert(const T);
	void make_heap(std::vector<T>&);
	void make_heap(T*,T*);
	void pop();
	T top() const;
	std::vector<T> heap_sort();
private:
	std::vector<T> vec;
	unsigned int num_element;
	unsigned short m_arry;
	void heapify_up(unsigned);
	void heapify_down(unsigned);
};


template<typename T, typename C>
Heap<T,C>::Heap(const unsigned short m):num_element(0),m_arry(m)
{
	//Intentionally left empty
}	


template<typename T, typename C>
Heap<T,C>::~Heap()
{
	//Intentionally left empty
}


template<typename T, typename C>
int Heap<T,C>::size()
{
	return num_element;
}


template<typename T, typename C>
void Heap<T,C>::clear()
{
	num_element = 0;
	vec.clear();
}


template<typename T, typename C>
void Heap<T,C>::insert(const T t)
{
	++num_element;
	vec.push_back(t);
	if(num_element == 1)
		return;
	heapify_up(num_element-1);
}
	

template<typename T, typename C>
void Heap<T,C>::make_heap(std::vector<T>& src)
{
	C comp;
	vec = src;
	num_element = vec.size();
	if(num_element <= 1)
		return;
	unsigned parent = 0;
	if(m_arry == 2)
		parent = (num_element-2)/m_arry;
	else if(m_arry > 2)
	{
		if((num_element-1) >= 2)
			parent = (num_element-3)/m_arry;
	}
	while(parent >= 0)
	{
		unsigned target = parent;
		bool changed = 0;
		for(unsigned i = 1; i <= m_arry; ++i)
		{
			unsigned child = 0;
			if((m_arry*parent+i) >= num_element)
				break;	
			else
				child = m_arry*parent+i;
			if(comp(vec[child],vec[target]))
			{
				changed = 1;
				target = child;
			}
		}
		if(changed)
		{
			T tmp = vec[target];
			vec[target] = vec[parent];
			vec[parent] = tmp;
			heapify_down(target);
		}
		if(parent > 0)
			--parent;
		else
			break;
	}
}


template<typename T, typename C>
void Heap<T,C>::make_heap(T* begin, T* end)
{
	std::vector<T> tmp_vec(begin,end);
	if(tmp_vec.size() <= 1)
	{
		vec = tmp_vec;
		return;
	}
	make_heap(tmp_vec);
}


template<typename T, typename C>
void Heap<T,C>::pop()
{
	if(num_element == 0)
		return;
	vec[0] = vec[num_element-1];
	vec.pop_back();
	--num_element;
	if(num_element <= 1)
		return;
	heapify_down(0);
}


template<typename T, typename C>
T Heap<T,C>::top() const
{
	return vec[0];
}


template<typename T, typename C>
std::vector<T> Heap<T,C>::heap_sort()
{
	std::vector<T> result, vec_copy = vec;
	unsigned int size_copy = num_element;
	for(unsigned i = 0; i < size_copy; ++i)
	{
		result.push_back(top());
		pop();
	}
	vec = vec_copy;
	num_element = size_copy;
	return result;
}


template<typename T, typename C>
void Heap<T,C>::heapify_up(unsigned loc)
{
	C comp;
	while(loc != 0)
	{
		unsigned pr_loc = 0;
		if(m_arry > 2)
		{
			if(loc >= 2)
				pr_loc = (loc - 2)/m_arry;
		}
		else if(m_arry == 2)
			pr_loc = (loc-1)/m_arry;
		if(comp(vec[loc],vec[pr_loc]))
		{
			T tmp = vec[pr_loc];
			vec[pr_loc] = vec[loc];
			vec[loc] = tmp;
			loc = pr_loc;
		}
		else
			break;
	}
}


template<typename T, typename C>
void Heap<T,C>::heapify_down(unsigned loc)
{
	C comp;
	while(loc < num_element)
	{
		bool update = 0;
		unsigned target = loc;
		for(unsigned short i = 1; i <= m_arry; ++i)
		{
			unsigned child_loc = loc * m_arry + i;
			if(child_loc < num_element)
			{
				if(comp(vec[child_loc],vec[target]))
				{
					update = 1;
					target = child_loc;
				}
			}
		}
		if(update)
		{
			T tmp = vec[target];
			vec[target] = vec[loc];
			vec[loc] = tmp;
			loc = target;			
		}
		else
			return;
	}
}

#endif