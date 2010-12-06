#ifndef TIMERCALLBACK_H
#define TIMERCALLBACK_H

#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vector>
#include "Node.h"

#define MOVESPEED .02
#define ANGLESPEED .01
#define TOLERANCE 1

#define PI 3.14159265

#define MOVEINCREMENT 0.2
#define ANGLEINCREMENT PI/24

#define COLLISIONBUFFER .4

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
	//moveCamera();
	rotateActors();








        vtkRenderWindowInteractor *rwi = this->Interactor;

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
        if(*upDown)
        {
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
        if(*downDown)
        {
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
        if(*rightDown)
        {
            angle -= ANGLEINCREMENT;
        }
        if(*leftDown)
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
        if (*leftDown)
            std::cout << "left down" << std::endl;
        if (*rightDown)
            std::cout << "right down" << std::endl;
        if (*upDown)
            std::cout << "up down" << std::endl;
        if (*downDown)
            std::cout << "down down" << std::endl;



        Interactor->GetRenderWindow()->Render();
    }
    void setMaze(std::vector<std::vector<Node*> > & m, std::vector<Node*> p)
    {
      maze = m;
      path = p;
      for(int y = 0; y < maze.size(); y++)
        for(int x = 0; x < maze[y].size(); x++)
          if(maze[y][x]->checkPolyhedron())
            rotators.push_back(maze[y][x]->getPolyhedron());

    }
    void clearVisit()
    {
	for(int y = 0; y < maze.size(); y++)
		for(int x = 0; x < maze[y].size(); x++)
			maze[y][x]->unvisit();
    }
    void setCamera(vtkSmartPointer<vtkCamera> & c, vtkSmartPointer<vtkRenderWindowInteractor> & r, bool &up, bool &down, bool &left, bool &right, std::vector<std::vector<Node*> > &nodes)
    {
        Camera = c;
        Interactor = r;
        tempMaze = nodes;
        upDown = &up;
        downDown = &down;
        leftDown = &left;
        rightDown = &right;
    }
    void moveCamera()
    {
	if( fabs(fabs(Camera->GetRoll()) - 90) > TOLERANCE)
		return;
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

		}
	}
	else
	{
		std::cout << "NONE\n";
	//std::cout << (current->getY() > next->getY()) << " " << (-yPosition > (double)row) << " " << ( fabs(xPosition - col) < TOLERANCE) << " " << ( xPosition != (double) col) <<"\n";
	}
        //std::cout << Camera->GetFocalPoint(.erase(openlist.begin())[0] << "," << Camera->GetFocalPoint()[1] << std::endl;

     if(Camera->GetRoll() > 10)
	{
	Camera->SetRoll((double)90 -.00001);
	}
     else
 	Camera->SetRoll((double)-90 + .00001);

    }

    void rotateActors()
    {
        double * p = Camera->GetPosition();
        double xPosition = p[0];
        double yPosition = p[1];
	std::cout << Camera->GetRoll() << " " <<(fabs(Camera->GetRoll()) - 90) << std::endl;
	if( fabs((fabs(Camera->GetRoll()) - 90)) > TOLERANCE)
	{
        std::cout <<"Roll" << std::endl;
		Camera->Roll(1);
	}
      for(int i = 0; i < rotators.size(); i++)
	{
	if(fabs(xPosition - rotators[i]->GetPosition()[0]) < TOLERANCE && fabs(yPosition - rotators[i]->GetPosition()[1]) < TOLERANCE)
		{
			rotators[i]->SetPosition(-10,10,-10);
			Camera->Roll(1);
		}
        rotators[i]->RotateZ(1);
	}
    }
  private:
    int TimerCount;
    std::vector<std::vector<Node*> > maze;
    std::vector<Node*> path;
    std::vector <vtkSmartPointer<vtkActor> > rotators;
    vtkSmartPointer<vtkCamera> Camera;
    vtkSmartPointer<vtkRenderWindowInteractor> Interactor;
    bool *upDown;
	bool *downDown;
	bool *leftDown;
	bool *rightDown;
    std::vector<std::vector<Node*> > tempMaze;
};

#endif
