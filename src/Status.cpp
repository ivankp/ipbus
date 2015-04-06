/**************************
 * Status
 *
 * IPbus version 2.0 status packet
 *
 * Carlos.Solans@cern.ch
 **************************/

#include "ipbus/Status.h"

using namespace ipbus;

Status::Status(){
  SetType(PacketHeader::STATUS);
  SetPkid(0);
  for(uint32_t i=0;i<SIZE;i++){
    values.push_back(0);
  }
}

Status::~Status(){}

uint32_t Status::GetBufferSize(){
  return values.at(1);
}

uint32_t Status::GetNextExpectedPacketId(){
  return (values.at(2)>>8)&0xFFFF;
}

void Status::SetBufferSize(uint32_t v){
  values.at(1)=v;
}

void Status::SetNextExpectedPacketId(uint32_t v){
  values.at(2)=v;
}

uint32_t Status::AddBytes(uint8_t* b, uint32_t pos){
  for(uint32_t i=0;i<SIZE;i++){
    pos=ByteTools::AddBytes(b,pos,values.at(i));
  }
  return pos;
}

uint32_t Status::SetBytes(uint8_t* b, uint32_t pos){
  values.clear();
  for(uint32_t i=0;i<SIZE;i++){
    values.push_back(ByteTools::Pack(b,pos));
    pos+=4;
  }
  return pos;
}

void Status::Sent(){}
