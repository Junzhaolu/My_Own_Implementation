#ifndef RB_H
#define RB_H
#include"print_bst.h"
#include<vector>
#include<stdexcept>
#include<iostream>

template<typename Key, typename Value>
struct RB_Node
{
	RB_Node(RB_Node* p, Key k, Value v):is_Red(1),
							  			double_Black(0),
							  			key(k),
							  			value(v),
						  	  			parent(p),
							  			right_Child(NULL),
							  			left_Child(NULL)
	{
		item.first = k;
		item.second = v;
	}
	bool is_Red;
	bool double_Black;
	Key key;
	Value value;
	std::pair<Key,Value> item;
	RB_Node* parent;
	RB_Node* right_Child;
	RB_Node* left_Child;
};

template<typename Key, typename Value>
class RB_TREE
{
public:
	RB_TREE();
	~RB_TREE();
	void erase();
	void insert(const std::pair<Key,Value>&);
	void remove(const Key&);
	void print() const;
	Value& operator[](const Key& key);
	const Value& operator[](const Key& key) const;
	friend void prettyPrintBST(RB_TREE<PPKey, PPValue> & tree);
public:
	class iterator
	{
	public:
		iterator();
		iterator& operator++();
		iterator operator++(int);
		std::pair<Key,Value>& operator*() const;
		std::pair<Key,Value>* operator->() const;
		bool operator==(const iterator& itr) const;
		bool operator!=(const iterator& itr) const;
	private:
		friend class RB_TREE<Key,Value>;
		iterator(const RB_Node<Key,Value>* node);
		RB_Node<Key,Value>* current;
	};
private:
	RB_Node<Key,Value>* root;
	iterator find(const Key&) const;
	iterator begin() const;
	iterator end() const;
	bool is_Valid_RB_Tree() const;
	void valid_Helper(RB_Node<Key,Value>*,std::vector<unsigned int>&,unsigned int&);
	void erase_Helper(RB_Node<Key,Value>*);
	void insert_Fix(RB_Node<Key,Value>*,RB_Node<Key,Value>*);
	void remove_Fix(RB_Node<Key,Value>*,bool);
	void rotate_Left(RB_Node<Key,Value>*);
	void rotate_Right(RB_Node<Key,Value>*);
	void swap_Color(RB_Node<Key,Value>*,RB_Node<Key,Value>*);
	RB_Node<Key,Value>* get_Sibling(RB_Node<Key,Value>*) const; 
	RB_Node<Key,Value>* predecessor(RB_Node<Key,Value>*) const;
	RB_Node<Key,Value>* successor(RB_Node<Key,Value>*) const;
	RB_Node<Key,Value>* internal_find(const Key& key) const;
};


//By default, successor is used
#define REMOVE_REPLACED_BY_SUCESSOR 1
//If want to use predecessor, comment-out
//the line above

#ifdef REMOVE_REPLACED_BY_SUCESSOR
	bool rm_by_succ = 1;
#else
	bool rm_by_succ = 0;
#endif

template<typename Key, typename Value>
RB_TREE<Key,Value>::RB_TREE():root(NULL)
{
}

template<typename Key, typename Value>
RB_TREE<Key,Value>::~RB_TREE()
{
	erase();
}

template<typename Key, typename Value>
void RB_TREE<Key,Value>::erase()
{
	erase_Helper(root);
	root = NULL;
}

template<typename Key, typename Value>
void RB_TREE<Key,Value>::insert(const std::pair<Key,Value>& pr)
{
	if(!root)
	{
		root = new RB_Node<Key,Value>(NULL,pr.first,pr.second);
		root->is_Red = 0;
		return;
	}
	RB_Node<Key,Value>* prev = root, *curr = NULL;
	curr = internal_find(pr.first);
	if(curr)
		return;
	curr = root;
	while(curr)
	{
		prev = curr;
		if(pr.first < curr->key)
			curr = curr->left_Child;
		else
			curr = curr->right_Child;
	}
	curr = new RB_Node<Key,Value>(prev,pr.first,pr.second);
	if(prev->key > pr.first)
		prev->left_Child = curr;
	else
		prev->right_Child = curr;
	if(prev->is_Red)
		insert_Fix(prev,curr);
}

