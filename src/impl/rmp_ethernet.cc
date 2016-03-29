#include "segwayrmp/segwayrmp.h"
#include "segwayrmp/impl/rmp_ethernet.h"

using namespace segwayrmp;

/////////////////////////////////////////////////////////////////////////////
// EthernetRMPIO

EthernetRMPIO::EthernetRMPIO() : configured(false) {
  this->connected = false;

  // TODO: We should remove this from the constructor, but this is the default.
  this->configure("192.168.0.40", 8080);
}

EthernetRMPIO::~EthernetRMPIO() {
  this->disconnect();
}

void EthernetRMPIO::configure(std::string ip, uint16_t port) {
  this->ip = inet_addr(ip.c_str());
  this->port = port;

  this->configured = true;
}

void EthernetRMPIO::connect() {
  fprintf(stderr, "Connecting in ethernet!\n");
  if (!this->configured) {
    RMP_THROW_MSG(ConnectionFailedException, "The ethernet port must be "
      "configured before connecting.");
  }

  try {
    if ((this->server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      RMP_THROW_MSG(ConnectionFailedException, "Failed to create socket.");
    }

    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(this->port);
    this->addr_len = sizeof(this->server);

    // TODO: add a test message to check connection. Of course this doesn't
    // mean we're actually 'connected', but makes sure the address is correct
    // and we can establish communication at least once.

  } catch (std::exception &e) {
    RMP_THROW_MSG(ConnectionFailedException, e.what());
  }

  fprintf(stderr, "Connected to ethernet!\n");
  this->connected = true;
}

void EthernetRMPIO::disconnect() {
  if (this->connected) {
    close(this->server_fd);
    this->connected = false;
  }
}

void debug_buffer(unsigned char *buffer, int size) {
  for (int i = 0; i < size; i++) {
    fprintf(stderr, "%0x ", buffer[i]);
  }
  fprintf(stderr, "\n");
}

int EthernetRMPIO::read(unsigned char *buffer, int size) {
  fprintf(stderr, "Reading: ");
  debug_buffer(buffer, size);
  return recvfrom(this->server_fd, buffer, size, 0,
    (struct sockaddr *)&this->server, &this->addr_len);
}

int EthernetRMPIO::write(unsigned char *buffer, int size) {
  fprintf(stderr, "Writing: ");
  debug_buffer(buffer, size);
  return sendto(this->server_fd, buffer, size, 0,
    (struct sockaddr *)&this->server, this->addr_len);
}
