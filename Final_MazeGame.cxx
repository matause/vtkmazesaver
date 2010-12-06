/*****************************************
Daniel Souza
Evan Sulivan
Intro to Visualization
Final 3D Maze

Summary:


Command Line Arguments:

******************************************/
#ifndef _MAZEGAME
#define _MAZEGAME

#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkActor.h>
#include <vtkArrowSource.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkTransform.h>
#include <vtkLinearTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkSelectEnclosedPoints.h>
#include <vtkMath.h>
#include <vtkTextProperty.h>
#include <vtkRendererCollection.h>
#include <vtkCornerAnnotation.h>
#include <vtkPlaneSource.h>
#include <vtkPentagonalPrism.h>
#include <vtkTetra.h>
#include <vtkCamera.h>
#include <vtkCameraActor.h>
#include <vtkMath.h>
#include <vtkImageData.h>
#include <vtkPNGReader.h>
#include <vtkTextureMapToPlane.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkActorCollection.h>
#include <vtkCellArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <stdlib.h>

#include "Node.h"
#include "KeyboardInteractor.h"
#include "vtkTimerCallback.h"

#define WindowX 200
#define WindowY 200

#define ROWS 10
#define COLUMNS 10

#define OFFSET -.5

#define WALLTEXTURE "Brick.png"
#define FLOORTEXTURE "Dirt.png"
#define CEILINGTEXTURE "Cement.png"
#define BOOKTEXTURE "BlueRedBookCover.png"

void initMaze(std::vector<std::vector<Node*> > & maze)
{
	for(int y=0; y < ROWS; y++)
	{
		std::vector<Node*> row;
		maze.push_back(row);
		for(int x=0; x < COLUMNS; x++)
			maze[y].push_back(new Node(x,y));
	}
}

bool finishedVisiting(std::vector<std::vector<Node*> > & maze, Node * & Current)
{
	int x = Current->getX();
	int y = Current->getY();
	//South
	if(y < ROWS - 1)
		{
		if(!maze[y+1][x]->visited()  && !Current->youCanGoSouth())
			{
				return false;
			}
		}
	if(y > 0)
		{
		if(!maze[y-1][x]->visited()  && !Current->youCanGoNorth())
			{
				return false;
			}
		}
			//West
	if(x < COLUMNS - 1)
		{
		if(!maze[y][x+1]->visited()  && !Current->youCanGoWest())
			{
				return false;
			}
		}
	//East
	if(x > 0)
		{
		if(!maze[y][x-1]->visited()  && !Current->youCanGoEast())
			{
				return false;
			}
	  	}
	return true;
}

void CreatePentagonalPrismActor(vtkActor* actor, int x, int y)
{
  // Create the points
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint(11, 10, 10);
  points->InsertNextPoint(13, 10, 10);
  points->InsertNextPoint(14, 12, 10);
  points->InsertNextPoint(12, 14, 10);
  points->InsertNextPoint(10, 12, 10);
  points->InsertNextPoint(11, 10, 14);
  points->InsertNextPoint(13, 10, 14);
  points->InsertNextPoint(14, 12, 14);
  points->InsertNextPoint(12, 14, 14);
  points->InsertNextPoint(10, 12, 14);

  // Pentagonal Prism
  vtkSmartPointer<vtkPentagonalPrism> pentagonalPrism =
    vtkSmartPointer<vtkPentagonalPrism>::New();
  pentagonalPrism->GetPointIds()->SetId(0,0);
  pentagonalPrism->GetPointIds()->SetId(1,1);
  pentagonalPrism->GetPointIds()->SetId(2,2);
  pentagonalPrism->GetPointIds()->SetId(3,3);
  pentagonalPrism->GetPointIds()->SetId(4,4);
  pentagonalPrism->GetPointIds()->SetId(5,5);
  pentagonalPrism->GetPointIds()->SetId(6,6);
  pentagonalPrism->GetPointIds()->SetId(7,7);
  pentagonalPrism->GetPointIds()->SetId(8,8);
  pentagonalPrism->GetPointIds()->SetId(9,9);

  vtkSmartPointer<vtkCellArray> cellArray =
    vtkSmartPointer<vtkCellArray>::New();
  cellArray->InsertNextCell(pentagonalPrism);

  // Add the points and pentagonal prism to an unstructured grid
  vtkSmartPointer<vtkUnstructuredGrid> uGrid =
    vtkSmartPointer<vtkUnstructuredGrid>::New();
  uGrid->SetPoints(points);
  uGrid->InsertNextCell(pentagonalPrism->GetCellType(), pentagonalPrism->GetPointIds());

  // Visualize
  vtkSmartPointer<vtkDataSetMapper> mapper =
    vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputConnection(uGrid->GetProducerPort());

  actor->SetMapper(mapper);
}

