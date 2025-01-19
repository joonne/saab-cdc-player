#ifndef CD_CHANGER_H
#define CD_CHANGER_H

#include "mcp_can.h"

class CdChanger {
public:
  static void handshake(MCP_CAN can);
  static void heartbeat(MCP_CAN can);
};

#endif // "CD_CHANGER_H"
