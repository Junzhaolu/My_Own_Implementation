/////////////////////////////////////////////
////Note: This is the version of skiplist////
////that does not support duplicate keys.////
/////////////////////////////////////////////

#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include<cstdlib>
#include<ctime>
#include<stdexcept>
#include<iostream>
#include<iomanip>

template<typename Key, typename Value>
struct Skip_List_Node
{
	Skip_List_Node():highest_level(1),prev(NULL),forward(NULL){}
	Skip_List_Node(const std::pair<Key,Value>& pr,int& level):key(pr.first),
													    value(pr.second),
													    highest_level(level),
													    prev(NULL),
													    forward(NULL)
	{
		item.first = pr.first;
		item.second = pr.second;	
	}
	Key key;
	Value value;
	std::pair<Key,Value> item;
	int highest_level;
	Skip_List_Node* prev;
	Skip_List_Node** forward;
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
		bool operator==(const iterator&) const;
		bool operator!=(const iterator&) const;
	private:
		friend class Skip_List<Key,Value>;
		Skip_List_Node<Key,Value>* curr_node;
		iterator(Skip_List_Node<Key,Value>*);
	};
public:
	Skip_List();
	~Skip_List();
	void clear();
	void insert(const std::pair<Key,Value>&);
	void erase(const Key&);
	void print() const;
	iterator find(const Key&) const;
	iterator begin() const;
	iterator end() const;
private:
	Skip_List_Node<Key,Value>* list_begin;
	Skip_List_Node<Key,Value>* list_end;
	Skip_List_Node<Key,Value>* internal_find(const Key&) const;
	int num_of_levels;
	int num_of_nodes;
	static int coin_toss(const int&);
};


template<typename Key, typename Value>
Skip_List<Key,Value>::Skip_List():num_of_levels(1),num_of_nodes(0)
{
	list_begin = new Skip_List_Node<Key,Value>();
	list_end = new Skip_List_Node<Key,Value>();
	list_begin->forward = new Skip_List_Node<Key,Value>*[num_of_levels];
	(list_begin->forward)[num_of_levels-1] = list_end;
	list_end->prev = list_begin;
}


template<typename Key, typename Value>
Skip_List<Key,Value>::~Skip_List()
{
	clear();
}


template<typename Key, typename Value>
typename Skip_List<Key,Value>::iterator Skip_List<Key,Value>::find(const Key& k) const
{
	Skip_List_Node<Key,Value>* node = internal_find(k);
	if((node->forward)[0]->forward && (node->forward)[0]->key == k)
	{
		typename Skip_List<Key,Value>::iterator itr = (node->forward)[0];
		return itr;
	}
	else if((!(node->forward)[0]->forward) || ((node->forward)[0]->forward && (node->forward)[0]->key != k))
	{
		typename Skip_List<Key,Value>::iterator itr;
		return itr;
	}
}


template<typename Key, typename Value>
Skip_List_Node<Key,Value>* Skip_List<Key,Value>::internal_find(const Key& k) const
{
	int level = num_of_levels;
	Skip_List_Node<Key,Value>* node = list_begin,
							  *node_next = (node->forward)[level-1];
	while(level >= 0)
	{
		if((node_next->forward && (node_next->key >= k)) || !(node_next->forward))
		{
			if(level >= 2)
			{
				--level;
				node_next = (node->forward)[level-1];
			}
			else
				return node;
		}
		else if(node_next->forward && (node_next->key < k))
		{
			node = node_next;
			node_next = (node->forward)[level-1];
		}
	}
	return node;
}


template<typename Key, typename Value>
void Skip_List<Key,Value>::clear()
{
	Skip_List_Node<Key,Value>* traverse = list_begin,
							   *traverse_next = NULL;
	while(traverse->forward)							   
	{
		traverse_next = (traverse->forward)[0];
		delete[] (traverse->forward);
		delete traverse;
		traverse = traverse_next;
	}
	delete traverse;
	list_begin = NULL;
	list_end = NULL;
	num_of_nodes = 0;
	num_of_levels = 0;
}


