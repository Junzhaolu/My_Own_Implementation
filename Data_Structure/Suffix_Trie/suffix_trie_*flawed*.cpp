//**************************************************//
//This suffix_trie is implemented using Ukkonen's   // 
//algorithm inspired by this video:				    //
//https://www.youtube.com/watch?v=aPRqocoBsFQ       //
//**************************************************//
//Note:In the above video, around 57:52, you will   //
//see that the author, Tushar Roy, made a mistake   //
//by not updating the "active_edge" and  		    //
//"active_length" when searching for the appropriate//
//edge. He also mistakenly thought that "root" was  //
//the active_node during this search.				//
//I realized this while I was looking for the "bug" //
//that causes my end result to be different from    //
//Tushar's.											//
//**************************************************//
//Therefore, I consider this implementation 	    //
//imperfect. I will start another implementation.	//
//**************************************************//

#include"suffix_trie.h"
#include<iostream>


Suffix_Trie::Suffix_Trie():root(new Trie_Node()),added_word(""),global_label_end(-1)
{
	//Left empty intentionally
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
}


void Suffix_Trie::insert(const std::string word)
{
	if(added_word != "")
	{
		added_word += (word + '$');
		rebuild_Trie();
	}
	else
	{
		added_word = word + '$';
		build();
	}	
}


void Suffix_Trie::erase()
{
	clear();
}


void Suffix_Trie::erase(const std::string word)
{
	std::string word_copy = word + '$';
	for(unsigned i = 0; i < added_word.size(); ++i)
	{
		if(added_word[i] == word_copy[0])
		{
			for(unsigned j = 1; j < word_copy.size(); ++j)
			{
				if(i+j >= added_word.size())
					return;
				if(added_word[i+j] != word_copy[j])
					break;
				if((j == word_copy.size()-1) && (added_word[i+j] == word_copy[j]))
				{
					added_word = added_word.substr(0,i) + added_word.substr(i+j+1);
					return;
				}
			}
		}
	}
}


void Suffix_Trie::print()
{
	std::cout << std::endl << "Printing Suffix Trie..." << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::string word = "";
	print_helper(root->next,word);
	std::cout << "-----------------------" << std::endl << std::endl;
}


void Suffix_Trie::rebuild_Trie()
{
	clear();
	insert(added_word);
}


void Suffix_Trie::build()
{
	short remainder = 0, active_length = 0, active_edge = -1;
	Trie_Node* active_node = root;
	for(unsigned i = 0; i < added_word.size(); ++i)
	{
		++remainder;
		++global_label_end;
		std::vector<Trie_Node*> suffix_nodes_record;
		while(remainder >= 1)
		{
			if(active_length == 0)
			{
				Trie_Node* tmp = root->next, *tmp_prev = root;
				while(tmp && (added_word[tmp->label_begin] != added_word[i]))
				{
					tmp_prev = tmp;
					tmp = tmp->next;
				}
				if(tmp)
				{
					active_edge = tmp->label_begin;
					++active_length;
					break;
				}
				tmp = new Trie_Node();
				tmp->label_begin = i;
				tmp_prev->next = tmp;
				--remainder;
			}
			else if(active_length >= 1)
			{
				Trie_Node* edge = find_edge(active_node,active_edge,active_length);
				if(!build_helper(edge,active_length,i,suffix_nodes_record))
				{
					++active_length;
					break;
				}
				else
				{
					--remainder;
					bool changed_node = 1;
					while(changed_node)
					{
						changed_node = 0;
						if(active_node == root)
						{
							while(active_length >= 1)
							{
								--active_length;
								++active_edge;
								if(active_length == 0)
									break;
								edge = find_edge(active_node,active_edge,active_length);
								if(!build_helper(edge,active_length,i,suffix_nodes_record))
								{
									++active_length;
									break;
								}
								else
									--remainder;
								if(active_node != root)
								{
									changed_node = 1;
									break;
								}
							}
						}
						else
						{
							while(active_node != root)
							{
								active_node = active_node->next_suffix;
								Trie_Node* edge = find_edge(active_node,active_edge,active_length);
								if(!build_helper(edge,active_length,i,suffix_nodes_record))
								{
									++active_length;
									break;
								}
								else
									--remainder;
							}
							--active_length;
							++active_edge;
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
	Trie_Node* tmp = node->next;
	delete node;
	clear_helper(tmp);
}


void Suffix_Trie::print_helper(Trie_Node* node, std::string word)
{
	std::string curr_node_word;
	if(node->label_end > global_label_end)
		curr_node_word = added_word.substr(node->label_begin,global_label_end-node->label_begin+1);
	else
		curr_node_word = added_word.substr(node->label_begin,node->label_end-node->label_begin+1);
	if(node->child)
		print_helper(node->child,word+curr_node_word);
	else
		std::cout << word + curr_node_word << std::endl;
	if(node->next)
		print_helper(node->next,word);
	else
		return;
}


bool Suffix_Trie::build_helper(Trie_Node* edge, const short& target_loc, const unsigned& compare, std::vector<Trie_Node*>& suffix_nodes_record)
{
	if(added_word[edge->label_begin+target_loc] == added_word[compare])
		return false;
	else
	{
		Trie_Node* first_split = new Trie_Node(), *second_split = new Trie_Node();
		first_split->label_begin = edge->label_begin+target_loc;
		first_split->label_end = edge->label_end;
		second_split->label_begin = compare;
		edge->next_suffix = root;
		if(suffix_nodes_record.size() >= 1)
			suffix_nodes_record.back()->next_suffix = edge;
		suffix_nodes_record.push_back(edge);
		edge->label_end = edge->label_begin + target_loc - 1;
		first_split->child = edge->child;
		edge->child = first_split;
		first_split->next = second_split;
		return true;
	}
}


Trie_Node* Suffix_Trie::find_edge(Trie_Node*& node, short& edge, short& length)
{
	Trie_Node* tmp;
	short old_i = 0, old_edge = edge;
	if(node == root)
		tmp = node->next;
	else
		tmp = node->child;
	for(short i = 0; i <= length; ++i)
	{
		if(tmp)
		{
			if(added_word[tmp->label_begin+i] != added_word[old_edge + i + old_i])
			{
				--i;
				tmp = tmp->next;
			}
			else if(added_word[tmp->label_begin+i] == added_word[old_edge + i + old_i])
			{
				if((i+1 <= length) && (tmp->label_begin+i+1 > tmp->label_end))
				{
					old_i = i + 1;
					i = -1;
					node = tmp;
					length -= (tmp->label_end - tmp->label_begin + 1); 
					tmp = tmp->child;
					edge = tmp->label_begin;
				}
			}
		}
	}
	return tmp;
}