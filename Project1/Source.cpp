#include<iostream>
#include"Node.h"
#include"BTree.h"
#include"IOClass.h"

int main(){
	BTree b("D:\\tests\\file.txt");
	std::pair<int, std::string> p;
	std::cin >> p.first >> p.second;
	b.push(p);
	//int n;
	//std::cin >> n;
	//std::string s;
	//b.search(n, b.root, s);
	//std::cout << b.search(n);
	b.writeToFile("D:\\tests\\file.txt");
	return 0;
}