template<typename Key, typename Value>
void Skip_List<Key,Value>::insert(const std::pair<Key,Value>& pr)
{
	Skip_List_Node<Key,Value>* prev_node = internal_find(pr.first);
	if((!(prev_node->forward)[0]->forward) || ((prev_node->forward)[0]->forward && (prev_node->forward)[0]->key != pr.first))
	{
		int promotion_level = coin_toss(num_of_levels);
		//If promotion_level exceeds begin_node's level, we need to update the level for begin_node
		if(promotion_level == num_of_levels)
		{
			++num_of_levels;
			Skip_List_Node<Key,Value>** new_forward = new Skip_List_Node<Key,Value>*[num_of_levels];
			for(int i = 0; i < num_of_levels - 1; ++i)
				new_forward[i] = (list_begin->forward)[i];
			new_forward[num_of_levels-1] = list_end;
			delete[] (list_begin->forward);
			list_begin->forward = new_forward;
			++list_begin->highest_level;
		}
		//Then allocate memory for new node, and manually reset connection between the new node,
		//the node prior, and the node after
		Skip_List_Node<Key,Value>* new_node = new Skip_List_Node<Key,Value>(pr,promotion_level);
		new_node->forward = new Skip_List_Node<Key,Value>*[promotion_level];
		new_node->prev = prev_node;
		(new_node->forward)[0] = (prev_node->forward)[0];
		(prev_node->forward)[0]->prev = new_node;
		(prev_node->forward)[0] = new_node;
		for(int i = 1; i < promotion_level; ++i)
		{
			//There could be a case where the prior node is not tall enough
			//for the new_node. In this case, we keep moving backward until
			//we find a node that is high enough
			if((prev_node->highest_level-1) < i)
			{
				while((prev_node->highest_level-1) < i)
					prev_node = prev_node->prev;
			}
			Skip_List_Node<Key,Value>* tmp = (prev_node->forward)[i];
			(prev_node->forward)[i] = new_node;
			(new_node->forward)[i] = tmp;
		}
		++num_of_nodes;
	}
	//If there already exists a node with the same key, then we just update the value
	else if(((prev_node->forward)[0]->forward) && ((prev_node->forward)[0]->key == pr.first))
	{
		(prev_node->forward)[0]->value = pr.second;
		return;
	}
}


template<typename Key, typename Value>
void Skip_List<Key,Value>::erase(const Key& k)
{
	Skip_List_Node<Key,Value>* prev_node = internal_find(k);
	if((!(prev_node->forward)[0]->forward) || ((prev_node->forward)[0]->forward && (prev_node->forward)[0]->key != k))
		return;
	else if((prev_node->forward)[0]->forward && (prev_node->forward)[0]->key == k)
	{
		Skip_List_Node<Key,Value>* target = (prev_node->forward)[0];
		for(int i = 0; i < target->highest_level; ++i)
		{
			if(i == 0)
				(target->forward)[0]->prev = prev_node;
			else
			{
				if(prev_node->highest_level-1 < i)
				{
					while(prev_node->highest_level-1 < i)
						prev_node = prev_node->prev;
				}
			}
			(prev_node->forward)[i] = (target->forward)[i];
			//There is a case where deleting a node results in an empty level on the otp
			if(i == target->highest_level-1)
			{
				if(prev_node == list_begin && (prev_node->forward)[i] == list_end)
				{
					--num_of_levels;
					Skip_List_Node<Key,Value>** new_forward = new Skip_List_Node<Key,Value>*[num_of_levels];
					for(int i = 0; i < num_of_levels; ++i)
						new_forward[i] = (list_begin->forward)[i];
					delete[] (list_begin->forward);
					list_begin->forward = new_forward;
					--list_begin->highest_level;
				}
			}
		}
		//Now to release the allocated memory
		delete[] (target->forward);
		delete target;
		--num_of_nodes;
	}
}