template<typename Key,typename Value>
void RB_TREE<Key,Value>::remove(const Key& k)
{
	bool node_is_lf = 0;
	unsigned char num_node_chld = 0;
	//If the node does not exists, return
	RB_Node<Key,Value>* node = interal_find(k);
	if(!node)
		return;
	//Determine whether the node is a lf or rt child
	if(node->parent)
	{
		if(node == node->parent->left_Child)
			node_is_lf = 1;
	}
	//Determine the number of child this node has
	if(node->left_Child && node->right_Child)
		num_node_chld = 2;
	else if((!(node->left_Child) && node->right_Child)
		     || (node->left_Child && !(node->right_Child)))
		num_node_chld = 1;
	//Depending on the number of children, we have 
	//3 different scenarios
	if(num_node_chld == 0)
	{
		if(node->is_Red)
		{
			if(node_is_lf)
				node->parent->left_Child = NULL;
			else
				node->parent->right_Child = NULL;
			delete node;
			return;
		}
		else
		{
			if(node == root)
			{
				delete node;
				root = NULL;
				return;
			}
			node->double_Black = 1;
			remove_Fix(node,0);
			delete node;
		}
	}
	else if(num_node_chld == 1)
	{
		if(node->is_Red)
		{
			if(node_is_lf)
			{
				if(node->left_Child)
				{
					node->parent->left_Child = node->left_Child;
					node->left_Child->parent = node->parent;
				}
				else
				{
					node->parent->left_Child = node->right_Child;
					node->right_Child->parent = node->parent;
				}
			}
			else
			{
				if(node->left_Child)
				{
					node->parent->right_Child = node->left_Child;
					node->left_Child->parent = node->parent;
				}
				else
				{
					node->parent->right_Child = node->right_Child;
					node->right_Child->parent = node->parent;
				}
			}
			delete node;
		}
		else
		{
			if(node == root)
			{
				if(node->left_Child)
				{
					root = node->left_Child;
					node->left_Child->parent = NULL;
					node->left_Child->is_Red = 0;
				}
				else
				{
					root = node->right_Child;
					node->right_Child->parent = NULL;
					node->right_Child->is_Red = 0;
				}
				delete node;
				return;
			}
			if(node_is_lf)
			{
				if(node->left_Child)
				{
					node->left_Child->is_Red = 0;
					node->parent->left_Child = node->left_Child;
					node->left_Child->parent = node->parent;
					node->left_Child->double_Black = 1;
					remove_Fix(node->left_Child,0);
				}
				else
				{
					node->right_Child->is_Red = 0;
					node->parent->left_Child = node->right_Child;
					node->right_Child->parent = node->parent;
					node->right_Child->double_Black = 1;
					remove_Fix(node->right_Child,0);	
				}
			}
			else
			{
				if(node->left_Child)
				{
					node->left_Child->is_Red = 0;
					node->parent->right_Child = node->left_Child;
					node->left_Child->parent = node->parent;
					node->left_Child->double_Black = 1;
					remove_Fix(node->left_Child,0);
				}
				else
				{
					node->right_Child->is_Red = 0;
					node->parent->right_Child = node->right_Child;
					node->right_Child->parent = node->parent;
					node->right_Child->double_Black = 1;
					remove_Fix(node->right_Child,0);	
				}
			}
			delete node;
		}
	}
	else if(num_node_chld == 2)
	{
		RB_Node<Key,Value>* replacement;
		//**********************************
		//See line#5 for how to modify
		if(rm_by_succ)
			replacement = successor(node);
		else if(!rm_by_succ)
			replacement = predecessor(node);
		//**********************************
		node->key = replacement->key;
		node->value = replacement->value;
		replacement->double_Black = 1;
		remove_Fix(replacement,0);
		delete replacement;
	}
}

template<typename Key,typename Value>
typename RB_TREE<Key,Value>::iterator RB_TREE<Key,Value>::find(const Key& k) const
{
	RB_TREE<Key,Value>::iterator itr = this->internal_find(k);
	return itr;
}

template<typename Key,typename Value>
bool RB_TREE<Key,Value>::is_Valid_RB_Tree() const
{
	if(!root)
		return true;
	std::vector<unsigned int>vec;	
	unsigned int path = 0;
	valid_Helper(root,vec,path);
	unsigned size = vec[0];
	for(unsigned i = 1; i < vec.size(); ++i)
	{
		if(vec[i] != size)
			return false;
	}
	return true;
}

template<typename Key,typename Value>
void RB_TREE<Key,Value>::valid_Helper(RB_Node<Key,Value>* node,std::vector<unsigned int>& vec,unsigned int& path)
{
	if(!node)
		return;
	if(!(node->is_Red))
		++vec[path];
	if(node->left_Child && node->right_Child)
	{
		valid_Helper(node->left_Child,vec,path);
		vec[path+1] = vec[path];
		valid_Helper(node->left_Child,vec,++path);
	}
	else
	{
		if(node->left_Child)
			valid_Helper(node->left_Child,vec,path);
		else
			valid_Helper(node->right_Child,vec,path);
	}
}

