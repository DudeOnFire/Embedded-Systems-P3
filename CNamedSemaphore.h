// File     CNamedSemaphore.h
// Version  1.0
// Author   Jens-Peter Akelbein
// Comment  Softwareentwicklung fuer Embedded Systeme - Exercise 3
#ifndef __CNAMEDSEMAPHORE_H
#define __CNAMEDSEMAPHORE_H

#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <semaphore.h>

// class encapsulating a named semaphore with just the functionality we need
class CNamedSemaphore {

public:

    // create and destroy the semaphore
    // @name - name of the semaphore being created in /run/shm
    // @value - initial value of the semaphore
    CNamedSemaphore(const char *name, unsigned int value);
    ~CNamedSemaphore();

    // increment value of the semaphore (release if value is zero)
    void increment(void);
    // decrement calue of the semaphore (wait blocked if value is zero)
    void decrement(void);

    // return the current value of the semaphore
    int value(void);

    bool allow_deconstruct = false;

private:
    sem_t       *semaphore;
    // what do I need to remember in the object being required lateron?
    // Add such information as additional private variables here


    const char *remember_my_name;
    pid_t       remember_my_creator;

    // print text with ERRNO and exit process
    void exitproc(const char *text, int);

    // enforce using constructor with parameters
    CNamedSemaphore();
};

#endif // __CNAMEDSEMAPHORE_H
