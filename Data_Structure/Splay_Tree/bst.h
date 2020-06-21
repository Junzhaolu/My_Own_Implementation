#ifndef BST_H
#define BST_H
#include<iostream>

//If the node which is about to be removed has 2 children,
//and we want it to be replaced by its predecessor,
//comment-out the following line
#define RM_BY_SUCCESSOR_WHEN_2_CHILDREN


//See comment above for modifiability of the following
//block of code
bool rm_by_successor =
#ifdef RM_BY_SUCCESSOR_WHEN_2_CHILDREN
	1;
#else
	0;
#endif


template<typename Key, typename Value>
struct Node
{
	Node(Node* n, Key k, Value v):key(k),
								  value(v),
								  item(std::make_pair(k,v)),
								  parent(n),
								  left_Child(NULL),
								  right_Child(NULL){}
	Key key;
	Value value;
	std::pair<Key,Value> item;
	Node* parent;
	Node* left_Child;
	Node* right_Child;
};

template<typename Key, typename	Value>
class BinarySearchTree
{
public:
	class iterator
	{
	public:
		iterator();
		iterator& operator++();
		iterator operator++(int);
		std::pair<Key,Value>& operator*() const;
		std::pair<Key,Value>* operator->() const;
		bool operator==(const iterator&) const;
		bool operator!=(const iterator&) const;
	private:
		Node<Key,Value>* current;
		friend class BinarySearchTree;
		iterator(Node<Key,Value>*);
	};
public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void clear();
	virtual void erase(const Key&);
	virtual void insert(const std::pair<Key,Value>&);
	virtual iterator find(const Key&) const;
	iterator begin() const;
	iterator end() const;
	Value& operator[](const Key&);
	const Value& operator[](const Key&) const;
	//
	//
	template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
    virtual void printRoot (Node<Key, Value> *) const;
    void print() const;
    //
    //
protected:
	void clear_Helper(Node<Key,Value>*);
	void rotate_left(Node<Key,Value>*);
	void rotate_right(Node<Key,Value>*);
	Node<Key,Value>* root;
	Node<Key,Value>* internal_find(const Key&) const;
	static Node<Key,Value>* predecessor(Node<Key,Value>*);
	static Node<Key,Value>* successor(Node<Key,Value>*);
	static Node<Key,Value>* largest_node(Node<Key,Value>*);
	static Node<Key,Value>* smallest_node(Node<Key,Value>*);
};


template<typename Key, typename	Value>
BinarySearchTree<Key,Value>::iterator::iterator():current(NULL)
{
}


template<typename Key, typename	Value>
typename BinarySearchTree<Key,Value>::iterator& BinarySearchTree<Key,Value>::iterator::operator++()
{
	current = successor(current);
	return (*this);
}


template<typename Key, typename	Value>
typename BinarySearchTree<Key,Value>::iterator BinarySearchTree<Key,Value>::iterator::operator++(int)
{
	BinarySearchTree<Key,Value>::iterator itr = (*this);
	current = successor(current);
	return itr;
}


template<typename Key, typename	Value>
std::pair<Key,Value>& BinarySearchTree<Key,Value>::iterator::operator*() const
{
	return current->item;
}


template<typename Key, typename	Value>
std::pair<Key,Value>* BinarySearchTree<Key,Value>::iterator::operator->() const
{
	return &(current->item);
}


template<typename Key, typename	Value>
bool BinarySearchTree<Key,Value>::iterator::operator==(const BinarySearchTree<Key,Value>::iterator& itr) const
{
	if(!(this->current) && !(itr.current))
		return true;
	else if((!(this->current) && itr.current) || (this->current && !(itr.current)))
		return false;
	return ((this->current->key == itr.current->key) && (this->current->value == itr.current->value));
}


template<typename Key, typename	Value>
bool BinarySearchTree<Key,Value>::iterator::operator!=(const BinarySearchTree<Key,Value>::iterator& itr) const
{
	if(!(this->current) && !(itr.current))
		return false;
	else if((!(this->current) && itr.current) || (this->current && !(itr.current)))
		return true;
	return ((this->current->key != itr.current->key) && (this->current->value != itr.current->value));
}


template<typename Key, typename	Value>
BinarySearchTree<Key,Value>::iterator::iterator(Node<Key,Value>* node):current(node)
{
}


template<typename Key, typename	Value>
BinarySearchTree<Key,Value>::BinarySearchTree():root(NULL)
{
}


template<typename Key, typename	Value>
BinarySearchTree<Key,Value>::~BinarySearchTree()
{
	clear()	;
}


template<typename Key, typename	Value>
void BinarySearchTree<Key,Value>::clear()
{
	clear_Helper(root);
	root = NULL;
}


