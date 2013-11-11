#include "python2.7/Python.h"
//#include "tuf_interface.c"
#include <stdbool.h>

bool Py_TUF_configure(char*, char*, char*);
bool Py_TUF_deconfigure(PyObject*);
//bool Py_TUF_urllib_urlopen(char*);
char* Py_TUF_urllib_urlopen(char*);
bool Py_TUF_urllib2_urlopen(char*);
bool Py_TUF_urllib_urlretreive(char*, char*);

 