#ifndef TIMERCALLBACK_H
#define TIMERCALLBACK_H

#include <vtkCommand.h>
#include <vector>
#include "Node.h"

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
        std::cout << this->TimerCount << endl;
    }
    void setMaze(std::vector<std::vector<Node*> > & m)
    {
      maze = m;
    }
  private:
    int TimerCount;
    std::vector<std::vector<Node*> > maze;
};

#endif