template<typename Key, typename	Value>
void BinarySearchTree<Key,Value>::erase(const Key& key)
{
	Node<Key,Value>* node = internal_find(key);
	if(!node)
		return;
	bool node_is_lf = 0;
	char num_of_chld = 0;
	if(node->parent)
	{
		if(node->parent->left_Child == node)
			node_is_lf = 1;
	}
	if(node->left_Child && node->right_Child)
		num_of_chld = 2;
	else if((node->left_Child && !(node->right_Child)) ||
			(!(node->left_Child) && node->right_Child))
		num_of_chld = 1;
	if(num_of_chld == 0)
	{
		if(node == root)
			root = NULL;
		else
		{
			if(node_is_lf)
				node->parent->left_Child = NULL;
			else
				node->parent->right_Child = NULL;
		}
		delete node;
	}
	else if(num_of_chld == 1)
	{
		if(node == root)
		{
			if(node->left_Child)
			{
				root = node->left_Child;
				node->left_Child->parent = NULL;
			}
			else
			{
				root = node->right_Child;
				node->right_Child->parent = NULL;
			}
		}
		else
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
		}
		delete node;
	}
	else if(num_of_chld == 2)
	{
		Node<Key,Value>* replacement, 
					    *node = internal_find(key);
		//See line #5 for explanation
		if(rm_by_successor)
			replacement = successor(node);
		else
			replacement = predecessor(node);
		node->key = replacement->key;
		node->value = replacement->value;
		node->item.first = node->key;
		node->item.second = node->value;
		bool replacement_is_lf = 0;
		if((replacement->parent->left_Child && replacement->parent->right_Child) ||
			(replacement->parent->left_Child && !(replacement->parent->right_Child)))
		{
			if(replacement == replacement->parent->left_Child)
				replacement_is_lf = 1;
		}
		if(!(replacement->left_Child) && replacement->right_Child)
		{
			if(replacement_is_lf)
				replacement->parent->left_Child = replacement->right_Child;
			else
				replacement->parent->right_Child = replacement->right_Child;
			replacement->right_Child->parent = replacement->parent;
		}
		else if(replacement->left_Child && !(replacement->right_Child))
		{
			if(replacement_is_lf)
				replacement->parent->left_Child = replacement->left_Child;
			else
				replacement->parent->right_Child = replacement->left_Child;
			replacement->left_Child->parent = replacement->parent;
		}
		else if(!(replacement->left_Child) && !(replacement->right_Child))
		{
			if(replacement_is_lf)
				replacement->parent->left_Child = NULL;
			else
				replacement->parent->right_Child = NULL;
		}
		delete replacement;
	}
}


template<typename Key, typename	Value>
void BinarySearchTree<Key,Value>::insert(const std::pair<Key,Value>& pr)
{
	if(!root)
	{
		Node<Key,Value>* node = new Node<Key,Value>(NULL,pr.first,pr.second);
		root = node;
		return;
	}
	Node<Key,Value>* tmp = internal_find(pr.first),
				    *tmp_prev = NULL;
	if(tmp)
		return;
	tmp = root;
	while(tmp)
	{
		tmp_prev = tmp;
		if(tmp->key < pr.first)
			tmp = tmp->right_Child;
		else if(tmp->key > pr.first)
			tmp = tmp->left_Child;
	}
	tmp = new Node<Key,Value>(tmp_prev,pr.first,pr.second);
	if(pr.first > tmp_prev->key)
		tmp_prev->right_Child = tmp;
	else
		tmp_prev->left_Child = tmp;
}


template<typename Key, typename	Value>
typename BinarySearchTree<Key,Value>::iterator BinarySearchTree<Key,Value>::find(const Key& key) const
{
	BinarySearchTree<Key,Value>::iterator itr = internal_find(key);
	return itr;
}


template<typename Key, typename	Value>
typename BinarySearchTree<Key,Value>::iterator BinarySearchTree<Key,Value>::begin() const
{
	BinarySearchTree<Key,Value>::iterator itr = smallest_node(root);
	return itr;
}


template<typename Key, typename	Value>
typename BinarySearchTree<Key,Value>::iterator BinarySearchTree<Key,Value>::end() const
{
	BinarySearchTree<Key,Value>::iterator itr = NULL;
	return itr;
}


template<typename Key, typename	Value>
Value& BinarySearchTree<Key,Value>::operator[](const Key& key)
{
	Node<Key,Value>* node = internal_find(key);
	return node->value;
}


template<typename Key, typename	Value>
const Value& BinarySearchTree<Key,Value>::operator[](const Key& key) const
{
	Node<Key,Value>* node = internal_find(key);
	return node->value;	
}


template<typename Key, typename	Value>
void BinarySearchTree<Key,Value>::clear_Helper(Node<Key,Value>* node)
{
	if(!node)
		return;
	clear_Helper(node->left_Child);
	clear_Helper(node->right_Child);
	delete node;
}


