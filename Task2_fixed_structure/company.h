#pragma once

#ifndef INCLUDED_COMPANY_14062020
#define INCLUDED_COMPANY_14062020
    #include "department.h"
    #include <map>
    #include <string>
    #include <vector>
    #include <iostream>
    namespace Company {
        class Company
        {
            mutable std::map<std::string, Department> dpts;
        public:
            // change the Company containts with te containts of other.
            void set_comp (const Company &c);
            // operator=
            Company& operator= (const Company &other);
            // modificators
            void init_department (const std::string &name, const Department &d);
            void add_department (const std::string &name);
            void delete_department (const std::string &name);
            void add_worker (const std::string &dpt,
                             const std::string &wrk,
                             const std::string &title,
                             const unsigned &salary);
            void delete_worker (const std::string &dpt, const std::string &wrk);
            void edit_worker_check (const std::string &dpt,
                                    const std::string &wrk,
                                    const std::string &newname);
            void edit_worker (const std::string &dpt,
                              const std::string &wrk,
                              const std::string &nwrk,
                              const std::string &nttl,
                              const unsigned &nsal);
            void change_department_name(const std::string &ex, const std::string &n);
            // getters
            std::pair <std::string, unsigned> find_wrk (const std::string &dpt, const std::string &wrk);
            Department find_dpt (const std::string &name);
            // representation
            std::vector <std::string> xml_repr () const;
            // printer
            void show () const;
        };
    }
#endif
