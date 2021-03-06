/*
 * Definitions for IP protocol implementation
 */

////
// Constants
////

#define IPV4_ADDRESS_SIZE	4
#define IPV4_STRING_MAX		16

#define	IPV4_VERSION		0x04
#define	IPV4_DEFAULT_TTL	0x30

#define IPV4_PROTOCOL_RAW	0x00
#define IPV4_PROTOCOL_ICMP	0x01
#define IPV4_PROTOCOL_TCP	0x06
#define IPV4_PROTOCOL_UDP	0x11

#define	IPV4_RETRANS_MAX	5
#define	IPV4_RETRANS_WAIT_TIME	500000

////
// Structures
////

#pragma pack(1)

typedef struct{
  unsigned char bytes[IPV4_ADDRESS_SIZE];
  } IPv4Address;

typedef struct{
  unsigned char mixed1; 
  unsigned char diffserv; 
  unsigned short int length;
  unsigned short int id;
  unsigned short int mixed2; 
  unsigned char ttl; 
  unsigned char protocol; 
  unsigned short int checksum;
  IPv4Address source;
  IPv4Address target;
  unsigned char options[1];
  } IPv4_fields;

#define IPv4_get_version(ip)	(((ip)->mixed1&0xf0)>>4)
#define IPv4_get_hlength(ip)	((ip)->mixed1&0x0f)
#define IPv4_get_flags(ip)	((ntohs((ip)->mixed2)&0xe000)>>13)
#define IPv4_get_offset(ip)	(ntohs((ip)->mixed2)&0x0fff)

#define IPv4_set_version(ip,v)	(ip)->mixed1=((v)<<4)|((ip)->mixed1&0x0f)
#define IPv4_set_hlength(ip,l)	(ip)->mixed1=((ip)->mixed1&0xf0)|((l)&0x0f)
#define IPv4_set_flags(ip,f)    (ip)->mixed2=htons( \
				  ((f&07)<<13)|(ntohs((ip)->mixed2)&0x0fff))
#define IPv4_set_offset(ip,o)   (ip)->mixed2=htons( \
				  (ntohs((ip)->mixed2)&0xe000)|o&0x0fff)

typedef struct{
  unsigned char code; 
  unsigned char length; 
  unsigned char data[1]; 
  } IPv4_option_fields;

typedef struct{
  IPv4Address source;
  IPv4Address target;
  unsigned char zero;
  unsigned char protocol;
  unsigned short int length;
  } IPv4_pseudo_header;

#pragma pack()

////
// Global variables
////

extern IPv4Address IPV4_ADDRESS_NULL;
extern IPv4Address IPV4_ADDRESS_BROADCAST;

////
// Prototypes
////

#ifdef VERBOSE
void displayIPv4Packet(FILE *output,IPv4_fields *ip,int size);
#endif
unsigned char ipDecodePacket(EventsEvent *event,EventsSelector *selector);
unsigned char ipSendPacket(EventsEvent *event,EventsSelector *selector);
IPv4Address ipNetmask(int mask);
IPv4Address ipNetwork(IPv4Address ip,int mask);
IPv4Address ipBroadcast(IPv4Address ip,int mask);
unsigned char ipCompare(IPv4Address ip1,IPv4Address ip2);
IPv4Address ipString2Address(char *string);
char *ipAddress2String(IPv4Address ip);
IPv4Address ipArray2Address(unsigned char *array);
void ipAddress2Array(IPv4Address ip,unsigned char *field);
unsigned short int pseudoHeaderChecksum(
  IPv4Address source,IPv4Address target,
  unsigned char protocol,unsigned char **bytes,int size);
