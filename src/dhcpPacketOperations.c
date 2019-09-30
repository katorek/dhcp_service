#include "custom_function.h"

struct dhcp_msg* receivePacketDHCP(int sockDesc) {
    struct dhcp_msg* msg;
    struct sockaddr_in clientAddr;
    unsigned int clientAddrLen, recvMsgSize;

    msg = (dhcp_msg *) malloc(sizeof(dhcp_msg));
    memset(msg, 0, sizeof(dhcp_msg));

    if((recvMsgSize = recvfrom(sockDesc, msg, MAX_MSG_LEN, 0, (struct sockaddr *) &clientAddr, &clientAddrLen)) < 0) {
      exit_error("dhcpPackerOperations.receivePacketDHCP(): recvfrom() error.");  
    }

    return msg;
}

// void sendPacketDHCP(enum dhcp_msg_type type, int sock, struct dhcp_msg* msg) {
//     struct sockaddr_in serverAddr;
//     ;
// }