template<typename Key,typename Value>
void RB_TREE<Key,Value>::erase_Helper(RB_Node<Key,Value>* node)
{
	if(!node)
		return;
	erase_Helper(node->left_Child);
	erase_Helper(node->right_Child);
	delete node;
}

template<typename Key,typename Value>
void RB_TREE<Key,Value>::insert_Fix(RB_Node<Key,Value>* p,RB_Node<Key,Value>* c)
{
	if(p == root)
		return;
	RB_Node<Key,Value>* sibling = get_Sibling(p);
	bool re_color = 0, re_structure = 0;
	if(!sibling)
		re_structure = 1;
	else
	{
		if(!(sibling->is_Red))
			re_structure = 1;
		else
			re_color = 1;
	}
	if(re_color && !re_structure)
	{
		p->is_Red = 0;
		sibling->is_Red = 0;
		if(p->parent == root)
			return;
		else
		{
			if(p->parent->is_Red)
				p->parent->is_Red = 0;
			else
				p->parent->is_Red = 1;
			insert_Fix(p->parent,p);
		}
	}
	else if(!re_color && re_structure)
	{
		bool p_is_lf = 0,c_is_lf = 0;
		if(p == p->parent->left_Child)
			p_is_lf = 1;
		if(c == p->left_Child)
			c_is_lf = 1;
		if(p_is_lf && c_is_lf)
			rotate_Right(p->parent);
		else if(p_is_lf && !c_is_lf)
		{
			RB_Node<Key,Value>* gp = p->parent;
			rotate_Left(p);
			rotate_Right(gp);
		}
		else if(!p_is_lf && c_is_lf)
		{
			RB_Node<Key,Value>* gp = p->parent;
			rotate_Right(p);
			rotate_Left(gp);
		}
		else if(!p_is_lf && !c_is_lf)
			rotate_Left(p->parent);
	}
}

template<typename Key,typename Value>
void RB_TREE<Key,Value>::remove_Fix(RB_Node<Key,Value>* node, bool no_need_to_delete_node)
{
	if(node == root)
	{
		if(node->double_Black)
		{
			node->double_Black = 0;
			return;
		}
	}
	bool node_is_lf = 0;
	RB_Node<Key,Value>* sibling = get_Sibling(node);
	if(node->parent->left_Child)
	{
		if(node == node->parent->left_Child)
			node_is_lf = 1;
	}
	if((!(node->parent->is_Red) && !(sibling->is_Red)) ||
	   (node->parent->is_Red && !(sibling->is_Red)))
	{
		if((!(sibling->left_Child) && !(sibling->right_Child)) ||
			((sibling->left_Child && sibling->right_Child) && (!(sibling->left_Child->is_Red) && !(sibling->right_Child->is_Red))))
		{
			sibling->is_Red = 1;
			if(node->parent->is_Red)
				node->parent->is_Red = 0;
			else
			{
				node->parent->double_Black = 1;
				remove_Fix(node->parent,1);
			}
		}
		else if(sibling->left_Child && sibling->right_Child)
		{
			if(node_is_lf)
			{
				if(sibling->right_Child->is_Red)
				{
					RB_Node<Key,Value>* tmp = sibling->right_Child;
					swap_Color(node->parent,sibling);
					rotate_Left(node->parent);
					tmp->is_Red = 0;
				}
				else
				{
					if(sibling->left_Child->is_Red)
					{
						RB_Node<Key,Value>* tmp = sibling->left_Child;
						swap_Color(sibling,sibling->left_Child);
						rotate_Right(sibling);
						swap_Color(node->parent,tmp);
						rotate_Left(node->parent);
						sibling->is_Red = 0;
					}
				}
			}
			else
			{
				if(sibling->left_Child->is_Red)
				{
					RB_Node<Key,Value>* tmp = sibling->left_Child;
					swap_Color(node->parent,sibling);
					rotate_Right(node->parent);
					tmp->is_Red = 0;
				}
				else
				{
					if(sibling->right_Child->is_Red)
					{
						RB_Node<Key,Value>* tmp = sibling->right_Child;
						swap_Color(sibling,sibling->right_Child);
						rotate_Left(sibling);
						swap_Color(node->parent,tmp);
						rotate_Right(node->parent);
						sibling->is_Red = 0;
					}
				}
			}
		}
		else if(!(sibling->left_Child) && sibling->right_Child)
		{
			if(!(sibling->right_Child->is_Red))
			{
				sibling->is_Red = 1;
				if(node->parent->is_Red)
					node->parent->is_Red = 0;
				else
				{
					node->parent->double_Black = 1;
					remove_Fix(node->parent,1);
				}
			}
			else
			{
				if(node_is_lf)
				{
					if(sibling->right_Child->is_Red)
					{
						RB_Node<Key,Value>* tmp = sibling->right_Child;
						swap_Color(node->parent,sibling);
						rotate_Left(node->parent);
						tmp->is_Red = 0;
					}
				}
				else
				{
					if(sibling->right_Child->is_Red)
					{
						RB_Node<Key,Value>* tmp = sibling->right_Child;
						swap_Color(tmp,sibling);
						rotate_Left(sibling);
						swap_Color(node->parent,tmp);
						rotate_Right(node->parent);
						sibling->is_Red = 0;
					}
				}
			}
		}
		else if(sibling->left_Child && !(sibling->right_Child))
		{
			if(!(sibling->left_Child->is_Red))
			{
				sibling->is_Red = 1;
				if(node->parent->is_Red)
					node->parent->is_Red = 0;
				else
				{
					node->parent->double_Black = 1;
					remove_Fix(node->parent,1);
				}
			}
			else
			{
				if(node_is_lf)
				{
					if(sibling->left_Child->is_Red)
					{
						RB_Node<Key,Value>* tmp = sibling->left_Child;
						swap_Color(tmp,sibling);
						rotate_Right(sibling);
						swap_Color(node->parent,tmp);
						rotate_Left(node->parent);
						node->double_Black = 0;
						sibling->is_Red = 0;
					}
				}
				else
				{
					if(sibling->left_Child->is_Red)
					{
						RB_Node<Key,Value>* tmp = sibling->left_Child;
						swap_Color(node->parent,sibling);
						rotate_Right(node->parent);
						node->double_Black = 0;
						tmp->is_Red = 0;
					}
				}
			}
		}
	}
	else if(!(node->parent->is_Red) && sibling->is_Red)
	{
		swap_Color(node->parent,sibling);
		if(node_is_lf)
			rotate_Left(node->parent);
		else
			rotate_Right(node->parent);
		remove_Fix(node,1);
	}
	if(!no_need_to_delete_node)
	{
		if(node_is_lf)
			node->parent->left_Child = NULL;
		else
			node->parent->right_Child = NULL;
	}
}

