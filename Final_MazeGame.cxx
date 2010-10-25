/*****************************************
Daniel Souza
souzad@rpi.edu
Intro to Visualization
HW01 3D DIAGRAM OF ENVIRONMENT OR OBJECT

Summary:
This program reads in a maze from a text
file, displays the maze using VTK, and 
lets the user interact with sphere to 
solve the maze.

The maze must be rectangular and the first
line of the text file must be the number of
rows followed by the number of columns 
(seperated by a space).
Maze Key:
* = Wall
S = Start
E = End
  = Space

Command Line Arguments:
The text file with the maze data.
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
#include <vtkMath.h>
#include <vtkImageData.h>
#include <vtkJPEGReader.h>
#include <vtkTextureMapToPlane.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Node.h"

#define MAXVELOCITY .2
#define ACCELERATION .02

#define ROWS 10
#define COLUMNS 10

#define TEXTURE1 "texture1.jpg"

// Define interaction style
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  private:
	vtkSmartPointer<vtkActor> ControlActor;  //Player
	vtkSmartPointer<vtkActor> EndActor;      //End Condition
	vtkSmartPointer<vtkPolyData> Cube;
	vtkSmartPointer<vtkPolyData> Sphere;
	std::vector <vtkSmartPointer<vtkActor> > MazeBlocks;
	bool End;
	double VELOCITY;
  public:
    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);
    void SetControlActor(vtkSmartPointer<vtkActor> actor, vtkSmartPointer<vtkPolyData> sphereSource)
    {
	ControlActor = actor;
	Sphere = sphereSource;
	VELOCITY = 0;
    }
    void SetMazeBlocks(std::vector <vtkSmartPointer<vtkActor> > Blocks, vtkSmartPointer<vtkPolyData> cubeSource,  vtkSmartPointer<vtkActor> EndBlock)
    {
	MazeBlocks = Blocks;
	Cube = cubeSource;
	EndActor = EndBlock;
	End = false;
    }
    //checks if actor is intersecting with maze or end condition
    //returns true if it is intersecting, and false otherwise
    bool isinside(vtkSmartPointer<vtkActor> actor) {
	double * SphereBounds = ControlActor->GetBounds();
	double Xmin = SphereBounds[0];
	double Xmax = SphereBounds[1];
	double Ymin = SphereBounds[2];
	double Ymax = SphereBounds[3];
	double * EndBounds = EndActor->GetBounds();
	//The player has reached the end.
	if((Xmin > EndBounds[0] && Xmin < EndBounds[1] || 
		Xmax > EndBounds[0] && Xmax < EndBounds[1]) &&
		(Ymin > EndBounds[2] && Ymin < EndBounds[3] ||
		Ymax > EndBounds[2] && Ymax < EndBounds[3]))
	{
	  // Setup the text and add it to the window
	vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = 
	      vtkSmartPointer<vtkCornerAnnotation>::New();
	cornerAnnotation->SetLinearFontScaleFactor( 2 );
	cornerAnnotation->SetNonlinearFontScaleFactor( 1 );
	cornerAnnotation->SetMaximumFontSize( 20 );
	cornerAnnotation->SetText( 2, "TheEnd" );
	cornerAnnotation->GetTextProperty()->SetColor( 1,0,0);
 
	this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddViewProp(cornerAnnotation);
	ControlActor->GetProperty()->SetColor(0,1,0);
	End = true;
	return true;
	}
	//Check for collisions
	for(int i = 0; i < MazeBlocks.size(); i++) {
		double * CubeBounds = MazeBlocks[i]->GetBounds();
		if((Xmin > CubeBounds[0] && Xmin < CubeBounds[1] || 
			Xmax > CubeBounds[0] && Xmax < CubeBounds[1]) &&
			(Ymin > CubeBounds[2] && Ymin < CubeBounds[3] ||
			Ymax > CubeBounds[2] && Ymax < CubeBounds[3]))
		{
			//SPLAT - Change cube color to player color and assign a new color to player for a splat effect
			MazeBlocks[i]->GetProperty()->SetColor(ControlActor->GetProperty()->GetColor());
			ControlActor->GetProperty()->SetColor(vtkMath::Random(0.5,1.0),vtkMath::Random(0.5,1.0),vtkMath::Random(0.5,1.0));
			VELOCITY = 0;
			return true;
		}
	}
	return false;
	}
//Almost got collision detection working the "right" way shown in class but
//came across issues since the polydata was coming from the source, not actor
//and there were many actors based on the same source.
/*
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(MazeBlocks[i]->GetMapper()->GetInputAsDataSet());
    vtkSmartPointer<vtkTransform> transform =
      vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(MazeBlocks[i]->GetMatrix());
 
    vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyData =
      vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformPolyData->SetInputConnection(polydata->GetProducerPort());
    transformPolyData->SetTransform(transform);
    transformPolyData->Update();

    vtkSmartPointer<vtkSelectEnclosedPoints> selectEnclosedPoints =
      vtkSmartPointer<vtkSelectEnclosedPoints>::New();
	selectEnclosedPoints->SetInput(transformPolyData->GetOutput());
	selectEnclosedPoints->SetSurface(Sphere);
	selectEnclosedPoints->Update();

 
    vtkDataArray* insideArray = vtkDataArray::SafeDownCast(selectEnclosedPoints->GetOutput()->GetPointData()->GetArray(1));
std::cout << "InsideArray:\n";
	for(vtkIdType i = 0; i < insideArray->GetNumberOfTuples(); i++)
	{
	std::cout << insideArray->GetComponent(i,0) << std::endl;
	if(insideArray->GetComponent(i,0) == 1)
		{
		std::cout << "Inside\n";
		//return true;
		}
	}
	}
	std::cout << "Not Inside\n";
	return false;

      }
*/
    //Handles Keypresses
    virtual void OnKeyPress() 
    {
      if(End == true)
	return;
      // Get the keypress
      vtkRenderWindowInteractor *rwi = this->Interactor;
      std::string key = rwi->GetKeySym();
      //Get actor position
      double * Position = ControlActor->GetPosition();
      double x = Position[0];
      double y = Position[1];
      double z = Position[2];

      // Handles the arrow keys and moves the Player accordingly
      if(key.compare("Up") == 0)
        {
	if(VELOCITY <= MAXVELOCITY)
	VELOCITY += ACCELERATION;
	ControlActor->SetPosition(x,y+VELOCITY,z);
        }
       if(key.compare("Down") == 0)
        {
	if(VELOCITY <= MAXVELOCITY)
	VELOCITY += ACCELERATION;
	ControlActor->SetPosition(x,y-VELOCITY,z);
        }
      if(key.compare("Left") == 0)
        {
	if(VELOCITY <= MAXVELOCITY)
	VELOCITY += ACCELERATION;
	ControlActor->SetPosition(x-VELOCITY,y,z);
        }
      if(key.compare("Right") == 0)
        {
	if(VELOCITY <= MAXVELOCITY)
	VELOCITY += ACCELERATION;

	ControlActor->SetPosition(x+VELOCITY,y,z);
        }
	//If the player is colliding with something, 
	if(isinside(ControlActor))
	ControlActor->SetPosition(x,y,z);
      this->Interactor->GetRenderWindow()->Render();
    }
 
};
vtkStandardNewMacro(KeyPressInteractorStyle);

