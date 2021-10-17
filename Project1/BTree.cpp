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


/*int BTree::erase(int& key, Node*& subRoot)
{
	if(subRoot!=nullptr)
	{
		bool isKeyInTree = false;
		int interval = subRoot->searchInterval(key, isKeyInTree);
		if (isKeyInTree)
		{
			if (subRoot->nodes_ptr[interval]==nullptr)
			{
				if (subRoot->data.size() > t - 1)
				{
					subRoot->data.erase(subRoot->data.begin() + interval - 1);
					subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval);
					return -1;
				}
				else
				{
					return interval;
				}
			}
			else
			{
				if (subRoot->nodes_ptr[interval - 1]->data.size() > t - 1 || subRoot->nodes_ptr[interval]->data.size() > t - 1)
				{
					if (subRoot->nodes_ptr[interval - 1]->data.size() > t - 1)
					{
						subRoot->data[interval - 1] = subRoot->nodes_ptr[interval - 1]->data.back();
						key = subRoot->nodes_ptr[interval - 1]->data.back().first;
						erase(key, subRoot->nodes_ptr[interval - 1]);
					}
					else
					{
						subRoot->data[interval - 1] = subRoot->nodes_ptr[interval]->data[0];
						key = subRoot->nodes_ptr[interval]->data[0].first;
						erase(key, subRoot->nodes_ptr[interval]);
					}
				}
				else
				{
					subRoot->nodes_ptr[interval - 1]->data.push_back(subRoot->data[interval - 1]);
					subRoot->nodes_ptr[interval - 1]->nodes_ptr.push_back(nullptr);
					subRoot->nodes_ptr[interval - 1]->join(subRoot->nodes_ptr[interval]);
					subRoot->data.erase(subRoot->data.begin() + interval - 1);
					subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval);
					erase(key, subRoot->nodes_ptr[interval - 1]);
					if (subRoot->data.size() >= t - 1 || subRoot == root)
					{
						if (subRoot->nodes_ptr[interval - 1]->data.size() >= 2*t - 1)
						{
							Node* node;
							node = subRoot->nodes_ptr[interval - 1]->split();
							subRoot->data.push_back(subRoot->nodes_ptr[interval - 1]->data.back());
							subRoot->nodes_ptr.insert(subRoot->nodes_ptr.begin() + interval, node);
							subRoot->nodes_ptr[interval - 1]->data.pop_back();
							subRoot->nodes_ptr[interval - 1]->nodes_ptr.pop_back();
						}
						if (subRoot->data.size() == 0)
						{
							root = subRoot->nodes_ptr[0];
						}
					}
					else
					{
						return -2;
					}
				}
			}
		}
		else
		{
			int intervalInSubRoot;
			intervalInSubRoot = erase(key, subRoot->nodes_ptr[interval]);
			if (intervalInSubRoot > 0 || intervalInSubRoot == -2)
			{
				if (intervalInSubRoot != -2)
				{
					if ((interval >0 && subRoot->nodes_ptr[interval - 1]->data.size() > t - 1) || (interval < subRoot->data.size() && subRoot->nodes_ptr[interval + 1]->data.size() > t - 1))
					{
						if (interval > 0 && subRoot->nodes_ptr[interval - 1]->data.size() > t - 1)
						{
							subRoot->nodes_ptr[interval]->data.insert(subRoot->nodes_ptr[interval]->data.begin(), subRoot->data[interval - 1]);
							subRoot->nodes_ptr[interval]->nodes_ptr.insert(subRoot->nodes_ptr[interval]->nodes_ptr.begin(), subRoot->nodes_ptr[interval]->nodes_ptr[0]);
							erase(key, subRoot->nodes_ptr[interval]);
							subRoot->data[interval - 1] = subRoot->nodes_ptr[interval - 1]->data.back();
							subRoot->nodes_ptr[interval - 1]->data.pop_back();
						}
						else
						{
							subRoot->nodes_ptr[interval]->data.insert(subRoot->nodes_ptr[interval]->data.end(), subRoot->data[interval]);
							subRoot->nodes_ptr[interval]->nodes_ptr.insert(subRoot->nodes_ptr[interval]->nodes_ptr.end(), subRoot->nodes_ptr[interval]->nodes_ptr.back());
							erase(key, subRoot->nodes_ptr[interval]);
							subRoot->data[interval] = subRoot->nodes_ptr[interval + 1]->data[0];
							subRoot->nodes_ptr[interval + 1]->data.erase(subRoot->nodes_ptr[interval + 1]->data.begin());
						}
					}
					else
					{
						if (interval > 0)
						{
							subRoot->nodes_ptr[interval - 1]->data.push_back(subRoot->data[interval - 1]);
							subRoot->nodes_ptr[interval - 1]->nodes_ptr.push_back(nullptr);
							subRoot->nodes_ptr[interval - 1]->join(subRoot->nodes_ptr[interval]);
							subRoot->data.erase(subRoot->data.begin() + interval - 1);
							subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval);
							erase(key, subRoot->nodes_ptr[interval - 1]);
						}
						else
						{
							subRoot->nodes_ptr[interval]->data.push_back(subRoot->data[interval]);
							subRoot->nodes_ptr[interval]->nodes_ptr.push_back(nullptr);
							subRoot->nodes_ptr[interval]->join(subRoot->nodes_ptr[interval + 1]);
							subRoot->data.erase(subRoot->data.begin() + interval);
							subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval + 1);
							erase(key, subRoot->nodes_ptr[interval]);
						}
						
						if (subRoot->data.size() < t - 1 || subRoot != root)
							return -2;
						else
						{
							if (subRoot == root && subRoot->data.size() == 0)
							{
								root = subRoot->nodes_ptr[0];
							}
						}
					}
				}
				else
				{
					if (intervalInSubRoot == -2)
					{
						if (interval < subRoot->data.size())
						{
							subRoot->nodes_ptr[interval]->data.push_back(subRoot->data[interval]);
							subRoot->nodes_ptr[interval]->nodes_ptr.push_back(nullptr);
							subRoot->nodes_ptr[interval]->join(subRoot->nodes_ptr[interval + 1]);
							if (subRoot->nodes_ptr[interval]->data.size() >= 2 * t - 1)
							{
								Node* node;
								node = subRoot->nodes_ptr[interval]->split();
								subRoot->data[interval] = subRoot->nodes_ptr[interval]->data.back();
								subRoot->nodes_ptr[interval + 1] = node;
								subRoot->nodes_ptr[interval]->data.pop_back();
								subRoot->nodes_ptr[interval]->nodes_ptr.pop_back();
							}
							else
							{
								if (subRoot != root || subRoot->data.size() > 0)
								{
									subRoot->data.erase(subRoot->data.begin() + interval);
									subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval + 1);
								}
								else
								{
									root = subRoot->nodes_ptr[0];
								}
							}
						}
						else
						{
							subRoot->nodes_ptr[interval - 1]->data.push_back(subRoot->data[interval - 1]);
							subRoot->nodes_ptr[interval - 1]->nodes_ptr.push_back(nullptr);
							subRoot->nodes_ptr[interval - 1]->join(subRoot->nodes_ptr[interval]);
							if (subRoot->nodes_ptr[interval - 1]->data.size() >= 2 * t - 1)
							{
								Node* node;
								node = subRoot->nodes_ptr[interval - 1]->split();
								subRoot->data[interval - 1] = subRoot->nodes_ptr[interval - 1]->data.back();
								subRoot->nodes_ptr[interval] = node;
								subRoot->nodes_ptr[interval - 1]->data.pop_back();
								subRoot->nodes_ptr[interval - 1]->nodes_ptr.pop_back();
							}
							else
							{
								if (subRoot != root || subRoot->data.size() > 1)
								{
									subRoot->data.erase(subRoot->data.begin() + interval - 1);
									subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval - 1);
								}
								else
								{
									root = subRoot->nodes_ptr[0];
								}
							}
						}
					}
				}
			}
		}
	}
	return -1;
}
*/

