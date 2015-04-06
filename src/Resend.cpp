/**************************
 * Resend
 *
 * IPbus version 2.0 resend packet
 *
 * Carlos.Solans@cern.ch
 **************************/

#include "ipbus/Resend.h"

using namespace ipbus;

Resend::Resend(){
  SetType(PacketHeader::RESEND);
  SetPkid(0);
}

Resend::~Resend(){}

uint32_t Resend::AddBytes(uint8_t* /*b*/, uint32_t pos){
  return pos;
}

uint32_t Resend::SetBytes(uint8_t* /*b*/, uint32_t pos){
  return pos;
}

void Resend::Sent(){}
