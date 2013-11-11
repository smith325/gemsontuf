/*
 * Usage: 
 * require 'GemsOnTuf'
 * tuf = GemsOnTuf::TUF.new( args )
 * tuf.method()
 * Methods:
 * deconfigure
 * urlOpen( 'urlString' )
 * etc
 */

#include "ruby.h"
#include "tuf_interface.h"

//this holds the objects 
VALUE GemsOnTuf = Qnil;
VALUE TUF = Qnil;

void Init_GemsOntuf();

//declare methods
VALUE method_TUFConfigure( VALUE self, VALUE par0, VALUE par1, VALUE par2 );
VALUE method_TUFurlOpen( VALUE self, VALUE rbUrl );
VALUE method_TUFDeconfigure( VALUE self );

//init methods ~ require 'GemsOnTuf'
void Init_GemsOnTuf() {
	//I'm thinking of initializing the interpreter here so it's the same for all function calls 
	//Py_Initialize();
	
    GemsOnTuf = rb_define_module("GemsOnTuf");
    TUF = rb_define_class_under( GemsOnTuf, "TUF", rb_cObject );
	rb_define_method( TUF, "initialize", method_TUFConfigure, 3 );
	rb_define_method( TUF, "deconfigure", method_TUFDeconfigure, 0 );
	rb_define_method( TUF, "urlOpen", method_TUFurlOpen, 1 );
}


//bool Py_TUFConfigure(char* tuf_intrp_json, char* p_repo_dir, char* p_ssl_cert_dir)
//returns a bool, I think the exception from Python closes the program anyway though
//so this might not be necessary. 
//Thinking about changing this to return self and use this method as 
//tuf = GemsOnTuf::TUF.configure
//so we can skip the new step but I don't know if it works that way 
VALUE method_TUFConfigure( VALUE self, VALUE par0, VALUE par1, VALUE par2 ) {
	char* argOne = StringValuePtr( par0 );
	char* argTwo = StringValuePtr( par1 );
	char* argThr = StringValuePtr( par2 );
	
	bool worked = Py_TUF_configure( argOne, argTwo, argThr );
	if ( worked )
		return self; 
	return Qnil;
}


/*bool Py_TUF_urllib_urlopen(char* url);
bool Py_TUF_urllib_urlretreive(char* url, char* fname);
bool Py_TUF_urllib2_urlopen(char* url) */

//char* Py_TUF_urllib_urlopen( char* url ) 
VALUE method_TUFurlOpen( VALUE self, VALUE rbUrl ) {
	char* url = StringValuePtr( rbUrl );
	
	char* readUrl = Py_TUF_urllib_urlopen( url );
	
	if ( readUrl == NULL )
		return rb_str_new2( "err" );
	return rb_str_new2( readUrl );
}

//void Py_TUFDeconfigure();
VALUE method_TUFDeconfigure( VALUE self ) {
	Py_TUFDeconfigure();
	return Qtrue;
}


