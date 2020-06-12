#include"rb.h"
#include<algorithm>

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

void RB_TREE::remove(unsigned short val)
{

}

bool RB_TREE::find(unsigned short val)
{
	RB_Node* tmp = find_Node(val);
	if(tmp)
		return true;
	else
		return false;
}

bool RB_TREE::is_Valid_RB_Tree()
{
	if(!root)
		return true;
	return valid_Helper1(root);
}

bool RB_TREE::valid_Helper1(RB_Node* node)
{
	if(!node)
		return true;
	unsigned int left_Count = valid_Helper2(node->left_Child),
				 right_Count = valid_Helper2(node-right_Child);
	if(left_Count != right_Count)
		return false;
	else
		return valid_Helper1(node->left_Child)
		   	   && valid_Helper1(node->right_Child);
}

unsigned int RB_TREE::valid_Helper2(RB_Node* node)
{

}

RB_Node* RB_TREE::find_Node(unsigned short val)
{
	RB_Node* tmp = root;
	while(!tmp)
	{
		if(tmp->value == val)
			return tmp;
		else if(tmp->value < val)
			tmp = tmp->right_Child;
		else if(tmp->value > val)
			tmp = tmp->left_Child;
	}
	return NULL;
}

