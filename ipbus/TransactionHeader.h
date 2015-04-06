//! Dear emacs this is -*-c++-*-

/**************************
 * TransactionHeader
 * IPbus version 2.0 
 *
 * R/W transaction header
 *
 * Carlos.Solans@cern.ch
 **************************/

#ifndef TransactionHeader_h
#define TransactionHeader_h 1

#include <stdint.h>

namespace ipbus{

  class TransactionHeader{

  public:
    
    //Type values
    static const uint32_t READ=0;
    static const uint32_t WRITE=1;
    static const uint32_t READFIFO=2;
    static const uint32_t WRITEFIFO=3;
    
    //Info values
    static const uint32_t RESPONSE=0x0;
    static const uint32_t REQUEST=0xF;

  private:
    
    uint32_t pver; //Protocol version 4 bits [28-31]
    uint32_t trid; //Transaction ID 12 bits [16-27]
    uint32_t size; //Number of words 8 bits [8-15]
    uint32_t type; //Type ID 4 bits [4-7]
    uint32_t info; //Info code 4 bits [0-3]
    
    uint32_t head;

  public:

    TransactionHeader();
    ~TransactionHeader();

    //Getters
    uint32_t GetSize();
    uint32_t GetType();
    uint32_t GetInfo();
    uint32_t GetTrid();
    uint32_t GetPver();
    uint32_t GetInt();

    //Get bytes
    uint8_t* getBytes();

    //Setters
    void SetInt(uint32_t v);
    void SetSize(uint32_t v);
    void SetType(uint32_t v);
    void SetInfo(uint32_t v);
    void SetTrid(uint32_t v);
    void SetPver(uint32_t v);
    
    //Set bytes
    void SetBytes(uint8_t* b, uint32_t pos);
    
  private:
    void Pack();
    void Unpack();

  };
}

#endif
