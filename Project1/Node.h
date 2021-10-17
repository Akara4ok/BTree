#pragma once
#include<vector>
#include<string>

struct Node
{
	std::vector<Node*> nodes_ptr;
	std::vector<std::pair<int, std::string>> data;
	Node() {};
	Node(std::pair<int, std::string> p) {
		data.push_back(p);
		nodes_ptr.push_back(nullptr);
		nodes_ptr.push_back(nullptr);
	};
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
	int searchKey(int key)
	{
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i].first == key)
			{
				return i;
			}
		}
		return -1;
	};
	int searchInterval(int key, bool& isKeyInTree)
	{
		if (key < data[0].first)
		{
			return 0;
		}
		for (int i = 0; i < data.size() - 1; i++)
		{
			if (data[i].first <= key && key < data[i + 1].first)
			{
				if (data[i].first == key)
					isKeyInTree = true;
				return i + 1;
			}
		}
		if (key >= data.back().first)
		{
			if (data.back().first == key)
				isKeyInTree = true;
			return data.size();
		}
	};
	void insertPair(std::pair<int, std::string> p, int interval)
	{
		data.push_back(std::pair<int, std::string>());
		nodes_ptr.push_back(nullptr);
		for (int i = data.size() - 1; i > interval; i--)
		{
			data[i] = data[i - 1];
			nodes_ptr[i + 1] = nodes_ptr[i];
		}
		data[interval] = p;
		nodes_ptr[interval + 1] = nodes_ptr[interval];
	}
	Node* split()
	{
		Node* node = new Node;
		for (int i = (data.size() + 1) / 2; i < data.size(); i++)
		{
			node->data.push_back(data[i]);
			node->nodes_ptr.push_back(nodes_ptr[i]);
		}
		node->nodes_ptr.push_back(nodes_ptr[data.size()]);

		for (int i = data.size() - 1; i > (data.size() + 1) / 2; i--)
		{
			data.pop_back();
			nodes_ptr.pop_back();
		}
		return node;
	}
	void join(Node* n)
	{
		nodes_ptr.back() = n->nodes_ptr[0];
		for (int i = 1; i < n->nodes_ptr.size(); i++)
		{
			nodes_ptr.push_back(n->nodes_ptr[i]);
			data.push_back(n->data[i - 1]);
		}
	}
};

