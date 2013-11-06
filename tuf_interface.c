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

PyObject *ptr;




/*
 * Method to call TUFs configure method.  This function takes the JSON interposition filename
 * as well as the parent repository directory and the parent ssl certificate directory, and
 * configures TUF to interpose on update calls
 */
//PyObject* Py_TUFConfigure(char* tuf_intrp_json, char* p_repo_dir, char* p_ssl_cert_dir){
void Py_TUFConfigure(char* tuf_intrp_json, char* p_repo_dir, char* p_ssl_cert_dir){
    // Init the python env
    Py_Initialize();

    // Get a hold of the configure function
    printf("Loading the 'configure' method from the tuf.interposition library. . .\n");
    PyObject *tuf_intrp_mod = PyImport_AddModule("tuf.interposition");
    PyObject *tuf_dict      = PyModule_GetDict(tuf_intrp_mod);
    PyObject *conf_function = PyDict_GetItemString(tuf_dict, "configure");
    PyObject *conf_args     = PyTuple_New(3);
    
    // Note: Scoping issue here, we need to hand back a PyObject pointer
    // as we will use this pointer later for the deconfigure function call...
    // So how do we get the 'config' object out of this scope, but also
    // make this function externally call-able
    PyObject *config_obj;
    
    // Convert the handed strings into PyStrings to pass to the configure method
    //printf("Converting arguments to PyStrings. . .\n");
    //PyObject *tij  = PyString_FromString(tuf_intrp_json);
    //PyObject *prd  = PyString_FromString(p_repo_dir);
    //PyObject *pscd = PyString_FromString(p_ssl_cert_dir);

    // Set the Arguments Tuple
    //printf("Loading the arguments. . .\n");
    //PyTuple_SetItem(conf_args, 0, tij);
    //PyTuple_SetItem(conf_args, 1, prd);
    //PyTuple_SetItem(conf_args, 2, pscd);

    //config_obj = PyObject_CallObject(conf_function, conf_args);
    printf("Calling tuf.interposition.configure. . .\n");
    /* Use of the following causes seg faults.  I was using this because a tutorial
     * said to, but after looking through the API, I don't see why we can't just use
     * PyObject_CallFunction.  I'm changing over to this format, but we may need to
     * revisit this idea later, as we'll need flexibility in returned data types for
     * the interplay between configure/deconfigure.*/
    //ptr = PyObject_CallObject(conf_function, conf_args);
    ptr = PyObject_CallFunction(conf_function, "sss", tuf_intrp_json, p_repo_dir, p_ssl_cert_dir);

    //Clean up
    printf("Cleaning up. . .\n");
    Py_XDECREF(tuf_intrp_mod);
    Py_XDECREF(tuf_dict);
    Py_XDECREF(conf_function);
    Py_XDECREF(conf_args);
    //Py_DECREF(conf_obj); // Figure out how to cleanly decrement this ref.  Think we need globals :S
    //Py_XDECREF(tij);
    //Py_XDECREF(prd);
    //Py_XDECREF(pscd);
    printf("Done.\n");
    //return config_obj;
}


int main(int argc, char* argv[]){

    printf("Configuring TUF Interposition.\n");
    Py_TUFConfigure("tuf.interposition.json", "./", "./");
    printf("TUF Interposition has been configured.\n");
    
    return 0;
}





