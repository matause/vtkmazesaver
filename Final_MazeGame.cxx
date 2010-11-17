/*****************************************
Daniel Souza
Evan Sulivan
Intro to Visualization
Final 3D Maze

Summary:


Command Line Arguments:

******************************************/

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
#include <vtkCamera.h>
#include <vtkCameraActor.h>
#include <vtkMath.h>
#include <vtkImageData.h>
#include <vtkJPEGReader.h>
#include <vtkTextureMapToPlane.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkActorCollection.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>

#include "Node.h"

#define ROWS 10
#define COLUMNS 10

#define OFFSET -.5
#define PI 3.14159265

#define MOVEINCREMENT 0.2
#define ANGLEINCREMENT PI/24

#define TEXTURE1 "texture1.jpg"

// Define interaction style
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  private:
	vtkSmartPointer<vtkCamera> Camera;
	vtkActorCollection * actorCollection;
	std::vector<std::vector<Node*> > tempMaze;
  public:
    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);

    void SetCamera(vtkSmartPointer<vtkCamera> c, vtkActorCollection * ac, std::vector<std::vector<Node*> > &nodes)
    {
        Camera = c;
        actorCollection = ac;
        tempMaze = nodes;
    }

    //Handles Keypresses
    virtual void OnKeyPress()
    {
        // Get the keypress
        vtkRenderWindowInteractor *rwi = this->Interactor;
        std::string key = rwi->GetKeySym();

        //Get actor position
        double * p = Camera->GetPosition();
        double xPosition = p[0];
        double yPosition = p[1];
        double * f = Camera->GetFocalPoint();
        double xFocus = f[0];
        double yFocus = f[1];

        double xLook = xFocus - xPosition;
        double yLook = yFocus - yPosition;
        double angle;

//////

        // Calculate the new position we will be at after moving

        // If the new position is in a valid region for that node, move there


        // Be careful not to divide by zero! If x is close enough to zero, we will just call it zero
        // and set the angle to Pi/2.
        if (fabs(xLook) > 0.0001)
        {
            angle = atan(yLook / xLook);
        }
        else
        {
            angle = PI / 2;
        }

        if (xLook < 0)
        {
            angle = PI + angle;
        }
        /*
        std::cout << "BEFORE MOVEMENT\n";
        std::cout << "Camera position: (" << xPosition << ", " << yPosition << ")." << std::endl;
        std::cout << "Focal position:  (" << xFocus << ", " << yFocus << ")." << std::endl;
        std::cout << "Look direction:  (" << xLook << ", " << yLook << ")." << std::endl;
        std::cout << "Angle:           " << angle << "\n" <<std::endl;
        */
        // Handles the arrow keys and moves the Player accordingly
        if(key.compare("Up") == 0)
        {
            Camera->SetPosition(xPosition + MOVEINCREMENT*(xLook), yPosition + MOVEINCREMENT*(yLook), 0);
        }
        if(key.compare("Down") == 0)
        {
            Camera->SetPosition(xPosition - MOVEINCREMENT*(xLook), yPosition - MOVEINCREMENT*(yLook), 0);
        }
        if(key.compare("Right") == 0)
        {
            angle -= ANGLEINCREMENT;
        }
        if(key.compare("Left") == 0)
        {
            angle += ANGLEINCREMENT;
        }

        if (angle > 2*PI)
        {
            angle -= 2*PI;
        }
        else if (angle < 0)
        {
            angle += 2*PI;
        }

        p = Camera->GetPosition();
        xPosition = p[0];
        yPosition = p[1];

        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);

        // Determine the current node in which we are located
        int row = -yPosition;
        int col = xPosition;
        std::cout << "X: " << xPosition << " Y: " << yPosition << std::endl;
        std::cout << "col: " << tempMaze[col][row]->getX() << " row: " << tempMaze[col][row]->getY() << std::endl;

        this->Interactor->GetRenderWindow()->Render();
    }
};

vtkStandardNewMacro(KeyPressInteractorStyle);

