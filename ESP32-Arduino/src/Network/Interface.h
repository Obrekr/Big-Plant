#ifndef Network_Interface_h
#define Network_Interface_h

#include "IPAddress.h"

namespace Network {
  class Interface {
    public:
      virtual bool APstart() = 0;
      virtual bool APstatus() = 0;
      virtual bool APstop() = 0;
  };
}
#endif
