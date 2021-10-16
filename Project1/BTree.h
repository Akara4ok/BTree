#pragma once
#include"Node.h"
#include"IOClass.h"

class BTree
{
	int t;
	Node* root = new Node;
public:
	BTree(int t);
	BTree(std::string path);
	void writeToFile(std::string path);
};