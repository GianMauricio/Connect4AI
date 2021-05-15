#pragma once
#include <vector>
#include <iostream>
using namespace std;
class CustomMap
{
public:
	CustomMap();
	~CustomMap();

	void emplace(pair<int, int> newKey);
	int at(pair<int, int> refKey);
private:

	vector<pair<pair<int, int>, int>> customMap;
};

