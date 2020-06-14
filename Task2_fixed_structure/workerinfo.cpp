#include "workerinfo.h"

namespace Company {
    WorkerInfo::WorkerInfo () {};
    WorkerInfo::WorkerInfo (const std::string &t, const unsigned &s): title(t), salary(s) {}
    WorkerInfo::WorkerInfo (const WorkerInfo &rhs): title(rhs.title), salary(rhs.salary) {}
    
    // setters
    void WorkerInfo::set_title (const std::string &t) {
        title = t;
    }
    void WorkerInfo::set_salary (const unsigned &s) {
        salary = s;
    }
    
    // getter
    std::pair <std::string, unsigned> WorkerInfo::GetData () const {
        return std::pair<std::string, unsigned> (title, salary);
    }
    
    // representations
    std::string WorkerInfo::repr () const {
        return "        ---->>Function: " + title + "\n" +
        "        ---->>Salary: " + std::to_string(salary) + "\n";
    }
    std::vector <std::string> WorkerInfo::xml_repr () const {
        std::string indent4 = "            ";
        std::vector <std::string> lines;
        lines.push_back(indent4 + NAMEDVAL("function", title));
        lines.push_back(indent4 + NAMEDVAL("salary", std::to_string(salary)));
        return lines;
    }
}
