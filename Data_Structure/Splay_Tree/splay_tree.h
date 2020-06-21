#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H
#include"../Binary_Search_Tree/bst.h"


//Splay tree removal has 2 methods: bottom-up
//and top-down. By default, this code uses the
//bottom-up method. To change to use the top-down
//method, simply uncomment the following line
#define REMOVE_BY_BOTTOM_UP_METHOD


//See line #6 for modifiability
bool remove_by_bottom_up_method =
#ifdef REMOVE_BY_BOTTOM_UP_METHOD
	1;
#else
	0;
#endif


template<typename Key,typename Value>
class Splay_Tree:public BinarySearchTree<Key,Value>
{
public:
	Splay_Tree();
	virtual void erase(const Key&);
	virtual void insert(const std::pair<Key,Value>&);
	virtual typename BinarySearchTree<Key,Value>::iterator find(const Key&);
private:
	void splay(Node<Key,Value>*);
	void bottom_up(const Key& key);
	void top_down(const Key& key);
};


template<typename Key,typename Value>
Splay_Tree<Key,Value>::Splay_Tree():BinarySearchTree<Key,Value>::BinarySearchTree()
{
}


template<typename Key,typename Value>
void Splay_Tree<Key,Value>::erase(const Key& key)
{	
	if(remove_by_bottom_up_method)
		bottom_up(key);
	else
		top_down(key);
}


template<typename Key,typename Value>
void Splay_Tree<Key,Value>::insert(const std::pair<Key,Value>& pr)
{
	Node<Key,Value>* node = BinarySearchTree<Key,Value>::internal_find(pr.first);
	if(node)
	{
		if(node == BinarySearchTree<Key,Value>::root)
			return;
		splay(node);
	}
	else
	{
		BinarySearchTree<Key,Value>::insert(pr);
		node = BinarySearchTree<Key,Value>::internal_find(pr.first);
		splay(node);
	}
}


template<typename Key,typename Value>
void Splay_Tree<Key,Value>::bottom_up(const Key& key)
{
	Node<Key,Value>* node = BinarySearchTree<Key,Value>::internal_find(key);
	if(node)
	{
		if(node == BinarySearchTree<Key,Value>::root)
			BinarySearchTree<Key,Value>::erase(key);
		else
		{
			node = node->parent;
			BinarySearchTree<Key,Value>::erase(key);
			if(node != BinarySearchTree<Key,Value>::root)
				splay(node);
		}
	}
	else
	{
		node = BinarySearchTree<Key,Value>::root;
		Node<Key,Value>* node_pr = NULL;
		while(node)
		{
			node_pr	= node;
			if(node->key < key)
				node = node->right_Child;
			else
				node = node->left_Child;
		}
		if(node_pr != BinarySearchTree<Key,Value>::root)
			splay(node_pr);
	}
}


template<typename Key,typename Value>
void Splay_Tree<Key,Value>::top_down(const Key& key)
{
	Node<Key,Value>* node = BinarySearchTree<Key,Value>::internal_find(key);
	if(node)
	{
		if(node->parent)
			splay(node);
		Node<Key,Value>* left_tree_root = node->left_Child,
						*right_tree_root = node->right_Child;
		if((left_tree_root && right_tree_root) || (left_tree_root && !(right_tree_root)))
		{
			left_tree_root->parent = NULL;
			left_tree_root = BinarySearchTree<Key,Value>::largest_node(left_tree_root);
			splay(left_tree_root);
			left_tree_root->right_Child = right_tree_root;
			if(right_tree_root)
				right_tree_root->parent = left_tree_root;
		}
		else if(!(left_tree_root) && right_tree_root)
		{
			right_tree_root->parent = NULL;
			BinarySearchTree<Key,Value>::root = right_tree_root;
		}
		delete node;
	}
	else
	{
		node = BinarySearchTree<Key,Value>::root;
		Node<Key,Value>* node_pr = NULL;
		while(node)
		{
			node_pr	= node;
			if(node->key < key)
				node = node->right_Child;
			else
				node = node->left_Child;
		}
		if(node_pr != BinarySearchTree<Key,Value>::root)
			splay(node_pr);
	}
}


template<typename Key,typename Value>
typename BinarySearchTree<Key,Value>::iterator Splay_Tree<Key,Value>::find(const Key& key)
{
	Node<Key,Value>* node = BinarySearchTree<Key,Value>::internal_find(key),
					*node_p = NULL;
	if(node)
		splay(node);
	else
	{
		node = BinarySearchTree<Key,Value>::root;
		while(node)		
		{
			node_p = node;
			if(node->key < key)
				node = node->right_Child;
			else
				node = node->left_Child;
		}
		splay(node_p);
	}
	typename BinarySearchTree<Key,Value>::iterator itr = BinarySearchTree<Key,Value>::find(key);
	return itr;
}


template<typename Key,typename Value>
void Splay_Tree<Key,Value>::splay(Node<Key,Value>* node)
{
	if(!(node->parent))
	{
		BinarySearchTree<Key,Value>::root = node;
		return;
	}
	Node<Key,Value>* p = node->parent,
				   * gp = node->parent->parent;
	if(gp)
	{
		bool p_is_lf = 0, node_is_lf = 0;
		if(p == gp->left_Child)
			p_is_lf = 1;
		if(node == p->left_Child)
			node_is_lf = 1;
		if(p_is_lf && node_is_lf)
		{
			BinarySearchTree<Key,Value>::rotate_right(gp);
			BinarySearchTree<Key,Value>::rotate_right(p);
			if(node->parent)
				splay(node);
			else
				BinarySearchTree<Key,Value>::root = node;
		}
		else if(p_is_lf && !node_is_lf)
		{
			BinarySearchTree<Key,Value>::rotate_left(p);
			BinarySearchTree<Key,Value>::rotate_right(gp);
			if(node->parent)
				splay(node);
			else
				BinarySearchTree<Key,Value>::root = node;
		}
		else if(!p_is_lf && node_is_lf)
		{
			BinarySearchTree<Key,Value>::rotate_right(p);
			BinarySearchTree<Key,Value>::rotate_left(gp);
			if(node->parent)
				splay(node);	
			else
				BinarySearchTree<Key,Value>::root = node;
		}
		else if(!p_is_lf && !node_is_lf)
		{
			BinarySearchTree<Key,Value>::rotate_left(gp);
			BinarySearchTree<Key,Value>::rotate_left(p);
			if(node->parent)
				splay(node);	
			else
				BinarySearchTree<Key,Value>::root = node;
		}
	}
	else
	{
		if(node == p->left_Child)
			BinarySearchTree<Key,Value>::rotate_right(p);
		else
			BinarySearchTree<Key,Value>::rotate_left(p);
		BinarySearchTree<Key,Value>::root = node;
	}
}

#endif