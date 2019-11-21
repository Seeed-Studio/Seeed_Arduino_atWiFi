#include "WiFiUdp.h"
#include "WiFiGeneric.h"
#include "UnifiedAtTcpIp.h"

#undef write
#undef read
constexpr int maxPacketSize = 1460;

// WiFiUDP::WiFiUDP()
// : udp_server(-1)
// , server_port(0)
// , remote_port(0)
// , tx_buffer(0)
// , tx_buffer_len(0)
// , rx_buffer(0)
// {}

WiFiUDP::WiFiUDP():
    local{}, remote{}, tx{}, rx{}, linkId{} {
}

WiFiUDP::~WiFiUDP(){
   stop();
}

uint8_t WiFiUDP::begin(IPAddress address, uint16_t port){
  // stop();

  // server_port = port;

  // tx_buffer = new char[1460];
  // if(!tx_buffer){
  //   log_e("could not create tx buffer: %d", errno);
  //   return 0;
  // }

  // if ((udp_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
  //   log_e("could not create socket: %d", errno);
  //   return 0;
  // }

  // int yes = 1;
  // if (setsockopt(udp_server,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
  //     log_e("could not set socket option: %d", errno);
  //     stop();
  //     return 0;
  // }

  // struct sockaddr_in addr;
  // memset((char *) &addr, 0, sizeof(addr));
  // addr.sin_family = AF_INET;
  // addr.sin_port = htons(server_port);
  // addr.sin_addr.s_addr = (in_addr_t)address;
  // if(bind(udp_server , (struct sockaddr*)&addr, sizeof(addr)) == -1){
  //   log_e("could not bind socket: %d", errno);
  //   stop();
  //   return 0;
  // }
  // fcntl(udp_server, F_SETFL, O_NONBLOCK);
  // return 1;

    ipv4 ip, gw, mask;
    stop();

    if (atIpMux(enable) == fail || 
        atStationIp(& ip, & gw, & mask) == fail){
        return 0;
    }

    local.address = *(ip_addr_t *) & ip;
    local.mask = *(ip_addr_t *) & mask;
    local.port = port;
    remote.address = 0;
    remote.port = 0;
    linkId = leaveOut;

    if (address) {
        local.address = address;
        return atStationIp(*(ipv4 *) & address) == success;
    }
    return 1;
}

uint8_t WiFiUDP::begin(uint16_t p){
  // return begin(IPAddress(INADDR_ANY), p);
    return begin(IPAddress(0), p);
}

uint8_t WiFiUDP::beginMulticast(IPAddress a, uint16_t p){
  // if(begin(IPAddress(INADDR_ANY), p)){
  //   if(a != 0){
  //     struct ip_mreq mreq;
  //     mreq.imr_multiaddr.s_addr = (in_addr_t)a;
  //     mreq.imr_interface.s_addr = INADDR_ANY;
  //     if (setsockopt(udp_server, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
  //         log_e("could not join igmp: %d", errno);
  //         stop();
  //         return 0;
  //     }
  //     multicast_ip = a;
  //   }
  //   return 1;
  // }
  // return 0;
    return begin(a, p);
}

void WiFiUDP::stop(){
  // if(tx_buffer){
  //   delete[] tx_buffer;
  //   tx_buffer = NULL;
  // }
  // tx_buffer_len = 0;
  // if(rx_buffer){
  //   cbuf *b = rx_buffer;
  //   rx_buffer = NULL;
  //   delete b;
  // }
  // if(udp_server == -1)
  //   return;
  // if(multicast_ip != 0){
  //   struct ip_mreq mreq;
  //   mreq.imr_multiaddr.s_addr = (in_addr_t)multicast_ip;
  //   mreq.imr_interface.s_addr = (in_addr_t)0;
  //   setsockopt(udp_server, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
  //   multicast_ip = IPAddress(INADDR_ANY);
  // }
  // close(udp_server);
  // udp_server = -1;
    if (linkId != leaveOut){
        atIpClose(linkId);
    }
}

int WiFiUDP::beginMulticastPacket(){
  // if(!server_port || multicast_ip == IPAddress(INADDR_ANY))
  //   return 0;
  // remote_ip = multicast_ip;
  // remote_port = server_port;
  // return beginPacket();

    IPAddress broadcast = WiFiGeneric::calculateBroadcast(remote.address, remote.mask);
    remote.ip = broadcast;
    remote.port = local.port;
    return beginPacket();
}

