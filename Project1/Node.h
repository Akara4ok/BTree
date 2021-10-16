#pragma once
#include<vector>
#include<string>

struct Node
{
	int numberOfChilds = 0;
	std::vector<Node*> nodes_ptr;
	std::vector<std::pair<int, std::string>> data;
	Node() {};
	void makePtrs() {
		for (int i = 0; i < data.size() + 1; i++)
		{
			nodes_ptr.push_back(nullptr);
		}
	}
	void addChild(Node* child, int ind)
	{
		nodes_ptr[ind] = child;
	};
	int search(int key)
	{

	}
};