void CreateTetraActor(vtkActor* actor, int x, int y)
{
  vtkSmartPointer< vtkPoints > points =
    vtkSmartPointer< vtkPoints > :: New();
  points->InsertNextPoint(-0.5, -0.2915, -0.2165);
  points->InsertNextPoint(0.5, -0.2915, -0.2165);
  points->InsertNextPoint(0, 0.5745, -0.2165);
  points->InsertNextPoint(0, 0, 0.6195);

  vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid =
    vtkSmartPointer<vtkUnstructuredGrid>::New();
  unstructuredGrid->SetPoints(points);

  vtkSmartPointer<vtkTetra> tetra =
    vtkSmartPointer<vtkTetra>::New();
  tetra->GetPointIds()->SetId(0, 0);
  tetra->GetPointIds()->SetId(1, 1);
  tetra->GetPointIds()->SetId(2, 2);
  tetra->GetPointIds()->SetId(3, 3);

  vtkSmartPointer<vtkCellArray> cellArray =
    vtkSmartPointer<vtkCellArray>::New();
  cellArray->InsertNextCell(tetra);
  unstructuredGrid->SetCells(VTK_TETRA, cellArray);

  // Create a mapper and actor
  vtkSmartPointer<vtkDataSetMapper> mapper =
    vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputConnection(unstructuredGrid->GetProducerPort());

  actor->SetMapper(mapper);
  actor->SetScale(0.5, 0.5, 0.5);
  actor->SetPosition(x, y, 0);
}


