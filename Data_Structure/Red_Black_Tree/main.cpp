#include"rb.h"
#include<iostream>
#include<string>
int main()
{
	RB_TREE<unsigned int,std::string> rb;
	rb.insert(std::make_pair(10,"what"));
	rb.insert(std::make_pair(18,"what"));
	rb.insert(std::make_pair(7,"what"));
	rb.insert(std::make_pair(15,"what"));
	rb.insert(std::make_pair(16,"what"));
	rb.insert(std::make_pair(30,"what"));
	rb.insert(std::make_pair(25,"what"));
	rb.insert(std::make_pair(40,"what"));
	rb.insert(std::make_pair(60,"what"));
	rb.insert(std::make_pair(2,"what"));
	rb.insert(std::make_pair(1,"what"));
	rb.insert(std::make_pair(70,"what"));
	
	return 0;
}