template<typename Key,typename Value>
void RB_TREE<Key,Value>::rotate_Left(RB_Node<Key,Value>* node)
{
	if(node->right_Child)
	{
		RB_Node<Key,Value>* tmp = node->right_Child;
		node->right_Child = tmp->left_Child;
		if(tmp->left_Child)
			tmp->left_Child->parent = node;
		tmp->left_Child = node;
		if(node->parent)
		{
			if(node == node->parent->right_Child)
				node->parent->right_Child = tmp;
			else
				node->parent->left_Child = tmp;
		}
		else
			root = tmp;
		tmp->parent = node->parent;
		node->parent = tmp;	
		swap_Color(tmp,node);
	}
}

template<typename Key,typename Value>
void RB_TREE<Key,Value>::rotate_Right(RB_Node<Key,Value>* node)
{
	if(node->left_Child)
	{
		RB_Node<Key,Value>* tmp = node->left_Child;
		if(tmp->right_Child)
			tmp->right_Child->parent = node;
		node->left_Child = tmp->right_Child;
		if(node->parent)
		{
			if(node == node->parent->left_Child)
				node->parent->left_Child = tmp;
			else
				node->parent->right_Child = tmp;
		}
		else
			root = tmp;
		tmp->parent = node->parent;
		node->parent = tmp;
		tmp->right_Child = node;
		swap_Color(tmp,node);
	}
}

template<typename Key,typename Value>
void RB_TREE<Key,Value>::swap_Color(RB_Node<Key,Value>* node1,RB_Node<Key,Value>* node2)
{
	bool color = node1->is_Red;
	node1->is_Red = node2->is_Red;
	node2->is_Red = color;
}

template<typename Key,typename Value>
RB_Node<Key,Value>* RB_TREE<Key,Value>::get_Sibling(RB_Node<Key,Value>* node) const
{
	if(!(node->parent))
		return NULL;
	if(node == node->parent->left_Child)
		return node->parent->right_Child;
	else
		return node->parent->left_Child;
}

template<typename Key,typename Value>
RB_Node<Key,Value>* RB_TREE<Key,Value>::predecessor(RB_Node<Key,Value>* node) const
{
	if(!node)
		return NULL;
	RB_Node<Key,Value>* tmp = node->left_Child;
	if(!tmp) 
	{
		if(!(node->right_Child) && node->parent)
		{
			if(node == node->parent->right_Child)
				return node->parent;
		}
		else if(node->right_Child && node->parent)
			return NULL;
	}	
	while(tmp->right_Child)
		tmp = tmp->right_Child;
	return tmp;
}

