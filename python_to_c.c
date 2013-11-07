
/*
 * Example of calling python functions from within C
 *
 * Obtained from:
 * http://stackoverflow.com/questions/13109205/simple-example-how-to-call-python-function-in-c-c
 * 
 * Args: argv[1] contains the path to your .py file argv[2] contains the name of the function you want to call
 */

int wmain(int argc, wchar_t** argv)
{
    PyObject* py_imp_str;
    PyObject* py_imp_handle;
    PyObject* py_imp_dict; //borrowed
    PyObject* py_imp_load_source; //borrowed
    PyObject* py_dir; //stolen
    PyObject* py_lib_name; //stolen
    PyObject* py_args_tuple;
    PyObject* py_lib_mod;
    PyObject* py_func;
    PyObject* py_ret;

    Py_Initialize();

    //import our python script using the imp library (the normal import doesn't allow you to grab py files in random directories)
    py_dir = PyUnicode_FromWideChar(argv[1], wcslen(argv[1]));
    py_imp_str = PyString_FromString("imp");
    py_imp_handle = PyImport_Import(py_imp_str); //normal python import for imp
    py_imp_dict = PyModule_GetDict(py_imp_handle); //borrowed
    py_imp_load_source = PyDict_GetItemString(py_imp_dict, "load_source"); //borrowed
    py_lib_name = PyUnicode_FromWideChar(argv[2], wcslen(argv[2]));

    //setup args for imp.load_source
    py_args_tuple = PyTuple_New(2);
    PyTuple_SetItem(py_args_tuple, 0, py_lib_name); //stolen
    PyTuple_SetItem(py_args_tuple, 1, py_dir); //stolen

    //call imp.load_source
    py_lib_mod = PyObject_CallObject(py_imp_load_source, py_args_tuple);
    py_lib_mod_dict = PyModule_GetDict(py_lib_mod); //borrowed

    //get function object
    py_func = PyDict_GetItem(py_lib_mod_dict, py_lib_name);
    //call function object
    py_ret = PyObject_CallFunction(py_func, NULL);

    if (py_ret)
    {
        printf("PyObject_CallFunction from wmain was successful!\n");
        Py_DECREF(py_ret);
    }
    else
        printf("PyObject_CallFunction from wmain failed!\n");

    Py_DECREF(py_imp_str);
    Py_DECREF(py_imp_handle);
    Py_DECREF(py_args_tuple);
    Py_DECREF(py_lib_mod);

    Py_Finalize();

    fflush(stderr);
    fflush(stdout);
    return 0;
}
