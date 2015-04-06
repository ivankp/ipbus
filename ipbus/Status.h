//! Dear emacs this is -*-c++-*-

/****************************
 * Status
 * IPbus 2.0 status packet
 *
 *
 * Carlos.Solans@cern.ch
 ****************************/

#ifndef Status_h
#define Status_h 1

#include "ipbus/Packet.h"
#include <stdint.h>
#include <vector>

namespace ipbus{

  class Status: public Packet{
    
  private:

    std::vector<uint32_t> values;

  public:

    static const uint32_t SIZE = 15;
    
    //Constructor
    Status();

    //Destructor
    ~Status();

    //Get the maximum number of packets in memory
    uint32_t GetBufferSize();

    //Get the next expected packet ID 
    uint32_t GetNextExpectedPacketId();

    //Set the maximum number of packets in memory
    void SetBufferSize(uint32_t v);

    //Set the next expected packet ID
    void SetNextExpectedPacketId(uint32_t v);

    //Inform this package was sent
    void Sent();

    //Add values to byte stream
    uint32_t AddBytes(uint8_t* b, uint32_t pos);

    //Parse byte stream
    uint32_t SetBytes(uint8_t* b, uint32_t pos);

  };
}
#endif
