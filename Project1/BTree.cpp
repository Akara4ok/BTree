#include "BTree.h"
#include<Queue>
#include<iostream>

BTree::BTree(int t)
{
	this->t = t;
}

BTree::BTree(std::string path)
{
	IOClass io(path, 0);
	io.parseFirstLine(t);
	std::queue<Node*> q;
	Node* node = new Node;
	io.parseNode(node->data);
	int numberOfChilds;
	Node* temp = new Node;
	if (!io.isEnd)
	{
		root = node;
		root->makePtrs();
		q.push(root);
		temp = root;
		numberOfChilds = root->data.size() + 1;
		bool isEnd = false;
	}
	while (!q.empty() && !io.isEnd)
	{
		if (numberOfChilds == 0)
		{
			q.pop();
			if (!q.empty())
			{
				temp = q.front();
				numberOfChilds = temp->data.size() + 1;
			}
			continue;
		}
		Node* node = new Node;
		io.parseNode(node->data);
		if (!io.isEnd)
		{
			node->makePtrs();
			temp->addChild(node, temp->data.size() + 1 - numberOfChilds);
			q.push(node);
		}
		numberOfChilds--;
	}
}

void BTree::writeToFile(std::string path)
{
	IOClass io(path, 1);
	io.writeFirstLine(t);
	if (root != nullptr)
	{
		std::queue<Node*> q;
		io.writeNode(root->data);
		q.push(root);
		Node* temp;
		while (!q.empty())
		{
			temp = q.front();
			for (int i = 0; i < temp->data.size() + 1; i++)
			{
				if (temp->nodes_ptr[i] != nullptr)
				{
					io.writeNode(temp->nodes_ptr[i]->data);
					q.push(temp->nodes_ptr[i]);
				}
			}
			q.pop();
		}
	}
}

void BTree::push(std::pair<int, std::string> p)
{
	if (root == nullptr)
		root = new Node(p);
	else
		push(p, root);
}

Node* BTree::push(std::pair<int, std::string>& p, Node*& subRoot)
{
	bool isKeyInTree = false;
	int interval = subRoot->searchInterval(p.first, isKeyInTree);
	if (isKeyInTree == true)
	{
		std::cout << "This key has already written";
		return nullptr;
	}
	if (subRoot->nodes_ptr[interval] != nullptr)
	{
		Node* node;
		node = push(p, subRoot->nodes_ptr[interval]);
		if (node == nullptr)
			return nullptr;
		else
		{
			subRoot->insertPair(p, interval);
			subRoot->nodes_ptr[interval + 1] = node;
			if (subRoot->data.size() < 2 * t - 1)
			{
				return nullptr;
			}
			else
			{
				Node* node;
				node = subRoot->split();
				p = subRoot->data.back();
				subRoot->data.pop_back();
				subRoot->nodes_ptr.pop_back();
				if (subRoot == root)
				{
					Node* newRoot = new Node(p);
					newRoot->addChild(subRoot, 0);
					newRoot->addChild(node, 1);
					root = newRoot;
					return nullptr;
				}
				{
					return node;
				}
			}
		}
	}
	else
	{
		subRoot->insertPair(p, interval);
		if (subRoot->data.size() < 2 * t - 1)
		{
			return nullptr;
		}
		else
		{
			Node* node;
			node = subRoot->split();
			p = subRoot->data.back();
			subRoot->data.pop_back();
			subRoot->nodes_ptr.pop_back();
			if (subRoot == root)
			{
				Node* newRoot = new Node(p);
				newRoot->addChild(subRoot, 0);
				newRoot->addChild(node, 1);
				root = newRoot;
				return nullptr;
			}
			{
				return node;
			}
		}
	}
}

std::string BTree::search(int key)
{
	std::string result;
	search(key, root, result);
	return result;
}

void BTree::search(int key, Node*subRoot, std::string& result)
{
	if (subRoot == nullptr)
		result = "No the match";
	else
	{
		bool isKeyInTree = false;
		int interval = subRoot->searchInterval(key, isKeyInTree);
		if (isKeyInTree)
		{
			result = subRoot->data[interval - 1].second;
		}
		else
		{
			search(key, subRoot->nodes_ptr[interval], result);
		}
	}
}
