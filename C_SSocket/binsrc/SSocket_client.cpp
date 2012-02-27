/***************************************************************************
                          SSocket_client.cpp  -  description
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
//      SSocket_client.cpp
//	$Id: SSocket_client.cpp,v 1.2 2009/10/28 17:37:45 rudolph Exp $
//
// DESCRIPTION
//
//      `SSocket_client.cpp' defines a simple client app for use with the
//	c_SSocket library.
//
// HISTORY
//
// 28 August 2001
// o Initial design and coding.
//
// 16 November 2004
// o Re-import / build into kdevelop-3.x
//
// 28 February 2005
// o Added '--silent' command line argument.
//
// 28 October 2009
// o Resurrection and correction for file/msg transmission.
// 

#include <iostream>
#include <string>
using namespace std;

#include <getopt.h>
#include <errno.h>
#include <stdlib.h>

#include "c_SSocket.h"

const string	str_self = "SSocket_client";
#define	VERSION	$Id: SSocket_client.cpp,v 1.2 2009/10/28 17:37:45 rudolph Exp $

static struct option const longopts[] =
{
  {"host",		required_argument, 	NULL, 'h'},
  {"port",		required_argument, 	NULL, 'p'},
  {"msg", 		required_argument, 	NULL, 't'},
  {"file", 		required_argument, 	NULL, 'f'},
  {"silent",		no_argument,		NULL, 's'},
  {NULL, 0, NULL, 0}
};

void
synopsis_show(void) {
    cout << "USAGE: " << str_self << " [options]"	                   << endl;
    cout << "\t--host <hostName>\tRemote host to transmit to\t[localhost]" << endl;
    cout << "\t--port <port>\t\tPort on remote host\t\t[1701]"             << endl;
    cout << "\t--msg <text>\t\tMessage to transmit\t\t[\"\"]"              << endl;
    cout << "\t--file <file>\t\tFile to transmit"                          << endl;
    cout << "\t--silent\t\t\tDo not echo internal messages to stdout"      << endl;
    cout << endl;
}

int main( int argc, char *argv[] )
{

    string	str_remoteHost	= "localhost";
    int 	port 		= 1701;
    string	str_msg		= "";
    string	str_filename	= "";
    bool	b_quiet		= false;
    bool        b_transmitMsg   = false;
    bool        b_transmitFile  = false;

    // Parse command line arguments
    while(1) {
	int	opt;
    	int	optionIndex	= 0;
	opt	= getopt_long(argc, argv, "", longopts, &optionIndex);
	if( opt == -1)
	    break;
	
	switch(opt) {
	    case 'h':
		str_remoteHost	= optarg;
	    break;
	    case 'p':
	    	port		= atoi(optarg);
	    break;
	    case 't':
	    	str_msg		= optarg;
                b_transmitMsg   = true;
	    break;
	    case 'f':
	    	str_filename	= optarg;
                b_transmitFile  = true;
	    break;
	    case 's':
	    	b_quiet		= true;
	    break;
	    case '?':
		synopsis_show();
		exit(1);
	    break;
	    default:
	    	cout << "?? getopt returned character code " << opt << endl;
	}
    }

    c_SSocket_UDP_transmit	CSSocketTransmit(
    					str_remoteHost,
    					port);
    
    if(!b_quiet) CSSocketTransmit.print();

    bool	b_localEcho	= !b_quiet;
    
    if(b_transmitMsg)   CSSocketTransmit.sendStr(str_msg,       b_localEcho);
    if(b_transmitFile)  CSSocketTransmit.sendFile(str_filename, b_localEcho);

    return 0;
}
