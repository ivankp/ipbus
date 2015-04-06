//! Dear emacs this is -*-c++-*-

/****************************
 * Resend
 * IPbus 2.0 status packet
 *
 * Carlos.Solans@cern.ch
 ****************************/

#ifndef Resend_h
#define Resend_h 1

#include "ipbus/Packet.h"
#include <stdint.h>

namespace ipbus{

  class Resend: public Packet{
    
  public:
    
    //Constructor
    Resend();
    
    //Destructor
    ~Resend();

    //This package was sent
    void Sent();

    //Add values to byte stream
    uint32_t AddBytes(uint8_t* /*b*/, uint32_t pos);
    
    //Parse byte stream
    uint32_t SetBytes(uint8_t* /*b*/, uint32_t pos);
    
  };
}

#endif
