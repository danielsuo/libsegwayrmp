/*!
 * \file rmp_ethernet.h
 * \author  Daniel Suo <dsuo@cs.princeton.edu>
 * \version 0.1
 *
 * \section LICENSE
 *
 * The BSD License
 *
 * Copyright (c) 2016 Daniel Suo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * \section DESCRIPTION
 *
 * This provides an Ethernet based implementation of the rmp_io interface.
 */

#ifndef RMP_ETHERNET_H
#define RMP_ETHERNET_H

#include "rmp_io.h"
#include <arpa/inet.h>

// NOTE: UDP is not a connection-oriented communications protocol, but we force
// it into this paradigm because rmp_io is designed this way.

namespace segwayrmp {
  /*!
   * Provides an ethernet-based interface for reading and writing packets.
   */
  class EthernetRMPIO : public RMPIO {
  public:
    EthernetRMPIO();
    ~EthernetRMPIO();

    /*!
     * Connects to the Ethernet UDP port if it has been configured. Can throw ConnectionFailedException.
     */
    void connect();

    /*!
     * Disconnects from the Ethernet UDP port if it is open.
     */
    void disconnect();

    /*!
     * Read Function, reads from the Ethernet UDP socket.
     *
     * \param buffer An unsigned char array for data to be read into.
     * \param size The amount of data to be read.
     * \return int Bytes read.
     */
    int read(unsigned char *buffer, int size);

    /*!
     * Write Function, writes to the Ethernet UDP port.
     *
     * \param buffer An unsigned char array of data to be written.
     * \param size The amount of data to be written.
     * \return int Bytes written.
     */
    int write(unsigned char *buffer, int size);

    /*!
     * Configures the Ethernet UDP port.
     *
     * \param ip The IP address of the server
     * \param port The port of the server
     */
    void configure(std::string ip, uint16_t port);

  private:
    bool configured;

    uint32_t ip;
    uint16_t port;

    struct sockaddr_in server;
    int server_fd;
    socklen_t addr_len;
  };
}

#endif
