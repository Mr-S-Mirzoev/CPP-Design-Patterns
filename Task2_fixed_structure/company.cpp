#include "company.h"

namespace Company {
    // change the Company containts with te containts of other.
    void Company::set_comp (const Company &c) {
        dpts = c.dpts;
    }
    
    // operator=
    Company& Company::operator= (const Company &other) {
        dpts = other.dpts;
        return (*this);
    }
    
    // modificators
    void Company::init_department (const std::string &name, const Department &d) {
        dpts[name] = d;
    }
    void Company::add_department (const std::string &name) {
        auto search = dpts.find(name);
        if (search == dpts.end()) {
            Department dpt(name);
            dpts[name] = dpt;
        } else {
            throw std::logic_error("There is a dept with the same name " + name);
        }
    }
    void Company::delete_department (const std::string &name) {
        auto search = dpts.find(name);
        if (search != dpts.end()) {
            dpts.erase(search);
        } else {
            throw std::logic_error("There was no dept with name " + name);
        }
    }
    void Company::add_worker (const std::string &dpt,
                     const std::string &wrk,
                     const std::string &title,
                     const unsigned &salary) {
        auto search = dpts.find(dpt);
        if (search != dpts.end()) {
            (*search).second.add_worker(wrk, title, salary);
        } else {
            throw std::logic_error("There was no dept with name " + dpt);
        }
    }
    void Company::delete_worker (const std::string &dpt, const std::string &wrk) {
        auto search = dpts.find(dpt);
        if (search != dpts.end()) {
            (*search).second.delete_worker(wrk);
        } else {
            throw std::logic_error("There was no dept with name " + dpt);
        }
    }
    void Company::edit_worker_check (
                            const std::string &dpt,
                            const std::string &wrk,
                            const std::string &newname) {
        auto search = dpts.find(dpt);
        if (search != dpts.end()) {
            (*search).second.edit_worker_check(wrk, newname);
        } else {
            throw std::logic_error("There was no dept with name " + dpt);
        }
    }
    void Company::edit_worker (const std::string &dpt,
                      const std::string &wrk,
                      const std::string &nwrk,
                      const std::string &nttl,
                      const unsigned &nsal) {
        auto search = dpts.find(dpt);
        if (search != dpts.end()) {
            (*search).second.edit_worker(wrk, nwrk, nttl, nsal);
        } else {
            throw std::logic_error("There was no dept with name " + dpt);
        }
    }
    void Company::change_department_name(const std::string &ex, const std::string &n) {
        auto search = dpts.find(ex);
        if (search != dpts.end()) {
            auto newsearch = dpts.find(n);
            if (newsearch == dpts.end()) {
                Department neo((*search).second);
                neo.change_name(n);
                dpts[n] = neo;
                dpts.erase(search);
            } else {
                throw std::logic_error("No department called " + n + " found.");
            }
        } else {
            throw std::logic_error("No department called " + ex + " found.");
        }
    }
    
    // getters
    std::pair <std::string, unsigned> Company::find_wrk (const std::string &dpt, const std::string &wrk) {
        auto search = dpts.find(dpt);
        if (search != dpts.end()) {
            return (*search).second.find_worker(wrk);
        } else {
            return std::make_pair(std::string(), unsigned());
        }
    }
    Department Company::find_dpt (const std::string &name) {
        auto search = dpts.find(name);
        if (search != dpts.end()) {
            return (*search).second;
        } else {
            return Department();
        }
    }
    
    // representation
    std::vector <std::string> Company::xml_repr () const {
        std::vector <std::string> lines;
        lines.push_back("<departments>");
        for (auto &dp : dpts) {
            for (auto &lns : dp.second.xml_repr()) {
                lines.push_back(lns);
            }
        }
        lines.push_back("</departments>");
        return lines;
    }
    
    // printer
    void Company::show () const {
        std::string s;
        s = ">>>>Company:\n";
        for (auto &dpt : dpts) {
            s += dpt.second.repr();
        }
        std::cout << s;
    }
}
