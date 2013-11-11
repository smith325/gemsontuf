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
VALUE TUF = Qnil;

void Init_GemsOntuf();

//declare methods
VALUE method_TUFConfigure( VALUE self, VALUE par0, VALUE par1, VALUE par2 );
VALUE method_TUFurlOpen( VALUE self, VALUE rbUrl );
VALUE method_TUFDeconfigure( VALUE self );

//init methods
void Init_GemsOnTuf() {
    GemsOnTuf = rb_define_module("GemsOnTuf");
    TUF = rb_define_class_under( GemsOnTuf, "TUF", rb_cObject );
	rb_define_method( TUF, "configure", method_TUFConfigure, 3 );
	rb_define_method( TUF, "deconfigure", method_TUFDeconfigure, 0 );
	rb_define_method( TUF, "urlOpen", method_TUFurlOpen, 1 );
}


//bool Py_TUFConfigure(char* tuf_intrp_json, char* p_repo_dir, char* p_ssl_cert_dir)
VALUE method_TUFConfigure( VALUE self, VALUE par0, VALUE par1, VALUE par2 ) {
	char* argOne = StringValuePtr( par0 );
	char* argTwo = StringValuePtr( par1 );
	char* argThr = StringValuePtr( par2 );
	
	bool worked = Py_TUFConfigure( argOne, argTwo, argThr );
	
	if ( worked )
		return Qtrue;
	else
		return Qfalse; 
}


/*bool Py_TUF_urllib_urlopen(char* url);
bool Py_TUF_urllib_urlopen(char* url);
bool Py_TUF_urllib_urlretreive(char* url, char* fname);
bool Py_TUF_urllib2_urlopen(char* url) */

//bool Py_TUF_urllib_urlopn( char* url ) 
//
VALUE method_TUFurlOpen( VALUE self, VALUE rbUrl ) {
	char* url = StringValuePtr( rbUrl );
	
	bool worked = Py_TUF_urllib2_urlopen( url );
	
	
	/*
	 * 	This function is returning an HTML file associated with the website
	 *  and stores it in temp
	 *  I don't know what to do with it yet
	 */
	if ( worked )
		worked = Py_TUF_urllib_urlretrieve( url )
	else 
		return Qfalse;
		
	if ( worked )
		worked = Py_TUF_urllib2_urlopen( url );
	else 
		return Qfalse;
		
	if ( worked ) 
		return Qtrue;
	else 
		return Qfalse;
}

//void Py_TUFDeconfigure();
VALUE method_TUFDeconfigure( VALUE self ) {
	Py_TUFDeconfigure();
	return Qtrue;
}


