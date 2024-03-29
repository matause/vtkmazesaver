#ifndef NODE_H
#define NODE_H

#include <math.h>
#define N 1
#define S 2
#define E 4
#define W 8

class Node {
private:
	int _x,_y;
	int _g;//path cost
	int _h;//heuristic
	bool _hasPolyhedron;
	bool _visited;
	unsigned int _openEdges;
	Node * _parent;
	Node * _child;
    vtkSmartPointer<vtkActor> polyhedron;   // Pointer to the polyhedron, if there is one.
public:
	Node() {_x = 0; _y = 0; _openEdges = 0; _visited = false; _hasPolyhedron = false;}
	Node(int x, int y) {_x = x; _y = y; _openEdges = 0; _visited = false; _g = 100; _h= 100; _hasPolyhedron = false;}
	int getX() {return _x;}
	int getY() {return _y;}
	bool setG(int g) {if(g < _g) _g=g; else return false; return true;}
	int getG() {return _g;}
	int setH(int x, int y) {_h = abs( _x - x) + abs(_y - y); return _h;}
	int getH() {return _h;}
	int getF() {return _g + _h;}
	void setParent(Node * parent) {_parent=parent;}
	Node * getParent() {return _parent;}
	void setChild(Node * child) {_child=child;}
	Node * getChild() {return _child;}
	void addEdge(unsigned int openEdges) {_openEdges = openEdges;}
	void visit() {_visited = true;}
	void unvisit() {_visited = false;}
	bool visited() {return _visited;}
	unsigned int getEdges() {return _openEdges;}
	void removeNorthWall() {if (!youCanGoNorth()) _openEdges +=N;}
	void removeSouthWall() {if (!youCanGoSouth()) _openEdges +=S;}
	void removeEastWall() {if (!youCanGoEast()) _openEdges +=E;}
	void removeWestWall() {if (!youCanGoWest()) _openEdges +=W;}
	void setPolyhedron(vtkSmartPointer<vtkActor> poly) {polyhedron = poly; _hasPolyhedron = true;}
	vtkSmartPointer<vtkActor> getPolyhedron() {return polyhedron;}
	bool checkPolyhedron() {return _hasPolyhedron;}
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
	    if (_openEdges == S || _openEdges == S+N || _openEdges == S+E ||
	        _openEdges == S+W || _openEdges == S+N+E || _openEdges == S+N+W || _openEdges == S+E+W || _openEdges == S+N+E+W)
	        return true;
        else
	        return false;
	}
};

#endif
