//! Dear emacs this is -*-c++-*-

/*******************************
 * Packet
 * IPbus 2.0 packet
 * Base class of the IPbus types
 *
 * Carlos.Solans@cern.ch
 ******************************/

#ifndef Packet_h
#define Packet_h 1

#include "ipbus/ByteTools.h"
#include "ipbus/PacketHeader.h"

namespace ipbus{

  class Packet{
  
  private:
      
    static uint32_t m_pkid;
    PacketHeader * header;
    uint8_t * bytes;
    uint32_t length;
    
  public:
    
    /**
     * Default Packet constructor.
     * Allocate data bytes and initialize the header with next expected packet ID
     */
    Packet();
    
    /**
     * Destructor
     * Nothing to do
     */
    virtual ~Packet();
    
    /**
     * Set the packet ID to the next expected packet ID
     */
    void Init();
    
    /**
     * Get the packet ID
     */
    uint32_t GetPkid() const;
    
    /**
     * Get the packet type
     */
    uint32_t GetType() const;
    
    /**
     * Set the packet ID
     * @param pkid The packet ID
     */
    void SetPkid(uint32_t pkid);
    
    /**
     * Set the packet type
     * @param type The packet type
     */
    void SetType(uint32_t type);
    
    /**
     * Get the packet length
     * @return the packet length in bytes
     */
    uint32_t GetLength();

    /**
     * Set the packet length
     * @param the new length
     */
    void SetLength(uint32_t v);
    
    /**
     * Print the packet contents through std out
     */
    void Dump() const;
    
    /**
     * Set the next expected packet ID.
     * @static
     * @param pkid a packet ID
     * Next created or initialized packet will use this ID.
     */
    static void SetNextPacketId(uint32_t pkid);

    /**
     * Get the next expected packet ID.
     * @static
     * @return a packet ID
     * Get the ID of the next created or initialized packet.
     */
    static uint32_t GetNextPacketId();
    
    /**
     * Increment by one count the next packet ID.
     * @static
     * Commodity function to increment the next packet ID programatically.
     */
    static void IncrementNextPacketId();

    /** 
     * Callback method for child classes upon packet send
     * @virtual
     * Increment the packet id in child classes 
     */
    virtual void Sent()=0;

    /**
     * Add values to byte stream
     * @virtual
     * @param bytes byte array
     * @param pos a first element of the byte array
     * @return number of bytes processed
     */
    virtual uint32_t AddBytes(uint8_t* /*b*/, uint32_t /*pos*/)=0;
    
    /**
     * Parse the byte stream
     * @virtual
     * @param bytes byte array
     * @param pos a first element of the byte array
     * @return number of bytes processed
     */
    virtual uint32_t SetBytes(uint8_t* /*b*/, uint32_t /*pos*/)=0;

    /**
     * Get packet as a byte array
     * @return byte array
     */
    uint8_t* GetBytes();

    /**
     * Set the contents from bytes
     * @param bytes array of bytes
     * @param length number of bytes to copy
     */
    void CopyBytes(uint8_t* b, uint32_t length);

    /**
     * Pack the objects into the stream
     */
    void Pack();

    /**
     * Unpack the stream into objects
     */
    void Unpack();

  };
}

#endif
