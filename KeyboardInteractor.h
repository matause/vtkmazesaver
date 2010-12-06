#ifndef _KEYBOARDINTERACTOR_H
#define _KEYBOARDINTERACTOR_H

#include <vector>
#include <iostream>
#include <stdlib.h>
#include "Node.h"



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

        // Handles the arrow keys and moves the Player accordingly
        if(key.compare("Up") == 0)
        {
            *upDown = true;
        }
        if(key.compare("Down") == 0)
        {
            *downDown = true;
        }
        if(key.compare("Right") == 0)
        {
            *rightDown = true;
        }
        if(key.compare("Left") == 0)
        {
            *leftDown = true;
        }
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
