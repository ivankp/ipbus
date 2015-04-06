/*******************************
 * Uhal
 * IPBus 2.0 client
 *
 * Carlos.Solans@cern.ch
 ******************************/

#include "ipbus/Uhal.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace ipbus;
using namespace std;

bool Uhal::verbose = false;

Uhal::Uhal(const string& hostname, uint32_t port){
  struct hostent * host = gethostbyname(hostname.c_str());
  memcpy(&server.sin_addr, host->h_addr_list[0], host->h_length);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  sock = socket(AF_INET,SOCK_DGRAM,0);
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
  Sync();
}

Uhal::~Uhal(){}

void Uhal::Sync(){
  Status * p = (Status*)Send(&status);
  if(p==NULL) return;
  Packet::SetNextPacketId(p->GetNextExpectedPacketId());
}

const Transaction* Uhal::Resync(const Transaction* req){
  Status * p = (Status*)Send(&status);
  if(p==NULL) return NULL;
  if(p->GetNextExpectedPacketId()>req->GetPkid()+p->GetBufferSize()) return NULL;
  resend.SetPkid(req->GetPkid());
  Transaction * rep = (Transaction*) Send(&resend);
  return rep;
}

const Packet * Uhal::Send(Packet *p){
  const Packet * reply = NULL;
  //send data
  p->Pack();
  sendto(sock,p->GetBytes(),p->GetLength(),0,(struct sockaddr*)&server,sizeof(server));
  if(verbose){ 
    cout << "Outgoing frame" << endl;
    p->Dump();
  }
  p->Sent();
  //receive acknowledge
  int32_t n=recvfrom(sock,pb.GetBytes(),10000,0,NULL,NULL);
  //fast return
  if(n<0){ return reply; }
  //unpack
  pb.SetLength(n);
  pb.Unpack();
  reply = pb.GetPacket();
  if(verbose){
    cout << "Incomming frame" << endl;
    reply->Dump();
  }
  return reply;
}

void Uhal::Write(const uint32_t address, const uint32_t value){
  req.Init();
  req.SetWrite(address,value);
  const Transaction * rep = (Transaction*) Send(&req);
  if(rep==NULL){rep = Resync(&req);}
  if(rep==NULL)return;
}

void Uhal::Write(const uint32_t address, const uint32_t * values, const uint32_t size, bool fifo){
  req.Init();
  req.SetWrite(address,values,size,fifo);
  const Transaction * rep = (Transaction*) Send(&req);
  if(rep==NULL){rep = Resync(&req);}
  if(rep==NULL)return;
}

void Uhal::Write(const uint32_t address, const vector<uint32_t> & values, bool fifo){
  req.Init();
  req.SetWrite(address,values,fifo);
  const Transaction * rep = (Transaction*) Send(&req);
  if(rep==NULL){rep = Resync(&req);}
  if(rep==NULL)return;
}

void Uhal::Read(const uint32_t address, uint32_t & value){
  req.Init();
  req.SetRead(address,1);
  const Transaction * rep = (Transaction*) Send(&req);
  if(rep==NULL){rep = Resync(&req);}
  if(rep==NULL){value=0; return;}
  if(rep->GetData().size()>0) value = rep->GetData().at(0);
}

void Uhal::Read(const uint32_t address, vector<uint32_t> & values, const uint32_t size, bool fifo){
  req.Init();
  req.SetRead(address,size,fifo);
  const Transaction * rep = (Transaction*) Send(&req);
  if(rep==NULL){rep = Resync(&req);}
  if(rep==NULL){return;}
  if(values.size()!=size){values.resize(size);}
  for(uint32_t i=0;i<rep->GetData().size();i++){
    values[i]=rep->GetData()[i];
  }
}

void Uhal::Read(const uint32_t address, uint32_t * values, const uint32_t size, bool fifo){
  req.Init();
  req.SetRead(address,size,fifo);
  const Transaction * rep = (Transaction*) Send(&req);
  if(rep==NULL){rep = Resync(&req);}
  if(rep==NULL){return;}
  for(uint32_t i=0;i<rep->GetData().size();i++){
    values[i]=rep->GetData()[i];
  }
}

void Uhal::SetVerbose(bool enable){
  verbose = enable;
}
