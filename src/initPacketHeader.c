#include "custom_function.h"


int length = 0;
// struct settings *settings;
struct settings* settings;

void setSubnetMask(struct dhcp_msg *packet) { //Subnet Mast '1'
    packet->option[length]   = 0x01; // 1
    packet->option[length+1] = 0x04; // len
    // int *ip = ipFromString(SUBNET_IP);

    packet->option[length+2] = *(settings->subnetmask + 0); 
    packet->option[length+3] = *(settings->subnetmask + 1); 
    packet->option[length+4] = *(settings->subnetmask + 2); 
    packet->option[length+5] = *(settings->subnetmask + 3);

    length += 6;
}

void setRouterIP(struct dhcp_msg *packet) { // Router '3'

    packet->option[length] = 0x03;
    packet->option[length+1] = 0x04;

    struct in_addr ip = getMyIpAddr();
    memcpy(packet->option + length + 2, &ip, 4);
    
    length += 6;
}

void setDNSs(struct dhcp_msg *packet) { // DNS '6'
    packet->option[length]   = 0x06;
    packet->option[length+1] = 0x08;

    packet->option[length+2] = *(settings->dns1 + 0);
    packet->option[length+3] = *(settings->dns1 + 1);
    packet->option[length+4] = *(settings->dns1 + 2);
    packet->option[length+5] = *(settings->dns1 + 3);

    packet->option[length+6] = *(settings->dns2 + 0);
    packet->option[length+7] = *(settings->dns2 + 1);
    packet->option[length+8] = *(settings->dns2 + 2);
    packet->option[length+9] = *(settings->dns2 + 3);

    length += 10;
}


void setIPLeaseTime(struct dhcp_msg *packet) // IP Address Lease Time '51'
{	
	packet->option[length] = 0x33;
	packet->option[length+1] = 0x04;
	packet->option[length+2] = TOTAL_TIME_3;
	packet->option[length+3] = TOTAL_TIME_2;
	packet->option[length+4] = TOTAL_TIME_1;
	packet->option[length+5] = TOTAL_TIME_0;

	length += 6;
}

void setMessageType(struct dhcp_msg *packet, int type) { // MessageType '53'
    packet->option[length]   = 0x35;
    packet->option[length+1] = 0x01;
    packet->option[length+2] = type;

    length += 3;
}

void setDHCPServerIdentifier(struct dhcp_msg *packet) { // DHCP Server Identifier '54'
    packet->option[length]   = 0x36;
    packet->option[length+1] = 0x04;

    packet->option[length+2] = *(settings->serverip + 0);
    packet->option[length+3] = *(settings->serverip + 1);
    packet->option[length+4] = *(settings->serverip + 2);
    packet->option[length+5] = *(settings->serverip + 3);

    length += 6;
}

void setRenewalTime(struct dhcp_msg *packet) // Renewal Time Value '58'
{	
	packet->option[length] = 0x3a;
	packet->option[length+1] = 0x04;
	packet->option[length+2] = TOTAL_TIME_3 / 2;
	packet->option[length+3] = TOTAL_TIME_2 / 2;
	packet->option[length+4] = TOTAL_TIME_1 / 2;
	packet->option[length+5] = TOTAL_TIME_0 / 2;

	length += 6;
}

void setRebindTImeValue(struct dhcp_msg *packet) // Rebinding Time Value '59'
{	
	packet->option[length] = 0x3b;
	packet->option[length+1] = 0x04;
	packet->option[length+2] = (TOTAL_TIME_3 / 8) * 7;
	packet->option[length+3] = (TOTAL_TIME_2 / 8) * 7;
	packet->option[length+4] = (TOTAL_TIME_1 / 8) * 7;
	packet->option[length+5] = (TOTAL_TIME_0 / 8) * 7;

	length += 6;
}


void setEnd(struct dhcp_msg *packet) { // End '255'
    packet->option[length] = 0xff;
}

void initPacketHeader(struct dhcp_msg *packet, uint8_t type) {
    if(settings == NULL) initSettings();
    length = 0;
    memset(packet->option, 0, sizeof(packet->option));
    packet->hdr.op = BOOTREPLY;

    switch (type) {

        
        case DHCP_OFFER:
        case DHCP_ACK:
            packet -> hdr.secs = 0x0000;
            setMessageType(packet, type);
            setIPLeaseTime(packet);
            setSubnetMask(packet);
            setRouterIP(packet);
            setDNSs(packet);
            setDHCPServerIdentifier(packet);
            setRenewalTime(packet);
            setRebindTImeValue(packet);
            break;

        case DHCP_IACK:
            packet -> hdr.flags = 0x0000;
            setMessageType(packet, DHCP_ACK);
            setSubnetMask(packet);
            setRouterIP(packet);
            setDNSs(packet);
            setDHCPServerIdentifier(packet);
            setRenewalTime(packet);
            setRebindTImeValue(packet);
            break;

        case DHCP_NAK:
            packet->hdr.ciaddr = 0;
			packet->hdr.yiaddr = 0;
			packet->hdr.flags = 0x0080;
			packet->hdr.secs = 0x0000;
		    packet->hdr.siaddr = 0x00000000;
		    packet->hdr.giaddr = 0;  
		    packet->hdr.dhcp_magic = 0x63538263;
		    setMessageType(packet, type);
		    setDHCPServerIdentifier(packet);
			break;
        
        default:
            exit_error("initPacketHeader(): Unknown packet type");
            break;
    }

    setEnd(packet);
}