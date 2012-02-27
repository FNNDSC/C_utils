/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Thu Sep  7 15:21:46 EDT 2000
    copyright            : (C) 2000 by Rudolph Pienaar
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>

#include "C_scanopt.h"

int main(int argc, char *argv[])
{
    C_scanopt   c_opt1("options.txt", e_EquLink);
    C_scanopt   c_opt2("options2.txt", e_EquLink);
    string*     pstr_value;
    bool        b_found = false;


    c_opt1.print();
    c_opt2.print();

    pstr_value = new string("");

    cout << "\n\nScanning for 'epsilon'...\n";
    b_found = c_opt1.scanFor("epsilon", pstr_value);

    if(b_found) {
        cout << "Found!\n";
        cout << "value = " << *pstr_value << endl;;
    } else
        cout << "Not found.\n";

    delete pstr_value;

    return(0);
}
