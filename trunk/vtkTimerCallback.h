#ifndef TIMERCALLBACK_H
#define TIMERCALLBACK_H

#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vector>
#include "Node.h"

#define MOVESPEED .02
#define ANGLESPEED .01
#define TOLERANCE .02

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
    void setMaze(std::vector<std::vector<Node*> > & m, std::vector<Node*> p)
    {
      maze = m;
      path = p;
    }
    void clearVisit()
    {
	for(int y = 0; y < maze.size(); y++)
		for(int x = 0; x < maze[y].size(); x++)
			maze[y][x]->unvisit();
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
	//std::cout << "(X,Y): " << xPosition << "," << yPosition << std::endl;
	int row = -yPosition;
	int col = xPosition;
	//std::cout << "(C,R): " << col << "," << row << std::endl;
        double * f = Camera->GetFocalPoint();
        double xFocus = f[0];
        double yFocus = f[1];
	//std::cout << "[x,y]: " << xFocus << " " << yFocus << std::endl;
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
/*
	if(current->getChild())
		{
		next = maze[row][col]->getChild();
		}
*/
	if(path.size() > 0 && path[0] != current)
	{
		std::cout << path[0]->getX() << " " << path[0]->getY() << " - " << current->getX() << " " << current->getY() <<  std::endl;
		path.erase(path.begin());
	}
	if(path.size() > 1)
	{
		next = path[1];
	}
	else
		{
		Camera->Roll(1);
		return;
		}
	
	//Go East
	if(current->getX() < next->getX() && fabs(yPosition - (double)-row) < TOLERANCE)
	{
		if(fabs(-yFocus - (double) next->getY()) < TOLERANCE && current->getX() < xFocus)
		{
			Camera->SetPosition(xPosition+ MOVESPEED,yPosition,0);
		        Camera->SetFocalPoint(xFocus + MOVESPEED, yFocus, 0);
		}
		else
		{
			if(yPosition < yFocus)
				angle -= ANGLESPEED;
			else
				angle += ANGLESPEED;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
		}
	}
	//Go South
	else if((current->getY() < next->getY()) && fabs(xPosition - current->getX()) < TOLERANCE)
	{	
		if((fabs(xFocus - (double)next->getX()) < TOLERANCE) && -current->getY() > yFocus)
		{
			//std::cout << "Move South\n";
			Camera->SetPosition(xPosition,yPosition - MOVESPEED,0);
		        Camera->SetFocalPoint(xFocus, yFocus - MOVESPEED, 0);
		}
		else
		{
			if(xPosition < xFocus)
				angle -= ANGLESPEED;
			else
				angle += ANGLESPEED;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			xFocus = xPosition + cos(angle);
			yFocus = yPosition + sin(angle);
		}
	}
	//Go West
	else if( ((fabs(-yPosition - (double)row)) < TOLERANCE ) && ( current->youCanGoEast() || (next->youCanGoEast() && current->youCanGoWest()) ) && (current->getX() > next->getX()) || ((fabs(xPosition - current->getX()) > TOLERANCE) && xPosition > col ))
	{
	if( (((fabs(next->getX() - xPosition) < TOLERANCE) || (col < xPosition)) && (fabs(fabs(yPosition) - fabs(yFocus)) < TOLERANCE)) && (xFocus < xPosition) && (xFocus < xPosition) )
		{
			//std::cout << "Move West\n";
			Camera->SetPosition(xPosition - MOVESPEED,yPosition,0);
		        Camera->SetFocalPoint(xFocus - MOVESPEED, yFocus, 0);
		}
		else
		{
			if(yPosition > yFocus)
				angle -= ANGLESPEED;
			else
				angle += ANGLESPEED;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);

		}
	}
//Go North
//
	else if((current->youCanGoSouth() || (next->youCanGoSouth() && current->youCanGoNorth()) ) &&  ((current->getY() > next->getY()) || (yPosition < (double)(row * -1)) ) || ( fabs(xPosition - col) < TOLERANCE) )
	{
	//std::cout << (current->getY() > next->getY()) << " " << (-yPosition > (double)row) << " " << ( fabs(xPosition - col) < TOLERANCE) << " " << ( xPosition != (double) col) <<" N\n";
	if((fabs(xFocus - (double)next->getX()) < TOLERANCE) && -current->getY() < -yFocus || (fabs(xFocus - xPosition) < TOLERANCE) && (yPosition < -row) && (yFocus > yPosition))

		{
			//std::cout << "Move North\n";
			Camera->SetPosition(xPosition,yPosition + MOVESPEED,0);
		        Camera->SetFocalPoint(xFocus, yFocus + MOVESPEED, 0);
		}
		else
		{
			if(xPosition > xFocus)
				angle -= ANGLESPEED;
			else
				angle += ANGLESPEED;
		        Camera->SetFocalPoint(xPosition + cos(angle), yPosition + sin(angle), 0);
			xFocus = xPosition + cos(angle);
			yFocus = yPosition + sin(angle);

		}
	}
	else
	{
		std::cout << "NONE\n";
	//std::cout << (current->getY() > next->getY()) << " " << (-yPosition > (double)row) << " " << ( fabs(xPosition - col) < TOLERANCE) << " " << ( xPosition != (double) col) <<"\n";
	}
        //std::cout << Camera->GetFocalPoint(.erase(openlist.begin())[0] << "," << Camera->GetFocalPoint()[1] << std::endl;
      
    }
  private:
    int TimerCount;
    std::vector<std::vector<Node*> > maze;
    std::vector<Node*> path;
    vtkSmartPointer<vtkCamera> Camera;
    vtkSmartPointer<vtkRenderWindowInteractor> Interactor;
};

#endif
