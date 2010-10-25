#include <math.h>
class Node {
private:
	int _x,_y;
	bool _visited;
	Node * _edge[4];
	int _edges;
public:
	Node() {_edges = 0; _visited = false;}
	Node(int x, int y) {_x=x; _y=y; _edges = 0; _visited=false;}
	int getX() {return _x;}
	int getY() {return _y;}
	void addEdge(Node * edge) {if(_edges < 4){_edge[_edges]=edge; _edges++;}}
	void visit() {_visited = true;}
	void unvisit() {_visited = false;}
	Node ** getEdges() {return _edge;}
	int getEdgeSize() {return _edges;}
};
