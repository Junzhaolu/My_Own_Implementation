//**************************************
//For now, this trie is for the string 
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
#endif

#include<iostream>
#include<string>

//////////////////////////////////////////////
#ifdef SPARSE_PREFIX_TRIE
template<typename Key>
struct Trie_Node
{
	Trie_Node():terminal(0),next(NULL),child(NULL){}
	char letter;
	bool terminal;
	Trie_Node<Key>* next;
	Trie_Node<Key>* child;
};


template<typename Key>
class Trie
{
public:
	Trie();
	~Trie();
	void clear();
	bool find(const Key) const;
	void insert(const Key);
	void erase(const Key);
	void print() const;
private:
	Trie_Node<Key>* root;
	void clear_helper(Trie_Node<Key>*);
	bool find_helper(const Key&, unsigned int&) const;
	bool erase_helper(Trie_Node<Key>*, const Key&, unsigned int&);
	void print_helper(Trie_Node<Key>*, std::string) const;
};


template<typename Key>
Trie<Key>::Trie()
{
	root = new Trie_Node<Key>();
	root->letter = ' ';
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
	root = NULL;
}


template<typename Key>
bool Trie<Key>::find(const Key key) const
{
	unsigned int loc = 0;
	return find_helper(key,loc);
}


template<typename Key>
void Trie<Key>::insert(const Key key)
{
	Trie_Node<Key>* node = root;
	unsigned int loc = 0;
	while(loc != key.size())
	{
		if(node->letter == key[loc])
		{
			++loc;
			if(loc == key.size())
			{
				node->terminal = 1;
				return;
			}
			if(node->child)
				node = node->child;
			else
			{
				node->child = new Trie_Node<Key>();
				node->child->letter = key[loc];
				node = node->child;
			}
		}
		else
		{
			if(node->next)
				node = node->next;
			else
			{
				node->next = new Trie_Node<Key>();
				node->next->letter = key[loc];
				node = node->next;
			}
		}
	}
}


template<typename Key>
void Trie<Key>::erase(const Key key)
{
	Trie_Node<Key>* node = root;
	if(!node)
		return;
	if(key == " ")
	{
		node->terminal = 0;
		return;
	}
	unsigned int loc = 0;
	while(node->next)
	{
		if(node->next->letter != key[loc] && node->next->next)
			node = node->next;
		else if(node->next->letter != key[loc] && !node->next->next)
			return;
		else if(node->next->letter == key[loc])
		{
			++loc;
			Trie_Node<Key>* tmp = node->next->next;
			if(loc == key.size())
			{
				if(node->next->child)
				{
					if(node->next->terminal)
						node->next->terminal = 0;
					return;
				}
				else
				{
					delete node->next;
					node->next = tmp;
				}
			}
			else
			{
				if(erase_helper(node->next,key,loc))
				{
					if(!node->next->terminal)
					{
						delete node->next;
						node->next = tmp;
					}
				}
				return;
			}
		}
	}
}


template<typename Key>
void Trie<Key>::print() const
{
	if(!root->next)
		std::cout << std::endl << "The trie is empty" << std::endl;
	else
	{
		std::cout << std::endl << "Printing... " << std::endl;
		std::cout << "----------------" << std:: endl;
		std::string tmp = "";
		print_helper(root,tmp);
	}
	std::cout << "----------------" << std:: endl;
}


template<typename Key>
void Trie<Key>::clear_helper(Trie_Node<Key>* node)
{
	if(!node)
		return;
	if(node->child)
		clear_helper(node->child);
	Trie_Node<Key>* tmp = node->next;
	delete node;
	if(tmp)
		clear_helper(tmp);
}


template<typename Key>
bool Trie<Key>::find_helper(const Key& key, unsigned int& loc) const
{
	if(key == " ")
	{
		if(root->terminal)
			return true;
		return false;
	}
	Trie_Node<Key>* node = root->next;
	while(node)
	{
		if(node->letter == key[loc])
		{
			++loc;
			if(loc == key.size())
			{
				if(node->terminal)
					return true;
				return false;				
			}
			node = node->child;
		}
		else
			node = node->next;
	}
	return false;
}


template<typename Key>
bool Trie<Key>::erase_helper(Trie_Node<Key>* node, const Key& key, unsigned int& loc)
{
	if(node->child)
	{
		if(node->child->letter == key[loc])
		{
			++loc;
			if(loc == key.size())
			{
				if(node->child->child && (node->child->next || !node->child->next))
				{
					node->child->terminal = 0;
					return false;
				}
				else if(!node->child->child && node->child->next)
				{
					Trie_Node<Key>* tmp = node->child->next;
					delete node->child;
					node->child = tmp;
					return false;
				}
				else if(!node->child->child && !node->child->next)
				{
					delete node->child;
					return true;
				}
			}
			else
			{
				if(erase_helper(node->child,key,loc))
				{
					if(node->child->next)
					{
						if(!node->child->terminal)	
						{
							Trie_Node<Key>* tmp = node->child->next;
							delete node->child;
							node->child = tmp;
						}
						else
							node->child->child = NULL;
						return false;
					}
					else
					{
						if(node->child->terminal)
						{
							node->child->child = NULL;
							return false;
						}
						delete node->child;
						return true;
					}
				}
				else
					return false;
			}
		}
		else
		{
			Trie_Node<Key>* tmp = node->child,
						   *tmp_prev = NULL;
			while(tmp && (tmp->letter != key[loc]))
			{
				tmp_prev = tmp;
				tmp = tmp->next;
			}
			if(tmp)
			{
				++loc;
				if(loc == key.size())
				{
					if(tmp->child && (tmp->next || !tmp->next))
					{
						tmp->terminal = 0;
					}
					else if(!tmp->child && tmp->next)
					{
						Trie_Node<Key>* next = tmp->next;
						delete tmp;
						tmp_prev->next = next;
					}
					else if(!tmp->child && !tmp->next)
					{
						delete tmp;
						tmp_prev->next = NULL;
					}
				}
				else
				{
					if(erase_helper(tmp,key,loc))
					{
						if(!tmp->terminal)
						{
							tmp_prev->next = tmp->next;
							delete tmp;
						}
						else
							tmp->child = NULL;
					}
				}
			}
			return false;
		}
	}
	return false;
}


template<typename Key>
void Trie<Key>::print_helper(Trie_Node<Key>* node, std::string word) const
{
	if(node->terminal)
		std::cout << word << node->letter << std::endl;
	if(node->child)
		print_helper(node->child,word+node->letter);
	if(node->next)
		print_helper(node->next,word);
}


#endif
//////////////////////////////////////////////






//////////////////////////////////////////////
#ifndef SPARSE_PREFIX_TRIE
template<typename Key>
struct Trie_Node
{
	
};
#endif
//////////////////////////////////////////////

#endif