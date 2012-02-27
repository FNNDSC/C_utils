/***************************************************************************
                          SSocket_server.cpp  -  description
                             -------------------
    begin                : Tue Aug 28 2001
    copyright            : (C) 2001-2004 by Rudolph Pienaar
    email                : rudolph@nmr.mgh.harvard.edu
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
//      SSocket_server.cpp
//	$Id: SSocket_server.cpp,v 1.3 2009/10/28 17:37:45 rudolph Exp $
//
// DESCRIPTION
//
//      `SSocket_server.cpp' defines a simple server app for use with the
//	c_SSocket library.
//
// HISTORY
//
// 27 August 2001
// o Initial design and coding.
//
// 16 November 2004
// o Re-import / build into kdevelop-3.x
//
// 28 October 2009
// o Resurrection and additional output control (--silent).
// 

#include <iostream>
#include <string>
using namespace std;

#include <getopt.h>
#include <stdlib.h>

#include "c_SSocket.h"

const string	str_self = "SSocket_server";
#define	VERSION	$Id: SSocket_server.cpp,v 1.3 2009/10/28 17:37:45 rudolph Exp $

static struct option const longopts[] =
{
  {"port",		required_argument, 	NULL, 'p'},
  {"silent",            no_argument,            NULL, 's'},
  {NULL, 0, NULL, 0}
};

void
synopsis_show(void) {
    cout << "USAGE: " << str_self << " [options]"		           << endl;
    cout << "\t--port <port>\t\tPort to listen on\t\t[1701]" 	           << endl;
    cout << "\t--silent\t\tDo not echo internal messages to stdout"      << endl;
    cout << endl;
}

int main( int argc, char *argv[] )
{

    int 	port 		= 1701;
    bool        b_quiet         = false;

    // Parse command line arguments
    while(1) {
	int	opt;
    	int	optionIndex	= 0;
	opt	= getopt_long(argc, argv, "", longopts, &optionIndex);
	if( opt == -1)
	    break;
	
	switch(opt) {
	    case 'p':
	    	port		= atoi(optarg);
	    break;
            case 's':
                b_quiet         = true;
                break;
            case '?':
		synopsis_show();
		exit(1);
	    break;
	    default:
	    	cout << "?? getopt returned character code " << opt << endl;
	}
    }

    c_SSocket_UDP_receive*	pCSSocketReceive;
    
    int timeoutSec	= 0;
    int	timeoutUsec	= 0;
    	
    pCSSocketReceive	= new c_SSocket_UDP_receive(port, timeoutSec, timeoutUsec);
    string		str_payload, str_print;

    if(!b_quiet) pCSSocketReceive->print();

    int loop	= 0;	// loop counted
    int	rval	= 0;	// return value from read() system call

    while(1) {
    	rval = 0;
	loop++;
    	rval = pCSSocketReceive->recv(str_payload);

	if(rval > 0) {
	    str_print = str_payload.substr(0, rval);
            if(!b_quiet) cout << loop << "\t( " << rval << " )----> "; 
            cout << str_print << endl;
	    str_payload = "";
	    str_print = "";
 	}	
    }
    delete pCSSocketReceive;
    return 0;
}
