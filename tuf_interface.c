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
PyObject *py_url;



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
	printf("Loading tuf.interposition. . .\n");
	PyObject *moduleName = PyString_FromString( "tuf.interposition" );
	PyObject *tufInterMod = PyImport_Import( moduleName );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( moduleName );
	
	int i = PyObject_HasAttrString(tufInterMod, "urllib_tuf");
	if(i){
		printf("\n");
	}



	/* Get the urllib_tuf class from the interposition module */
	printf("Loading tuf.interposition.urllib_tuf. . .\n");
	PyObject *urllibClass = PyObject_GetAttrString( tufInterMod, "urllib_tuf" );
	if ( urllibClass == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( tufInterMod );


	py_url = PyObject_CallMethod(urllibClass, "urlopen", "s", url);
	printf("UrlOpen was called.\n");

	if(py_url == NULL){
		PyErr_Print();
		return false;
	}

	/* Get the urlopen method from the urllib_tuf class */
	printf("Loading tuf.interposition.urllib_tuf.urlopen. . .\n");
	PyObject *urlopenFunction = PyObject_GetAttrString( urllibClass, "urlopen" );
	if ( urlopenFunction == NULL ) {
		PyErr_Print();
		return false;
	}
	//Py_XDECREF( urllibClass );
	
	/* Convert arguements into Python types and create tuple for CallObject function */
	PyObject *args = PyTuple_New( 1 );
    PyObject *arg  = PyString_FromString( url );
    if ( arg == NULL ) {
		PyErr_Print();
		return false;
	}
    PyTuple_SetItem(args, 0, arg);

    Py_XDECREF( arg );

	// Calls the tuf.interposition.urlopen() function with a specified url	
	printf("Calling the urlopen function. . .\n");
	if(urlopenFunction == NULL){
		printf("urlopen was null");
		PyErr_Print();
		return false;
	}
	if(args == NULL){
		printf("args was null");
		PyErr_Print();
		return false;
	}
	//py_url = PyObject_CallFunction( urlopenFunction, args ); // segfault here.
	py_url = PyObject_CallObject( urlopenFunction, args ); // segfault here.
	//PyObject_CallObject( urlopenFunction, args );

	/*
	if ( py_url == NULL ) {
		PyErr_Print();
		return false;
	}
	*/

	printf("Decrementing the function ptr. . .\n");
	Py_XDECREF( urlopenFunction );



	printf( "URL Opened." );
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

	//import TUF module
	PyObject *moduleName = PyString_FromString( "tuf.interposition" );
	PyObject *tufInterMod = PyImport_Import( moduleName );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( moduleName );
	
	//get the configure function from tuf.interposition
	PyObject *urlopenFunction = PyObject_GetAttrString( tufInterMod, "tuf.interposition.urllib2_tuf.urlopen" );
	if ( urlopenFunction == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( tufInterMod );
	
	//convert arguements into Python types and create tuple for CallObject function
	PyObject *args = PyTuple_New( 1 );
    PyObject *arg  = PyString_FromString( url );
    PyTuple_SetItem(args, 0, arg);

	// Calls the tuf.interposition.urlopen() function with a specified url	
	py_url = PyObject_CallObject( urlopenFunction, args );

	Py_XDECREF( arg );
	Py_XDECREF( urlopenFunction );

	if ( py_url == NULL ) {
		PyErr_Print();
		return false;
	}

	printf( "URL Opened." );
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

	//import TUF module
	PyObject *moduleName = PyString_FromString( "tuf.interposition" );
	PyObject *tufInterMod = PyImport_Import( moduleName );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( moduleName );
	
	//get the configure function from tuf.interposition
	PyObject *urlretrieveFunction = PyObject_GetAttrString( tufInterMod, "tuf.interposition.urllib_tuf.urlretrieve" );
	if ( urlretrieveFunction == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( tufInterMod );
	
	//convert arguements into Python types and create tuple for CallObject function
	PyObject *args = PyTuple_New( 2 );
    PyObject *arg0 = PyString_FromString( url );
    PyTuple_SetItem(args, 0, arg0);
    PyObject *arg1 = PyString_FromString( fname );
    PyTuple_SetItem(args, 1, arg1);

	// Calls the tuf.interposition.urlopen() function with a specified url	
	py_url = PyObject_CallObject( urlretrieveFunction, args );

	Py_XDECREF( arg0 );
	Py_XDECREF( arg1 );
	Py_XDECREF( urlretrieveFunction );

	if ( py_url == NULL ) {
		PyErr_Print();
		return false;
	}

	printf( "URL Retrieved." );
	return true;
}





int main(int argc, char* argv[]){


    bool hello = Py_TUF_configure("tuf.interposition.json", "./", "./");
    hello = Py_TUF_urllib_urlopen("http://www.google.com");
    //hello = Py_TUF_urllib2_urlopen("http://www.google.com");
    //hello = Py_TUF_urllib_urlretrieve("http://www.google.com", "file.txt");
    
    return 0;
}


