#ifndef TIMERCALLBACK_H
#define TIMERCALLBACK_H

#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vector>
#include "Node.h"

#define MOVESPEED .01

class vtkTimerCallback : public vtkCommand
{

  public:
    static vtkTimerCallback *New()
    {
      vtkTimerCallback *cb = new vtkTimerCallback;
      cb->TimerCount = 0;
      return cb;
    }

    virtual void Execute(vtkObject *vtkNotUsed(caller), unsigned long eventId,
                         void *vtkNotUsed(callData))
    {
      if (vtkCommand::TimerEvent == eventId)
        {
        ++this->TimerCount;
        }
        //std::cout << this->TimerCount << endl;
	moveCamera();
        Interactor->GetRenderWindow()->Render();
    }
    void setMaze(std::vector<std::vector<Node*> > & m)
    {
      maze = m;
    }
    void setCamera(vtkSmartPointer<vtkCamera> & c, vtkSmartPointer<vtkRenderWindowInteractor> & r)
    {
      Camera = c;
      Interactor = r;
    }
    void moveCamera()
    {
        double * p = Camera->GetPosition();
        double xPosition = p[0];
        double yPosition = p[1];
	int row = -yPosition;
	int col = xPosition;
        double * f = Camera->GetFocalPoint();
        double xFocus = f[0];
        double yFocus = f[1];

        double xLook = xFocus - xPosition;
        double yLook = yFocus - yPosition;
        double angle;

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
	Node * current = maze[row][col];
	Node * next;
	if(current->getChild())
		next = maze[row][col]->getChild();
	else if(current->getParent())
		next = maze[row][col]->getParent();
/*
	//Go West
	if(current->getX() > next->getX() || current->getX() < col)
	{
		std::cout << "W\n";
		std::cout << "Y " << yFocus << " " << next->getY() <<" "<<fabs(yFocus - (double) next->getY()) << std::endl;
		std::cout << "X " << current->getX() << " " << xFocus << std::endl;
		if(fabs(-yFocus - (double) next->getY()) < 0.01 && current->getX() > xFocus)
		{
			Camera->SetPosition(xPosition- MOVESPEED,yPosition,0);
		        Camera->SetFocalPoint(xFocus - MOVESPEED, yFocus, 0);
		}
		else
		{
			angle -= .01;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
		if(fabs(-yFocus - (double) next->getY()) < 0.01 && current->getX() > xFocus)
			        Camera->SetFocalPoint(next->getX(), next->getY(), 0);
		}
	}
	//Go North
	else if(current->getY() > next->getY() || current->getY() < row)
	{
		std::cout <<"\nN\n"<< (fabs(xFocus - (double)next->getX())) << " X " << xFocus << " " << next->getX() << " " << fabs(xFocus - (double)next->getX()) << " \n";
		std::cout << (current->getY() < yFocus) << " Y " << current->getY() << " " << yFocus << std::endl;
		if((fabs(xFocus - (double)next->getX()) < 0.01) && -current->getY() < -yFocus)
		{
			std::cout << "Move North\n";
			Camera->SetPosition(xPosition,yPosition + MOVESPEED,0);
		        Camera->SetFocalPoint(xFocus, yFocus + MOVESPEED, 0);
		}
		else
		{
			angle += .01;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			f = Camera->GetFocalPoint();
			xFocus = f[0];
			yFocus = f[1];
			if((fabs(xFocus - (double)next->getX()) < 0.01) && -current->getY() < -yFocus)
			        Camera->SetFocalPoint(next->getX(), next->getY(), 0);

		}
	}
	//Go East
	else*/ if(current->getX() < next->getX())
	{
		//std::cout << "Y " << yFocus << " " << next->getY() <<" "<<fabs(yFocus - (double) next->getY()) << std::endl;
		//std::cout << "X " << current->getX() << " " << xFocus << std::endl;
		if(fabs(-yFocus - (double) next->getY()) < 0.01 && current->getX() < xFocus)
		{
			Camera->SetPosition(xPosition+ MOVESPEED,yPosition,0);
		        Camera->SetFocalPoint(xFocus + MOVESPEED, yFocus, 0);
		}
		else
		{
			angle += .01;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			if(fabs(-yFocus - (double) next->getY()) < 0.01 && current->getX() < xFocus)
			        Camera->SetFocalPoint(next->getX(), next->getY(), 0);
		}
	}
	//Go South
	else if(current->getY() < next->getY())
	{	
		//std::cout <<"\nS\n"<<(fabs(xFocus - (double)next->getX()) < 0.01) << " x " << xFocus << " " << next->getX() << " " << fabs((double)xFocus - (double)next->getX()) << " \n";
		//std::cout << (-current->getY() < yFocus) << " y " << -current->getY() << " " << yFocus << std::endl;
		if((fabs(xFocus - (double)next->getX()) < 0.01) && -current->getY() > yFocus)
		{
			//std::cout << "Move South\n";
			Camera->SetPosition(xPosition,yPosition - MOVESPEED,0);
		        Camera->SetFocalPoint(xFocus, yFocus - MOVESPEED, 0);
		}
		else
		{
			angle -= .01;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			if((fabs(xFocus - (double)next->getX()) < 0.01) && -current->getY() > yFocus)
			        Camera->SetFocalPoint(next->getX(), next->getY(), 0);
		}
	}

        //std::cout << Camera->GetFocalPoint()[0] << "," << Camera->GetFocalPoint()[1] << std::endl;
      
    }
  private:
    int TimerCount;
    std::vector<std::vector<Node*> > maze;
    vtkSmartPointer<vtkCamera> Camera;
    vtkSmartPointer<vtkRenderWindowInteractor> Interactor;
};

#endif
