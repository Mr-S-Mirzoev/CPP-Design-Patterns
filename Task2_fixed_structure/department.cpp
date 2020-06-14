#include "department.h"

namespace Company {
    Department::Department () {}
    Department::Department (const std::string &n): name(n), sum(0U), q(0U) {}
    Department::Department (const Department &rhs): name(rhs.name), workers(rhs.workers), sum(rhs.sum), q(rhs.q) {}
    
    // modificators
    void Department::add_worker (const std::string &wrk,
                     const std::string &title,
                     const unsigned &salary) {
        auto search = workers.find(wrk);
        if (search == workers.end()) {
            sum += salary;
            ++q;
            workers[wrk] = WorkerInfo(title, salary);
        } else {
            throw std::logic_error("There is already a worker with such name " + wrk);
        }
    }
    
    void Department::edit_worker_check (const std::string &wrk, const std::string &newname) {
        auto search = workers.find(wrk);
        if (search != workers.end()) {
            if (wrk != newname) {
                auto newsearch = workers.find(newname);
                if (newsearch != workers.end()) {
                    throw std::logic_error("There is already another worker with name " + wrk);
                }
            }
        } else {
            throw std::logic_error("There is no workers with such name " + wrk);
        }
    }
    
    void Department::edit_worker (const std::string &wrk,
                      const std::string &nwrk,
                      const std::string &nttl,
                      const unsigned &nsal) {
        auto search = workers.find(wrk);
        if (wrk != nwrk) {
            sum -= (*search).second.GetData().second;
            sum += nsal;
            workers[nwrk] = WorkerInfo(nttl, nsal);
            workers.erase(search);
        } else {
            workers[wrk] = WorkerInfo(nttl, nsal);
        }
    }
    
    void Department::delete_worker (const std::string &wrk) {
        auto search = workers.find(wrk);
        if (search != workers.end()) {
            sum -= (*search).second.GetData().second;
            --q;
            workers.erase(search);
        } else {
            throw std::logic_error("There is no worker with name "+wrk);
        }
    }
    
    void Department::change_name (const std::string &newname) {
        name = newname;
    }
    
    // getters
    std::pair <std::string, unsigned> Department::find_worker (const std::string &wrk) const {
        auto search = workers.find(wrk);
        if (search != workers.end()) {
            return (*search).second.GetData();
        } else {
            return std::make_pair(std::string(), unsigned());
        }
    }
    
    std::string Department::get_department_name () const {
        return name;
    }
    
    bool Department::empty () const {
        return (name.empty() && workers.empty());
    }
    
    // representation
    std::string Department::repr () const {
        std::string s;
        s += "    ====" + name + "\n";
        unsigned avg = (q == 0) ? 0 : sum/q;
        s += "    ====Average salary:" + std::to_string(avg) + "\n";
        for (auto &wrk : workers) {
            s += "        ----Name: " + wrk.first + "\n" + wrk.second.repr();
        }
        return s;
    }
    
    std::vector <std::string> Department::xml_repr () const {
        std::string indent = "   ";
        std::string indent2 = "      ";
        std::string indent3 = "         ";
        std::string indent4 = "            ";
        std::vector <std::string> lines;
        lines.push_back(indent + "<department name=\"" + name + "\">");
        lines.push_back(indent2 + "<employments>");
        for (auto &wrk : workers) {
            lines.push_back(indent3 + "<employment>");
            auto x = string_manip::split(wrk.first, " ");
            lines.push_back(indent4 + NAMEDVAL("surname", x[0]));
            lines.push_back(indent4 + NAMEDVAL("name", x[1]));
            lines.push_back(indent4 + NAMEDVAL("middleName", x[2]));
            for (auto &lns : (wrk.second).xml_repr()) {
                lines.push_back(lns);
            }
            lines.push_back(indent3 + "</employment>");
        }
        lines.push_back(indent2 + "</employments>");
        lines.push_back(indent + "</department>");
        return lines;
    }
}
