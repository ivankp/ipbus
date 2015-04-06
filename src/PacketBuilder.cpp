/*******************************
 * PacketBuilder
 * Build a packet from bytes
 *
 * Carlos.Solans@cern.ch
 ******************************/

#include "ipbus/PacketBuilder.h"
#include "ipbus/Transaction.h"
#include "ipbus/Status.h"
#include "ipbus/Resend.h"

using namespace ipbus;
using namespace std;

PacketBuilder::PacketBuilder(){
  bytes = new uint8_t[1472];
  length = 1472;
  transaction = new Transaction();
  status = new Status();
  resend = new Resend();
}

PacketBuilder::~PacketBuilder(){
  delete bytes;
  delete transaction;
  delete status;
  delete resend;
}

uint8_t* PacketBuilder::GetBytes(){
  return bytes;
}

void PacketBuilder::SetLength(uint32_t v){
  length = v;
}

void PacketBuilder::Dump(){
  ByteTools::DumpBytes(bytes,0,length);
}

void PacketBuilder::Unpack(){
  header.SetBytes(bytes,0);
}

const PacketHeader& PacketBuilder::GetHeader(){
  return header;
}
    
bool PacketBuilder::CheckHeader(){
  if(header.GetPver()!=header.VERSION || 
     header.GetByoq()!=header.BYOQ){
    return false;
  }
  return true;
}
    
const Packet* PacketBuilder::GetPacket(){
  Packet * p = NULL;
  if(header.GetType()==header.CONTROL){ p = transaction;}
  else if(header.GetType()==header.STATUS){ p = status;}
  else if(header.GetType()==header.RESEND){ p = resend;}
  else{return NULL;}
  p->CopyBytes(bytes,length);
  p->Unpack();
  return p;
}

