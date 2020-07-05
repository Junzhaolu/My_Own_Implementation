#include"suffix_trie.h"
#include<string>
#include<vector>

Suffix_Trie::Suffix_Trie():root(new Trie_Node()),added_word(""),global_label_end(0)
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
		internal_build();
	}	
}


void Suffix_Trie::erase()
{
	clear();
}


void Suffix_Trie::erase(const std::string word)
{

}


void Suffix_Trie::print() const
{
	std::cout << "Printing..." << std::endl;
	std::cout << "-------------" << std::endl;
	print_helper(root);
	std::cout << "-------------" << std::endl;
}


void Suffix_Trie::rebuild_Trie()
{
	clear();
	insert(added_word);
}


void Suffix_Trie::internal_build()
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
			if(active_node == root && active_length == 0)
			{
				Trie_Node* tmp = root->next, tmp_prev = root;
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
				else
				{
					tmp = new Trie_Node();
					tmp->label_begin = i;
					tmp_prev->next = tmp;
					--remainder;
				}
			}
			else if(active_node == root && active_length ！= 0)
			{
				Trie_Node* edge = find_edge(root,active_edge);
				if(!build_helper(edge,active_length + edge->label_begin,suffix_nodes_record))
				{
					++active_length;
					break;
				}
				else
				{
					--remainder;
					while(active_length >= 1)
					{
						--active_length;
						++active_edge;
						edge = find_edge(root,active_edge);
						if(!build_helper(edge,active_length-edge->label_begin,suffix_nodes_record))
						{
							++active_length;
							break;
						}
						else
							--remainder;
					}
				}
			}
			else if(active_node != root)
			{
				active_node = active_node->next_suffix;
				short target_loc = active_node->label_begin + active_length;
				Trie_Node* edge = find_edge(active_node,target_loc);
				while(target_loc > (edge->label_end - edge->label_begin + 1))
				{
					active_node = edge;
					edge = find_edge(active_node,target_loc);
				}
				if(added_word[target_loc] == added_word[i])
				{
					++active_length;
					break;
				}
				else
				{
					
				}
			}
		}
	}
}


void Suffix_Trie::clear_helper(Trie_Node* node)
{

}


void Suffix_Trie::print_helper(Trie_Node* node)
{

}


bool Suffix_Trie::build_helper(Trie_Node* edge,const short& target_loc,std::vector<Trie_Node*>& suffix_nodes_record)
{
	while(target_loc > edge->label_end)
	{
		active_node = edge;
		edge = find_edge(active_node,target_loc);
	}
	if(added_word[target_loc] == added_word[i])
		return false;
	else
	{
		Trie_Node* 1st_split = new Trie_Node(), *2nd_split = new Trie_Node();
		1st_split->label_begin = target_loc;
		1st_split->label_end = edge->label_end;
		2nd_split->label_begin = i;
		edge->next_suffix = root;
		if(suffix_nodes_record.size() >= 1)
			suffix_nodes_record.back()->next_suffix = edge;
		suffix_nodes_record.push_back(edge);
		edge->label_end = target_loc - 1;
		1st_split->child = edge->child;
		edge->child = 1st_split;
		1st_split->next = 2nd_split;
	}
	return true;
}