vtkSmartPointer<vtkActor> CreateCubeActor(vtkSmartPointer<vtkPolyDataMapper> mapper,double x, double y, double z, double R, double G, double B);
vtkSmartPointer<vtkActor> CreateArrowActor(vtkSmartPointer<vtkPolyDataMapper> mapper,double x, double y, double z, double degZ, double R, double G, double B);
int mazereader (std::vector<std::string*> &maze,int &rows, int &columns, char * filename);
int mazesolver (std::vector<std::string*> &maze,int &rows, int &columns);
int mazeprinter (std::vector<std::string*> &maze, int &rows, int &columns);

void initMaze(std::vector<std::vector<Node*> > & maze)
{
	for(int y=0; y < COLUMNS; y++)
	{
		std::vector<Node*> row;
		maze.push_back(row);
		for(int x=0; x < ROWS; x++)
			maze[y].push_back(new Node(x,y));
	}
	for(int y=0; y < COLUMNS; y++)
		for(int x=0; x < ROWS; x++)
		{
			std::cout << "1";
			if(x > 0) {
				maze[y][x]->addEdge(maze[y][x-1]);
			}
			std::cout << "2";
			if(x < ROWS - 1) {
				maze[y][x]->addEdge(maze[y][x+1]);
			}
			std::cout << "3";
			if(y > 0) {
				maze[y][x]->addEdge(maze[y-1][x]);
			}
			std::cout << "4\n";
			if(y < COLUMNS - 1) {
				maze[y][x]->addEdge(maze[y+1][x]);
			}
		}
}

