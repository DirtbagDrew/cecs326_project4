#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <semaphore.h>
using namespace std;

//const int MAXCHAR = 10;
//const int BUFFSIZE = 3;
enum{semU, semV,heelp};
sem_t U,V,W;


//g++ main.cpp -o main -lpthread

void procs(sem_t a,sem_t b, sem_t c, string d)
{
  sem_wait(&c);
  while(true)
  {
    if(sem_wait(&a)==0)
    {
      cout<<d<<" U"<<endl;
      //while(true){}
      //sem_post(&a);
      break;
      break;
    }
    else if(sem_wait(&b)==0)
    {
      cout<<d<<" V"<<endl;
      //while(true){}
      //sem_post(&b);
      break;
      break;
    }
  }
  //sem_post(&c);
}
/*
void procsRev(sem_t *a,sem_t *b, sem_t *c, string d)
{
  sem_wait(a);
  while(true)
  {
    if(sem_trywait(b)==0)
    {
      cout<<d<<" V"<<endl;
      sem_post(b);
      break;
    }
    if(sem_trywait(a)==0)
    {
      cout<<d<<" U"<<endl;
      sem_post(&a);
      break;
    }
  }
  sem_post(&c);
}
*/
int main() {

  /*char *x = mmap(NULL, sizeof(char)*10, PROT_READ | PROT_WRITE,
               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  strcpy(x, "0");
*/
  // PID for the fork
  pid_t childPID1, childPID2,childPID3,childPID4;

  sem_init(&U,0,1);
  sem_init(&V,0,1);
  sem_init(&W,0,2);




  //sem.V(semU);
  //sem.V(semV);

  childPID1 = fork();
  string end;
  if(childPID1)  //parent
  {

    cout<<"enter !wq to exit"<<endl;
    getline(cin,end);
    if(end=="!wq") //termination command
    {
      cout<<"exiting..."<<endl;
      kill(childPID1,SIGTERM);
      exit(1);  //process terminates, close out program
    }
  }
  else
  {
    childPID2=fork();
    if(childPID2) //first child
    {
      while(true){/*procs(U,V,W,"a");*/}
    }
    else
    {
      childPID3=fork();
      if(childPID3) //second child
      {
        while(true){/*procs(U,V,W,"b");*/}
      }
      else
      {
        childPID4=fork();
        if(childPID4) //third child
        {
          while(true){/*procs(U,V,W,"c");*/}
        }
        else  //fourth child
        {
          while(true){/*procs(U,V,W,"d");*/}
        }
      }
    }
  }
  while(true){}
  return 0;
}
