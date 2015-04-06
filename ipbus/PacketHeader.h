//! Dear emacs this is -*-c++-*-
/**************************
 * PacketHeader
 *
 * IPbus version 2.0 Packet header
 *
 * Carlos.Solans@cern.ch
 **************************/

#ifndef PacketHeader_h
#define PacketHeader_h 1

#include <stdint.h>

namespace ipbus{

  class PacketHeader{
    
  private:
    uint32_t pver; //Protocol version 4 bits [28-31]
    uint32_t pkid; //Packet ID 16 bits [8-23]
    uint32_t byoq; //Byte order qualifier [4-7]
    uint32_t type; //Type ID 4 bits [0-3]
    
    uint32_t head; //encoded header

  public:

    //Public types
    static const uint32_t CONTROL=0;
    static const uint32_t STATUS=1;
    static const uint32_t RESEND=2;
    
    //Byte order qualifier
    static const uint32_t BYOQ=0xF;

    //Version
    static const uint32_t VERSION=2;

    //Constructor
    PacketHeader();
    
    //Destructor
    ~PacketHeader();
    
    //Dump on the screen
    void Dump();

    //Getters
    uint32_t GetPver();
    uint32_t GetPkid();
    uint32_t GetType();
    uint32_t GetByoq();
    uint32_t GetInt();

    //Setters
    void SetInt(uint32_t v);
    void SetPkid(uint32_t v);
    void SetType(uint32_t v);
    void SetByoq(uint32_t v);
    void SetPver(uint32_t v);

    //Set bytes
    void SetBytes(uint8_t * b, uint32_t pos);

  private:
    //Convert beans into head
    void Pack();

    //Convert head into beans
    void Unpack();

    
  };
}

#endif