vtkSmartPointer<vtkActor> CreatePlaneActor(vtkSmartPointer<vtkPolyDataMapper> mapper,vtkSmartPointer<vtkTexture> texture, double x, double y, double z, double rotx, double roty, double rotz, double R, double G, double B) {
	vtkSmartPointer<vtkActor> actor = 
	    vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->SetTexture(texture);

	//Transform
	actor->RotateX(rotx);
	actor->RotateY(roty);
	actor->RotateZ(rotz);
	actor->SetPosition(x,y,z);
	//Change Color
	actor->GetProperty()->SetColor(R,G,B);
	return actor;
}

void printMaze(std::vector<std::vector<Node*> > & maze)
{

  vtkSmartPointer<vtkJPEGReader> jPEGReader =
    vtkSmartPointer<vtkJPEGReader>::New();
  jPEGReader->SetFileName ( TEXTURE1 );

  //Create a plane
  vtkSmartPointer<vtkPlaneSource> planeSource =
    vtkSmartPointer<vtkPlaneSource>::New();
///    planeSource->SetPoint1(0,0,0);
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
 
  //Create a renderer, render window and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 

  //Add the actors to the scene
//  renderer->AddActor(actor);
	for(int y=0; y < COLUMNS; y++)
		{
		for(int x=0; x < ROWS; x++)
			{
				Node * current = maze[y][x];
				Node ** edges = current->getEdges();
				for(int i = 0; i < current->getEdgeSize(); i++)
					{
						if(current->getY() < edges[i]->getY())
renderer->AddActor	( CreatePlaneActor(mapper, texture, edges[i]->getX(), - 1 * edges[i]->getY(),0,0,0,90, 0,1,0));
						if(current->getX() > edges[i]->getX())
						renderer->AddActor( CreatePlaneActor(mapper, texture, edges[i]->getX(), - 1 * edges[i]->getY(), 0, 0,0,0,1,0,0));

					}
			}
		}
  renderer->SetBackground(.1,.2,.3); // Background color dark blue
 
  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();
}

int main(int argc,char *argv[])
{
	std::vector<std::vector<Node*> > maze;
	initMaze(maze);
	printMaze(maze);
/*
  std::vector<std::string*> maze;
  int rows, columns;
  //Reads in the Maze from File
   if(argc == 2)
	{
		mazereader(maze,rows,columns,argv[1]);
	}
   else {
		mazereader(maze,rows,columns,(char*) "default.txt");
	}


  //Solves the Maze
  //mazesolver(maze,rows,columns);
  //Creates and Renders the Maze in VTK
   mazeprinter(maze,rows,columns);
*/
  return EXIT_SUCCESS;
}



