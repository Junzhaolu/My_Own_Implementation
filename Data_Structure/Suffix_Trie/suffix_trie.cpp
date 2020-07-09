//*****************************************************************//
//This implementation is inspired by diagram on page 33 of:		   //
//https://www.cs.cmu.edu/~ckingsf/bioinfo-lectures/suffixtrees.pdf //
//-----------------------------------------------------------------//
//I feel that this implementation is simpler and easier than the   //
//Ukkonen's algorithm which was used to implement the same suffix  //
//trie in the 'suffix_trie_*flawed*.cpp' file.                     //
//*****************************************************************//


#include"suffix_trie.h"
#include<iostream>


Suffix_Trie::Suffix_Trie()
{
	root = new Trie_Node();
	added_word = "";
	global_label_end = -1;
}
	

Suffix_Trie::~Suffix_Trie()
{
	clear();
	delete root;
}


void Suffix_Trie::clear()
{
	clear_helper(root->next);
	root->next = NULL;
	added_word = "";
	global_label_end = -1;
}


void Suffix_Trie::insert(const std::string word)
{
	if(added_word == "")
	{
		added_word = word + '$';
		build();
	}
	else
	{
		added_word += (word + '$');
		rebuild_Trie();
	}
}


void Suffix_Trie::erase()
{
	clear();
}


void Suffix_Trie::erase(const std::string word)
{
	if(added_word == "")
		return;
	std::string word_copy = word + '$';
	for(short i = 0; i < (short)added_word.size(); ++i)
	{
		for(short j = 0; j < (short)word_copy.size(); ++j)		
		{
			if(added_word[i] != word_copy[j])
			{
				if(j == 0 && (i < (short)added_word.size() - 1))
					++i;
				else if(j == 0 && (i >= (short)added_word.size() - 1))
					return;
				j = -1;
			}
			else
			{
				if(j == (short)word_copy.size() - 1)
				{
					added_word = added_word.substr(0,i-j) + added_word.substr(i+1);
					if(i >= (short)added_word.size() - 1)
						added_word += '$';
					rebuild_Trie();
					return;
				}
				if(i < (short)added_word.size() - 1)
					++i;
				else
				{
					if(j < (short)word_copy.size() - 1)
						return;
				}
			}
		}
	}
}


void Suffix_Trie::print()
{
	std::string word = "";
	std::cout << std::endl << "Printing the suffix tree..." << std::endl;
	std::cout << "------------------------" << std::endl;
	print_helper(root->next,word);
	std::cout << "------------------------" << std::endl;
}


void Suffix_Trie::rebuild_Trie()
{
	std::string word_copy = added_word;
	clear();
	added_word = word_copy;
	build();
}


void Suffix_Trie::build()
{
	std::vector<std::pair<Trie_Node*,short>> suffix_link;
	short zero = 0;
	suffix_link.push_back(std::make_pair(root,zero));
	for(unsigned i = 0; i < added_word.size(); ++i)
	{
		++global_label_end;
		unsigned length = suffix_link.size();
		for(unsigned j = 0; j < length; ++j)
		{
			Trie_Node* node = suffix_link[j].first;
			short loc = suffix_link[j].second;
			if(loc != -1)
			{
				if(node == root)
				{
					Trie_Node* tmp = root->next, *tmp_prev = root;
					while(tmp && (added_word[tmp->label_begin] != added_word[i]))
					{
						tmp_prev = tmp;
						tmp = tmp->next;
					}
					if(tmp)
						suffix_link.push_back(std::make_pair(tmp,zero+1));
					else
					{
						tmp = new Trie_Node();
						tmp->label_begin = i;
						if(tmp_prev == root)
							root->next = tmp;
						else if(tmp_prev != root)
							tmp_prev->next = tmp;
					}
				}
				else
				{
					if(node->label_begin + loc > node->label_end)
					{
						loc -= (node->label_end - node->label_begin + 1);
						Trie_Node* tmp = node->child;
						while(tmp && (added_word[tmp->label_begin+loc] != added_word[i]))
						{
							node = tmp;
							tmp = tmp->next;
						}
						if(tmp)
						{
							suffix_link[j].first = tmp;
							suffix_link[j].second = loc + 1;
						}	
						else
						{
							Trie_Node* new_node = new Trie_Node();
							new_node->label_begin = i;
							node->next = new_node;
							suffix_link[j].second = -1;
						}
					}
					else
					{
						if(added_word[node->label_begin+loc] == added_word[i])
							suffix_link[j].second += 1;
						else
						{
							Trie_Node* new_node1 = new Trie_Node(), *new_node2 = new Trie_Node();
							new_node1->label_begin = node->label_begin + loc;
							new_node1->child = node->child;
							new_node1->next = new_node2;
							new_node1->label_end = node->label_end;
							new_node2->label_begin = i;
							node->child = new_node1;
							node->label_end = node->label_begin + loc - 1;
							suffix_link[j].second = -1;
						}
					}
				}
			}
		}
	}
}


void Suffix_Trie::clear_helper(Trie_Node* node)
{
	if(!node)
		return;
	clear_helper(node->child);
	clear_helper(node->next);
	delete node;
}


void Suffix_Trie::print_helper(Trie_Node* node, std::string word)
{
	if(!node)
		return;
	std::string this_nodes_word = "";
	if(node->label_end > global_label_end)
		this_nodes_word = added_word.substr(node->label_begin,global_label_end - node->label_begin + 1);
	else
		this_nodes_word = added_word.substr(node->label_begin,node->label_end - node->label_begin + 1);
	if(!node->child)
		std::cout << word + this_nodes_word << std::endl;
	else
		print_helper(node->child,word+this_nodes_word);
	if(node->next)
		print_helper(node->next,word);
}