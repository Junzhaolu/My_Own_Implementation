//**************************************
//For now, this trie is for the char* 
//data type only. The reason for this 
//being a template is that I want to
//practice.
//**************************************

#ifndef PREFIX_TRIE_H
#define PREFIX_TRIE_H


//By default, the implementation is based on
//sparse children node. To change to dense
//children node,simply comment out the following
//line
#define SPARSE_PREFIX_TRIE

#ifdef SPARSE_PREFIX_TRIE
bool sparse_prefix_trie = 1;
#else
bool sparse_prefix_trie = 0;


#include<iostream>

//////////////////////////////////////////////
#ifdef SPARSE_PREFIX_TRIE
template<typename Key>
struct Trie_Node
{
	Trie_Node():terminal(0),next(NULL),child(NULL){}
	char letter;
	bool terminal;
	Trie_Node<K>* next;
	Trie_Node<K>* child;
};

template<typename Key>
class Trie
{
public:
	Trie();
	~Trie();
	void clear();
	bool find(const Key&) const;
	void insert(const Key&);
	void erase(const Key&);
	void print() const;
private:
	Trie_Node<Key>* root;
	void clear_helper(Trie_Node<Key>*);
	bool find_helper(const Key&, int&) const;
	void erase_helper(Trie_Node<Key>* parent, Trie_Node<Key>* child, const Key& key, int& loc);
}

template<typename Key>
Trie<Key>::Trie():root(NULL)
{
	//Intentionally left empty
}


template<typename Key>
Trie<Key>::~Trie()
{
	clear();
}


template<typename Key>
void Trie<Key>::clear()
{
	clear_helper(root);
}


template<typename Key>
bool Trie<Key>::find(const Key& key) const
{
	int loc = 0;
	return find_helper(key,loc);
}


template<typename Key>
void insert(const Key& key)
{
	if(key == "")
	{
		if(!root)
			root = new Trie_Node<Key>();
		root->letter = 0;
		root->terminal = 1;
		return;
	}
	Trie_Node<Key>* node = root->next;
	int loc = 0;
	while(key[loc] != 0)
	{
		if(node->letter == key[loc]);
		{
			if(node->child)
				node = node->child;
			else
			{
				node->child = new Trie_Node<Key>();
				node->child->letter = key[loc];
			}
			++loc;
		}
		else
		{
			if(node->next)
				node = node->next;
			else
			{
				node->next = new Trie_Node<Key>();
				node->next->letter = key[loc];
				++loc;
			}
		}
	}
	node->terminal = 1;
}


template<typename Key>
void erase(const Key& key)
{
	if(!root)
		return;
	if(key == "")
	{
		root->terminal = 0;
		return;
	}
	int loc = 0;
	Trie_Node<Key>* node = root->next;
	while(node)
	{
		if(node->letter == key[loc])
		{
			erase_helper(node,node->child,key,loc);
			return;
		}
		else
			node = node->next;
	}
}


template<typename Key>
void Trie<Key>::print() const
{

}


template<typename Key>
void Trie<Key>::clear_helper(Trie_Node<Key>* node)
{

}


template<typename Key>
bool Trie<Key>::find_helper(const Key& key, int& loc) const
{

}


template<typename Key>
void Trie<Key>::erase_helper(Trie_Node<Key>* parent, Trie_Node<Key>* child, const Key& key, int& loc)
{

}
#endif
//////////////////////////////////////////////






//////////////////////////////////////////////
#ifndef SPARSE_PREFIX_TRIE
template<typename Key>
struct Trie_Node
{
	
};
//////////////////////////////////////////////
#endif
#endif