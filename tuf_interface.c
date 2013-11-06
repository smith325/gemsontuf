/*
 * Nick Anderson
 * TUF -> C Interface
 *
 * Compiling:
 * gcc tuf_interface.c -o tuf_interface -lpython2.7
 * 
 * You must have python-dev installed for the linker to find the python2.7 libraries.
 */

#include "python2.7/Python.h"


int main(int argc, char* argv[]){
    printf("Hello!\n");
    return 0;
}





