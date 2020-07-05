#ifndef SUFFIX_TRIE_H
#define SUFFIX_TRIE_H

#include<limits>
struct Trie_Node
{
	Trie_Node():suffix_sequence(-1),
			    label_begin(-1),
			    label_end(std::numeric_limits<short>::max()),
			    next(NULL),
			    child(NULL),
			    next_suffix(NULL){}
	short suffix_sequence;
	short label_begin;
	short label_end;
	Trie_Node* next;
	Trie_Node* child;
	Trie_Node* next_suffix;
};

class Suffix_Trie
{
public:
	Suffix_Trie();
	~Suffix_Trie();
	void clear();
	void insert(const std::string);
	void erase();
	void erase(const std::string);
	void print() const;
	void rebuild_Trie();
private:
	Trie_Node* root;
	std::string added_word;
	short global_label_endl;
	void internal_build();
	bool build_helper(Trie_Node*,const short&,std::vector<Trie_Node*>&);
	void clear_helper(Trie_Node*);
	void print_helper(Trie_Node*);
	Trie_Node* find_edge(Trie_Node*, const short&) const;
};

#endif