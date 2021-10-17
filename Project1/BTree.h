#pragma once
#include"Node.h"
#include"IOClass.h"

class BTree
{
	int t;
public:
	Node* root = nullptr;
	BTree(int t);
	BTree(std::string path);
	void writeToFile(std::string path);
	void push(std::pair<int, std::string> p);
	Node* push(std::pair<int, std::string>& p, Node*& subRoot);
	std::string search(int key);
	void search(int key, Node* subRoot, std::string& result);
	void erase(int key);
	int erase(int&key, Node*& subRoot, int dir, std::pair<int, std::string>& p);
};