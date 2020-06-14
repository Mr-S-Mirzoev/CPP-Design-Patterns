#pragma once

#ifndef INCLUDED_DEPARTMENT_14062020
#define INCLUDED_DEPARTMENT_14062020
    #include <map>
    #include <string>
    #include <vector>
    #include "workerinfo.h"
    #include "stringmanip.h"
    namespace Company {
        class Department
        {
            std::string name;
            mutable std::map<std::string, Company::WorkerInfo> workers;
            mutable unsigned sum, q;
        public:
            // constructors
            Department ();
            Department (const std::string &n);
            Department (const Department &rhs);
            // modificators
            void add_worker (const std::string &wrk,
                             const std::string &title,
                             const unsigned &salary);
            
            void edit_worker_check (const std::string &wrk, const std::string &newname);
            void edit_worker (const std::string &wrk,
                              const std::string &nwrk,
                              const std::string &nttl,
                              const unsigned &nsal);
            void delete_worker (const std::string &wrk);
            void change_name (const std::string &newname);
            // getters
            std::pair <std::string, unsigned> find_worker (const std::string &wrk) const;
            std::string get_department_name () const;
            bool empty () const;
            // representation
            std::string repr () const;
            std::vector <std::string> xml_repr () const;
        };
    }
#endif
