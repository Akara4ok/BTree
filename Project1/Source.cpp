#include<iostream>
#include"Node.h"
#include"BTree.h"
#include"IOClass.h"

int main(){
	BTree b("D:\\tests\\file.txt");
	//for (int i = 0; i < 17; i++)
	/*{

		std::cin >> p.first >> p.second;
		b.push(p);
	}*/
	int k;
	std::cin >> k;
	b.erase(k);
	//int n;
	//std::cin >> n;
	//std::string s;
	//b.search(n, b.root, s);
	//std::cout << b.search(n);
	b.writeToFile("D:\\tests\\file.txt");
	return 0;
}