#include<iostream>
#include"splay_tree.h"
#include<string>
#include<cstdlib>
#include<ctime>
int main()
{
	std::string x = "what";
	Splay_Tree<int,std::string> s;
	s.insert(std::make_pair(15,x));
	s.print();
	s.insert(std::make_pair(25,x));
	s.print();
	s.insert(std::make_pair(5,x));
	s.print();
	s.insert(std::make_pair(10,x));
	s.print();
	s.erase(12);
	s.print();
	return 0;
}