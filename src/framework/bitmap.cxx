/*
  this file if GPLv3
*/

#include <cstdio> //for FILE, std::fopen
#include <string> //std::printf
#include "bitmap.hxx"
#include "../sys/error.hxx"

namespace bmp {
  enum type_e {
    PALETTIZED = 1,
    ALPHA,
    SOMETHING_ELSE,
    TRUE_COLOUR
  };

  void Extract( const char* filename ) { 
    std::printf( "starting extraction.\n\n" );
  
    FILE *f = std::fopen( filename, "rb" ); //read it as a binary file
    if( f == nullptr ) {
      sys::Terminate( "Couldn't open file '%s', terminating.\n", filename );
    }

    
    int number;
    std::fread( &number, 4, 1, f );
    std::printf( "File '%s' has %d textures\n", filename, number );
    
    for( int i = 0; i < number; ++i ) {
      int two, checksum, size, nameLength, type, width, height, dataLength;
      std::fread( &two, 2, 1, f );
      std::fread( &checksum, 4, 1, f );
      std::fread( &size, 4, 1, f );
      std::fread( &nameLength, 4, 1, f );
      //char* name = (char*)std::malloc( nameLength+5 );
      char* name = (char*)std::malloc( nameLength );
      std::fread( name, 1, nameLength, f );
      /*
      name[nameLength] = '.';
      name[nameLength+1] = 'b';
      name[nameLength+2] = 'm';
      name[nameLength+3] = 'p';
      name[nameLength+4] = '\0';
      */
      std::string name_s = std::string( name );
      //std::printf( "name_s is: '%s'.\n", name_s.c_str() );
      name_s.append( ".bmp\0" );     
      
      std::fread( &type, 4, 1, f );
      std::fread( &width, 4, 1, f );
      std::fread( &height, 4, 1, f );
      dataLength = size - 12;
      unsigned char *data = (unsigned char *)std::malloc( dataLength );
      std::fread( data, 1, dataLength, f );
      
      std::printf( "Saving... %s,  -> %dx%d with: %dbytes ", name_s.c_str(), width, height, size );

      if( type == type_e::PALETTIZED ) {
        std::printf("as palettized.\n" );
        unsigned char *palette = data + width*height;//data_len - 768+3;
	      unsigned char *truecolour = (unsigned char *)std::malloc( width*height*3 );
	      for( int j = 0; j < height; ++j ) {
	        for( int k = j*width; k < ( j+1 )*width; ++k ) {
	          truecolour[3*k] = palette[data[k]*3+2] << 2;
	          truecolour[3*k+1] = palette[data[k]*3+1] << 2;
	          truecolour[3*k+2] = palette[data[k]*3] << 2;
          }
        }
	      Save( name_s.c_str(), truecolour, width, height );
	      std::free(truecolour);
      } else if( type == type_e::ALPHA ) {
        std::printf("as alpha.\n" );
        unsigned char *truecolour = (unsigned char *)std::malloc( width*height*3 );
	      for( int j = 0; j < height; ++j ) {
	        for( int k = j*width; k < ( j+1 )*width; ++k ) {
	          truecolour[3*k]   = data[k];
	          truecolour[3*k+1] = data[k];
	          truecolour[3*k+2] = data[k];
	        }
        }
	      Save( name_s.c_str(), truecolour, width, height );
	      std::free(truecolour);
      } else if( type == type_e::TRUE_COLOUR ) {
        std::printf("as true colour.\n" );
        int Colour = width*height;
        unsigned char *truecolour = (unsigned char *)std::malloc( Colour*3 );         
        for( int j = 0; j < height; ++j ) {
	        //for( int k = j*width; k < ( j+1 )*width; ++k ) {
	        for( int k = 0; k < width; ++k ) {
	          truecolour[curr_h + curr_w]             = data[curr_h + curr_w];
	          truecolour[Colour + curr_h + curr_w]    = data[Colour + curr_h + curr_w];
	          truecolour[2*Colour + curr_h + curr_w]  = data[2*Colour + curr_h + curr_w]; 
	          /*
	          truecolour[3*k]   = data[2*k];
	          truecolour[3*k+1] = data[1*k];
	          truecolour[3*k+2] = data[0*k];
	          */
	        }
        }
        Save( name_s.c_str(), truecolour, width, height );
      } else {
        std::free( data ); //so we don't leak memory
        std::free( name );
        sys::Terminate( "Sorry, I don't know the type %d of texture %s\n", type, name_s.c_str() );
      }
      
      std::free( data );
      std::free( name );
    } //for loop
    
    std::printf( "extraction done.\n" );
  } //extract  function
  
  const int BYTES_PER_PIXEL = 3; /// red, green, & blue
  const int FILE_HEADER_SIZE = 14;
  const int INFO_HEADER_SIZE = 40;
  
  void Save( const char *filename, unsigned char *buffer, int width, int height ) {
    int widthInBytes = width * BYTES_PER_PIXEL;
    
    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = ( 4 - ( widthInBytes ) % 4 ) % 4;

    int stride = widthInBytes + paddingSize;
    
    FILE* f = std::fopen( filename, "wb" ); //write binary
    
    //handle the header of the bmp
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + ( stride * height );

    static unsigned char fileHeader[] = {
      0,0,     /// signature
      0,0,0,0, /// image file size in bytes
      0,0,0,0, /// reserved
      0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)( 'B' );
    fileHeader[ 1] = (unsigned char)( 'M' );
    fileHeader[ 2] = (unsigned char)( fileSize );
    fileHeader[ 3] = (unsigned char)( fileSize >>  8 );
    fileHeader[ 4] = (unsigned char)( fileSize >> 16 );
    fileHeader[ 5] = (unsigned char)( fileSize >> 24 );
    fileHeader[10] = (unsigned char)( FILE_HEADER_SIZE + INFO_HEADER_SIZE );

    std::fwrite( fileHeader, 1, FILE_HEADER_SIZE, f );    //done with the header
    
    //handle the info header
    static unsigned char infoHeader[] = {
      0,0,0,0, /// header size
      0,0,0,0, /// image width
      0,0,0,0, /// image height
      0,0,     /// number of color planes
      0,0,     /// bits per pixel
      0,0,0,0, /// compression
      0,0,0,0, /// image size
      0,0,0,0, /// horizontal resolution
      0,0,0,0, /// vertical resolution
      0,0,0,0, /// colors in color table
      0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)( INFO_HEADER_SIZE );
    infoHeader[ 4] = (unsigned char)( width );
    infoHeader[ 5] = (unsigned char)( width >> 8 );
    infoHeader[ 6] = (unsigned char)( width >> 16 );
    infoHeader[ 7] = (unsigned char)( width >> 24 );
    infoHeader[ 8] = (unsigned char)( height );
    infoHeader[ 9] = (unsigned char)( height >>  8 );
    infoHeader[10] = (unsigned char)( height >> 16 );
    infoHeader[11] = (unsigned char)( height >> 24) ;
    infoHeader[12] = (unsigned char)( 1 );
    infoHeader[14] = (unsigned char)( BYTES_PER_PIXEL*8 );
    
    std::fwrite( infoHeader, 1, INFO_HEADER_SIZE, f); //done with info header
    
    //write the image data
    for( int i = 0; i < height; i++) {
        std::fwrite( buffer + ( i*widthInBytes ), BYTES_PER_PIXEL, width, f );
        std::fwrite( padding, 1, paddingSize, f );
    }

    std::fclose( f );
  } //save function

} //namespace bmp

