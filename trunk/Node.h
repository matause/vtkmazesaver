#include <math.h>
#define N 1
#define S 2
#define E 4
#define W 8

class Node {
private:
	int _x,_y;
	bool _visited;
	unsigned int _openEdges;
public:
	Node() {_x = 0; _y = 0; _openEdges = 0; _visited = false;}
	Node(int x, int y) {_x = x; _y = y; _openEdges = 0; _visited = false;}
	int getX() {return _x;}
	int getY() {return _y;}
	void addEdge(unsigned int openEdges) {_openEdges = openEdges;}
	void visit() {_visited = true;}
	void unvisit() {_visited = false;}
	unsigned int getEdges() {return _openEdges;}
	void removeNorthWall() {if (!youCanGoNorth()) _openEdges +=N;}
	void removeSouthWall() {if (!youCanGoSouth()) _openEdges +=S;}
	void removeEastWall() {if (!youCanGoEast()) _openEdges +=E;}
	void removeWestWall() {if (!youCanGoWest()) _openEdges +=W;}
	bool youCanGoNorth()
	{
	    if (_openEdges == 1 || _openEdges == 3 || _openEdges == 5 ||
	        _openEdges == 9 || _openEdges == 7 || _openEdges == 11 || _openEdges == 13 || _openEdges == 15)
	        return true;
        else
	        return false;
	}
    bool youCanGoWest()
	{
	    if (_openEdges == 8 || _openEdges == 9 || _openEdges == 10 ||
	        _openEdges == 12 || _openEdges == 11 || _openEdges == 13 || _openEdges == 14 || _openEdges == 15)
	        return true;
        else
	        return false;
	}
    bool youCanGoEast()
	{
	    if (_openEdges == 4 || _openEdges == 5 || _openEdges == 7 ||
	        _openEdges == 12 || _openEdges == 6 || _openEdges == 13 || _openEdges == 14 || _openEdges == 15)
	        return true;
        else
	        return false;
	}
    bool youCanGoSouth()
	{
	    if (_openEdges == 8 || _openEdges == 9 || _openEdges == 10 ||
	        _openEdges == 12 || _openEdges == 11 || _openEdges == 13 || _openEdges == 14 || _openEdges == 15)
	        return true;
        else
	        return false;
	}
};
