#include"rb.h"

RB_TREE::RB_TREE():root(NULL)
{
}

RB_TREE::~RB_TREE()
{
	erase();
}

void RB_TREE::erase()
{
	erase_helper(root);
}

void RB_TREE::insert(unsigned short val)
{
	
}