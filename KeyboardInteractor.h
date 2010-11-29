#ifndef _KEYBOARDINTERACTOR_H
#define _KEYBOARDINTERACTOR_H

#include <vector>

#include "Node.h"

#define PI 3.14159265

#define MOVEINCREMENT 0.2
#define ANGLEINCREMENT PI/24

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
        std::cout << "col: " << tempMaze[row][col]->getX() << " row: " << tempMaze[row][col]->getY() << std::endl;

        this->Interactor->GetRenderWindow()->Render();
    }
};

vtkStandardNewMacro(KeyPressInteractorStyle);
#endif
