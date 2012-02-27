/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Mon Sep 24 11:25:21 EDT 2001
    copyright            : (C) 2001 by Rudolph Pienaar
    email                : pienaar@bme.ri.ccf.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//
// NAME
//
//	main.cpp
//	$Id: c_smessage.cpp,v 1.2 2005/04/05 19:38:10 rudolph Exp $
//
// DESC
//
//	`main.cpp'
//
// HISTORY
//

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
using namespace std;

#include <stdlib.h>

#include "c_SMessage.h"

int main(int argc, char *argv[])
{
  C_SMessage	csm_file(	"Hello, world!\n",
                eSM_raw,
                "out.txt",
                eSM_cpp);

  C_SMessage	csm_socket( 	"Hello, world!\n",
		eSM_raw,
		"localhost:1834",
		eSS);

  bool              b_syslogPrepend = true;

  csm_file.str_syslogID_set("main[0]:");
  csm_socket.str_syslogID_set("main[0]:");

  csm_file.timer(eSM_start);
  csm_file.dump(b_syslogPrepend);
  csm_file.str_payload_set("Another message\n");
  csm_file.dump(b_syslogPrepend);
  csm_file.dump(b_syslogPrepend, "This is an out-of-bounds message\n");
  for(int i=0; i<100000000; i++);
  csm_file.timer(eSM_stop);

  for(int loop=0; loop<2; loop++) {
    csm_socket.timer(eSM_start);
    csm_socket.dump(b_syslogPrepend);
    csm_socket.str_payload_set("Another message\n");
    csm_socket.dump(b_syslogPrepend);
    csm_socket.dump(b_syslogPrepend, "This is an out-of-bounds message\n");
    csm_socket.timer(eSM_stop);
    csm_socket.file_changeTo("localhost:1836", eSS);
  }

  csm_file.file_changeTo("stdout");
  csm_file.dump(b_syslogPrepend, "And another out-of-bounds message\n");
  csm_file.file_changeTo("out2.txt");
  csm_file.str_payload_set("And this message is sent off to another destination sink\n");
  csm_file.dump(b_syslogPrepend);
  csm_file.file_changeTo("stderr");
  csm_file.dump(b_syslogPrepend, "While this goes to stderr\n");

  return EXIT_SUCCESS;
}
