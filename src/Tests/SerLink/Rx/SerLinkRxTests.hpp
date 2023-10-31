/*
 * SerLinkRxTests.hpp
 *
 *  Created on: 30 Jun 2023
 *      Author: rw123
 */

#ifndef SERLINKRXTESTS_HPP_
#define SERLINKRXTESTS_HPP_

class SerLinkRxTests
{
public:
  SerLinkRxTests();
  static void test1();
  static void instantHandler1(); // Instant rx handler registered for the protocol
  static void stdRx1(); // Instant rx handler NOT registered for the protocol - standard rx
  void operator()();
};



#endif /* SERLINKRXTESTS_HPP_ */