template<typename Key,typename Value>
RB_Node<Key,Value>* RB_TREE<Key,Value>::successor(RB_Node<Key,Value>* node) const
{
	if(!node)
		return NULL;
	RB_Node<Key,Value>* tmp = node->right_Child;
	if(!tmp)
	{
		if(!(node->left_Child) && node->parent)
		{
			if(node == node->parent->left_Child)
				return node->parent;
		}
		else if(node->left_Child && node->parent)
			return NULL;
	}
	while(tmp->left_Child)
		tmp = tmp->left_Child;
	return tmp;
}

template<typename Key,typename Value>
RB_Node<Key,Value>* RB_TREE<Key,Value>::internal_find(const Key& key) const
{
	RB_Node<Key,Value>* tmp = root;
	while(tmp)
	{
		if(tmp->key == key)
			return tmp;
		else if(tmp->key < key)
			tmp = tmp->right_Child;
		else if(tmp->key > key)
			tmp = tmp->left_Child;
	}
	return NULL
;}

template<typename Key,typename Value>
typename RB_TREE<Key,Value>::iterator RB_TREE<Key,Value>::find(const Key& k) const
{
	RB_TREE<Key,Value>::iterator it = internal_find(k);
	return it;
}

template<typename Key,typename Value>
typename RB_TREE<Key,Value>::iterator RB_TREE<Key,Value>::begin() const
{
	RB_Node<Key,Value>* tmp = root;
	while(tmp->left_Child)
		tmp = tmp->left_Child;
	RB_TREE<Key,Value>::iterator it = tmp;
	return it;
}

template<typename Key,typename Value>
typename RB_TREE<Key,Value>::iterator RB_TREE<Key,Value>::end() const
{
	RB_Node<Key,Value> node(NULL,0,0);
	node->invalid = 1;
	return node;
}

template<typename Key,typename Value>
Value& RB_TREE<Key,Value>::operator[](const Key& key)
{
	RB_Node<Key,Value>* node = internal_find(key);
	if(node)
		return node->value;
	else
		throw std::out_of_range("Key does not exists");
}

template<typename Key,typename Value>
const Value& RB_TREE<Key,Value>::operator[](const Key& key) const
{
	RB_Node<Key,Value>* node = internal_find(key);
	if(node)
		return node->value;
	else
		throw std::out_of_range("Key does not exists");	
}

template<typename Key,typename Value>
RB_TREE<Key,Value>::iterator::iterator():current(NULL)
{
}

template<typename Key,typename Value>
RB_TREE<Key,Value>::iterator::iterator(const RB_Node<Key,Value>* node):current(node)
{
}

template<typename Key,typename Value>
typename RB_TREE<Key,Value>::iterator& RB_TREE<Key,Value>::iterator::operator++()
{
	current = successor(current);
	return (*this);
}

template<typename Key,typename Value>
typename RB_TREE<Key,Value>::iterator RB_TREE<Key,Value>::iterator::operator++(int)
{
	current = successor(current);
	RB_TREE<Key,Value>::iterator itr = current;
	return itr;
}

template<typename Key,typename Value>
std::pair<Key,Value>& RB_TREE<Key,Value>::iterator::operator*() const
{
	current->item.first = current->key;
	current->item.second = current->value;
	return current->item;
}

template<typename Key,typename Value>
std::pair<Key,Value>* RB_TREE<Key,Value>::iterator::operator->() const
{
	current->item.first = current->key;
	current->item.second = current->value;
	return &(current->item);
}

template<typename Key,typename Value>
bool RB_TREE<Key,Value>::iterator::operator==(const typename RB_TREE<Key,Value>::iterator& itr) const
{
	if((this->current && !(itr.current)) || (!(this->current) && itr.current))
		return false;
	else if(!(this->current) && !(itr.current))
		return true;
	return this->current->value == itr.current->value;
}

template<typename Key,typename Value>
bool RB_TREE<Key,Value>::iterator::operator!=(const typename RB_TREE<Key,Value>::iterator& itr) const
{
	if((this->current && !(itr.current)) || (!(this->current) && itr.current))
		return true;
	else if(!(this->current) && !(itr.current))
		return false;
	return this->current->value != itr.current->value;	
}


template<typename Key, typename Value>
void RB_TREE<Key, Value>::print() const
{
    printRoot(root);
    std::cout << "\n";
}

#endif