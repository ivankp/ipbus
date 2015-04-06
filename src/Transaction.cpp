/*******************************
 * Transaction
 * IPbus 2.0 R/W/M/S transaction
 *
 * Request action on one or more values starting from base address
 * get response with one or multiple values
 *
 * Carlos.Solans@cern.ch
 ******************************/

#include "ipbus/Transaction.h"
using namespace ipbus;
using namespace std;

Transaction::Transaction(){
  SetType(PacketHeader::CONTROL);
  theader = new TransactionHeader();
  address = 0;
  theader->SetType(TransactionHeader::READ);
}

Transaction::~Transaction(){
  delete theader;
}

uint32_t Transaction::GetTransactionType(){return theader->GetType();}
    
TransactionHeader* Transaction::GetTransactionHeader(){return theader;}

void Transaction::SetSize(uint32_t size){values.resize(size);}

uint32_t Transaction::GetSize(){return theader->GetSize();}
    
void Transaction::SetAddress(uint32_t addr){ address = addr; }

uint32_t Transaction::GetAddress(){return address;}
    
const vector<uint32_t>& Transaction::GetData() const{return values;}

uint32_t Transaction::GetData(uint32_t i){return values.at(i);}
    
void Transaction::SetRead(uint32_t _address, uint32_t size, bool fifo){
  if(fifo){ theader->SetType(TransactionHeader::READFIFO); }
  else{ theader->SetType(TransactionHeader::READ); }
  address=_address;
  theader->SetSize(size);
}
      
void Transaction::SetWrite(uint32_t _address, uint32_t value){
  theader->SetType(TransactionHeader::WRITE);
  address=_address;
  if(values.size()<1){values.push_back(value);}
  else{values.at(0)=value;}
  theader->SetSize(1);
}

void Transaction::SetWrite(uint32_t _address, const uint32_t * _values, uint32_t size, bool fifo){
  if(fifo){ theader->SetType(TransactionHeader::WRITEFIFO); }
  else{ theader->SetType(TransactionHeader::WRITE); }
  address=_address;
  values.clear();
  values.reserve(size);
  for(uint32_t i=0;i<size;i++){
    values.push_back(_values[i]);
  }
  theader->SetSize(values.size());
}

void Transaction::SetWrite(uint32_t _address, vector<uint32_t> _values, bool fifo){
  if(fifo){ theader->SetType(TransactionHeader::WRITEFIFO); }
  else{ theader->SetType(TransactionHeader::WRITE); }
  address=_address;
  values.clear();
  values.reserve(_values.size());
  for(uint32_t i=0;i<_values.size();i++){
    values.push_back(_values.at(i));
  }
  theader->SetSize(values.size());
}

//Increment packet id
void Transaction::Sent(){
  IncrementNextPacketId();
}

//Add values to byte stream
uint32_t Transaction::AddBytes(uint8_t* b, uint32_t pos){
  //Encode header
  uint32_t newpos=ByteTools::AddBytes(b,pos,theader->GetInt());
  
  //Encode according to header
  if(theader->GetType()==TransactionHeader::READ||theader->GetType()==TransactionHeader::READFIFO){
    if(theader->GetInfo()==TransactionHeader::REQUEST){
      newpos=ByteTools::AddBytes(b,newpos,address);
    }else if(theader->GetInfo()==TransactionHeader::RESPONSE){
      for(uint32_t i=0;i<theader->GetSize();i++){
	newpos=ByteTools::AddBytes(b,newpos,values.at(i));
      }
    }
  }else if(theader->GetType()==TransactionHeader::WRITE||theader->GetType()==TransactionHeader::WRITEFIFO){
    if(theader->GetInfo()==TransactionHeader::REQUEST){
      newpos=ByteTools::AddBytes(b,newpos,address);	
      for(uint32_t i=0;i<theader->GetSize();i++){
	newpos=ByteTools::AddBytes(b,newpos,values.at(i));
      }
    }else if(theader->GetInfo()==TransactionHeader::RESPONSE){
      //Nothing to do;
    }
  }
  return newpos;
}
	
//Parse byte stream
uint32_t Transaction::SetBytes(uint8_t* b, uint32_t pos){
  //Parse header
  theader->SetInt(ByteTools::Pack(b,pos));
  pos+=4;
  
  //Interpret the packet according to the header	
  if(theader->GetType()==TransactionHeader::READ||theader->GetType()==TransactionHeader::READFIFO){
    if(theader->GetInfo()==TransactionHeader::REQUEST){
      address = ByteTools::Pack(b,pos);
      pos+=4;
    }else if(theader->GetInfo()==TransactionHeader::RESPONSE){
      values.clear();
      for(uint32_t i=0;i<theader->GetSize();i++){
	values.push_back(ByteTools::Pack(b,pos));
	pos+=4;
      }
    }
  }else if(theader->GetType()==TransactionHeader::WRITE||theader->GetType()==TransactionHeader::WRITEFIFO){
    if(theader->GetInfo()==TransactionHeader::REQUEST){
      address = ByteTools::Pack(b,pos);
      pos+=4;
      for(uint32_t i=0;i<theader->GetSize();i++){
	values.push_back(ByteTools::Pack(b,pos));
	pos+=4;
      }
    }else if(theader->GetInfo()==TransactionHeader::RESPONSE){
      //Nothing to do
    }
  }
  return pos;
}
