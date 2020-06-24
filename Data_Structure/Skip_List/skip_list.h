/////////////////////////////////////////////
/////////////////////////////////////////////
////Note: this is the version of skiplist////
////that supports duplicate keys.	     ////
/////////////////////////////////////////////
/////////////////////////////////////////////


#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include<cstdlib>
#include<ctime>
#include<stdexcept>

template<typename Key, typename Value>
struct Skip_List_Node
{
	Skip_List_Node():highest_level(1),curr_level(1),prev(NULL),next(NULL),structure(NULL){}
	Skip_List_Node(std::pair<Key,Value>& pr,unsigned int& hi_lv,unsigned int& cr_lv)
	{
		key(pr.first),
		value(pr.second),
		item.first = pr.first;
		item.second = pr.second;
		highest_level(hi_lv),
		curr_level(cr_lv),
		prev(NULL),
		next(NULL),
		structure(NULL)
	}
	Key key;
	Value value;
	std::pair<Key,Value> item;
	unsigned int highest_level;
	unsigned int curr_level;
	Skip_List_Node* prev;
	Skip_List_Node* next;
	Skip_List_Node** structure;
};

template<typename Key, typename Value>
class Skip_List
{
public:
	class iterator
	{
	public:
		iterator();
		std::pair<Key,Value>* operator->();
		std::pair<Key,Value>& operator*();
		iterator& operator++();
		iterator operator++(int);
		bool operator==();
		bool operator!=();
	private:
		friend class Skip_List;
		Skip_List_Node<Key,Value>* curr_node;
		iterator(Skip_List_Node<Key,Value>*);
	};
public:
	Skip_List();
	~Skip_List();
	void clear();
	void insert(std::pair<Key,Value>&);
	void erase(Key&);
	void print() const;
	iterator& find(Key&) const;
	iterator begin() const;
	iterator end() const;
private:
	Skip_List_Node<Key,Value>* list;
	unsigned int num_of_levels;
	unsigned int num_of_nodes;
	unsigned int num_of_moves;
	void insert_helper(std::pair<Key,Value>&,const unsigned int&);
	static unsigned int rand_generator(unsigned int&);
};


template<typename Key, typename Value>
Skip_List<Key,Value>::Skip_List():num_of_levels(1),num_of_nodes(0),num_of_moves(0)
{
	//Update level count
	++num_of_levels;
	Skip_List_Node<Key,Value>* begin_node = new Skip_List_Node<Key,Value>::Skip_List_Node(),
							   end_node = new Skip_List_Node<Key,Value>::Skip_List_Node();						   
	begin_node->structure = new Skip_List_Node<Key,Value>*[1];
	(begin_node->structure)[0] = begin_node;
	end_node->structure = new Skip_List_Node<Key,Value>*[1];
	(end_node->structure)[0] = end_node;
	begin_node->next = end_node;
	end_node->prev = begin_node;
	list = begin_node;					
}


template<typename Key, typename Value>
Skip_List<Key,Value>::~Skip_List()
{
	clear();
}


template<typename Key, typename Value>
typename Skip_List<Key,Value>::iterator Skip_List<Key,Value>::find(Key& k) const
{
	unsigned int level = num_of_levels;
	Skip_List<Key,Value>* node = (begin->structure)[level-1]->next,
					      tmp;
	while(level >= 1)
	{
		if(node->key == k)	
		{
			++num_of_moves;
			print();
			num_of_moves = 0;
			//starting from here
			tmp = node;
			while(tmp->prev && tmp->prev->prev)
			{
				tmp = tmp->prev;
				if(tmp->key == node->key)
					vec.push_back(tmp->value);
			}
			tmp = node;
			while(tmp->next && tmp->next->next)
			{
				tmp = tmp->next;
				if(tmp->key == node->key)
					vec.push_back(tmp->value);
			}
			//ending in here is the portion that
			//checks for duplicate keys
			return vec;
		}
		else if(node->key > key)
		{
			++num_of_moves;
			if(node->prev && node->prev->prev)
				node = node->prev;
			else if(node->prev && !(node->prev->prev))
			{
				if(level >= 2)
					--level;
				else
					level = 0;
			}
		}
		else if(node->key < key)
		{
			++num_of_moves;
			if(node->next && node->next->next)
				node = node->next;
			else if(node->next && !(node->next-next))
			{
				if(level >= 2)
					--level;
				else
					level = 0;
			}
		}
	}
	print();
	num_of_moves = 0;
	return vec;
}


template<typename Key, typename Value>
void Skip_List<Key,Value>::clear()
{
	Skip_List_Node<Key,Value>* tmp1 = list[0];
	while(tmp1->next)
	{
		Skip_List_Node<Key,Value>* tmp2 = tmp1->next;
		delete tmp1;
		tmp1 = tmp2;
	}
	delete tmp1;
	delete list;
}


template<typename Key, typename Value>
void Skip_List<Key,Value>::insert(std::pair<Key,Value>& pr);
{
	//without Skip_List<Key,Value>:: still works?
	unsigned int promotion = rand_generator(num_of_nodes);
	insert_helper(pr,promotion);
}


template<typename Key, typename Value>
void Skip_List<Key,Value>::erase(Key& k)
{
	Skip_List_Node<Key,Value>* node = list[num_of_levels-1]->next;
	unsigned int level = num_of_levels - 1;
	while(level >= 1)
	{
		if(node->key == k)
		{
			Skip_List_Node<Key,Value>* tmp = node;
			unsigned int tmp_level = level;
			while(node >= 0)
			{
				
			}
		}
	}
}


template<typename Key, typename Value>
void Skip_List<Key,Value>::print() const
{

}


template<typename Key, typename Value>
unsigned int Skip_List<Key,Value>::rand_generator(unsigned int& num_of_nodes)
{
	srand(ctime(0));
	///////////////////////////////////////////////////
	//Could instead #import<cmath> and use log instead
	unsigned int num_node = num_of_nodes+1,
				 cap = 0,
				 num_promotion = 0;
	while(num_node > 0)
	{
		num_of_nodes /= 2;
		++cap;
	}
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	while(rand()%2 == 0)
	{
		++num_promotion;
		if(num_promotion == cap)
			break;
	}
	return num_promotion;
}


template<typename Key, typename Value>
void Skip_List<Key,Value>::insert_helper(std::pair<Key,Value>& pr,const unsigned int& promotion)
{

}


template<typename Key, typename Value>
void Skip_List<Key,Value>::update_begin_and_end(const unsigned int& promotion, const Skip_List_Node<Key,Value>& node)
{

}

#endif