/**************************
 * PacketHeader
 *
 * IPbus version 2.0 Packet header
 *
 * Carlos.Solans@cern.ch
 **************************/

#include "ipbus/PacketHeader.h"
#include "ipbus/ByteTools.h"
#include <iostream>

using namespace ipbus;
using namespace std;

PacketHeader::PacketHeader(){ 
  pver = VERSION;
  pkid = 0;
  byoq = BYOQ;
  type = 0;
  head = 0;
  Pack();
}

PacketHeader::~PacketHeader(){}

//Convert beans into head
void PacketHeader::Pack(){
  head = 0;
  head |= (pver & 0xF)    << 28;
  head |= (pkid & 0xFFFF) << 8;
  head |= (byoq & 0xF)    << 4;
  head |= (type & 0xF)    << 0;
}
    
//Convert head into beans
void PacketHeader::Unpack(){
  pver = (int)((0xF0000000 & head) >> 28);
  pkid = (int)((0x00FFFF00 & head) >> 8);
  byoq = (int)((0x000000F0 & head) >> 4);
  type = (int)(0x0000000F & head);
}
    
void PacketHeader::Dump(){
  cout << "Pver: " << pver << endl
       << "Pkid: " << pkid << endl
       << "Byoq: " << byoq << endl
       << "Type: " << type << endl;
}

//Getters
uint32_t PacketHeader::GetPver(){return pver;}
uint32_t PacketHeader::GetPkid(){return pkid;}
uint32_t PacketHeader::GetType(){return type;}
uint32_t PacketHeader::GetByoq(){return byoq;}
uint32_t PacketHeader::GetInt(){return head;}

//Setters
void PacketHeader::SetInt(uint32_t v){ head = v; Unpack(); }
void PacketHeader::SetPkid(uint32_t v){ pkid = v; Pack(); }
void PacketHeader::SetType(uint32_t v){ type = v; Pack(); }
void PacketHeader::SetByoq(uint32_t v){ byoq = v; Pack(); }
void PacketHeader::SetPver(uint32_t v){ pver = v; Pack(); }

//Set bytes
void PacketHeader::SetBytes(uint8_t* b, uint32_t pos){
  head = ByteTools::Pack(b,pos);	
  Unpack();
}