void BTree::erase(int key)
{
	std::pair<int, std::string> p;
	erase(key, root, 0, p);
	if (root->data.size() == 0)
	{
		root = root->nodes_ptr[0];
	}
}

int BTree::erase(int& key, Node*& subRoot, int dir, std::pair<int, std::string>& p)
{
	if (subRoot != nullptr)
	{
		bool isKeyInTree = false;
		int interval = subRoot->searchInterval(key, isKeyInTree);
		int sit = 0;
		if (isKeyInTree || dir != 0)
		{
			if (isKeyInTree)
				p = subRoot->data[interval - 1];
			if (dir == 1)
				interval = subRoot->data.size();
			if (dir == -1)
				interval = 0;
			if (subRoot->nodes_ptr[interval] == nullptr)
			{
				if (dir == -1)
				{
					p = subRoot->data[0];
					key = p.first;
				}
				if (subRoot->data.size() > t - 1)
				{
					if (dir == 1)
					{
						p = subRoot->data.back();
						key = p.first;
					}
					if (dir == -1)
					{
						interval = 1;
					}
					subRoot->data.erase(subRoot->data.begin() + interval - 1);
					subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval);
					return 0;
				}
				else
				{
					if (dir != 1)
						return -1;
					else
						return 1;
				}
			}
			else
			{
				if (dir == 0)
					sit = erase(key, subRoot->nodes_ptr[interval - 1], 1, p);
				else
					sit = erase(key, subRoot->nodes_ptr[interval], 1, p);
				if (sit == 0)
				{
					dir = 0;
					if (isKeyInTree)
						subRoot->data[interval - 1] = p;
				}
				else
				{
					if (sit == 1)
					{
						sit = erase(key, subRoot->nodes_ptr[interval], -1, p);
						if (sit == 0)
						{
							dir = 0;
							subRoot->data[interval - 1] = p;
						}
						if (sit == -1)
						{
							dir = 0;
							if (isKeyInTree)
								subRoot->data[interval - 1] = p;
						}
					}
				}
			}
		}
		else
		{
			sit = erase(key, subRoot->nodes_ptr[interval], dir, p);
		}
		if (sit == -1)
		{
			if ((interval > 0 && subRoot->nodes_ptr[interval - 1]->data.size() > t - 1) || (interval < subRoot->data.size() && subRoot->nodes_ptr[interval + 1]->data.size() > t - 1))
			{
				if (interval > 0 && subRoot->nodes_ptr[interval - 1]->data.size() > t - 1)
				{
					subRoot->nodes_ptr[interval]->data.insert(subRoot->nodes_ptr[interval]->data.begin(), subRoot->data[interval - 1]);
					subRoot->nodes_ptr[interval]->nodes_ptr.insert(subRoot->nodes_ptr[interval]->nodes_ptr.begin(), subRoot->nodes_ptr[interval]->nodes_ptr[0]);
					erase(key, subRoot->nodes_ptr[interval], dir, p);
					subRoot->data[interval - 1] = subRoot->nodes_ptr[interval - 1]->data.back();
					subRoot->nodes_ptr[interval - 1]->data.pop_back();
					subRoot->nodes_ptr[interval - 1]->nodes_ptr.pop_back();
				}
				else
				{
					subRoot->nodes_ptr[interval]->data.insert(subRoot->nodes_ptr[interval]->data.end(), subRoot->data[interval]);
					subRoot->nodes_ptr[interval]->nodes_ptr.insert(subRoot->nodes_ptr[interval]->nodes_ptr.end(), subRoot->nodes_ptr[interval]->nodes_ptr.back());
					erase(key, subRoot->nodes_ptr[interval], dir, p);
					subRoot->data[interval] = subRoot->nodes_ptr[interval + 1]->data[0];
					subRoot->nodes_ptr[interval + 1]->data.erase(subRoot->nodes_ptr[interval + 1]->data.begin());
				}
			}
			else
			{
				if (interval < subRoot->data.size())
				{
					subRoot->nodes_ptr[interval]->data.push_back(subRoot->data[interval]);
					subRoot->nodes_ptr[interval]->nodes_ptr.push_back(nullptr);
					subRoot->nodes_ptr[interval]->join(subRoot->nodes_ptr[interval + 1]);
					erase(key, subRoot->nodes_ptr[interval], dir, p);
					subRoot->data.erase(subRoot->data.begin() + interval);
					subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval + 1);
					if (subRoot->data.size() < t - 1)
						return -2;
				}
				else
				{
					subRoot->nodes_ptr[interval - 1]->data.push_back(subRoot->data[interval - 1]);
					subRoot->nodes_ptr[interval - 1]->nodes_ptr.push_back(nullptr);
					subRoot->nodes_ptr[interval - 1]->join(subRoot->nodes_ptr[interval]);
					erase(key, subRoot->nodes_ptr[interval - 1], dir, p);
					subRoot->data.erase(subRoot->data.begin() + interval - 1);
					subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval);
					if (subRoot->data.size() < t - 1)
						return -2;
				}
			}
		}
		else
		{
			if (sit == -2)
			{
				if ((interval > 0 && subRoot->nodes_ptr[interval - 1]->data.size() > t - 1) || (interval < subRoot->data.size() && subRoot->nodes_ptr[interval + 1]->data.size() > t - 1))
				{
					if (interval > 0 && subRoot->nodes_ptr[interval - 1]->data.size() > t - 1)
					{
						subRoot->nodes_ptr[interval]->data.insert(subRoot->nodes_ptr[interval]->data.begin(), subRoot->data[interval - 1]);
						subRoot->nodes_ptr[interval]->nodes_ptr.insert(subRoot->nodes_ptr[interval]->nodes_ptr.begin(), subRoot->nodes_ptr[interval - 1]->nodes_ptr.back());
						subRoot->data[interval - 1] = subRoot->nodes_ptr[interval - 1]->data.back();
						subRoot->nodes_ptr[interval - 1]->data.pop_back();
						subRoot->nodes_ptr[interval - 1]->nodes_ptr.pop_back();
					}
					else
					{
						subRoot->nodes_ptr[interval]->data.insert(subRoot->nodes_ptr[interval]->data.end(), subRoot->data[interval]);
						subRoot->nodes_ptr[interval]->nodes_ptr.insert(subRoot->nodes_ptr[interval]->nodes_ptr.end(), subRoot->nodes_ptr[interval + 1]->nodes_ptr[0]);
						subRoot->data[interval] = subRoot->nodes_ptr[interval + 1]->data[0];
						subRoot->nodes_ptr[interval + 1]->data.erase(subRoot->nodes_ptr[interval + 1]->data.begin());
					}
				}
				else
				{
					if (interval < subRoot->data.size())
					{
						subRoot->nodes_ptr[interval]->data.push_back(subRoot->data[interval]);
						subRoot->nodes_ptr[interval]->nodes_ptr.push_back(nullptr);
						subRoot->nodes_ptr[interval]->join(subRoot->nodes_ptr[interval + 1]);
						subRoot->data.erase(subRoot->data.begin() + interval);
						subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval + 1);
						if (subRoot->data.size() < t - 1)
							return -2;
					}
					else
					{
						subRoot->nodes_ptr[interval - 1]->data.push_back(subRoot->data[interval - 1]);
						subRoot->nodes_ptr[interval - 1]->nodes_ptr.push_back(nullptr);
						subRoot->nodes_ptr[interval - 1]->join(subRoot->nodes_ptr[interval]);
						subRoot->data.erase(subRoot->data.begin() + interval - 1);
						subRoot->nodes_ptr.erase(subRoot->nodes_ptr.begin() + interval);
						if (subRoot->data.size() < t - 1)
							return -2;
					}
				}
			}
		}
	}
	return 0;
}