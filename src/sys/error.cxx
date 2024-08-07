/*
  this file if GPLv3
*/

#include <cstdio>   //for printf
#include <cstdlib>  //for exit, EXIT_FAILURE
#include <cstdarg>  //std::va_list, va_start, va_end
#include "error.hxx"

namespace sys {

  void Terminate( const char* str, ... ) {
    char* result;
    std::va_list arg;
    va_start( arg, str );
    std::vsprintf( result, str, arg );
    va_end( arg );  
    std::printf( "%sERROR: %s%s.\n", "\033[31m", result, "\033[0m" );
    std::exit( EXIT_FAILURE );
  }

} //namespace sys

