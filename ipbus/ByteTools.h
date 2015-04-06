//! Dear emacs this is -*-c++-*-

/************************
 * ByteTools
 * Useful byte array packing tools
 * Carlos.Solans@cern.ch
 ************************/

#ifndef ByteTools_h
#define ByteTools_h 1

#include <stdint.h>

namespace ipbus{

  class ByteTools{

  private:
    ByteTools(){}
    ~ByteTools(){}

  public:
    
    static uint32_t AddBytes(uint8_t* dest, uint32_t pos, uint32_t src);

    static uint32_t Pack(uint8_t * b, uint32_t pos);
    
    static void DumpBytes(uint8_t * b, uint32_t pos, uint32_t length);
  
  };

}

#endif
