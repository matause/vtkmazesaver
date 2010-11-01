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

#include "Node.h"

#define MAXVELOCITY .2
#define ACCELERATION .02

#define ROWS 10
#define COLUMNS 10

#define OFFSET -.5

#define TEXTURE1 "texture1.jpg"

// Define interaction style
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  private:
	vtkSmartPointer<vtkActor> ControlActor;  //Player
	vtkSmartPointer<vtkActor> EndActor;      //End Condition
	vtkSmartPointer<vtkCamera> Camera;
	vtkSmartPointer<vtkPolyData> Cube;
	vtkSmartPointer<vtkPolyData> Sphere;
	std::vector <vtkSmartPointer<vtkActor> > MazeBlocks;
	vtkActorCollection * actorCollection;
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
    void SetCamera(vtkSmartPointer<vtkCamera> c, vtkActorCollection * ac)
    {
        Camera = c;
	actorCollection = ac;
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
      // Get the keypress
      vtkRenderWindowInteractor *rwi = this->Interactor;
      std::string key = rwi->GetKeySym();
      //Get actor position
	double * p = Camera->GetPosition();
	double x = p[0];
	double y = p[1];
	double z = p[2];
	double * f = Camera->GetFocalPoint();
	
      // Handles the arrow keys and moves the Player accordingly
      if(key.compare("Up") == 0)
        {
	Camera->SetPosition(x+1,y,z);
	Camera->SetFocalPoint(f[0]+1, f[1], f[2]);
/*
	if(VELOCITY <= MAXVELOCITY)
	VELOCITY += ACCELERATION;
	ControlActor->SetPosition(x,y+VELOCITY,z);
*/
        }
       if(key.compare("Down") == 0)
        {
	Camera->SetPosition(x-1,y,z);
	Camera->SetFocalPoint(f[0]-1, f[1], f[2]);
/*
	if(VELOCITY <= MAXVELOCITY)
	VELOCITY += ACCELERATION;
	ControlActor->SetPosition(x,y-VELOCITY,z);
*/
        }
      if(key.compare("Left") == 0)
        {
	Camera->SetPosition(x,y-1,z);
	Camera->SetFocalPoint(f[0], f[1] - 1, f[2]);
/*
	if(VELOCITY <= MAXVELOCITY)
	VELOCITY += ACCELERATION;
	ControlActor->SetPosition(x-VELOCITY,y,z);
*/
        }
      if(key.compare("Right") == 0)
        {
	Camera->SetPosition(x,y+1,z);
	Camera->SetFocalPoint(f[0], f[1]+1, f[2]);
/*
	if(VELOCITY <= MAXVELOCITY)
	VELOCITY += ACCELERATION;

	ControlActor->SetPosition(x+VELOCITY,y,z);
*/
        }
	//If the player is colliding with something,

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
		vtkSmartPointer<vtkActor> a = CreatePlaneActor(mapper, texture, current->getX() + OFFSET, - 1 * current->getY(), 0, 0,0,0,1,0,0);
                renderer->AddActor(a);
		actorCollection->AddItem(a);
		}
            if(y == COLUMNS - 1 && !current->youCanGoWest())
		{
		vtkSmartPointer<vtkActor> a = CreatePlaneActor(mapper, texture, current->getX(), - 1 * (current->getY() + 1) + OFFSET, 0, 0,0,90,0,1,0);
                renderer->AddActor(a);
		actorCollection->AddItem(a);
		}
            if(x == 0 && !current->youCanGoEast()) 
		{
		vtkSmartPointer<vtkActor> a = CreatePlaneActor(mapper, texture, current->getX() - 1 + OFFSET, -1 * current->getY(), 0, 0,0,0,1,0,0);
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
	printMaze(maze, renderer,actorCollection);

  // Camera
  vtkSmartPointer<vtkCamera> camera =
    vtkSmartPointer<vtkCamera>::New();
//  camera->SetPosition(ROWS/2, -1 * COLUMNS/2, 10);
//  camera->SetFocalPoint(ROWS/2, -1 * COLUMNS/2, 00);
    camera->SetPosition(.5,-.5,0);
    camera->SetFocalPoint(1,-.5,0);
    camera->Roll(90);
  renderer->SetActiveCamera(camera);

//Keyboard Style
  vtkSmartPointer<KeyPressInteractorStyle> keystyle =
	    vtkSmartPointer<KeyPressInteractorStyle>::New();
  keystyle->SetCamera(camera, actorCollection);
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
