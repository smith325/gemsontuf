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
* from other languages. This module should sit between Python, and
* whichever destination language one desires.
*
* Compiling:
* gcc tuf_interface.c -o tuf_interface -lpython2.7
*
* You must have python-dev installed for the linker to find the python2.7 libraries.
* Also note, that you *must* have the Python.h include before other standard header
* includes. This is per Python 3's documentation regarding how to do the python extensions
* with c.
*
* Additionally you should have TUF installed via python. This is required because
* we import the tuf module directly in the code here.
*
* Best tutorial I've found thus far: http://www.linuxjournal.com/article/8497?page=0,2
*/

#include "python2.7/Python.h"
#include <stdbool.h>

//#include "tuf_interface.h"


PyObject *ptr;
PyObject *configDict;
PyObject *py_url = NULL;



/*
* Method to call TUFs configure method. This function takes the JSON interposition filename
* as well as the parent repository directory and the parent ssl certificate directory, and
* configures TUF to interpose on update calls
*/
//PyObject* Py_TUF_configure(char* tuf_intrp_json, char* p_repo_dir, char* p_ssl_cert_dir){
bool Py_TUF_configure(char* tuf_intrp_json, char* p_repo_dir, char* p_ssl_cert_dir) {
    // Init the python env
    Py_Initialize();

	//add the current directory to the places to search for TUF
	PyObject *path = PySys_GetObject( (char *)"path" );
	PyObject *currentDirectory = PyString_FromString( "." );
	PyList_Append( path, currentDirectory );
	Py_XDECREF( currentDirectory );

	//import TUF module
	PyObject *moduleName = PyString_FromString( "tuf.interposition" );
	PyObject *tufInterMod = PyImport_Import( moduleName );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( moduleName );
	
	//get the configure function from tuf.interposition
	PyObject *configFunction = PyObject_GetAttrString( tufInterMod, "configure" );
	if ( configFunction == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( tufInterMod );
	
	//convert arguements into Python types and create tuple for CallObject function
	PyObject *args = PyTuple_New( 3 );
    PyObject *arg0 = PyString_FromString( tuf_intrp_json );
    PyTuple_SetItem(args, 0, arg0);
    PyObject *arg1 = PyString_FromString( p_repo_dir );
    PyTuple_SetItem(args, 1, arg1);
    PyObject *arg2 = PyString_FromString( p_ssl_cert_dir );
    PyTuple_SetItem(args, 2, arg2);

	//calls the config function from the tuf.interposition module
	//returns a dictionary with the configurations	
	//we are currently storing this globally 	
	configDict = PyObject_CallObject( configFunction, args );

	Py_XDECREF( arg0 );
	Py_XDECREF( arg1 );
	Py_XDECREF( arg2 );
	Py_XDECREF( args );
	Py_XDECREF( configFunction );

	if ( configDict == NULL ) {
		PyErr_Print();
		return false;
	}


	printf( "TUF configured.\n" );
	return true;
}


/*
* This method calls the TUF urlopen function, which opens a URL through TUF.
*/
bool Py_TUF_urllib_urlopen(char* url) {
    // Init the python env
    Py_Initialize();

	//add the current directory to the places to search for TUF
	PyObject *path = PySys_GetObject( (char *)"path" );
	PyObject *currentDirectory = PyString_FromString( "." );
	PyList_Append( path, currentDirectory );
	Py_XDECREF( currentDirectory );

	/* Load the tuf.interposition module */
	PyObject *mod1 = PyString_FromString( "tuf.interposition" );
	PyObject *tufInterMod = PyImport_Import( mod1 );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		return false;
	}

	/* Load the urllib_tuf module */
	PyObject *mod2 = PyString_FromString( "urllib_tuf" );
	PyObject *urllibMod = PyImport_Import( mod2 );
	if ( urllibMod == NULL ) {
		PyErr_Print();
		return false;
	}
	
	/* Get the urlopen method from the urllib_tuf class */
	PyObject *urlopenFunction = PyObject_GetAttrString( urllibMod, "urlopen" );
	if ( urlopenFunction == NULL ) {
		PyErr_Print();
		return false;
	}

	/* Convert arguements into Python types and create tuple for CallObject function */
	PyObject *args = PyTuple_New( 1 );
    PyObject *arg0 = PyString_FromString( url );
    PyTuple_SetItem(args, 0, arg0);
	
	// Left for debugging purposes.
	//PyObject_Print(args, stdout, Py_PRINT_RAW);
	//printf("\n");

    py_url = PyObject_CallObject( urlopenFunction, args );

    if(py_url == NULL){
    	PyErr_Print();
    	return false;
    }

    // Cleaning up References
	Py_XDECREF( urlopenFunction );
	Py_XDECREF( arg0 );
	Py_XDECREF( args );
	Py_XDECREF( mod1 );
	Py_XDECREF( mod2 );

	return true;
}

