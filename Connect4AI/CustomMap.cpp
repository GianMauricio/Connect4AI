#include "CustomMap.h"

CustomMap::CustomMap()
{
	
}

CustomMap::~CustomMap()
{
	//Kill stored values
	customMap.erase(customMap.begin(), customMap.end());
}

void CustomMap::emplace(pair<int, int> newKey)
{
	//Placeholder int
	int newVal = 0;
	
	//Copy over values of given key
	pair<int, int> newID;
	newID.first = newKey.first;
	newID.second = newKey.second;

	pair<pair<int, int>, int> newStoredPair = make_pair(newID, newVal);

	customMap.push_back(newStoredPair);
}

int CustomMap::at(pair<int, int> refKey)
{
	for(pair<pair<int, int>, int> storedPair : customMap)
	{
		if(storedPair.first.first == refKey.first && storedPair.first.second == refKey.second)
		{
			return storedPair.second;
		}
	}

	cout << "Custom key {" << refKey.first << ", " << refKey.second << "} not found." << endl;
	return NULL;
}