void generateMaze(std::vector<std::vector<Node*> > & maze, Node * & Current)
{
	if(finishedVisiting(maze,Current)) return;
	//vtkSmartPointer<vtkMinimalStandardRandomSequence> sequence =
	//    vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
	// initialize the sequence
	//sequence->SetSeed(1);
//	int i = (int) (sequence->GetValue() * 100);
	bool found = false;
	Current->visit();
	while(!found)
	{
	int x = Current->getX();
	int y = Current->getY();
        int i = (int) vtkMath::Random(0.0,100.0);
		//South
		if(i >= 0 && i <25)
		{
			if(y < ROWS - 1)
			{
				if(!maze[y+1][x]->visited()  && !Current->youCanGoSouth())
				{
					maze[y+1][x]->removeNorthWall();
					Current->removeSouthWall();
					generateMaze(maze,maze[y+1][x]);
if(finishedVisiting(maze,Current))
					found = true;
				}


			}
	  	}
		if(i >= 25 && i < 50)
		{
		//North
			if(y > 0)
			{
				if(!maze[y-1][x]->visited()  && !Current->youCanGoNorth())
				{
					maze[y-1][x]->removeSouthWall();
					Current->removeNorthWall();
					generateMaze(maze,maze[y-1][x]);
if(finishedVisiting(maze,Current))
					found = true;
				}


			}
	  	}
		if(i >= 50 && i < 75)
		{
			//West
			if(x < COLUMNS - 1)
			{
				if(!maze[y][x+1]->visited()  && !Current->youCanGoWest())
				{
					maze[y][x+1]->removeEastWall();
					if(x != 0)
					Current->removeWestWall();
					generateMaze(maze,maze[y][x+1]);
if(finishedVisiting(maze,Current))
					found = true;
				}


			}

	  	}
		if(i >= 75 && i <100)
		{
			//East
			if(x > 0)
			{
				if(!maze[y][x-1]->visited()  && !Current->youCanGoEast())
				{
					maze[y][x-1]->removeWestWall();
					if(x != COLUMNS - 1)
					Current->removeEastWall();
					generateMaze(maze,maze[y][x-1]);
						found = true;
				}
			}
	  	}
	}
	//if(!finishedVisiting(maze,Current)) generateMaze(maze,Current);
}
std::vector <Node*> generateMaze2(std::vector<std::vector<Node*> > & maze)
{
	std::vector <Node*> solution;
	Node * current = maze[0][0];
	int visited = 1;
	int total = ROWS * COLUMNS;
	std::stack <Node*> CellStack;
	//CellStack.push(current);

	// come up with up to 10 random nodes in which we will place a polyhedron
	// A polyhedron cannot be in either [0][0] or [ROWS-1][COLUMNS-1]
	int polyhedronRow[10];
	int polyhedronCol[10];
	int modifier = 1;
	for (int i = 0; i < 10; i++, modifier *= 2)
	{
	    polyhedronRow[i] = (int)vtkMath::Random(1.0, (ROWS - 1) * modifier);
	    polyhedronCol[i] = (int)vtkMath::Random(1.0, (COLUMNS - 1) * modifier);
	    std::cout << polyhedronCol[i] << ", " << polyhedronRow[i] << std::endl;
	}
	/*// RIGGED!
	polyhedronRow[0] = 1;
    polyhedronCol[0] = 0;
    polyhedronRow[1] = 0;
    polyhedronCol[1] = 1;*/

	while(visited < total)
	{
		solution.push_back(current);
		current->visit();
		int x = current->getX();
		int y = current->getY();
		std::vector <Node*> adjacentcells;
		int i = (int) vtkMath::Random(0.0, 100.0);

        // If the current cell satisfies x + y = a number in polyhedron, then add a random polyhedron
        for (int i = 0; i < 10; i++)
        {
            if (x == polyhedronCol[i] && y == polyhedronRow[i])
            {
                std::cout << "Node (" << x << ", " << y << ")" << std::endl;
                polyhedronCol[i] = COLUMNS;
                polyhedronRow[i] = ROWS;

                // determine which kind of polyhedron to add.
                //int type = (int) vtkMath::Random(0, 1); //0 = tetrahedron, 1 = pentagonal prism
                int type = 0;
                if (type == 0)
                {
                    // make the polyhedron at this location and point the current node to it
                    vtkSmartPointer<vtkActor> tetraActor = vtkSmartPointer<vtkActor>::New();
                    CreateTetraActor(tetraActor, x, -y);
                    current->setPolyhedron(tetraActor);

                }
                else
                {
                    // make the pentagonal prism at this (x, y) and point the current node to it
                    vtkSmartPointer<vtkActor> pentagonalPrismActor = vtkSmartPointer<vtkActor>::New();
                    CreatePentagonalPrismActor(pentagonalPrismActor, x, -y);
                    current->setPolyhedron(pentagonalPrismActor);
                }
            }
        }

		//North
		if(y > 0)
		{
			if(!maze[y-1][x]->visited())
				adjacentcells.push_back(maze[y-1][x]);
		}
		//South
		if(y < ROWS - 1)
		{
			if(!maze[y+1][x]->visited())
				adjacentcells.push_back(maze[y+1][x]);
		}
		//East
		if(x > 0)
		{
			if(!maze[y][x-1]->visited())
				adjacentcells.push_back(maze[y][x-1]);
		}
		if(x < COLUMNS - 1)
		{
			if(!maze[y][x+1]->visited())
				adjacentcells.push_back(maze[y][x+1]);
		}

		if(adjacentcells.size() == 0)
		{
			current = CellStack.top();
			CellStack.pop();
		}
		else
		{
			i = i % adjacentcells.size();
			if(x == adjacentcells[i]->getX())
			{
				if(y < adjacentcells[i]->getY())
				{
					current->removeSouthWall();
					adjacentcells[i]->removeNorthWall();
				}
				else
				{
					current->removeNorthWall();
					adjacentcells[i]->removeSouthWall();
				}
			}
			else
			{
				if(x < adjacentcells[i]->getX())
				{
					current->removeEastWall();
					adjacentcells[i]->removeWestWall();
				}
				else
				{
					current->removeWestWall();
					adjacentcells[i]->removeEastWall();
				}
			}
			visited++;
			CellStack.push(current);
			current = adjacentcells[i];
		}
	}
	return solution;
}

