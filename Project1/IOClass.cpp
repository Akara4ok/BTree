#include "IOClass.h"

IOClass::IOClass(std::string path, int mode)
{
	this->mode = mode;
	if (mode == 0)
		fin.open(path);
	else
		fout.open(path);
}

void IOClass::parseFirstLine(int& t)
{
	std::string s;
	std::getline(fin, s);
	t = std::stoi(s);
}

void IOClass::parseNode(int& numberOfChilds, int& indexOfParent,  std::vector<std::pair<int, std::string>>& data)
{
	std::string s;
	int nextKey = 0;
	int currentValue = 0;
	int lastKey = 0;
	std::getline(fin, s);
	nextKey = s.find(';', lastKey);

	currentValue = s.find(':', lastKey);
	numberOfChilds = std::stoi(s.substr(lastKey, currentValue - lastKey));
	indexOfParent = std::stoi(s.substr(currentValue + 1, nextKey - currentValue - 1));
	lastKey = nextKey + 1;
	nextKey = s.find(';', lastKey);

	while (nextKey != s.npos)
	{
		currentValue = s.find(':', lastKey);
		data.push_back(std::pair<int, std::string>());
		data.back().first = std::stoi(s.substr(lastKey, currentValue - lastKey));
		data.back().second = s.substr(currentValue + 1, nextKey - currentValue - 1);
		lastKey = nextKey + 1;
		nextKey = s.find(';', lastKey);
	}
}

void IOClass::writeFirstLine(int t)
{
	fout << t << "\n";
}

void IOClass::writeNode(int numberOfChilds, int indexOfParent, std::vector<std::pair<int, std::string>> data)
{
	fout << numberOfChilds << ":" << indexOfParent << ";";
	for (int i = 0; i < data.size(); i++)
	{
		fout << data[i].first << ":" << data[i].second << ";";
	}
	fout << "\n";
}

IOClass::~IOClass()
{
	if (mode == 0)
		fin.close();
	else
		fout.close();
}
