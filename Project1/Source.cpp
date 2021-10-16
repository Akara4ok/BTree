#include<iostream>
#include"Node.h"
#include"BTree.h"
#include"IOClass.h"

int main(){
	/*std::vector<std::pair<int, std::string>> data;
	IOClass io("D:\\tests\\file.txt", 0);
	int a, b;
	io.parseNode(a, b, data);
	std::cout << a << " : " << b << std::endl;
	for (int i = 0; i < data.size(); i++)
	{
		std::cout << data[i].first << " : " << data[i].second << std::endl;
	}*/
	BTree b("D:\\tests\\file.txt");
	b.writeToFile("D:\\tests\\output.txt");
	return 0;
}