//check if a given node should be added to the open list or updated
void add_directions(std::vector<std::vector<Node*> > & maze, std::vector <Node*> &openlist, int x, int y, Node * Parent, Node * End)
{
	Node * current; //the new direction
	if((x >= 0 && x < COLUMNS) &&
		(y >= 0 && y < ROWS))
		current = maze[y][x];
	else return;
	//ignore if the node is already on the closed list or is unreachable
	if(current->visited())
		return;
	//if it's not open, add it to the open list and calculate G,H and F
	openlist.push_back(current);
	current->setH(End->getX(),End->getY());
	if(current->setG(Parent->getG() + 1))
	{
		if(current != Parent)
		{
            current->setParent(Parent);
		}
	}
}
void clearVisit(std::vector<std::vector<Node*> > & maze)
{
	for(int y = 0; y < ROWS; y++)
		for(int x = 0; x < COLUMNS; x++)
			maze[y][x]->unvisit();
}
//Give this function the maze, a start and an end node and it will calculate the shortest path to the end node.
void astar(std::vector<std::vector<Node*> > & maze, Node * Start, Node * End)
{
	clearVisit(maze);
	Start->setG(0);
	std::vector <Node*> openlist;
	add_directions(maze, openlist,Start->getX(),Start->getY(),Start, End);
	while(openlist.size() > 0 && !End->visited())
	{
		unsigned int currentposition = 0;
		Node * current = openlist[0];
		for(unsigned int i = 1; i < openlist.size(); i++)
			if(openlist[i]->getF() < current->getF())
				{current = openlist[i]; currentposition = i;}
	//Add any available directions to the open list
	int x = current->getX(); int y = current->getY();
	if(current->youCanGoNorth())
		add_directions(maze, openlist,x,y-1,current, End);
	if(current->youCanGoEast())
		add_directions(maze,openlist,x+1,y,current,End);
	if(current->youCanGoWest())
		add_directions(maze,openlist,x-1,y,current,End);
	if(current->youCanGoSouth())
		add_directions(maze,openlist,x,y+1,current,End);
	//move current node to closed list
	openlist.erase(openlist.begin() + currentposition);
	current->visit();
	}
	Node * current = End;
	while(current && current != Start)
		{
		//std::cout<< current->getX() << " " << current->getY() << std::endl;
		current->getParent()->setChild(current);
		current = current->getParent();
		}
	//std::cout << "Shortest Path (Start->Finish)\n";
	int steps = 0;
	current = Start;
	//std::cout<< current->getX() << " " << current->getY() << std::endl;
	while(current && current != End)
	{
		current = current->getChild();
		//std::cout<< current->getX() << " " << current->getY() << std::endl;
		steps++;
	}
	std::cout << "It takes " << steps << " steps to complete the maze\n";
}

	std::vector <Node *> DepthFirst(std::vector<std::vector<Node*> > & maze, Node * start, int endx, int endy)
	{
		std::vector <Node *> path;
		clearVisit(maze);
		Node * current = start;
		while(current != maze[endy][endx])
		{
		path.push_back(current);
		Node * next = NULL;
		current->visit();
		int x = current->getX();
		int y = current->getY();
		int distance = abs(endy - y) + abs(endx - x);
		if (distance == 0)
			break;
		int bestdistance = distance + 2;
		if(current->youCanGoSouth())// && checkBest(maze[y+1][x],endx,endy,bestdistance) )
			next = maze[y+1][x];
		if(current->youCanGoEast())// && checkBest(maze[y][x+1],endx,endy,bestdistance))
			if(next == NULL || (next->visited() == true))
				next = maze[y][x+1];
		if(current->youCanGoWest())// && checkBest(maze[y][x-1],endx,endy,bestdistance))
			if(next == NULL || (next->visited() == true))
				next = maze[y][x-1];
		if(current->youCanGoNorth())// && checkBest(maze[y-1][x],endx,endy,bestdistance))
			if(next == NULL || (next->visited() == true))
				next = maze[y-1][x];
		if(next->visited() == true || next == NULL && current->getParent() != NULL)
			current = current->getParent();
		else
		{
			current->setChild(next);
			next->setParent(current);
			current = next;
		}
		}
		path.push_back(maze[endy][endx]);
		return path;
	}


