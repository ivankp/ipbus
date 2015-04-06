/*******************************
 * Packet
 * IPbus 2.0 packet
 *
 * Carlos.Solans@cern.ch
 ******************************/

#include "ipbus/Packet.h"
#include <iostream>

using namespace ipbus;
using namespace std;

uint32_t Packet::m_pkid = 0;

Packet::Packet(){
  header = new PacketHeader();
  header->SetPkid(m_pkid);
  bytes = new uint8_t[1472];
  length = 1472;
}

Packet::~Packet(){
  delete header;
}

void Packet::Init(){
  header->SetPkid(m_pkid);
}

uint32_t Packet::GetPkid() const{
  return header->GetPkid();
}

uint32_t Packet::GetType() const{
  return header->GetType();
}

void Packet::SetType(uint32_t type){
  header->SetType(type);
}

void Packet::SetPkid(uint32_t pkid){
  header->SetPkid(pkid);
}

uint32_t Packet::GetLength(){
  return length;
}

void Packet::SetLength(uint32_t v){
  length = v;
}
    
void Packet::Dump() const{
  ByteTools::DumpBytes(bytes,0,length);
}
    
void Packet::SetNextPacketId(uint32_t pkid){
  //cout << "Next packet id: " << pkid << endl;
  m_pkid=pkid;
  if(m_pkid>0xFFFF){m_pkid=0;}
}

uint32_t Packet::GetNextPacketId(){
  return m_pkid;
}
    
void Packet::IncrementNextPacketId(){
  m_pkid++;
  if(m_pkid>0xFFFF){m_pkid=0;}
  //cout << "Next packet id: " << m_pkid << endl;
}

//Get the contents as bytes
uint8_t * Packet::GetBytes(){
  return bytes;
}

//Set the contents from bytes
void Packet::CopyBytes(uint8_t * b, uint32_t len){
  length = len;
  for(uint32_t i=0;i<len;i++){bytes[i]=b[i];}
}

//Pack the objects into the stream
void Packet::Pack(){
  uint32_t pos=ByteTools::AddBytes(bytes,0,header->GetInt());
  pos = AddBytes(bytes,pos);
  //FIXME: if(bytes.length>pos) copyBytes(bytes,pos);
  length = pos;
}

//Unpack the stream into objects
void Packet::Unpack(){
  header->SetInt(ByteTools::Pack(bytes,0));
  SetBytes(bytes,4);
}