void initMaze(std::vector<std::vector<Node*> > & maze)
{
	for(int y=0; y < COLUMNS; y++)
	{
		std::vector<Node*> row;
		maze.push_back(row);
		for(int x=0; x < ROWS; x++)
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
			if(y < COLUMNS - 1)
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
	while(visited < total)
	{
		solution.push_back(current);
		current->visit();
		int x = current->getX();
		int y = current->getY();
		std::vector <Node*> adjacentcells;
		int i = (int) vtkMath::Random(0.0,100.0);
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
	if((x >= 0 && x < ROWS) &&
		(y >= 0 && y < COLUMNS))
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
//Give this function the maze, a start and an end node and it will calculate the shortest path to the end node.
void astar(std::vector<std::vector<Node*> > & maze, Node * Start, Node * End)
{
	for(int y = 0; y < COLUMNS; y++)
		for(int x = 0; x < ROWS; x++)
			maze[y][x]->unvisit();
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
		std::cout<< current->getX() << " " << current->getY() << std::endl;
		current->getParent()->setChild(current);
		current = current->getParent();
		}
	std::cout << "Shortest Path (Start->Finish)\n";
	int steps = 0;
	current = Start;
	std::cout<< current->getX() << " " << current->getY() << std::endl;
	while(current && current != End)
	{
		current = current->getChild();
		std::cout<< current->getX() << " " << current->getY() << std::endl;
		steps++;
	}
	std::cout << "It takes " << steps << " steps to complete the maze\n";
}

vtkSmartPointer<vtkActor> CreatePlaneActor(vtkSmartPointer<vtkPolyDataMapper> mapper,vtkSmartPointer<vtkTexture> texture, double x, double y, double z, double rotx, double roty, double rotz, double R, double G, double B) {
	vtkSmartPointer<vtkActor> actor =
	    vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->SetTexture(texture);

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

  vtkSmartPointer<vtkJPEGReader> jPEGReader =
    vtkSmartPointer<vtkJPEGReader>::New();
  jPEGReader->SetFileName ( TEXTURE1 );

  //Create a plane
  vtkSmartPointer<vtkPlaneSource> planeSource =
    vtkSmartPointer<vtkPlaneSource>::New();
//    planeSource->SetPoint1(0,0,0);
//    planeSource->SetPoint2(1,1,1);
  planeSource->SetCenter(1.0, 0.0, 0.0);
  planeSource->SetNormal(1.0, 0.0, 0.0);

  // Apply the texture
  vtkSmartPointer<vtkTexture> texture =
    vtkSmartPointer<vtkTexture>::New();
  texture->SetInput(jPEGReader->GetOutput());

  vtkSmartPointer<vtkTextureMapToPlane> texturePlane =
    vtkSmartPointer<vtkTextureMapToPlane>::New();
  texturePlane->SetInput(planeSource->GetOutput());

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(texturePlane->GetOutputPort());


 /*
  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);*/


  //Add the actors to the scene
//  renderer->AddActor(actor);
	for(int y=0; y < COLUMNS; y++)
    {
		for(int x=0; x < ROWS; x++)
        {
            Node * current = maze[y][x];
		// If there is not an opening to the north, make a wall there.
            if(!current->youCanGoNorth())
		{
		vtkSmartPointer<vtkActor> a = CreatePlaneActor(mapper, texture, current->getX(), - 1 * current->getY() + OFFSET,0,0,0,90, 0,1,0);
                renderer->AddActor(a);
		actorCollection->AddItem(a);
		}

// If there is not an opening to the west, make a wall there.
            if(!current->youCanGoWest())
		{
		vtkSmartPointer<vtkActor> a = CreatePlaneActor(mapper, texture, current->getX() - OFFSET - 2, - 1 * current->getY(), 0, 0,0,0,1,0,0);
                renderer->AddActor(a);
		actorCollection->AddItem(a);
		}

            if(y == ROWS - 1 && !current->youCanGoSouth())
		{
		vtkSmartPointer<vtkActor> a = CreatePlaneActor(mapper, texture, current->getX(), - 1 * (current->getY() + 1) + OFFSET, 0, 0,0,90,0,1,0);
                renderer->AddActor(a);
		actorCollection->AddItem(a);
		}

            if(x == COLUMNS -1 && !current->youCanGoEast())
		{
		vtkSmartPointer<vtkActor> a = CreatePlaneActor(mapper, texture, current->getX() + OFFSET, -1 * current->getY(), 0, 0,0,0,1,0,0);
                renderer->AddActor(a);
		actorCollection->AddItem(a);
		}

        }
    }
    actorCollection->InitTraversal();
}

int main(int argc,char *argv[])
{
	  //Create a renderer, render window and interactor
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
  vtkActorCollection * actorCollection = vtkActorCollection::New();

	std::vector<std::vector<Node*> > maze;
	initMaze(maze);
	vtkMath::RandomSeed(time(NULL));
	//generateMaze(maze, maze[0][0]);
	generateMaze2(maze);
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
astar(maze, maze[0][0], maze[COLUMNS-1][ROWS-1]);
std::cout << "Shortest Path\n";
for(int j = 0; j < COLUMNS; j++)
	{
	for(int i = 0; i < ROWS; i++)
		{
			std::cout << maze[j][i]->getF() << "\t";
		}
		std::cout << endl;
	}
	printMaze(maze, renderer,actorCollection);

  // Camera
  vtkSmartPointer<vtkCamera> camera =
    vtkSmartPointer<vtkCamera>::New();
//  camera->SetPosition(ROWS/2, -1 * COLUMNS/2, 10);
//  camera->SetFocalPoint(ROWS/2, -1 * COLUMNS/2, 00);
    camera->SetPosition(0,0,0);
    camera->SetFocalPoint(1,0,0);
    camera->Roll(90);
  renderer->SetActiveCamera(camera);

//Keyboard Style
  vtkSmartPointer<KeyPressInteractorStyle> keystyle =
	    vtkSmartPointer<KeyPressInteractorStyle>::New();
  keystyle->SetCamera(camera, actorCollection, maze);
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

  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
