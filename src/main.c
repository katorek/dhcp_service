#include "custom_function.h"

int main(int argc, char const *argv[]) {
    int sockDesc = createSocketBC();

    struct dhcp_msg *message;
    struct in_addr addr;
    info_message("Start dhcp service");

    while(0) {
       message = receivePacketDHCP(sockDesc);

        switch (message->option[2]) {
            case DHCP_INFORM:
            break;
            
            case DHCP_RELEASE:
                info_message("Received DHCP_RELEASE");

                addr.s_addr = message -> hdr.ciaddr;
                char* ipStr = inet_ntoa(addr);


            break;

            case DHCP_DISCOVER:
            break;

            case DHCP_REQUEST:
            break;
        }
        
        free(message);
    }
    
    return 0;
}