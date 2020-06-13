#include"rb.h"
#include<algorithm>
#include<vector>

//By default, successor is used
#define REMOVE_REPLACED_BY_SUCESSOR
//If want to use predecessor, comment-out
//the line above

bool rm_by_succ = 0;
#ifdef REMOVE_REPLACED_BY_SUCESSOR
rm_by_succ = 1;
#endif

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
	root = NULL;
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
	if(prev->is_Red)
		insert_Fix(prev,curr);
}

void RB_TREE::remove(unsigned short val)
{
	RB_Node* node = find_Node(val);
	if(!node)
		return;
	if(node == root && (!(node->left_Child) && !(node->right_Child)))
	{
		delete node;
		root = NULL;
		return;
	}
	bool node_is_lf = 0;
	if(node == node->parent->left_Child)
		node_is_lf = 1;
	if(!(node->left_Child) && !(node->right_Child))
	{
		if(node->is_Red)
		{
			if(node_is_lf)
				node->parent->left_Child = NULL;
			else
				node->parent->right_Child = NULL;
			delete node;
			return;
		}
		else
		{
			node->double_Black = 1;
			remove_Fix(node);
		}
	}
	else if(!(node->left_Child) && node->right_Child)
	{
		if(node->is_Red)
		{
			if(node_is_lf)
				node->parent->left_Child = node->right_Child;
			else
				node->parent->right_Child = node->right_Child;
			node->right_Child->parent = node->parent;
			delete node;
			return;
		}
		else
		{
			node->value = right_Child->value;
			remove_Fix(node->right_Child);
		}
	}
	else if(node->left_Child && !(node->right_Child))
	{
		if(node->is_Red)
		{
			if(node_is_lf)
				node->parent->left_Child = node->left_Child;
			else
				node->parent->right_Child = node->left_Child;
			node->left_Child->parent = node->parent;
			delete node;
			return;
		}
		else
		{
			node->value = left_Child->value;
			remove_Fix(node->left_Child);
		}
	}
	else if(node->left_Child && node->right_Child)
	{
		//**********************************
		//See line#5 for how to modify
		RB_Node* replacement;
		if(rm_by_succ)
			replacement = successor(node);
		else if(!rm_by_succ)
			replacement = predecessor(node);
		//**********************************
		node->value = replacement->value;
		remove_Fix(replacement);
	}
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

void RB_TREE::insert_Fix(RB_Node* p,RB_Node* c)
{
	if(p == root)
		return;
	RB_Node* sibling = get_Sibling(p);
	bool re_color = 0, re_structure = 0;
	if(!sibling)
		re_structure = 1;
	else
	{
		if(!(sibling->is_Red))
			re_structure = 1;
		else
			re_color = 1;
	}
	if(re_color && !re_structure)
	{
		p->is_Red = 0;
		sibling->is_Red = 0;
		if(p->parent == root)
			return;
		else
		{
			if(p->parent->is_Red)
				p->parent->is_Red = 0;
			else
				p->parent->is_Red = 1;
			insert_Fix(p->parent,p);
		}
	}
	else if(!re_color && re_structure)
	{
		bool p_is_lf = 0,c_is_lf = 0;
		if(p == p->parent->left_Child)
			p_is_lf = 1;
		if(c == p->left_Child)
			c_is_lf = 1;
		if(p_is_lf && c_is_lf)
			rotate_Right(p->parent);
		else if(p_is_lf && !c_is_lf)
		{
			RB_Node* gp = p->parent;
			rotate_Left(p);
			rotate_Right(gp);
		}
		else if(!p_is_lf && c_is_lf)
		{
			RB_Node* gp = p->parent;
			rotate_Right(p);
			rotate_Left(gp);
		}
		else if(!p_is_lf && !c_is_lf)
			rotate_Left(p->parent);
	}
}

void RB_TREE::remove_Fix(RB_Node* node)
{
	if(node == root)
	{
		if(node->double_Black)
		{
			node->double_Black = 0;
			return;
		}
		//**********************************
		//See line#5 for how to modify
		RB_Node* replacement;
		if(rm_by_succ)
			replacement = successor(node);
		else if(!rm_by_succ)
			replacement = predecessor(node);
		//**********************************
		node->value = replacement->value;
		remove_Fix(replacement);
		return;
	}
	bool node_is_lf = 0;
	if(node->is_Red)
	{

	}
	else
	{

	}
}

void RB_TREE::rotate_Left(RB_Node* node)
{
	if(node->right_Child)
	{
		RB_Node* tmp = node->right_Child;
		node->right_Child = tmp->left_Child;
		if(tmp->left_Child)
			tmp->left_Child->parent = node;
		tmp->left_Child = node;
		if(node->parent)
		{
			if(node == node->parent->right_Child)
				node->parent->right_Child = tmp;
			else
				node->parent->left_Child = tmp;
		}
		else
			root = tmp;
		tmp->parent = node->parent;
		node->parent = tmp;	
		swap_Color(tmp,node);
	}
}

void RB_TREE::rotate_Right(RB_Node* node)
{
	if(node->left_Child)
	{
		RB_Node* tmp = node->left_Child;
		if(tmp->right_Child)
			tmp->right_Child->parent = node;
		node->left_Child = tmp->right_Child;
		if(node->parent)
		{
			if(node == node->parent->left_Child)
				node->parent->left_Child = tmp;
			else
				node->parent->right_Child = tmp;
		}
		else
			root = tmp;
		tmp->parent = node->parent;
		node->parent = tmp;
		tmp->right_Child = node;
		swap_Color(tmp,node);
	}
}

void swap_color(RB_Node* node1,RB_Node* node2)
{
	bool color = node1->is_Red;
	node1->is_Red = node2->is_Red;
	node2->is_Red = color;
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