/*
* This method calls the TUF urlopen function from tuf.interposition.urllib2_tuf
*/
bool Py_TUF_urllib2_urlopen(char* url) {
    // Init the python env
    Py_Initialize();

	//add the current directory to the places to search for TUF
	PyObject *path = PySys_GetObject( (char *)"path" );
	PyObject *currentDirectory = PyString_FromString( "." );
	PyList_Append( path, currentDirectory );
	Py_XDECREF( currentDirectory );

	/* Load the tuf.interposition module */
	PyObject *mod1 = PyString_FromString( "tuf.interposition" );
	PyObject *tufInterMod = PyImport_Import( mod1 );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		return false;
	}

	/* Load the urllib_tuf module */
	PyObject *mod2 = PyString_FromString( "urllib2_tuf" );
	PyObject *urllibMod = PyImport_Import( mod2 );
	if ( urllibMod == NULL ) {
		PyErr_Print();
		return false;
	}
	
	/* Get the urlopen method from the urllib_tuf class */
	PyObject *urlopenFunction = PyObject_GetAttrString( urllibMod, "urlopen" );
	if ( urlopenFunction == NULL ) {
		PyErr_Print();
		return false;
	}

	/* Convert arguements into Python types and create tuple for CallObject function */
	PyObject *args = PyTuple_New( 1 );
    PyObject *arg0 = PyString_FromString( url );
    PyTuple_SetItem(args, 0, arg0);

    py_url = PyObject_CallObject( urlopenFunction, args );

    if(py_url == NULL){
    	PyErr_Print();
    	return false;
    }

    // Cleaning up references
	Py_XDECREF( urlopenFunction );
	Py_XDECREF( arg0 );
	Py_XDECREF( args );
	Py_XDECREF( mod1 );
	Py_XDECREF( mod2 );

	return true;
}



/*
* This method calls the TUF urlretreive function, which retreives a URL through TUF.
*/
bool Py_TUF_urllib_urlretrieve(char* url, char* fname) {
     // Init the python env
    Py_Initialize();

	//add the current directory to the places to search for TUF
	PyObject *path = PySys_GetObject( (char *)"path" );
	PyObject *currentDirectory = PyString_FromString( "." );
	PyList_Append( path, currentDirectory );
	Py_XDECREF( currentDirectory );

	/* Load the tuf.interposition module */
	PyObject *mod1 = PyString_FromString( "tuf.interposition" );
	PyObject *tufInterMod = PyImport_Import( mod1 );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		return false;
	}

	/* Load the urllib_tuf module */
	PyObject *mod2 = PyString_FromString( "urllib_tuf" );
	PyObject *urllibMod = PyImport_Import( mod2 );
	if ( urllibMod == NULL ) {
		PyErr_Print();
		return false;
	}
	
	/* Get the urlopen method from the urllib_tuf class */
	PyObject *urlretrieveFunction = PyObject_GetAttrString( urllibMod, "urlretrieve" );
	if ( urlretrieveFunction == NULL ) {
		PyErr_Print();
		return false;
	}

	/* Convert arguements into Python types and create tuple for CallObject function */
	PyObject *args = PyTuple_New( 1 );
    PyObject *arg0 = PyString_FromString( url );
    PyTuple_SetItem(args, 0, arg0);

    py_url = PyObject_CallObject( urlretrieveFunction, args );

    if(py_url == NULL){
    	PyErr_Print();
    	return false;
    }

	Py_XDECREF( urlretrieveFunction );
	Py_XDECREF( arg0 );
	Py_XDECREF( args );
	Py_XDECREF( mod1 );
	Py_XDECREF( mod2 );

	return true;
}





int main(int argc, char* argv[]){

	// Each of these works independently of the others...
	// which is better than where we were earlier today >.>

	//bool hello = Py_TUF_configure("tuf.interposition.json", "./", "./");
	//bool hello = Py_TUF_urllib_urlopen("http://www.google.com");
    //bool hello = Py_TUF_urllib2_urlopen("http://www.google.com");
    //bool hello = Py_TUF_urllib_urlretrieve("http://www.google.com", "file.txt");
    
    return 0;
}


