/*
  this file is GPLv3
*/

#ifndef __BITMAP_HXX_
#define __BITMAP_HXX_

namespace bmp {  
  void Extract( const char* filename );
  void Save( const char *filename, unsigned char *buffer, int width, int height );
} //namespace bmp
#endif /* !__BITMAP_HXX_ */
