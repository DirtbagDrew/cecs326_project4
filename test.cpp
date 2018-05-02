#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include "semaphore.h"
using namespace std;

const int MAXCHAR = 10;
const int BUFFSIZE = 3;
enum{semU, semV,semW};


//g++ main.cpp -o main -lpthread


int main() {
  // PID for the fork
  pid_t childPID1, childPID2,childPID3,childPID4;

  SEMAPHORE sem(3);

  sem.V(semU);
  sem.V(semV);
  sem.V(semW);
  sem.V(semW);

  childPID1 = fork();
  string end;
  if(childPID1)  //parent
  {

  }
  else
  {
    childPID2=fork();
    if(childPID2) //first child
    {

    }
    else
    {
      childPID3=fork();
      if(childPID3) //second child
      {

      }
      else
      {
        childPID4=fork();
        if(childPID4) //third child
        {

        }
        else  //fourth child
        {

        }
      }
    }
  }
  
  return 0;
}
