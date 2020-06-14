#pragma once

#ifndef INCLUDED_XMLHANDLER_14062020
#define INCLUDED_XMLHANDLER_14062020
    #include <iostream>
    #include <string>
    #include <fstream>
    #include "stringmanip.h"
    #include "company.h"
    class XML_reader {
        Company::Company comp;
    public:
        XML_reader (const std::string &filename);
        Company::Company get_comp () const;
        Company::Company& get_comp_lnk ();
        void show () const;
    };

    class XML_writer {
    public:
        XML_writer (const std::string &filename, const Company::Company &c);
    };
#endif
