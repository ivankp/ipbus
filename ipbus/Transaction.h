//! Dear emacs this is -*-c++-*-

/*******************************
 * Transaction
 * IPbus 2.0 R/W/M/S transaction
 *
 * Request action on one or more values starting from base address
 * get response with one or multiple values
 *
 * Carlos.Solans@cern.ch
 ******************************/

#ifndef Transaction_h
#define Transaction_h 1

#include "ipbus/Packet.h"
#include "ipbus/TransactionHeader.h"
#include <stdint.h>
#include <vector>

namespace ipbus{

  class Transaction: public Packet{

  private:
    
    TransactionHeader * theader;
    uint32_t address;
    std::vector<uint32_t> values;

  public:
      
    Transaction();
  
    ~Transaction();
        
    uint32_t GetTransactionType();
    
    TransactionHeader* GetTransactionHeader();

    void SetSize(uint32_t size);

    uint32_t GetSize();
    
    void SetAddress(uint32_t addr);

    uint32_t GetAddress();
    
    const std::vector<uint32_t>& GetData() const;

    uint32_t GetData(uint32_t i);
        
    void SetRead(uint32_t _address, uint32_t size, bool fifo=false);
      
    void SetWrite(uint32_t _address, uint32_t value);
    
    void SetWrite(uint32_t _address, const uint32_t * _values, uint32_t size, bool fifo=false);

    void SetWrite(uint32_t _address, std::vector<uint32_t> _values, bool fifo=false);

    //Increment packet id
    void Sent();

    //Add values to byte stream
    uint32_t AddBytes(uint8_t* b, uint32_t pos);
	
    //Parse byte stream
    uint32_t SetBytes(uint8_t* b, uint32_t pos);
  };
}

#endif
