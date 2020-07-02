#include "prefix_trie.h"

int main()
{
	Trie<std::string> t;
	t.insert("w");
	t.insert("we");
	t.insert("what");
	t.insert("weed");
	t.insert("wed");
	t.insert("ok");
	t.insert("ko");
	t.insert("mc");
	t.insert("cm");
	t.insert("mm");
	t.insert("mmm");
	t.print();
	std::cout << t.find("weed");
	return 0;
}