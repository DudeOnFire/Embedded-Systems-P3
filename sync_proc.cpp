// File     sync_proc.h
// Version  1.0
// Author   Jens-Peter Akelbein
// Comment  Softwareentwicklung fuer Embedded Systeme - Exercise 3
// Always execute the it with sudo because of the nature of sem_open!

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "CNamedSemaphore.h"
#include "SensorConfiguration.h"
#include "SensorCommunication.h"
#include "SensorTag.h"

using namespace std;

// valid states for our two processes, we use the impicit ordering of values
// by an enum starting with the value 1
enum EProc_State {
    STATE_ACTIVE_CHILD = 1,
    STATE_ACTIVE_PARENT,  //2
    STATE_TERMINATE       //3
};

#define NUMBER_OF_LOOPS     10

const char sem_name1[] = "/semaphore";
const char sem_name2[] = "/state";
CNamedSemaphore semaphore(sem_name1, 1);
CNamedSemaphore state(sem_name2, STATE_ACTIVE_CHILD);

//sensor
SensorTag sensorTag;

// function being executed as parent or as child process to perform ping pong
// between both processes
void pingpong(bool parent) {

  int count = NUMBER_OF_LOOPS;

  while(count != 0) {

    if(state.value() == 1 && !parent){

      //enter critical
      semaphore.decrement();
      std::cout << "Child: " << std::endl;
      sensorTag.writeMovementConfig();
      sensorTag.printMotion();
      semaphore.increment();
      //leave critical

      //to state 2
      state.increment();
      count--;

    }
    else if(state.value() == 1 && parent){

    }
    else if(state.value() == 2 && parent){

      //enter critical
      semaphore.decrement();
      std::cout << "Parent: " << std::endl;
      sensorTag.writeMovementConfig();
      sensorTag.printMotion();
      semaphore.increment();
      //leave critical

      //to state 1
      state.decrement();
      count--;

    }
    else if(state.value() == 2 && !parent){

    }
  }

  //ende der schleife von child und parent auf state1, danach auf 3


  //terminate
  if(state.value() == 1 && !parent){
    //to state 3 from 1
    state.increment();
    state.increment();
  }

  return;
}

// main function, here we are just forking into two processes both calling
// pingpong() and indicating with a boolean on who is who
int main() {

//========== init sensortag ==========
sensorTag.initRead();


//1. fork
pid_t child_pid = fork();

if (child_pid < 0) {
  perror("fork() failed");
  exit(EXIT_FAILURE);
} else if (child_pid == 0) {

  printf("from child: pid=%d, parent_pid=%d\n",(int)getpid(), (int)getppid());

  //==========

  pingpong(false);
  std::cout << "ping pong child exit" << std::endl;
  //==========

  //exit child
  exit(42);
} else if (child_pid > 0) {
  // Print message from parent process.
  printf("from parent: pid=%d child_pid=%d\n",(int)getpid(), (int)child_pid);

  //==========

  pingpong(true);
  std::cout << "ping pong parent exit" << std::endl;

  if(state.value() == 3) {
    semaphore.allow_deconstruct = true;
  }

  //==========

  // Wait until child process exits or terminates.
  int status;
  pid_t waited_pid = waitpid(child_pid, &status, 0);

  if (waited_pid < 0) {
    perror("waitpid() failed");
    exit(EXIT_FAILURE);
  } else if (waited_pid == child_pid) {
    if (WIFEXITED(status)) {
    /* WIFEXITED(status) returns true if the child has terminated
     * normally. In this case WEXITSTATUS(status) returns child's
     * exit code.
     */
      printf("from parent: child exited with code %d\n",WEXITSTATUS(status));
    }
  }
 }



 return 0;
}
