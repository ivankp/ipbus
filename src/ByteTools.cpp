/************************
 * ByteTools
 * Useful byte array packing tools
 * Carlos.Solans@cern.ch
 ************************/

#include "ipbus/ByteTools.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <stdio.h>
using namespace std;
using namespace ipbus;

uint32_t ByteTools::AddBytes(uint8_t* dest, uint32_t pos, uint32_t src){
  for(uint32_t i=0;i<4;i++){
    dest[pos+3-i] = (uint8_t)((src >> 8*i) & 0xFF);
  }
  return pos+4;
}

uint32_t ByteTools::Pack(uint8_t * b, uint32_t pos){
  uint32_t des = 0;
  for(uint32_t i=0;i<4;i++){
    des |= (uint32_t)((0x00FF & b[pos+3-i])<< (8*i));
  }
  return des;
}

void ByteTools::DumpBytes(uint8_t * b, uint32_t pos, uint32_t length){
  for(uint32_t i=pos;i<length;i+=4){
    string out;
    for(uint32_t j=0;j<4;j++){
      if(j==0) out += "0x";
      char * buffer = new char[2];
      sprintf(buffer,"%02X",b[pos+i+j]);
      out += buffer;
    }
    cout << out << endl;
  }
}