//Return a cube actor with translation and color
vtkSmartPointer<vtkActor> CreateCubeActor(vtkSmartPointer<vtkPolyDataMapper> mapper, double x, double y, double z, double R, double G, double B) {
	vtkSmartPointer<vtkActor> actor = 
	  vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	//Transform 
	actor->SetPosition(x,y,z);
/*      equivalent
	vtkSmartPointer<vtkTransform> transform =
	  vtkSmartPointer<vtkTransform>::New();
	transform->Translate(x,y,z);
	actor->SetUserTransform(transform);
*/
	//Change Colors
	actor->GetProperty()->SetColor(R,G,B);
	return actor;
}
//Return an arrow actor with translation, zrotation, and color
vtkSmartPointer<vtkActor> CreateArrowActor(vtkSmartPointer<vtkPolyDataMapper> mapper,double x, double y, double z,double degZ, double R, double G, double B) {
	vtkSmartPointer<vtkActor> actor = 
	    vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	//Transform
	actor->SetPosition(x,y,z);
	//Change Color
	actor->GetProperty()->SetColor(R,G,B);

	return actor;
}
//Return a sphere actor with translation and color
vtkSmartPointer<vtkActor> CreateSphereActor(vtkSmartPointer<vtkPolyDataMapper> mapper,double x, double y, double z, double R, double G, double B) {
	vtkSmartPointer<vtkActor> actor = 
	    vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	//Transform
	actor->SetPosition(x,y,z);
	//Change Color
	actor->GetProperty()->SetColor(R,G,B);
	return actor;
}

