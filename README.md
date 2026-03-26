# 42-Codexion
Codexion project from 42KL core.

All threads within a process share the same address space.
Threads in the same process share:

    Process instructions
    Most data
    open files (descriptors)
    signals and signal handlers
    current working directory
    User and group id

Each thread has a unique:

    Thread ID
    set of registers, stack pointer
    stack for local variables, return addresses
    signal mask
    priority
    Return value: errno

pthread functions return "0" if OK

##Resourses

POSIX Thread lib:

https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
