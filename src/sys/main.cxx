/*
  this file if GPLv3
*/

#include <cstdio>   //for printf
#include <cstdlib>  //for exit, EXIT_FAILURE, EXIT_SUCCESS
#include <string>
#include "../framework/bitmap.hxx"

int main( int argc, char** argv ) {

  std::printf( "\n\t\t..::Blade Of Darkness texture dumper::..\n\n" );

  if( argc < 2 ) {
    std::printf( "Usage: MMPdumper file.mmp\n" );
    std::exit( EXIT_FAILURE );
  }
  
  std::string firstArgument( argv[1] );
  if( firstArgument.compare( "-h" ) == 0 ) {
    std::printf( "Usage: MMPdumper file.mmp\n" );
    std::exit( EXIT_FAILURE );
  }
  
  bmp::Extract( argv[1] );

  std::printf( "shutdown.\n" );
  std::exit( EXIT_SUCCESS );
}
