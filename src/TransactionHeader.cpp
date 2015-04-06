/**************************
 * TransactionHeader
 * IPbus version 2.0 
 *
 * R/W transaction header
 *
 * Carlos.Solans@cern.ch
 **************************/

#include "ipbus/TransactionHeader.h"
#include "ipbus/ByteTools.h"
#include <iostream>

using namespace ipbus;
using namespace std;

TransactionHeader::TransactionHeader(){ 
  pver = 2;
  trid = 0;
  size = 0;
  type = 0;
  info = 0xF;
  Pack();
}

TransactionHeader::~TransactionHeader(){}

//Convert beans into head
void TransactionHeader::Pack(){
  head = 0;
  head |= (pver & 0xF)   << 28;
  head |= (trid & 0xFFF) << 16;
  head |= (size & 0xFF)  << 8;
  head |= (type & 0xF)   << 4;
  head |= (info & 0xF)   << 0;
}
    
//Convert head into beans
void TransactionHeader::Unpack(){
  pver = (head>>28) & 0xF;
  trid = (head>>16) & 0xFFF;
  size = (head>>8) & 0XFF;
  type = (head>>4) & 0xF;
  info = (head>>0) & 0xF;
}

//Getters
uint32_t TransactionHeader::GetSize(){return size;}
uint32_t TransactionHeader::GetType(){return type;}
uint32_t TransactionHeader::GetInfo(){return info;}
uint32_t TransactionHeader::GetTrid(){return trid;}
uint32_t TransactionHeader::GetPver(){return pver;}
uint32_t TransactionHeader::GetInt() {return head;}

//Setters
void TransactionHeader::SetInt(uint32_t v) { head = v; Unpack(); }
void TransactionHeader::SetSize(uint32_t v){ size = v; Pack(); }
void TransactionHeader::SetType(uint32_t v){ type = v; Pack(); }
void TransactionHeader::SetInfo(uint32_t v){ info = v; Pack(); }
void TransactionHeader::SetTrid(uint32_t v){ trid = v; Pack(); }
void TransactionHeader::SetPver(uint32_t v){ pver = v; Pack(); }

//Set bytes
void TransactionHeader::SetBytes(uint8_t* b, uint32_t pos){
  head = ByteTools::Pack(b,pos);
  Unpack();
}

