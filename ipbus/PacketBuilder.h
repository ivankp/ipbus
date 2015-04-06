//! Dear emacs this is -*-c++-*-

/*******************************
 * PacketBuilder
 * Build a packet from bytes
 *
 * Carlos.Solans@cern.ch
 ******************************/

#ifndef PacketBuilder_h
#define PacketBuilder_h 1

#include "ipbus/Packet.h"
#include "ipbus/PacketHeader.h"
#include <cstdlib>

namespace ipbus{

  class PacketBuilder{

  private:
    
    PacketHeader header;
    uint8_t* bytes;
    uint32_t length;
    Packet * transaction;
    Packet * resend;
    Packet * status;

  public:
    
    //Constructor
    PacketBuilder();
    
    //Destructor
    ~PacketBuilder();
			     
    //Get the buffer used to build a packet
    uint8_t* GetBytes();

    //Set the length of the buffer
    void SetLength(uint32_t v);

    //Print the bytes through std output
    void Dump();

    //Decode the bytes into a packet
    void Unpack();
    
    //Get the packet header
    const PacketHeader& GetHeader();
    
    //Check consistency of the packet
    bool CheckHeader();

    //Get the decoded packet
    const Packet * GetPacket();
    
  };
}

#endif
