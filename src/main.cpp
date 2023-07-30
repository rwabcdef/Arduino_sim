/*
 * main.cpp
 *
 *  Created on: 23 May 2023
 *      Author: rw123
 */


#include <stdio.h>
#include <stdlib.h>
#include "env.hpp"
#include "Global.hpp"
#include "testSys.hpp"

#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include "uart_wrapper.hpp"
#include "BasicHwTests.hpp"
#include "SerLinkRxTests.hpp"
#include "SerLinkTxTests.hpp"
#include "ButtonTests.hpp"

using namespace std;

void t1()
{
  uint8_t a = 3;
  ASSERT(a == 2);
}

bool runAllTests(int argc, char const *argv[]) {

  std::function<void()> bind;// = []() { BasicHwTests::test1(); };
  cute::suite s { };

  //TODO add your test here
  //s.push_back(BasicHwTests{ });

//  bind = []() { BasicHwTests::test1(); }; s.push_back(bind);
//  bind = []() { BasicHwTests::test2(); }; s.push_back(bind);

  //bind = []() { SerLinkRxTests::test2(); }; s.push_back(bind);

  //bind = []() { SerLinkTxTests::test1(); }; s.push_back(bind);
  bind = []() { ButtonTests::test1(); }; s.push_back(bind);


  cute::xml_file_opener xmlfile(argc, argv);
  cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
  auto runner = cute::makeRunner(lis, argc, argv);
  bool success = runner(s, "AllTests");
  return success;
}

void run1(){ BasicHwTests::test1(); }

int main(int argc, char const *argv[])
{
	printf("Main Start 1\n");

	//initDebug();
//	initRun(5000);
//
//	debugPrint->writeLine("Main End", DebugPrint_defs::Zero);
//
//	  fflush(stdout);
//	return 0;


	//InitTimestamp();
	//initSys(5000);

	TESTSYS_mode = TESTSYS_TEST_MODE_NONE;

	if(argc > 0)
	{
	  if(argv[1][0] == '1')
	  {
	    TESTSYS_mode = TESTSYS_TEST_MODE_UNIT;
	  }
	}


/*
	//char const *argv[] = {};
	bool success; // = runAllTests(0, {});

	std::function<void()> bind = []() { BasicHwTests::test1(); };
	cute::suite s { };
	//s.push_back(t1);
	//s.push_back(run1);
	s.push_back(bind);
	//

	cute::xml_file_opener xmlfile(0, {});
  cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
  auto runner = cute::makeRunner(lis, 0, {});
  success = runner(s, "AllTests"); //runner(t1);

  */
	if(TESTSYS_mode == TESTSYS_TEST_MODE_NONE){
	  //BasicHwTests::test1();
	  //SerLinkTxTests::test1();
	  SerLinkRxTests::test2();
	  //printf("nothing\n");
	}
	else if(TESTSYS_mode == TESTSYS_TEST_MODE_UNIT){

	  bool success = runAllTests(0, {});
    if(success)
    {
      printf("passed\n");
    }
    else
    {
      printf("failed\n");
    }
	}




	//printf("Main end");
	debugPrint->writeLine("Main End", DebugPrint_defs::Zero);

	fflush(stdout);

	return 0;
}

