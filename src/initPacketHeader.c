#include "custom_function.h"


int length = 0;

void setRouterIP(struct dhcp_msg *packet) { // Router '3'

    packet->option[length] = 0x03;
    packet->option[length+1] = 0x04;

    struct in_addr ip = getMyIpAddr();
    memcpy(packet->option + length + 2, &ip, 4);
    
    length += 6;
}

void setSubnetMask(struct dhcp_msg *packet) { //Subnet Mast '1'
    packet->option[length]   = 0x01; // 1
    packet->option[length+1] = 0x04; // len
    
    packet->option[length+2] = SUBNET_1; 
    packet->option[length+3] = SUBNET_2; 
    packet->option[length+4] = SUBNET_3; 
    packet->option[length+5] = SUBNET_4; 

    length += 6;
}

void setDNSs(struct dhcp_msg *packet) { // DNS '6'
    packet->option[length]   = 0x06;
    packet->option[length+1] = 0x08;
    // google's 8.8.8.8
    packet->option[length+2] = 0x08;
    packet->option[length+3] = 0x08;
    packet->option[length+4] = 0x08;
    packet->option[length+5] = 0x08;
    // google's 8.8.8.9
    packet->option[length+6] = 0x08;
    packet->option[length+7] = 0x08;
    packet->option[length+8] = 0x08;
    packet->option[length+9] = 0x09;

    length += 10;
}

void setMessageType(struct dhcp_msg *packet, int type) { // MessageType '53'
    packet->option[length]   = 0x35;
    packet->option[length+1] = 0x01;
    packet->option[length+1] = type;

    length += 3;
}

void setDHCPServerIdentifier(struct dhcp_msg *packet) { // DHCP Server Identifier '54'
    packet->option[length]   = 0x36;
    packet->option[length+1] = 0x36;
    // 192.168.56.1
    packet->option[length+1] = 0xc0;
    packet->option[length+1] = 0xa8;
    packet->option[length+1] = 0x38;
    packet->option[length+1] = 0x01;

    length += 6;
}

void setEnd(struct dhcp_msg *packet) { // End '255'
    packet->option[length] = 0xff;
}


void initPacketHeader(struct dhcp_msg *packet, uint8_t type) {
    length = 0;
    memset(packet->option, 0, sizeof(packet->option));
    packet->hdr.op = BOOTREPLY;

    switch (type) {
        case DHCP_OFFER:
            setMessageType(packet, type);
            setSubnetMask(packet);
            setRouterIP(packet);
            setDNSs(packet);
            setDHCPServerIdentifier(packet);
            break;
    

        default:
            exit_error("initPacketHeader(): Unknown packet type");
            break;
    }

    setEnd(packet);
}