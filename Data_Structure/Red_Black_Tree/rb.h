#ifndef RB_H
#define RB_H

struct RB_Node
{
	RB_Node(RB_Node* p,unsigned short val):is_Red(1),
										   double_Black(0),
										   value(val),
						  				   parent(p),
										   right_child(NULL),
										   left_child(NULL){}
	bool is_Red;
	bool double_Black;
	//*******************
	unsigned short value;
	//*******************
	RB_Node* parent;
	RB_Node* right_Child;
	RB_Node* left_Child;
};

class RB_TREE
{
public:
	RB_TREE();
	~RB_TREE();
	void erase();
	//*******************************
	void insert(unsigned short);
	void remove(unsigned short);
	bool find(unsigned short);
	//*******************************
	void print_Tree();
private:
	RB_Node* root;
	bool is_Valid_RB_Tree();
	bool valid_Helper(RB_Node*);
	void erase_helper(RB_Node*);
	void insert_fix(RB_Node*);
	void remove_fix(RB_Node*);
	void rotate_Left(RB_Node*);
	void rotate_Right(RB_Node*);
	RB_Node* get_Sibling(RB_Node*);
	RB_Node* find_Node(unsigned short);
	RB_Node* predecessor(RB_Node*);
	RB_Node* successor(RB_Node*);
};
#endif RB_H