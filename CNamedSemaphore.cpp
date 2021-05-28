// File     CNamedSemaphore.cpp
// Version  1.0
// Author   Jens-Peter Akelbein
// Comment  Softwareentwicklung fuer Embedded Systeme - Exercise 3

#include "CNamedSemaphore.h"

using namespace std;

// construct a new named semaphore
// @name - name of the semaphore being created in /run/shm
// @value - initial value of the semaphore
CNamedSemaphore::CNamedSemaphore(const char *name, int value) {
  this->semaphore = sem_open(name, O_CREAT, O_RDWR, value);
  this->remember_my_name = name;
}

// deconstruct the semaphore
// question:
// How do we delete the semaphore by the last process only?
CNamedSemaphore::~CNamedSemaphore() {
  if(this->allow_deconstruct){
    int i = sem_close(this->semaphore);
  }
}


void CNamedSemaphore::increment(void) {
  int i = sem_post(this->semaphore);
}


void CNamedSemaphore::decrement(void) {
  int i = sem_wait(this->semaphore);
}


int CNamedSemaphore::value(void) {
  int value;
  return sem_getvalue(this->semaphore, &value);
}


// helper function to display errors and terminate our
// process as a very simple error handling mechanism
void CNamedSemaphore::exitproc(const char *text, int err) {

    cout << text;
    switch (err) {
        case EACCES:
            cerr << "EACCES";
            break;
        case EEXIST:
            cerr << "EEXIST";
            break;
        case EINVAL:
            cerr << "EINVAL";
            break;
        case EMFILE:
            cerr << "EMFILE";
            break;
        case ENAMETOOLONG:
            cerr << "TOOLNG";
            break;
        case ENOENT:
            cerr << "ENOENT";
            break;
        case ENOMEM:
            cerr << "ENOMEM";
            break;
        case EOVERFLOW:
            cerr << "EOVERFLOW";
            break;
        default:
            cerr << "UNKNWN";
    }
    cout << endl;
    exit(1);
}
