cplusplus {{
#include "ApplicationPacket_m.h"
}}

class ApplicationPacket;

struct addr {
      unsigned short src;  			// the packet original source
      unsigned short dst;			// the packet ultimate destination
      unsigned short rt_hstr [10];	//routing history (1 if had been at node X)
      unsigned short ttl;			//time to live
}

packet HiPacket extends ApplicationPacket {
      addr address;
}