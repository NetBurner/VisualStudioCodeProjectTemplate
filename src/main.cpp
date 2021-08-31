/* Revision: 3.3.2 */

/******************************************************************************
* Copyright 1998-2021 NetBurner, Inc.  ALL RIGHTS RESERVED
*
*    Permission is hereby granted to purchasers of NetBurner Hardware to use or
*    modify this computer program for any use as long as the resultant program
*    is only executed on NetBurner provided hardware.
*
*    No other rights to use this program or its derivatives in part or in
*    whole are granted.
*
*    It may be possible to license this or other NetBurner software for use on
*    non-NetBurner Hardware. Contact sales@Netburner.com for more information.
*
*    NetBurner makes no representation or warranties with respect to the
*    performance of this computer program, and specifically disclaims any
*    responsibility for any damages, special or consequential, connected with
*    the use of this program.
*
* NetBurner
* 16855 W Bernardo Dr
* San Diego, CA 92127
* www.netburner.com
******************************************************************************/

#include <init.h>
#include <nbrtos.h>

// Specify application name to appear in IPSetup utility
const char *AppName = "GDB Demo";

int add(int a, int b) {
  return a + b;
}

int square(int x) {
  return x * x;
}

int addAndSquare(int a, int b) {
  return square(add(a, b));
}

void TaskSimple(void *pd) {
  uint32_t loopCount = 0;
  int result = 0;

  while (1) {
    iprintf("TaskSimple, %ld\r\n", loopCount);
    loopCount++;

    for (size_t i = 0; i < 5; i++) {
      for (size_t j = 0; j < 5; j++) {
        result = addAndSquare(i, j);
        iprintf("addAndSquare[%d,%d]=%d \r\n", i, j, result);
        OSTimeDly(TICKS_PER_SECOND * 2);
      }
    }
  }
}

/**
 * UserMain
 **/
void UserMain(void *pd) {
  init();                                      // Initialize network stack
  StartHttp();                                 // Start web server, default port 80
  WaitForActiveNetwork(TICKS_PER_SECOND * 5);  // Wait for DHCP address

  iprintf("GDB Demo Application started\n");

  int counter = 0;

  OSSimpleTaskCreatewName(TaskSimple, MAIN_PRIO - 2, "TaskSimple");

  while (1) {
    OSTimeDly(TICKS_PER_SECOND * 10);
    for (size_t i = 0; i < 1000; i++) {
      if (i % 2 == 0) {
        counter = counter + 1;
        iprintf("Counter = %d \r\n", counter);
      }
    }
  }
}