/*
 * Application Security Fall 2013
 * Team Red - Project 3
 *
 * Nick Anderson
 * Pan Chan
 * Anthony
 * Nektarios
 *
 * This C code acts as the interface to call TUF functionality
 * from other languages.  This module should sit between Python, and
 * whichever destination language one desires.
 *
 * Compiling:
 * gcc tuf_interface.c -o tuf_interface -lpython2.7
 * 
 * You must have python-dev installed for the linker to find the python2.7 libraries.
 * Also note, that you *must* have the Python.h include before other standard header
 * includes.  This is per Python 3's documentation regarding how to do the python extensions
 * with c.
 */

#include "python2.7/Python.h"

static PyObject *my_callback = NULL;

static PyObject *my_set_callback(PyObject *dummy, PyObject *args){

    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "0:set_callback", &temp)){
        if(!PyCallable_Check(temp)){
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);           /* Add a reference to new callback */
        Py_XDECREF(my_callback);    /* Dispose of previous callback */
        my_callback = temp;         /* remember the new callback */
        /* Boilerplate to retun "None" */
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}



int main(int argc, char* argv[]){
    printf("Hello!\n");
    return 0;
}