int WiFiUDP::beginPacket(){
  // if(!remote_port)
  //   return 0;

  // // allocate tx_buffer if is necessary
  // if(!tx_buffer){
  //   tx_buffer = new char[1460];
  //   if(!tx_buffer){
  //     log_e("could not create tx buffer: %d", errno);
  //     return 0;
  //   }
  // }

  // tx_buffer_len = 0;

  // // check whereas socket is already open
  // if (udp_server != -1)
  //   return 1;

  // if ((udp_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
  //   log_e("could not create socket: %d", errno);
  //   return 0;
  // }

  // fcntl(udp_server, F_SETFL, O_NONBLOCK);

  // return 1;
    if (!remote.port){
        return 0;
    }

    if (tx.buf == nullptr){
        tx.buf = new uint8_t[maxPacketSize];
        tx.length = 0;
    }

    if (tx.buf == nullptr){
        return 0;
    }

}

int WiFiUDP::beginPacket(IPAddress ip, uint16_t port){
  // remote_ip = ip;
  // remote_port = port;
  // return beginPacket();
    remote.port = port;
    remoet.ip = ip;
    return beginPacket();
}

int WiFiUDP::beginPacket(const char *host, uint16_t port){
  // struct hostent *server;
  // server = gethostbyname(host);
  // if (server == NULL){
  //   log_e("could not get host from dns: %d", errno);
  //   return 0;
  // }
  // return beginPacket(IPAddress((const uint8_t *)(server->h_addr_list[0])), port);
    ipv4 ip;
    if (atGetIpByDomainName(host, & ip) == fail){
        return 0;
    }
    return beginPacket(IPAddress(ip), port);
}

int WiFiUDP::endPacket(){
  // struct sockaddr_in recipient;
  // recipient.sin_addr.s_addr = (uint32_t)remote_ip;
  // recipient.sin_family = AF_INET;
  // recipient.sin_port = htons(remote_port);
  // int sent = sendto(udp_server, tx_buffer, tx_buffer_len, 0, (struct sockaddr*) &recipient, sizeof(recipient));
  // if(sent < 0){
  //   log_e("could not send data: %d", errno);
  //   return 0;
  // }
  // return 1;
    if (tx.buf && tx.length){
        // delete tx.buf in atIpSend (param 3 is true)
        bool r = atIpSend(tx.buf, tx.length, true, linkId) == success;
        delete [] tx.buf;
        tx.length = 0;
        tx.buf = nullptr;
        return r;
    }
    return 0;
}

size_t WiFiUDP::write(uint8_t data){
  // if(tx_buffer_len == 1460){
  //   endPacket();
  //   tx_buffer_len = 0;
  // }
  // tx_buffer[tx_buffer_len++] = data;
  // return 1;

    tx_buffer[tx.length++] = data;

    if (tx.length == maxPacketSize){
        endPacket();
    }
}

size_t WiFiUDP::write(const uint8_t *buffer, size_t size){
  size_t i;
  for(i=0;i<size;i++)
    write(buffer[i]);
  return i;
}

int WiFiUDP::parsePacket(){
  // if(rx_buffer)
  //   return 0;
  // struct sockaddr_in si_other;
  // int slen = sizeof(si_other) , len;
  // char * buf = new char[1460];
  // if(!buf){
  //   return 0;
  // }
  // if ((len = recvfrom(udp_server, buf, 1460, MSG_DONTWAIT, (struct sockaddr *) &si_other, (socklen_t *)&slen)) == -1){
  //   delete[] buf;
  //   if(errno == EWOULDBLOCK){
  //     return 0;
  //   }
  //   log_e("could not receive data: %d", errno);
  //   return 0;
  // }
  // remote_ip = IPAddress(si_other.sin_addr.s_addr);
  // remote_port = ntohs(si_other.sin_port);
  // if (len > 0) {
  //   rx_buffer = new cbuf(len);
  //   rx_buffer->write(buf, len);
  // }
  // delete[] buf;
  // return len;
    
}

int WiFiUDP::available(){
  // if(!rx_buffer) return 0;
  // return rx_buffer->available();

}

int WiFiUDP::read(){
  // if(!rx_buffer) return -1;
  // int out = rx_buffer->read();
  // if(!rx_buffer->available()){
  //   cbuf *b = rx_buffer;
  //   rx_buffer = 0;
  //   delete b;
  // }
  // return out;
    
}

int WiFiUDP::read(unsigned char* buffer, size_t len){
  return read((char *)buffer, len);
}

int WiFiUDP::read(char* buffer, size_t len){
  if(!rx_buffer) return 0;
  int out = rx_buffer->read(buffer, len);
  if(!rx_buffer->available()){
    cbuf *b = rx_buffer;
    rx_buffer = 0;
    delete b;
  }
  return out;
}

int WiFiUDP::peek(){
  if(!rx_buffer) return -1;
  return rx_buffer->peek();
}

void WiFiUDP::flush(){
  if(!rx_buffer) return;
  cbuf *b = rx_buffer;
  rx_buffer = 0;
  delete b;
}

IPAddress WiFiUDP::remoteIP(){
  // return remote_ip;
    return remote.ip;
}

uint16_t WiFiUDP::remotePort(){
  // return remote_port;
    return remote.port;
}