vtkSmartPointer<vtkActor> CreatePlaneActor(vtkSmartPointer<vtkPolyDataMapper> mapper,vtkSmartPointer<vtkTexture> texture, double x, double y, double z, double rotx, double roty, double rotz, double R, double G, double B) {
	vtkSmartPointer<vtkActor> actor =
	    vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->SetTexture(texture);
	actor->GetProperty()->SetAmbient(100);

	//Transform
	actor->SetPosition(x,y,z);
	actor->RotateX(rotx);
	actor->RotateY(roty);
	actor->RotateZ(rotz);
	//Change Color
	actor->GetProperty()->SetColor(R,G,B);
	return actor;
}

void printMaze(std::vector<std::vector<Node*> > & maze, vtkSmartPointer<vtkRenderer> &renderer,   vtkActorCollection * &actorCollection)
{
    // Load the textures for the walls, floor, and ceiling
    vtkSmartPointer<vtkPNGReader> wallTextureSource = vtkSmartPointer<vtkPNGReader>::New();
    wallTextureSource->SetFileName(WALLTEXTURE);

    vtkSmartPointer<vtkPNGReader> floorTextureSource = vtkSmartPointer<vtkPNGReader>::New();
    floorTextureSource->SetFileName(FLOORTEXTURE);

    vtkSmartPointer<vtkPNGReader> ceilingTextureSource = vtkSmartPointer<vtkPNGReader>::New();
    ceilingTextureSource->SetFileName(CEILINGTEXTURE);

    vtkSmartPointer<vtkPNGReader> bookTextureSource = vtkSmartPointer<vtkPNGReader>::New();
    bookTextureSource->SetFileName(BOOKTEXTURE);

    //Create a plane
    vtkSmartPointer<vtkPlaneSource> planeSource =
        vtkSmartPointer<vtkPlaneSource>::New();
    planeSource->SetCenter(1.0, 0.0, 0.0);
    planeSource->SetNormal(1.0, 0.0, 0.0);

    // Apply the textures
    vtkSmartPointer<vtkTexture> wallTexture = vtkSmartPointer<vtkTexture>::New();
    wallTexture->SetInput(wallTextureSource->GetOutput());

    vtkSmartPointer<vtkTexture> floorTexture = vtkSmartPointer<vtkTexture>::New();
    floorTexture->SetInput(floorTextureSource->GetOutput());

    vtkSmartPointer<vtkTexture> ceilingTexture = vtkSmartPointer<vtkTexture>::New();
    ceilingTexture->SetInput(ceilingTextureSource->GetOutput());

    vtkSmartPointer<vtkTexture> bookTexture = vtkSmartPointer<vtkTexture>::New();
    bookTexture->SetInput(bookTextureSource->GetOutput());
    // Other things
    vtkSmartPointer<vtkTextureMapToPlane> texturePlane =
        vtkSmartPointer<vtkTextureMapToPlane>::New();
    texturePlane->SetInput(planeSource->GetOutput());

    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(texturePlane->GetOutputPort());

    int bookprobability = 10;
    // Create the walls, floor, and ceiling of the maze
	for(int y=0; y < ROWS; y++)
    {
		for(int x=0; x < COLUMNS; x++)
        {
            Node * current = maze[y][x];

            // If there is not an opening to the north, make a wall there.
            if(!current->youCanGoNorth())
            {
		vtkSmartPointer<vtkActor> a;
		if(vtkMath::Random(0.0,bookprobability) < 2)
		{
			bookprobability = bookprobability * 2;
			a = CreatePlaneActor(mapper, bookTexture, current->getX(), - 1 * current->getY() + OFFSET,0,0,0,90, 1,1,1);
	        }
		else
		{
			a = CreatePlaneActor(mapper, wallTexture, current->getX(), - 1 * current->getY() + OFFSET,0,0,0,90, 1,1,1);
		}
                renderer->AddActor(a);
                actorCollection->AddItem(a);
            }

            // If there is not an opening to the west, make a wall there.
            if(!current->youCanGoWest())
            {
		vtkSmartPointer<vtkActor> a;
		if(vtkMath::Random(0.0,bookprobability) < 2)
		{
			bookprobability = bookprobability * 2;
	                a = CreatePlaneActor(mapper, bookTexture, current->getX() - OFFSET - 2, - 1 * current->getY(), 0, 0,0,0,1,1,1);
		}
		else
	                a = CreatePlaneActor(mapper, wallTexture, current->getX() - OFFSET - 2, - 1 * current->getY(), 0, 0,0,0,1,1,1);
                renderer->AddActor(a);
                actorCollection->AddItem(a);
            }

            // If there is not an opening to the south, make a wall there (last row only).
            if(y == ROWS - 1 && !current->youCanGoSouth())
            {
		vtkSmartPointer<vtkActor> a;
		if(vtkMath::Random(0.0,bookprobability) < 2)
		{
			bookprobability = bookprobability * 2;
	                a = CreatePlaneActor(mapper, bookTexture, current->getX(), - 1 * (current->getY() + 1) + OFFSET, 0, 0,0,90,1,1,1);
		}
		else
		{
	                a = CreatePlaneActor(mapper, wallTexture, current->getX(), - 1 * (current->getY() + 1) + OFFSET, 0, 0,0,90,1,1,1);
		}
                renderer->AddActor(a);
                actorCollection->AddItem(a);
            }

            // If there is not an opening to the east, make a wall there (last column onln->setH(endx,endy);y).
            if(x == COLUMNS -1 && !current->youCanGoEast())
            {
		vtkSmartPointer<vtkActor> a;
		if(vtkMath::Random(0.0,bookprobability) < 2)
		{
			bookprobability = bookprobability * 2;
	                a = CreatePlaneActor(mapper, wallTexture, current->getX() + OFFSET, -1 * current->getY(), 0, 0,0,0,1,1,1);
		}
		else
	                a = CreatePlaneActor(mapper, wallTexture, current->getX() + OFFSET, -1 * current->getY(), 0, 0,0,0,1,1,1);
                renderer->AddActor(a);
                actorCollection->AddItem(a);
            }

            // Create the floor
            vtkSmartPointer<vtkActor> b = CreatePlaneActor(mapper, floorTexture, current->getX(), -1 * current->getY(), -OFFSET, 0,90,0,1,1,1);
            renderer->AddActor(b);
            actorCollection->AddItem(b);

            // Create the ceiling
            vtkSmartPointer<vtkActor> c = CreatePlaneActor(mapper, ceilingTexture, current->getX(), -1 * current->getY(), OFFSET, 0,-90,0,1,1,1);
            renderer->AddActor(c);
            actorCollection->AddItem(c);

            // Add the polygon, if there is one
            if (current->checkPolyhedron())
            {
                renderer->AddActor(current->getPolyhedron());
            }
            // Save the polygon's pointer in the array of polygons so they can be animated
        }
    }
    actorCollection->InitTraversal();
}

