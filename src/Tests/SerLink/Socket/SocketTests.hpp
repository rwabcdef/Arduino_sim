/*
 * SocketTests.hpp
 *
 *  Created on: 19 Dec 2023
 *      Author: rw123
 */

#ifndef SOCKETTESTS_HPP_
#define SOCKETTESTS_HPP_

// Tests used to develop the internal workings of Socket.
class SocketTests
{
public:
  SocketTests(){};
  static void TxThenAck1();
  static void RxThenStdAck(); // standard ack, i.e., no instant callback handler used.
};




#endif /* SOCKETTESTS_HPP_ */
