/**
 * Measure the rate of the ipbus communication
 * Author: Carlos.Solans@cern.ch
 *
 */
#include <iostream>
#include <ctime>
#include <vector>
#include "ipbus/Uhal.h"

using namespace std;
using namespace ipbus;

int main(int argc, const char* argv[]){

  if(argc<3){
    cout << "Usage: " << argv[0] << " <ipaddress> <nsamps> <nevents> " << endl;
    return 0;
  }

  const char * address = argv[1];
  uint32_t nsmps = strtoul(argv[2],NULL,10);
  uint32_t nevts = strtoul(argv[3],NULL,10);

  cout << "Measure the DAQ rate on"
       << " ipaddress: " << address 
       << " nsmps: " << nsmps 
       << " nevts: " << nevts
       << endl;
  
  Uhal * client = new Uhal(address);
  //client->SetVerbose(true);
  vector<uint32_t> v;
  time_t t1,t2;
  time(&t1);
  for(uint32_t evt=0;evt<nevts;evt++){
    client->Read(0x1500, v, nsmps);
  }
  time(&t2);
  
  uint32_t seconds = difftime(t2,t1);
  
  cout << "Elapsed time (seconds): " << seconds << endl;
  cout << "Event rate (Hz): " << float(nevts)/float(seconds) << endl;  
  delete client;
  
  return 0;
}

