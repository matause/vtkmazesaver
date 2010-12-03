#ifndef TIMERCALLBACK_H
#define TIMERCALLBACK_H

#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vector>
#include "Node.h"

#define MOVESPEED .02
#define ANGLESPEED .01
#define TOLERANCE .03

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
	std::cout << "(X,Y): " << xPosition << "," << yPosition << std::endl;
	int row = -yPosition;
	int col = xPosition;
	std::cout << "(C,R): " << col << "," << row << std::endl;
        double * f = Camera->GetFocalPoint();
        double xFocus = f[0];
        double yFocus = f[1];
	std::cout << "[x,y]: " << xFocus << " " << yFocus << std::endl;
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
	
	//Go East
	else */
	if(current->getX() < next->getX() && fabs(yPosition - (double)-row) < TOLERANCE)
	{
		//std::cout << "Y " << yFocus << " " << next->getY() <<" "<<fabs(yFocus - (double) next->getY()) << std::endl;
		//std::cout << "X " << current->getX() << " " << xFocus << std::endl;
		if(fabs(-yFocus - (double) next->getY()) < TOLERANCE && current->getX() < xFocus)
		{
			Camera->SetPosition(xPosition+ MOVESPEED,yPosition,0);
		        Camera->SetFocalPoint(xFocus + MOVESPEED, yFocus, 0);
		}
		else
		{
			angle += ANGLESPEED;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			if(fabs(-yFocus - (double) next->getY()) < TOLERANCE && current->getX() < xFocus)
			        Camera->SetFocalPoint(next->getX(), -next->getY(), 0);
		}
	}
	//Go South
	else if((current->getY() < next->getY()))
	{	
		//std::cout <<"\nS\n"<<(fabs(xFocus - (double)next->getX()) < 0.01) << " x " << xFocus << " " << next->getX() << " " << fabs((double)xFocus - (double)next->getX()) << " \n";
		//std::cout << (-current->getY() < yFocus) << " y " << -current->getY() << " " << yFocus << std::endl;
		if((fabs(xFocus - (double)next->getX()) < TOLERANCE) && -current->getY() > yFocus)
		{
			//std::cout << "Move South\n";
			Camera->SetPosition(xPosition,yPosition - MOVESPEED,0);
		        Camera->SetFocalPoint(xFocus, yFocus - MOVESPEED, 0);
		}
		else
		{
			angle -= ANGLESPEED;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			if((fabs(xFocus - (double)next->getX()) < TOLERANCE) && -current->getY() > yFocus)
			        Camera->SetFocalPoint(next->getX(), -next->getY(), 0);
		}
	}
/*
	//Go West
		else if(current->getX() > next->getX() || xPosition > col && fabs(xFocus - col) < .01)
	{
	std::cout << "W\n";
	if(fabs(-yFocus - (double) next->getY()) < 0.01 && current->getX() > xFocus|| (fabs(yFocus - yPosition) < 0.01) && (xPosition > col))
		{
			std::cout << "Move West\n";
			Camera->SetPosition(xPosition - MOVESPEED,yPosition + MOVESPEED,0);
		        Camera->SetFocalPoint(xFocus - MOVESPEED, yFocus, 0);
		}
		else
		{
			angle += ANGLESPEED;;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			f = Camera->GetFocalPoint();
			xFocus = f[0];
			yFocus = f[1];
			if(fabs(-yFocus - (double) next->getY()) < 0.01 && current->getX() > xFocus)
			{
				std::cout << "Next " << next->getX() << " " << next->getY() << std::endl;
				Camera->SetFocalPoint(next->getX(), next->getY(), 0);
			}

		}
	}
*/
	//Go West
	else if( (current->getX() > next->getX()) || fabs(xPosition - current->getX()) > TOLERANCE  )
	{
	if( ( fabs(xFocus - next->getX()) < TOLERANCE && fabs(-yFocus - next->getY()) < TOLERANCE ) || (fabs(xPosition - current->getX()) < TOLERANCE ) )
		{
			std::cout << "Move West\n";
			Camera->SetPosition(xPosition - MOVESPEED,yPosition,0);
		        Camera->SetFocalPoint(xFocus - MOVESPEED, yFocus, 0);
			std::cin >>col;
		}
		else
		{
			angle += ANGLESPEED;;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			f = Camera->GetFocalPoint();
			xFocus = f[0];
			yFocus = f[1];
			if((fabs(yFocus - (double)next->getY()) < TOLERANCE) && current->getX() < xFocus)
			        Camera->SetFocalPoint(next->getX(), -next->getY(), 0);

		}
	}
//Go North
	else if((current->getY() > next->getY()) || 
	(	(-yPosition > (double)row) && 
		  ( fabs(xPosition - col) < TOLERANCE) && 
		  ( fabs(xPosition - (double) col) > 0.00001) 
	) )
	{
	std::cout << (current->getY() > next->getY()) << " " << (-yPosition > (double)row) << " " << ( fabs(xPosition - col) < TOLERANCE) << " " << ( xPosition != (double) col) <<" N\n";
	std::cout << fabs(xPosition - (double) col) << std::endl;
	if((fabs(xFocus - (double)next->getX()) < TOLERANCE) && -current->getY() < -yFocus || (fabs(xFocus - xPosition) < TOLERANCE) && (yPosition < -row))
		{
			std::cout << "Move North\n";
			Camera->SetPosition(xPosition,yPosition + MOVESPEED,0);
		        Camera->SetFocalPoint(xFocus, yFocus + MOVESPEED, 0);
		}
		else
		{
			angle += ANGLESPEED;;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			f = Camera->GetFocalPoint();
			xFocus = f[0];
			yFocus = f[1];
			if((fabs(xFocus - (double)next->getX()) < TOLERANCE) && -current->getY() < -yFocus)
			        Camera->SetFocalPoint(next->getX(), -next->getY(), 0);

		}
	}
/*
//Go West
	else if(current->getX() > next->getX() || ((xPosition > (double)col) && ((fabs(-yPosition - row)) < TOLERANCE)) && (-yPosition != row))
	{
	std::cout << (fabs(xFocus - xPosition) < TOLERANCE) << " "<<  (yPosition < -row)<< " W\n";
	if((fabs(-yFocus - (double)next->getY()) < TOLERANCE) && current->getX() < xFocus || (fabs(yFocus - yPosition) < TOLERANCE) && (xPosition < col))
		{
			std::cout << "Move West\n";
			Camera->SetPosition(xPosition - MOVESPEED,yPosition,0);
		        Camera->SetFocalPoint(xFocus - MOVESPEED, yFocus, 0);
		}
		else
		{
			angle += ANGLESPEED;;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			f = Camera->GetFocalPoint();
			xFocus = f[0];
			yFocus = f[1];
			if((fabs(-yFocus - (double)next->getY()) < TOLERANCE))
			        Camera->SetFocalPoint(next->getX(), -next->getY(), 0);

		}
	}*/
	else
	{
		std::cout << "NONE\n";
	std::cout << (current->getY() > next->getY()) << " " << (-yPosition > (double)row) << " " << ( fabs(xPosition - col) < TOLERANCE) << " " << ( xPosition != (double) col) <<"\n";
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
