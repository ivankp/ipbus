/**
 * Test the ipbus communication
 * Author: Carlos.Solans@cern.ch
 *
 */
#include <iostream>
#include "ipbus/Uhal.h"

using namespace std;
using namespace ipbus;

int main(int argc, const char* argv[]){

  if(argc<3){
    cout << "Usage: " << argv[0] << " <ipaddress> <arguments> " << endl;
    cout << " arguments: " << endl;
    cout << "\t read [address]" << endl;
    cout << "\t write [address] [value]" << endl;
    return 0;
  }

  const char * address = argv[1];
  uint port = 50001;
  string func(argv[2]);
  uint offset = strtoul(argv[3],NULL,16);

  Uhal * client = new Uhal(address,port);

  if(func=="read"){ 
    uint value = 0;
    client->Read(offset,value);
    cout << "Read from: 0x" << hex << offset << " value: 0x" << value << dec << endl;
  }
  else if(func=="write"){
    if(argc<4){ cout << "Missing argument" << endl;}
    else{
      uint value = strtoul(argv[4],NULL,16);
      cout << "Write to: 0x" << hex << offset << " value: 0x" << value << dec << endl;
      client->Write(offset, value);
    }
  }
  else{
    cout << "Argument " << func << " not understood" << endl;
  }
  
  delete client;
  
  return 0;
}
