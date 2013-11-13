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

//PyObject *ptr;
PyObject *configDict;
//PyObject *py_url = NULL;
//char* fname = "tmp_data_dump.raw";



/*
* Method to call TUFs configure method. This function takes the JSON interposition filename
* as well as the parent repository directory and the parent ssl certificate directory, and
* configures TUF to interpose on update calls
*/
bool Py_TUF_configure(char* tuf_intrp_json, char* p_repo_dir, char* p_ssl_cert_dir) {
//PyObject* Py_TUF_configure(char* tuf_intrp_json, char* p_repo_dir, char* p_ssl_cert_dir){
    // Init the python env
    Py_Initialize();
    PyObject *moduleName;
    PyObject *tufInterMod;
    PyObject *path;
    PyObject *currentDirectory;
    PyObject *configFunction;
    PyObject *args;
    PyObject *arg0;
    PyObject *arg1;
    PyObject *arg2;


	//add the current directory to the places to search for TUF
	path = PySys_GetObject( "path" );
	currentDirectory = PyString_FromString( "." );
	PyList_Append( path, currentDirectory );
	Py_XDECREF( currentDirectory );

	//import TUF module
	moduleName = PyString_FromString( "tuf.interposition" );
	tufInterMod = PyImport_Import( moduleName );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( moduleName );
	
	//get the configure function from tuf.interposition
	configFunction = PyObject_GetAttrString( tufInterMod, "configure" );
	if ( configFunction == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( tufInterMod );
	
	//convert arguements into Python types and create tuple for CallObject function
	args = PyTuple_New( 3 );
    arg0 = PyString_FromString( tuf_intrp_json );
    PyTuple_SetItem(args, 0, arg0);
    arg1 = PyString_FromString( p_repo_dir );
    PyTuple_SetItem(args, 1, arg1);
    arg2 = PyString_FromString( p_ssl_cert_dir );
    PyTuple_SetItem(args, 2, arg2);

	//calls the config function from the tuf.interposition module
	//returns a dictionary with the configurations	
	//we are currently storing this globally 	
	configDict = PyObject_CallObject( configFunction, args );

	//Py_XDECREF( arg0 );
	//Py_XDECREF( arg1 );
	//Py_XDECREF( arg2 );
	//Py_XDECREF( args );
	//Py_XDECREF( configFunction );

	if ( configDict == NULL ) {
		PyErr_Print();
		return false;
	}

	printf( "TUF configured.\n" );
	return true;
}



/*
* Method to call TUFs configure method. This function takes the JSON interposition filename
* as well as the parent repository directory and the parent ssl certificate directory, and
* configures TUF to interpose on update calls
*/
bool Py_TUF_deconfigure(PyObject* tuf_config_obj) {
    // Init the python env
    Py_Initialize();
	PyObject *path;
	PyObject *currentDirectory;
	PyObject *moduleName;
	PyObject *tufInterMod;
	PyObject *configFunction;

	//add the current directory to the places to search for TUF
	path = PySys_GetObject( (char *)"path" );
	currentDirectory = PyString_FromString( "." );
	PyList_Append( path, currentDirectory );
	Py_XDECREF( currentDirectory );

	//import TUF module
	moduleName = PyString_FromString( "tuf.interposition" );
	tufInterMod = PyImport_Import( moduleName );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( moduleName );
	
	//get the configure function from tuf.interposition
	configFunction = PyObject_GetAttrString( tufInterMod, "deconfigure" );
	if ( configFunction == NULL ) {
		PyErr_Print();
		return false;
	}
	Py_XDECREF( tufInterMod );

	//calls the config function from the tuf.interposition module
	//returns a dictionary with the configurations	
	//we are currently storing this globally 	
	configDict = PyObject_CallObject( configFunction, tuf_config_obj );

	//Py_XDECREF( arg0 );
	//Py_XDECREF( arg1 );
	//Py_XDECREF( arg2 );
	//Py_XDECREF( args );
	//Py_XDECREF( configFunction );

	if ( configDict == NULL ) {
		PyErr_Print();
		return false;
	}


	printf( "TUF deconfigured.\n" );
	return true;
	//return configDict;
}



/*
* This method calls the TUF urlopen function, which opens a URL through TUF.
*/
//bool Py_TUF_urllib_urlopen(char* url) {
//PyObject* Py_TUF_urllib_urlopen(char* url) {
char* Py_TUF_urllib_urlopen(char* url) {
    // Init the python env
    //this Init can be removed but it doesn't do anything if it's called twice
    Py_Initialize();
    char* fname = "./.tmp_data_dump.raw";
    PyObject *urllibMod;
	PyObject *urlopenFunction;
	PyObject *args;
	PyObject *arg0;
	PyObject* pySocket;
	PyObject *pySocketRead;
	PyObject* http_resp;

	/* Load the urllib_tuf module */
	urllibMod = PyImport_AddModule( "urllib_tuf" );
	if ( urllibMod == NULL ) {
		PyErr_Print();
		return NULL;
	}
	
	/* Get the urlopen method from the urllib_tuf class */
	urlopenFunction = PyObject_GetAttrString( urllibMod, "urlopen" );
	if ( urlopenFunction == NULL ) {
		PyErr_Print();
		return NULL;
	}

	/* Convert arguements into Python types and create tuple for CallObject function */
	args = PyTuple_New( 1 );
    arg0 = PyString_FromString( url );
    PyTuple_SetItem(args, 0, arg0);
	
	pySocket = PyObject_CallObject( urlopenFunction, args );
	if(pySocket == NULL){
		PyErr_Print();
		//return false;
		return NULL;
	}

	/* Calls the socket.read() function in Python */
	pySocketRead = PyObject_GetAttrString( pySocket, "read" );
	if ( pySocketRead == NULL ) {
		PyErr_Print();
		//return false;
		return NULL;
	} 

	 

	/* Build a temporary tuple that we can call read() with */
	args = PyTuple_New(0);
	http_resp = PyObject_CallObject(pySocketRead, args);
	if( http_resp == NULL ){
    	PyErr_Print();
    	return NULL;
    }

    /* Dump the data out to a file */
    FILE *fp;
    fp = fopen(fname, "w");
    PyObject_Print(http_resp, fp, Py_PRINT_RAW);
    fclose(fp);

    // Return the name of the file
	return fname;
}

/*
* This method calls the TUF urlopen function from tuf.interposition.urllib2_tuf
*/
char* Py_TUF_urllib2_urlopen(char* url) {
    // Init the python env
    //this Init can be removed but it doesn't do anything if it's called twice
    Py_Initialize();
    char* fname = "./.tmp_data_dump.raw";
    PyObject *urllibMod;
	PyObject *urlopenFunction;
	PyObject *args;
	PyObject *arg0;
	PyObject* pySocket;
	PyObject *pySocketRead;
	PyObject* http_resp;

	/* Load the urllib_tuf module */
	urllibMod = PyImport_AddModule( "urllib2_tuf" );
	if ( urllibMod == NULL ) {
		PyErr_Print();
		return NULL;
	}
	
	/* Get the urlopen method from the urllib_tuf class */
	urlopenFunction = PyObject_GetAttrString( urllibMod, "urlopen" );
	if ( urlopenFunction == NULL ) {
		PyErr_Print();
		return NULL;
	}

	/* Convert arguements into Python types and create tuple for CallObject function */
	args = PyTuple_New( 1 );
    arg0 = PyString_FromString( url );
    PyTuple_SetItem(args, 0, arg0);
	
	pySocket = PyObject_CallObject( urlopenFunction, args );
	if(pySocket == NULL){
		PyErr_Print();
		//return false;
		return NULL;
	}

	/* Calls the socket.read() function in Python */
	pySocketRead = PyObject_GetAttrString( pySocket, "read" );
	if ( pySocketRead == NULL ) {
		PyErr_Print();
		//return false;
		return NULL;
	} 

	 

	/* Build a temporary tuple that we can call read() with */
	args = PyTuple_New(0);
	http_resp = PyObject_CallObject(pySocketRead, args);
	if( http_resp == NULL ){
    	PyErr_Print();
    	return NULL;
    }

    /* Dump the data out to a file */
    FILE *fp;
    fp = fopen(fname, "w");
    PyObject_Print(http_resp, fp, Py_PRINT_RAW);
    fclose(fp);

    // Return the name of the file
	return fname;
}


/*
* This method calls the TUF urlretreive function, which retrieves a URL through TUF.
* The value returned is the name of the locally retrieved file.
*/

char* Py_TUF_urllib_urlretrieve(char* url, char* fname) {
	
    // Init the python env
    //this Init can be removed but it doesn't do anything if it's called twice
    Py_Initialize();
    PyObject *urllibMod;
	PyObject *urlopenFunction;
	PyObject *args;
	PyObject *arg0;
	PyObject* pySocket;
	PyObject *pySocketRead;
	PyObject* http_resp;

	/* Load the urllib_tuf module */
	urllibMod = PyImport_AddModule( "urllib_tuf" );
	if ( urllibMod == NULL ) {
		PyErr_Print();
		return NULL;
	}
	
	/* Get the urlopen method from the urllib_tuf class */
	urlopenFunction = PyObject_GetAttrString( urllibMod, "urlretrieve" );
	if ( urlopenFunction == NULL ) {
		PyErr_Print();
		return NULL;
	}

	/* Convert arguements into Python types and create tuple for CallObject function */
	args = PyTuple_New( 1 );
    arg0 = PyString_FromString( url );
    PyTuple_SetItem(args, 0, arg0);
	
	pySocket = PyObject_CallObject( urlopenFunction, args );
	if(pySocket == NULL){
		PyErr_Print();
		//return false;
		return NULL;
	}

	/* Calls the socket.read() function in Python */
	pySocketRead = PyObject_GetAttrString( pySocket, "read" );
	if ( pySocketRead == NULL ) {
		PyErr_Print();
		//return false;
		return NULL;
	} 

	 

	/* Build a temporary tuple that we can call read() with */
	args = PyTuple_New(0);
	http_resp = PyObject_CallObject(pySocketRead, args);
	if( http_resp == NULL ){
    	PyErr_Print();
    	return NULL;
    }

    /* Dump the data out to a file */
    FILE *fp;
    fp = fopen(fname, "w");
    PyObject_Print(http_resp, fp, Py_PRINT_RAW);
    fclose(fp);

    // Return the name of the file
	return fname;
}


int main(int argc, char* argv[]){

	// Each of these works independently of the others...
	// which is better than where we were earlier today >.>

	//PyObject* obj = Py_TUF_configure("tuf.interposition.json", "./", "./");
	//PyObject* obj = Py_TUF_urllib_urlopen("http://www.google.com");
	//PyObject* obj = Py_TUF_urllib2_urlopen("http://www.google.com");
	//PyObject* obj = Py_TUF_urllib_urlretrieve("http://www.google.com", "file.txt");
	/*
	
	Py_Initialize();
	PyObject *path = PySys_GetObject( (char *)"path" );
	PyObject *currentDirectory = PyString_FromString( "." );
	PyList_Append( path, currentDirectory );
	Py_XDECREF( currentDirectory );
	*/

	bool hello = Py_TUF_configure("tuf.interposition.json", "./", "./");
	char* s = Py_TUF_urllib_urlopen("https://rubygems.org/latest_specs.4.8.gz");

	if( s == NULL ){
		printf("HTTP Response was NULL!\n");
	}
	else{
		printf("%s\n", s);
	}
	
	//hello = Py_TUF_urllib_urlretrieve("http://www.google.com", "file.txt");
    //hello = Py_TUF_urllib2_urlopen("http://www.google.com");

    return 0;
}




/*
void test() { 
	
	Py_Initialize();
	PyObject *path = PySys_GetObject( (char *)"path" );
	PyObject *currentDirectory = PyString_FromString( "." );
	PyList_Append( path, currentDirectory );
	
	PyObject *mod1 = PyString_FromString( "t" );
	PyObject *tufInterMod = PyImport_Import( mod1 );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		//return false;
		return;
	}
	
	PyObject *test = PyObject_GetAttrString( tufInterMod, "f" );
	if ( test == NULL ) {
		PyErr_Print();
		return; 
	}
	
	PyObject *error = PyObject_CallFunction( test, NULL );
	if ( error == NULL ) {
		PyErr_Print();
		return;
	}
	
	test = PyObject_GetAttrString( tufInterMod, "k" );
	if ( test == NULL ) {
		PyErr_Print();
		return; 
	}
	
	error = PyObject_CallFunction( test, NULL );
	if ( error == NULL ) {
		PyErr_Print();
		return;
	}
	
	printf("\n");
}

void test2() {
	Py_Initialize();
	PyObject *path = PySys_GetObject( (char *)"path" );
	PyObject *currentDirectory = PyString_FromString( "." );
	PyList_Append( path, currentDirectory );
	
	PyObject *mod1 = PyString_FromString( "t" );
	PyObject *tufInterMod = PyImport_Import( mod1 );
	if ( tufInterMod == NULL ) {
		PyErr_Print();
		//return false;
		return;
	}
	
	PyObject *test = PyObject_GetAttrString( tufInterMod, "f" );
	if ( test == NULL ) {
		PyErr_Print();
		return; 
	}
	
	PyObject *error = PyObject_CallFunction( test, NULL );
	if ( error == NULL ) {
		PyErr_Print();
		return;
	}
	
	test = PyObject_GetAttrString( tufInterMod, "k" );
	if ( test == NULL ) {
		PyErr_Print();
		return; 
	}
}
*/

