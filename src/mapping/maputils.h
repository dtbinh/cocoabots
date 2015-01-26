#ifndef MAPUTILS_H
#define MAPUTILS_H

#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <math.h>
#include <cmath>

struct mapPosition {
	int x;
	int y;

	bool operator==(const mapPosition& p)
	{
		return (x==p.x) && (y==p.y);
	}
	mapPosition():x(0),y(0){}
};

struct mapCubeStack {
	int bot;
	int mid;
	int top;
	mapCubeStack():bot(0),mid(0),top(0){}
};

struct mapPositionComparator {
	bool operator()(const mapPosition& p1, const mapPosition& p2) const {
		return (p1.x<p2.x) || (p1.x==p2.x && p1.y<p2.y);
	}
};

// bool operator<(mapCubeStack const& c1, mapCubeStack const& c2)
// {
// 	return (c1.bot<c2.bot) || (c1.bot==c2.bot && c1.mid<c2.mid) || (c1.bot==c2.bot && c1.mid==c2.mid && c1.top<c2.top);
// }

typedef std::vector< std::vector<int> > mapGridMap;
typedef std::vector<struct mapPosition> mapPositionVector;
typedef mapPositionVector::iterator mapPositionVectorIterator;
typedef std::vector<std::string> mapStringVector;
typedef std::map<struct mapPosition, struct mapCubeStack, mapPositionComparator> mapStackDictionary;

class mapUtils {
	public:
		mapUtils();
};

#endif