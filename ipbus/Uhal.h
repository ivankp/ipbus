//! Dear emacs this is -*-c++-*-

/*******************************
 * Uhal
 * IPBus 2.0 client
 *
 * Carlos.Solans@cern.ch
 ******************************/

#ifndef Uhal_h
#define Uhal_h 1

#include "ipbus/PacketBuilder.h"
#include "ipbus/Packet.h"
#include "ipbus/Transaction.h"
#include "ipbus/Status.h"
#include "ipbus/Resend.h"
#include <cstdlib>
#include <string>
#include <vector>
#include <netdb.h>

namespace ipbus{
  
  class Uhal{

  private:

    struct sockaddr_in server;
    int32_t sock;
    PacketBuilder pb;
    static bool verbose;
    Transaction req;
    Status status;
    Resend resend;

  public:

    /**
     * Ipbus client constructor
     * @param hostname host name or ip address of the server
     * @param _port port number on the server (50001 by default)
     * Open a socket in UDP mode
     */
    Uhal(const std::string& hostname, uint32_t _port=50001);
    
    /**
     * destructor
     * Close the socket
     */
    ~Uhal();
    
    /**
     * Syncronize with server
     * Get the next expected packet id from the server through a status request
     */
    void Sync();

    /**
     * Resyncronize with server
     * Try to recover lost packet
     */
    const Transaction * Resync(const Transaction * t);

    /**
     * Send a packet to server and receive response
     * @param p packet to send to server
     * @return a packet with the response from the server
     */
    const Packet * Send(Packet * p);

    /**
     * Write a value into an address
     * @param address address where to write from 0 to max unsigned integer 
     * @param value address where to write from 0 to max unsigned integer 
     */
    void Write(const uint32_t address, const uint32_t value);

    /**
     * Write multiple values 
     * @param address where to write
     * @param value reference to a vector containing the values to be written
     * @param size number of words to be written
     * @param fifo if enabled write always to the same register, else write to consecutive ones
     * Write an array of values into a sequence of registers starting from address (fifo=false)
     * or write the array of values into the same address (fifo=true)
     */
    void Write(const uint32_t address, const uint32_t * values, const uint32_t size, bool fifo=false);

    /**
     * Write multiple values 
     * @param address where to write
     * @param value reference to a vector containing the values to be written
     * @param fifo if enabled write always to the same register, else write to consecutive ones
     * Write a vector of values into a sequence of registers starting from address (fifo=false)
     * or write the vector of values into the same address (fifo=true)
     */
    void Write(const uint32_t address, const std::vector<uint32_t> & values, bool fifo=false);

    /**
     * Read once from one register
     * @param address where to read from
     * @param value reference to the unsigned integer that will store the value read
     */
    void Read(const uint32_t address, uint32_t & value);

    /**
     * Read multiple values into one array without memory allocation checks
     * @param address where to read from 
     * @param values pointer to array of unsigned integers that will contain the values read
     * @param size number of values to read
     * @param fifo if enabled read always from the same register, else read from consecutive ones
     * Read from a sequence of registers (fifo=false) or read a sequence from a single register (fifo=true)
     */
    void Read(const uint32_t address, uint32_t * values, const uint32_t size, bool fifo=false);

    /**
     * Read multiple values into one vector
     * @param address where to read from 
     * @param values reference to vector of unsigned integers that will contain the values read
     * @param size number of values to read
     * @param fifo if enabled read always from the same register, else read from consecutive ones
     * Read from a sequence of registers (fifo=false) or read a sequence from a single register (fifo=true)
     */
    void Read(const uint32_t address, std::vector<uint32_t> & values, const uint32_t size, bool fifo=false);

    /**
     * Enable verbosity
     * @param enable enable verbose mode
     * Static function that can be called with no object Uhal::SetVerbose(true)
     */
    static void SetVerbose(bool enable);

  };
}

#endif
