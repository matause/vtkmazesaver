#ifndef _KEYBOARDINTERACTOR_H
#define _KEYBOARDINTERACTOR_H

#include <vector>
#include <iostream>
#include <stdlib.h>
#include "Node.h"

#define PI 3.14159265

#define MOVEINCREMENT 0.2
#define ANGLEINCREMENT PI/24

#define COLLISIONBUFFER .4

// Define interaction style
class KeyPressInteractorStyle : public vtkInteractorStyle/*vtkInteractorStyleTrackballCamera*/
{
  private:
	vtkSmartPointer<vtkCamera> Camera;
	vtkActorCollection * actorCollection;
	std::vector<std::vector<Node*> > tempMaze;
	bool *upDown;
	bool *downDown;
	bool *leftDown;
	bool *rightDown;
  public:
    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);

    void SetCamera(vtkSmartPointer<vtkCamera> c, vtkActorCollection * ac, std::vector<std::vector<Node*> > &nodes, bool &up, bool &down, bool &left, bool &right)
    {
        Camera = c;
        actorCollection = ac;
        tempMaze = nodes;
        upDown = &up;
        downDown = &down;
        leftDown = &left;
        rightDown = &right;
    }

    //Handles Keypresses
    virtual void OnKeyPress()
    {
        system("xset r off");
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

        // Handles the arrow keys and moves the Player accordingly
        if(key.compare("Up") == 0)
        {
            *upDown = true;
            double newX = xPosition + MOVEINCREMENT*(xLook);
            double newY = yPosition + MOVEINCREMENT*(yLook);
            int row = -yPosition;
            int col = xPosition;
            if(!tempMaze[row][col]->youCanGoWest() && newX < col - COLLISIONBUFFER)
                newX = col - COLLISIONBUFFER;
            else if(!tempMaze[row][col]->youCanGoEast() && newX > col + COLLISIONBUFFER)
                newX = col + COLLISIONBUFFER;
            if(!tempMaze[row][col]->youCanGoNorth() && newY > -row + COLLISIONBUFFER)
                newY = -row + COLLISIONBUFFER;
            else if(!tempMaze[row][col]->youCanGoSouth() && newY < -row - .4)
                newY = -row - COLLISIONBUFFER;

            Camera->SetPosition(newX, newY, 0);
        }
        if(key.compare("Down") == 0)
        {
            *downDown = true;
            double newX = xPosition - MOVEINCREMENT*(xLook);
            double newY = yPosition - MOVEINCREMENT*(yLook);
            int row = -yPosition;
            int col = xPosition;
            if(!tempMaze[row][col]->youCanGoWest() && newX < col - COLLISIONBUFFER)
                newX = col - COLLISIONBUFFER;
            else if(!tempMaze[row][col]->youCanGoEast() && newX > col + COLLISIONBUFFER)
                newX = col + COLLISIONBUFFER;
            if(!tempMaze[row][col]->youCanGoNorth() && newY > -row + COLLISIONBUFFER)
                newY = -row + COLLISIONBUFFER;
            else if(!tempMaze[row][col]->youCanGoSouth() && newY < -row - COLLISIONBUFFER)
                newY = -row - COLLISIONBUFFER;

            Camera->SetPosition(newX, newY, 0);
        }
        if(key.compare("Right") == 0)
        {
            *rightDown = true;
            angle -= ANGLEINCREMENT;
        }
        if(key.compare("Left") == 0)
        {
            *leftDown = true;
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

        this->Interactor->GetRenderWindow()->Render();
    }

    virtual void OnKeyRelease()
    {
        system("xset r off");
        //std::cout << "Key Release" << std::endl;

        // Get the keypress
        vtkRenderWindowInteractor *rwi = this->Interactor;
        std::string key = rwi->GetKeySym();

        if(key.compare("Up") == 0)
        {
            *upDown = false;
        }
        if(key.compare("Down") == 0)
        {
            *downDown = false;
        }
        if(key.compare("Left") == 0)
        {
            *leftDown = false;
        }
        if(key.compare("Right") == 0)
        {
            *rightDown = false;
        }
    }
};

vtkStandardNewMacro(KeyPressInteractorStyle);
#endif
