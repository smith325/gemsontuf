#include "ruby.h"
#include "tuf_interface.h"

/*
void exec_interactive_interpreter()
{
  int argc = 0;
  char* test[] = { "-V", NULL };
  char** argv = test;
  Py_Initialize();
  Py_Main( argc, argv );
  Py_Finalize();
}
*/

//?
VALUE GemsOnTuf = Qnil;

void Init_GemsOntuf();

//declare methods
VALUE method_TUFConfigure( VALUE self, VALUE par0, VALUE par1, VALUE par2 );
VALUE method_TUFDeconfigure( VALUE self );

//init methods
void Init_GemsOnTuf() {
    GemsOnTuf = rb_define_module("GemsOnTuf");
	rb_define_method( GemsOnTuf, "TUFConfigure", method_TUFConfigure, 3 );
	rb_define_method( GemsOnTuf, "TUFDeconfigure", method_TUFDeconfigure, 0 );
}


// void Py_TUFConfigure(char* par0, char* par1, char* par2)
VALUE method_TUFConfigure( VALUE self, VALUE par0, VALUE par1, VALUE par2 ) {
	char* argOne = StringValuePtr( par0 );
	char* argTwo = StringValuePtr( par1 );
	char* argThr = StringValuePtr( par2 );
	
	Py_TUFConfigure( argOne, argTwo, argThr );

	return Qtrue;
}

//void Py_TUFDeconfigure();
VALUE method_TUFDeconfigure( VALUE self ) {
	Py_TUFDeconfigure();
	return Qtrue;
}


