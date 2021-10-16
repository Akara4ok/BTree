#include "BTree.h"
#include<Queue>

BTree::BTree(int t)
{
	this->t = t;
}

BTree::BTree(std::string path)
{
	IOClass io(path, 0);
	io.parseFirstLine(t);
	std::queue<Node*> q;
	int ind;
	io.parseNode(root->numberOfChilds, ind, root->data);
	root->makePtrs();
	q.push(root);
	Node* temp = root;
	int numberOfChilds = root->numberOfChilds;
	while (!q.empty())
	{
		if (numberOfChilds == 0)
		{
			q.pop();
			if (!q.empty())
			{
				temp = q.front();
				numberOfChilds = temp->numberOfChilds;
			}
			continue;
		}
		Node* node = new Node;
		io.parseNode(node->numberOfChilds, ind, node->data);
		node->makePtrs();
		temp->addChild(node, ind);
		q.push(node);
		numberOfChilds--;
	}
}

void BTree::writeToFile(std::string path)
{
	IOClass io(path, 1);
	io.writeFirstLine(t);
	std::queue<Node*> q;
	io.writeNode(root->numberOfChilds, 0, root->data);
	q.push(root);
	Node* temp;
	while (!q.empty())
	{
		temp = q.front();
		for (int i = 0; i < temp->numberOfChilds; i++)
		{
			if (temp->nodes_ptr[i] != nullptr)
			{
				io.writeNode(temp->nodes_ptr[i]->numberOfChilds, i, temp->nodes_ptr[i]->data);
				q.push(temp->nodes_ptr[i]);
			}
		}
		q.pop();
	}
}
