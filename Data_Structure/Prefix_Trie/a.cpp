#include "prefix_trie.h"

int main()
{
	std::string x = "abaaba$";
	Trie<std::string> t;
	for(unsigned i = 0; i < x.size(); ++i)
		t.insert(x.substr(0+i,x.size()));
	t.print();
	return 0;
}