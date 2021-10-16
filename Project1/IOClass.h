#pragma once
#include<string>
#include<vector>
#include <fstream>

class IOClass
{
	int mode;
	std::ifstream fin;
	std::ofstream fout;
public:
	IOClass(std::string path, int mode);
	void parseFirstLine(int& t);
	void parseNode(int& numberOfChilds, int& indexOfParent, std::vector<std::pair<int, std::string>>& data);
	void writeFirstLine(int t);
	void writeNode(int numberOfChilds, int indexOfParent, std::vector<std::pair<int, std::string>> data);
	~IOClass();
};