//Check if file esists
bool fexists(const char *filename)
{
  ifstream ifile(filename);
  return ifile;
}
//opens maze file and stores data into a vector of strings (essentially a 2D character array)
int  mazereader (std::vector<std::string*> &maze, int &rows, int &columns, char * filename) {
	if(!fexists(filename)) {
		std::cout << "File " << filename << "  not found\n";
		exit(1);
	}
	std::ifstream mazefile(filename);
	//Read in rows x columns
	mazefile >> rows;
	mazefile >> columns;
	std::string buffer;
	getline(mazefile,buffer);//clearline
	if(rows < 0 || columns < 0) {
		std::cout << "Error parsing rows X columns.\n";
		exit(1);
	}
	int current = 0;
	while(mazefile && current < rows)
	{
		std::string * line = new std::string;
		std::getline(mazefile,*line);
		//Specified columns size does not match expected
		if(line->length() < columns) {
			std::cout << "Specified columns size does not match expected on line: "<< current+1 << " " << columns << " " << line->length() << "\n";
			exit(1);
		}
		else
		{
			maze.push_back(line);
			//cout << *line << std::endl; //prints out maze in ascii
			current++;
		}

	}
	//Specified rows and actual rows do not match
	if(current < rows) {
		std::cout << "Specified number of rows does not match expected: "<< current+1 << " \n";
		exit(1);
	}

	mazefile.close();
	return 0;
}
//Recursive function that iterates through the 2D maze to find the End "E"
//Marks bad paths with X, and keeps track of the actual path with directional character U,D,L,R
int mazestep(std::vector<std::string*> &maze,  int &rows,  int & columns, int x, int y)
{
	//End found
	if((*maze[y])[x] == 'E')
		{
			return 1;
		}
	//down
	if(y + 1 < rows && (*maze[y+1])[x]==' ' || (*maze[y+1])[x]=='E')
		{
			(*maze[y])[x] = 'D';
			if(mazestep(maze,rows,columns,x,y+1) < 1)
				{
					(*maze[y])[x] ='X';
				}
			else
				return 1;
		}
	//right
	if((x + 1) < columns && (*maze[y])[x+1]==' ' || (*maze[y])[x+1]=='E')
		{
			(*maze[y])[x] = 'R';
			if(mazestep(maze,rows,columns,x+1,y) < 1)
				{
					(*maze[y])[x] ='X';
				}
			else
				return 1;
		}
	//up
	if((y - 1) < rows && (y - 1) >= 0 && (*maze[y-1])[x]== ' ' || (*maze[y-1])[x]=='E')
		{
			(*maze[y])[x] = 'U';
			if(mazestep(maze,rows,columns,x,y-1) < 1)
				{
					(*maze[y])[x] ='X';
				}
			else
				return 1;
		}
	//left
	if((x - 1) < columns && (x - 1) >= 0 && (*maze[y])[x-1]==' ' || (*maze[y])[x-1]=='E')
		{
			(*maze[y])[x] = 'L';
			if(mazestep(maze,rows,columns,x-1,y) < 1)
				{
					(*maze[y])[x] ='X';
				}
			else
				return 1;
		}
	return 0;
}
//Finds Start "S" in maze and calls recursive function mazestep to fill in the maze
//with the solution
int mazesolver (std::vector<std::string*> &maze,  int &rows,  int &columns)
{
	int startx;
	int starty;
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < columns; j++)
			{
			   if((*maze[i])[j] == 'S')
				{
				mazestep(maze,rows,columns,j,i);
				(*maze[i])[j] = 'S';
				continue;
				}			
		}
		//std::cout << "i: " << *maze[i] << std::endl; //print solved ascii maze on commandline
	}
	return 0;
}
//Create Rendering Environment and Populate With Actors
int mazeprinter (std::vector<std::string*> &maze, int &rows, int &columns)
{
	vtkSmartPointer<vtkActor> Player;
	vtkSmartPointer<vtkActor> EndBlock;
	std::vector <vtkSmartPointer<vtkActor> > MazeBlocks;
	// Create a cube.
	vtkSmartPointer<vtkCubeSource> cubeSource = 
	  vtkSmartPointer<vtkCubeSource>::New();
	// Create a sphere.
	vtkSmartPointer<vtkSphereSource> sphereSource = 
	  vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(.4);
	// Create an arrow.
	vtkSmartPointer<vtkArrowSource> arrowSource = 
	  vtkSmartPointer<vtkArrowSource>::New();
	// Create a mappers
	vtkSmartPointer<vtkPolyDataMapper> cubemapper = 
	  vtkSmartPointer<vtkPolyDataMapper>::New();
	cubemapper->SetInputConnection(cubeSource->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> spheremapper = 
	  vtkSmartPointer<vtkPolyDataMapper>::New();
	spheremapper->SetInputConnection(sphereSource->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> arrowmapper = 
	  vtkSmartPointer<vtkPolyDataMapper>::New();
	arrowmapper->SetInputConnection(arrowSource->GetOutputPort());
	// Create a renderer, render window, and interactor
	vtkSmartPointer<vtkRenderer> renderer = 
	  vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = 
	  vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
	  vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderer->SetBackground(0, 0, 0); // Set Background Black

	//Add Interactor
  	vtkSmartPointer<KeyPressInteractorStyle> style = 
	    vtkSmartPointer<KeyPressInteractorStyle>::New();
	renderWindowInteractor->SetInteractorStyle(style);

	//populate scene with actors
	//organized this way to easily change colors of different elements
	for(int y = 0; y < rows; y++) {
		for(int x = 0; x < columns; x++)
			{
			   if((*maze[y])[x] == '*') {
				vtkSmartPointer<vtkActor> Block = CreateCubeActor(cubemapper,x,-y,0,1,1,1);
				MazeBlocks.push_back(Block);
				renderer->AddActor(Block);  //White Block
				}
			   else if((*maze[y])[x] == 'S') {
				Player = CreateSphereActor(spheremapper,x,-y,0,1,1,0);
				renderer->AddActor(Player);  //Green Block
				}
			   else if((*maze[y])[x] == 'E')
				{
				EndBlock =   CreateCubeActor(cubemapper,x,-y,0,0,1,0); //Green Block
				renderer->AddActor(EndBlock);
				}
			   else if((*maze[y])[x] == 'U')
				renderer->AddActor(CreateArrowActor(arrowmapper,x,-y,0,90,1,0,0)); //Red Arrow 
			   else if((*maze[y])[x] == 'D')
				renderer->AddActor(CreateArrowActor(arrowmapper,x,-y,0,270,1,0,0)); //Red Arrow 
			   else if((*maze[y])[x] == 'L')
				renderer->AddActor(CreateArrowActor(arrowmapper,x,-y,0,180,1,0,0)); //Red Arrow 
			   else if((*maze[y])[x] == 'R')
				renderer->AddActor(CreateArrowActor(arrowmapper,x,-y,0,0,1,0,0)); //Red Arrow 
		}
		delete maze[y]; //free dynamic memory
	}


	//Set Player and Maze With Interactor
	style->SetControlActor(Player,sphereSource->GetOutput());
	style->SetMazeBlocks(MazeBlocks, cubeSource->GetOutput(), EndBlock);
	// Render and interact
	renderWindow->Render();
	renderWindowInteractor->Start();
	return 0;	
}
