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
 *
 * Additionally you should have TUF installed via python.  This is required because
 * we import the tuf module directly in the code here.
 *
 * Best tutorial I've found thus far: http://www.linuxjournal.com/article/8497?page=0,2
 */

#include "python2.7/Python.h"


int main(int argc, char* argv[]){

    Py_Initialize();

    //PyObject *main_module = PyImport_AddModule("__main__");
    printf("Adding the tuf.interposition module\n");
    PyObject *tuf_int_mod = PyImport_AddModule("tuf.interposition");
    printf("Adding the Tuf modules dictionary\n");
    PyObject *tuf_dict   = PyModule_GetDict(tuf_int_mod); // Get the TUF module's global dictionary
    printf("Adding the 'configure' function\n");
    PyObject *conf_func  = PyDict_GetItemString(tuf_dict, "configure"); // Get tuf.interposition.configure method
    printf("Adding the 'deconfigure' function\n");
    PyObject *dconf_func = PyDict_GetItemString(tuf_dict, "deconfigure"); // Get tuf.interposition.deconfigure method
    // TODO: Build the arguments for tuf.interposition
    //conf_args  = NULL; // Arguments to be passed to tuf.interposition.configure
    //dconf_args = NULL; 

    // Call the interposition function
    printf("Calling the configure function with NULL as args\n");
    PyObject_CallObject(conf_func, NULL);
    printf("Calling the deconfigure function with NULL as args\n");
    PyObject_CallObject(dconf_func, NULL);
    
    // Decrement the reference counter for each PyObj we've played with
    Py_DECREF(tuf_int_mod);
    Py_DECREF(tuf_dict);
    Py_DECREF(conf_func);
    Py_DECREF(dconf_func);
    printf("Hello!\n");

    return 0;
}