template<typename Key, typename Value>
void Skip_List<Key,Value>::print() const
{
	std::cout << std::endl << "Printing the list..." << std::endl;
	std::cout << std::setw(20) << std::setfill('-') << "-" << std::endl;
	for(int i = num_of_levels; i >= 1; --i)
	{
		Skip_List_Node<Key,Value>* traverse = (list_begin->forward)[i-1],
								 *correction = list_begin->forward[0];
		std::cout << "Level " << i << ": ";
		for(int j = 0; j < num_of_nodes; ++j)
		{
			if((!traverse->forward && j < num_of_nodes) || (traverse->forward && (traverse->key != correction->key)))
				std::cout << std::setw(5) << std::setfill(' ') << " ";
			else if(traverse->forward && (traverse->key == correction->key))
			{
				std::cout << std::setw(5) << std::setfill(' ') << traverse->key;
				traverse = traverse->forward[i-1];
			}
			correction = correction->forward[0];
		}
		std::cout << std::endl;
	}
}


template<typename Key, typename Value>
int Skip_List<Key,Value>::coin_toss(const int& current_max_level)
{
	int result = 0;
	while(rand()%2 == 0)
	{
		++result;
		if(result == current_max_level)
			break;
	}
	if(result == 0)
		return 1;
	else
		return result;
}


template<typename Key, typename Value>
Skip_List<Key,Value>::iterator::iterator():curr_node(NULL)
{
	//Itentionally left empty
}


template<typename Key, typename Value>
std::pair<Key,Value>* Skip_List<Key,Value>::iterator::operator->()
{
	curr_node->item.first = curr_node->key;
	curr_node->item.second = curr_node->value;
	return &curr_node->item;
}


template<typename Key, typename Value>
std::pair<Key,Value>& Skip_List<Key,Value>::iterator::operator*()
{
	curr_node->item.first = curr_node->key;
	curr_node->item.second = curr_node->value;
	return curr_node->item;
}

template<typename Key, typename Value>
typename Skip_List<Key,Value>::iterator& Skip_List<Key,Value>::iterator::operator++()
{
	if((curr_node->forward)[0]->forward)
		curr_node = (curr_node->forward)[0];
	else
		curr_node = NULL;
	return *(this);
}


template<typename Key, typename Value>
typename Skip_List<Key,Value>::iterator Skip_List<Key,Value>::iterator::operator++(int)
{
	if((curr_node->forward)[0]->forward)
		curr_node = (curr_node->forward)[0];
	else
		curr_node = NULL;
	Skip_List<Key,Value>::iterator itr = curr_node;
	return itr; 
}


template<typename Key, typename Value>
bool Skip_List<Key,Value>::iterator::operator==(const typename Skip_List<Key,Value>::iterator& itr) const
{
	if(!curr_node && !itr.curr_node)
		return true;
	else if((!curr_node && itr.curr_node) || (curr_node && !itr.curr_node))
		return false;
	return ((curr_node->key == itr.curr_node->key) && (curr_node->value == itr.curr_node->value));
}


template<typename Key, typename Value>
bool Skip_List<Key,Value>::iterator::operator!=(const typename Skip_List<Key,Value>::iterator& itr) const
{
	if(!curr_node && !itr.curr_node)
		return false;
	else if((!curr_node && itr.curr_node) || (curr_node && !itr.curr_node))
		return true;
	return ((curr_node->key != itr.curr_node->key) && (curr_node->value != itr.curr_node->value));
}


template<typename Key, typename Value>
Skip_List<Key,Value>::iterator::iterator(Skip_List_Node<Key,Value>* node):curr_node(node)
{
	//Intentionally left empty
}


template<typename Key, typename Value>
typename Skip_List<Key,Value>::iterator Skip_List<Key,Value>::begin() const
{
	if(num_of_nodes == 0)	
	{
		Skip_List<Key,Value>::iterator itr;
		return itr;
	}
	else
	{
		Skip_List<Key,Value>::iterator itr = (list_begin->forward)[0];
		return itr;
	}
}


template<typename Key, typename Value>
typename Skip_List<Key,Value>::iterator Skip_List<Key,Value>::end() const
{
	Skip_List<Key,Value>::iterator itr;
	return itr;	
}


#endif