template<typename Key, typename	Value>
Node<Key,Value>* BinarySearchTree<Key,Value>::internal_find(const Key& key) const
{
	if(!root)
		return NULL;
	Node<Key,Value>* node = root;
	while(node)
	{
		if(node->key > key)
			node = node->left_Child;
		else if(node->key < key)
			node = node->right_Child;
		else if(node->key == key)
			return node;
	}
	return NULL;
}


template<typename Key, typename	Value>
Node<Key,Value>* BinarySearchTree<Key,Value>::predecessor(Node<Key,Value>* node)
{
	if(!node)
		return NULL;
	Node<Key,Value>* tmp = node->left_Child;
	if(!tmp)
	{
		if(node->parent)
		{
			Node<Key,Value>* tmp_p = node->parent;
			tmp = node;
			if(tmp == tmp_p->right_Child)
				return tmp_p;
			else
			{
				while((tmp && tmp_p) && (tmp == tmp_p->left_Child))
				{
					tmp = tmp_p;
					tmp_p = tmp->parent;
				}
				return tmp_p;
			}
		}
		else
			return NULL;
	}
	else
	{
		while(tmp->right_Child)
			tmp = tmp->right_Child;
		return tmp;
	}
}


template<typename Key, typename	Value>
Node<Key,Value>* BinarySearchTree<Key,Value>::successor(Node<Key,Value>* node)
{
	if(!node)
		return NULL;
	Node<Key,Value>* tmp = node->right_Child;
	if(!tmp)
	{
		if(node->parent)
		{
			if(node == node->parent->left_Child)
				return node->parent;
			else
			{
				Node<Key,Value>* tmp_p = node->parent;
				tmp = node;
				while((tmp_p && tmp) && (tmp_p->right_Child == tmp))
				{
					tmp = tmp_p;
					tmp_p = tmp_p->parent;
				}
				return tmp_p;
			}
		}
		else
			return NULL;
	}	
	else
	{
		while(tmp->left_Child)
			tmp = tmp->left_Child;
		return tmp;
	}
}


template<typename Key, typename	Value>
Node<Key,Value>* BinarySearchTree<Key,Value>::largest_node(Node<Key,Value>* node)
{
	Node<Key,Value>* tmp = node;
	if(!tmp)
		return NULL;
	while(tmp->right_Child)
		tmp = tmp->right_Child;
	return tmp;
}


template<typename Key, typename Value>
Node<Key,Value>* BinarySearchTree<Key,Value>::smallest_node(Node<Key,Value>* node)
{
	Node<Key,Value>* tmp = node;
	if(!tmp)
		return NULL;
	while(tmp->left_Child)
		tmp = tmp->left_Child;
	return tmp;
}


template<typename Key, typename Value>
void BinarySearchTree<Key,Value>::rotate_left(Node<Key,Value>* node)
{
	if(node == root)
	{
		if(node->right_Child)
		{
			root = node->right_Child;
			Node<Key,Value>* tmp = node->right_Child->left_Child;
			node->right_Child->parent = NULL;
			node->right_Child->left_Child = node;
			node->parent = node->right_Child;
			node->right_Child = tmp;
			if(tmp)
				tmp->parent = node;
		}
		else
			return;
	}
	else
	{
		bool node_is_lf = 0;
		if(node == node->parent->left_Child)
			node_is_lf = 1;
		if(node->right_Child)
		{
			Node<Key,Value>* tmp = node->right_Child->left_Child;
			node->right_Child->parent = node->parent;
			node->right_Child->left_Child = node;
			if(node_is_lf)
				node->parent->left_Child = node->right_Child;
			else
				node->parent->right_Child = node->right_Child;
			node->parent = node->right_Child;
			node->right_Child = tmp;
			if(tmp)
				tmp->parent = node;
		}
		else
			return;
	}
}


template<typename Key, typename Value>
void BinarySearchTree<Key,Value>::rotate_right(Node<Key,Value>* node)
{
	if(node == root)
	{
		if(node->left_Child)
		{
			root = node->left_Child;
			Node<Key,Value>* tmp = node->left_Child->right_Child;
			node->left_Child->parent = NULL;
			node->left_Child->right_Child = node;
			node->parent = node->left_Child;
			node->left_Child = tmp;
			if(tmp)
				tmp->parent = node;
		}
		else
			return;
	}
	else
	{
		bool node_is_lf = 0;
		if(node == node->parent->left_Child)
			node_is_lf = 1;
		if(node->left_Child)
		{
			Node<Key,Value>* tmp = node->left_Child->right_Child;
			node->left_Child->parent = node->parent;
			node->left_Child->right_Child = node;
			if(node_is_lf)
				node->parent->left_Child = node->left_Child;
			else
				node->parent->right_Child = node->left_Child;
			node->parent = node->left_Child;
			node->left_Child = tmp;
			if(tmp)
				tmp->parent = node;
		}
		else
			return;
	}
}


#include"print_bst.h"
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root);
    std::cout << "\n";
}
#endif