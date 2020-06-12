#include"rb.h"
#include<algorithm>
#include<vector>

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
	if(!root)
	{
		root = new RB_Node(NULL,val);
		root->is_Red = 0;
		return;
	}
	RB_Node* prev = root, *curr = NULL;
	curr = find_Node(val);
	if(curr)
		return;
	curr = root;
	while(curr)
	{
		prev = curr;
		if(val < curr->value)
			curr = curr->left_Child;
		else
			curr = curr->right_Child;
	}
	curr = new RB_Node(prev,val);
	if(prev->value > val)
		prev->left_Child = curr;
	else
		prev->right_Child = curr;
	insert_Fix(curr);
}

void RB_TREE::remove(unsigned short val)
{
	RB_Node* node = find_Node(val);
	if(!node)
		return;
	
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
	std::vector<unsigned int> vec;	
	valid_Helper(root,vec,0);
	unsigned int size = vec[0].size();
	for(unsigned i = 1; i < vec.size(); ++i)
	{
		if(vec[i].size() != size)
			return false;
	}
	return true;
}

void RB_TREE::valid_Helper(RB_Node* node,std::vector<unsigned int>& vec,unsigned int& path)
{
	if(!node)
		return;
	if(!(node->is_Red))
		++vec[path];
	if(node->left_Child && node->right_Child)
	{
		valid_Helper(node->left_Child,vec,path);
		vec[path+1] = vec[path];
		valid_Helper(node->left_Child,vec,++path);
	}
	else
	{
		if(node->left_Child)
			valid_Helper(node->left_Child,vec,path);
		else
			valid_Helper(node->right_Child,vec,path);
	}
}

RB_Node* RB_TREE::find_Node(unsigned short val)
{
	RB_Node* tmp = root;
	while(tmp)
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

void RB_TREE::erase_Helper(RB_Node* node)
{
	if(!node)
		return;
	erase_helper(node->left_Child);
	erase_helper(node->right_Child);
	delete node;
}

void RB_TREE::insert_Fix(RB_Node* node)
{

}

void RB_TREE::remove_Fix(RB_Node* node)
{

}

void RB_TREE::rotate_Left(RB_Node* node)
{

}

void RB_TREE::rotate_Right(RB_Node* node)
{
	
}

RB_Node* RB_TREE::get_Sibling(RB_Node* node)
{
	if(!node)
		return NULL;
	if(!(node->parent))
		return NULL;
	if(node == node->parent->left_Child)
		return node->parent->right_Child;
	else
		return node->parent->left_Child;
}

RB_Node* RB_TREE::predecessor(RB_Node* node)
{
	if(!node)
		return NULL;
	RB_Node* tmp = node->left_Child;
	if(!tmp) 
	{
		if(!(node->right_Child) && node->parent)
		{
			if(node == node->parent->right_Child)
				return node->parent;
		}
		else if(node->right_Child && node->parent)
			return NULL;
	}	
	while(tmp->right_Child)
		tmp = tmp->right_Child;
	return tmp;
}

RB_Node* RB_TREE::successor(RB_Node* node)
{
	if(!node)
		return NULL;
	RB_Node* tmp = node->right_Child;
	if(!tmp)
	{
		if(!(node->left_Child) && node->parent)
		{
			if(node == node->parent->left_Child)
				return node->parent;
		}
		else if(node->left_Child && node->parent)
			return NULL;
	}
	while(tmp->left_Child)
		tmp = tmp->left_Child;
	return tmp;
}