int main(int argc,char *argv[])
{
	  //Create a renderer, render window and interactor
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(WindowX,WindowY);
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
  vtkActorCollection * actorCollection = vtkActorCollection::New();
	std::cout <<"1";
	std::vector<std::vector<Node*> > maze;
	initMaze(maze);
	std::cout <<"2";
	vtkMath::RandomSeed(time(NULL));
	//generateMaze(maze, maze[0][0]);
	generateMaze2(maze);
	std::cout <<"3";
/*
std::cout << "NORTH\n";
for(int j = 0; j < COLUMNS; j++)
	{
	for(int i = 0; i < ROWS; i++)
		{

			std::cout << maze[j][i]->youCanGoNorth();
		}
		std::cout << endl;
	}
std::cout << "SOUTH\n";
for(int j = 0; j < COLUMNS; j++)
	{
	for(int i = 0; i < ROWS; i++)
		{
			std::cout << maze[j][i]->youCanGoSouth();
		}
		std::cout << endl;
	}

std::cout << "WEST\n";
for(int j = 0; j < COLUMNS; j++)
	{
	for(int i = 0; i < ROWS; i++)
		{

			std::cout << maze[j][i]->youCanGoWest();
		}
		std::cout << endl;
	}

std::cout << "EAST\n";
for(int j = 0; j < COLUMNS; j++)
	{
	for(int i = 0; i < ROWS; i++)
		{
			std::cout << maze[j][i]->youCanGoEast();
		}
		std::cout << endl;
	}
*/
    system("xset r on");

    bool upDown = false;
    bool downDown = false;
    bool leftDown = false;
    bool rightDown = false;

std::vector <Node*> path = DepthFirst(maze, maze[0][0], COLUMNS-1, ROWS-4);
astar(maze, maze[0][0], maze[ROWS-1][COLUMNS-4]);
std::cout << "Shortest Path\n";
for(int j = 0; j < ROWS; j++)
	{
	for(int i = 0; i < COLUMNS; i++)
		{
			std::cout << maze[j][i]->getF() << "\t";
		}
		std::cout << endl;
	}
	printMaze(maze, renderer,actorCollection);
std::cout << "DFP: " << path.size() << std::endl;
  // Camera
  vtkSmartPointer<vtkCamera> camera =
    vtkSmartPointer<vtkCamera>::New();
//  camera->SetPosition(ROWS/2, -1 * COLUMNS/2, 10);
//  camera->SetFocalPoint(ROWS/2, -1 * COLUMNS/2, 00);
    camera->SetPosition(0,0,0);
    camera->SetFocalPoint(1,0,0);
    camera->Roll((double)90);
  renderer->SetActiveCamera(camera);

//Keyboard Style
  vtkSmartPointer<KeyPressInteractorStyle> keystyle =
	    vtkSmartPointer<KeyPressInteractorStyle>::New();
  keystyle->SetCamera(camera, actorCollection, maze, upDown, downDown, leftDown, rightDown);
  renderWindowInteractor->SetInteractorStyle(keystyle);

  vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
    vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//  renderWindowInteractor->SetInteractorStyle(style);

  //renderer->SetActiveCamera(camera);
  renderer->SetBackground(.1,.2,.3); // Background color dark blue


    //Axes made here
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    vtkSmartPointer<vtkOrientationMarkerWidget> widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    widget->SetOrientationMarker( axes );
    widget->SetInteractor( renderWindowInteractor );
    widget->SetViewport( 0.0, 0.0, 0.4, 0.4 );
    widget->SetEnabled( 1 );
    widget->InteractiveOn();


  // Initialize must be called prior to creating timer events.
  renderWindowInteractor->Initialize();

  // Sign up to receive TimerEvent
  vtkSmartPointer<vtkTimerCallback> cb =
    vtkSmartPointer<vtkTimerCallback>::New();
  renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb); //PUT THIS LINE BACK IN FOR AUTOMATED MOTION
  cb->setMaze(maze, path);
  cb->setCamera(camera, renderWindowInteractor, upDown, downDown, leftDown, rightDown, maze);
  int timerId = renderWindowInteractor->CreateRepeatingTimer(1);
  std::cout << "timerId: " << timerId << std::endl;

  // Start the interaction and timer
  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  system("xset r on");
  return EXIT_SUCCESS;
}

#endif
