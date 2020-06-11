#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <iterator>
#include <cstdio>
#include <windows.h>
#include <set>
#include <map>

#ifdef _WIN32
   #include <io.h> 
   #define access    _access_s
#else
   #include <unistd.h>
#endif

bool FileExists( const std::string &Filename )
{
    return access( Filename.c_str(), 0 ) == 0;
}

#define SYSTEM(comp) comp == "show" || comp == "undo" || comp == "redo" || comp == "commands"
#define ADD_DEPT "add_dpt"
#define ADD_WORKER "add_wrk"
#define UNDO "undo"
#define REDO "redo"
#define CHANGE_DEPT_NAME "chgname_dpt"
#define EDIT_DEPT_CONTAINS "edit_dpt"
#define DELETE_DEPT "del_dpt"
#define DELETE_WORKER "del_wrk"
#define EXIT "exit"
#define HELP "commands"
#define LOAD "load"
#define SAVE "save"

#define XOPEN(str) "<" + str + ">"
#define XCLOS(str) "</" + str + ">"
#define NAMEDVAL(name, val) XOPEN(name) + val + XCLOS(name)

namespace string_manip {
    // trim from start (in place)
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    static inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    static inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    static inline std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }

    //split string
    static inline std::vector<std::string> split (const std::string &s, const std::string &delim) {
        std::vector<std::string> results;
        auto start = 0U;
        auto end = s.find(delim);
        while (end != std::string::npos)
        {
            results.push_back(s.substr(start, end - start));
            start = end + delim.length();
            end = s.find(delim, start);
        }
        results.push_back(s.substr(start, end));
        return results;
    }
}

namespace Company {
    class WorkerInfo 
    {
        std::string title;
        unsigned salary;
    public:
        WorkerInfo () {};
        WorkerInfo (const std::string &t, const unsigned &s): title(t), salary(s) {}
        std::string repr () const {
            return "        ---->>Function: " + title + "\n" + 
                    "        ---->>Salary: " + std::to_string(salary) + "\n";
        }
        std::pair <std::string, unsigned> GetData () const {
            return std::pair<std::string, unsigned> (title, salary);
        }
        std::vector <std::string> xml_repr () const {
            std::string indent4 = "            ";
            std::vector <std::string> lines;
            lines.push_back(indent4 + NAMEDVAL("function", title));
            lines.push_back(indent4 + NAMEDVAL("salary", std::to_string(salary)));
            return lines;
        }
    };

    struct StrPtrComp 
    {
        bool operator() (const std::string *lhs, const std::string *rhs) const {
            return ((*lhs) < (*rhs));
        }
    };

    class Department 
    {
        std::string name;
        std::map<const std::__cxx11::basic_string<char>*, WorkerInfo *, StrPtrComp> workers;
        unsigned sum, q;
    public:
        Department (const std::string &n): name(n), sum(0), q(0) {}
        Department (const Department &rhs): name(rhs.name), workers(rhs.workers), sum(rhs.sum), q(rhs.q) {}
        Department () {}
        void newwrk (const std::string &wrk, 
                    const std::string &title, 
                    const unsigned &salary) {
            std::string *x = new std::string (wrk);
            auto search = workers.find(x);
            if (search == workers.end()) {
                sum += salary;
                ++q;
                workers[x] = new WorkerInfo(title, salary);
            } else {
                throw std::logic_error("There is already a worker with such name "+wrk);
            }
        } 
        void editwrk_elightability (const std::string &wrk, const std::string &newname) {
            auto search = workers.find(&wrk);
            if (search != workers.end()) {
                if (wrk != newname) {
                    auto newsearch = workers.find(&newname);
                    if (newsearch != workers.end()) {
                        throw std::logic_error("There is no workers with such name "+wrk);
                    }
                }
            } else {
                throw std::logic_error("There is no workers with such name "+wrk);
            }
        } 
        void editwrk (const std::string &wrk,
                    const std::string &nwrk,
                    const std::string &nttl,
                    const unsigned &nsal) {
            auto search = workers.find(&wrk);
            if (wrk != nwrk) {
                sum -= (*search).second->GetData().second;
                sum += nsal;
                workers[&nwrk] = new WorkerInfo(nttl, nsal);
                workers.erase(search);
            } else {
                workers[&wrk] = new WorkerInfo(nttl, nsal);
            }
        }
        void newname (const std::string &x) {
            name = x;
        }
        void delwrk (const std::string &wrk) {
            auto search = workers.find(&wrk);
            if (search != workers.end()) {
                sum -= (*search).second->GetData().second;
                --q;
                workers.erase(search);
            } else {
                throw std::logic_error("There is no worker with name "+wrk);
            }
        }
        bool operator< (const Department & w) const {
            return (name < w.name);
        }
        std::pair <std::string, unsigned> find_wrk (const std::string &wrk) const {
            auto search = workers.find(&wrk);
            if (search != workers.end()) {
                return (*search).second->GetData();
            } else {
                return std::make_pair(std::string(), unsigned());
            }
        }
        std::string repr () const {
            std::string s;
            s += "    ====" + name + "\n";
            unsigned avg = (q == 0) ? 0 : sum/q;
            s += "    ====Average salary:" + std::to_string(avg) + "\n";
            for (auto &wrk : workers) {
                s += "        ----Name: " + *(wrk.first) + "\n" + wrk.second->repr();
            }
            return s;
        }
        std::vector <std::string> xml_repr () const {
            std::string indent = "   ";
            std::string indent2 = "      ";
            std::string indent3 = "         ";
            std::string indent4 = "            ";
            std::vector <std::string> lines;
            lines.push_back(indent + "<department name=\"" + name + "\">");
            lines.push_back(indent2 + "<employments>");
            for (auto &wrk : workers) {
                lines.push_back(indent3 + "<employment>");
                auto x = string_manip::split(*wrk.first, " ");
                lines.push_back(indent4 + NAMEDVAL("surname", x[0]));
                lines.push_back(indent4 + NAMEDVAL("name", x[1]));
                lines.push_back(indent4 + NAMEDVAL("middleName", x[2]));
                for (auto &lns : (wrk.second)->xml_repr()) {
                    lines.push_back(lns);
                }
                lines.push_back(indent3 + "</employment>");
            }
            lines.push_back(indent2 + "</employments>");
            lines.push_back(indent + "</department>");
            return lines;
        }
    };

    struct DeptPtrComp 
    {
        bool operator() (const Department *lhs, const Department *rhs) const {
            return ((*lhs) < (*rhs));
        }
    };

    class Company 
    {
        std::set<Department *, DeptPtrComp> dpts;
    public:
        void set_comp (const Company &c) {
            dpts = c.dpts;
        }
        void show () const {
            std::string s;
            s = ">>>>Company:\n";
            for (auto &dpt : dpts) {
                s += dpt->repr();
            }
            std::cout << s;
        }
        void newdp (const std::string &dp, Department *ptr) {
            auto search = dpts.find(ptr);
            if (search == dpts.end()) {
                dpts.insert(ptr);
            } else {
                throw std::logic_error("There is a dept with the same name "+dp);
            }
        }
        void deldp (const std::string &dp) {
            auto search = dpts.find(new Department(dp));
            if (search != dpts.end()) {
                dpts.erase(search);
            } else {
                throw std::logic_error("There was no dept with name "+dp);
            }
        }
        void editwrk_elightability (
                    const std::string &dpt,
                    const std::string &wrk,
                    const std::string &newname) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                (*search)->editwrk_elightability(wrk, newname);
            } else {
                throw std::logic_error("There was no dept with name "+dpt);
            }
        }
        void editwrk (const std::string &dpt,
                    const std::string &wrk,
                    const std::string &nwrk,
                    const std::string &nttl,
                    const unsigned &nsal) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                (*search)->editwrk(wrk, nwrk, nttl, nsal);
            } else {
                throw std::logic_error("There was no dept with name "+dpt);
            }
        }
        void newwrk (const std::string &dpt, 
                    const std::string &wrk, 
                    const std::string &title, 
                    const unsigned &salary) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                (*search)->newwrk(wrk, title, salary);
            } else {
                throw std::logic_error("There was no dept with name "+dpt);
            }
        }
        void delwrk (const std::string &dpt, const std::string &wrk) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                (*search)->delwrk(wrk);
            } else {
                throw std::logic_error("There was no dept with name "+dpt);
            }
        }
        std::pair <std::string, unsigned> find_wrk (const std::string &dpt, const std::string &wrk) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                return (*search)->find_wrk(wrk);
            } else {
                return std::make_pair(std::string(), unsigned());
            }
        }
        Department* find_dpt (const std::string &name) {
            auto search = dpts.find(new Department(name));
            if (search != dpts.end()) {
                return (*search);
            } else {
                return nullptr;
            }
        }
        void chg_dpt(const std::string &ex, const std::string &n) {
            Department *x = find_dpt(ex);
            if (x != nullptr) {
                x->newname(n);
            } else {
                throw std::logic_error("No department called " + ex + " found.");
            }
        }
        std::vector <std::string> xml_repr () const {
            std::vector <std::string> lines;
            lines.push_back("<departments>");
            for (auto &dp : dpts) {
                for (auto &lns : dp->xml_repr()) {
                    lines.push_back(lns);
                }
            }
            lines.push_back("</departments>");
            return lines;
        }
    };
}

class XML_reader {
    Company::Company comp;
public:
    XML_reader (const std::string &filename) {
        std::ifstream in("xml_data.xml");
        std::string line;
        std::string lastdpt, name, ttl;
        unsigned salary;

        // Читаем файл построчно
        while(std::getline(in, line)) {
            if (line[2] == '?') {
                continue;
            }
            if (line == "<departments>") {
                continue;
            }
            std::string s_trimmed((string_manip::trim_copy(line)));
            if (s_trimmed.substr(1, 10) == "department") {
                lastdpt = s_trimmed.substr(18, s_trimmed.length() - 20);
                comp.newdp(lastdpt, new Company::Department(lastdpt));
            } else if (s_trimmed == "<employments>") {
                continue;
            } else if (s_trimmed == "<employment>") {
                name.erase();
                ttl.erase();
                salary = 0;
            } else if (s_trimmed.substr(1, 7) == "surname") {
                size_t begin = s_trimmed.find('>', 0) + 1;
                size_t end = s_trimmed.find('<', 1);
                name = s_trimmed.substr(begin, end - begin);
            } else if (s_trimmed.substr(1, 4) == "name") {
                size_t begin = s_trimmed.find('>', 0) + 1;
                size_t end = s_trimmed.find('<', 1);
                name += " " + s_trimmed.substr(begin, end - begin);
            } else if (s_trimmed.substr(1, 10) == "middleName") {
                size_t begin = s_trimmed.find('>', 0) + 1;
                size_t end = s_trimmed.find('<', 1);
                name += " " + s_trimmed.substr(begin, end - begin);
            } else if (s_trimmed.substr(1, 8) == "function") {
                size_t begin = s_trimmed.find('>', 0) + 1;
                size_t end = s_trimmed.find('<', 1);
                ttl = s_trimmed.substr(begin, end - begin);
            } else if (s_trimmed.substr(1, 6) == "salary") {
                size_t begin = s_trimmed.find('>', 0) + 1;
                size_t end = s_trimmed.find('<', 1);
                salary = std::stoul(s_trimmed.substr(begin, end - begin));
            } else if (s_trimmed == "</employment>") {
                comp.newwrk(lastdpt, name, ttl, salary);
            } else {
                continue;
            }
        }
    }
    Company::Company get_comp () const {
        return comp;
    }
    Company::Company& get_comp_lnk () {
        return comp;
    }
    void show () const {
        comp.show();
    }
};

class XML_writer {
public:
    XML_writer (const std::string &filename, const Company::Company &c) {
        std::ofstream testFile;
        testFile.open(filename, std::ios::out | std::ios::binary);
        testFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        auto lines = c.xml_repr();
        for (auto &ln : lines) {
            testFile << ln << std::endl;
        }
        testFile.close();
    }
};

namespace Commands { 
    class Command 
    {
    protected:    
        Company::Company *comp;
    public:
        virtual ~Command() {}
        virtual void Execute() = 0;
        virtual void unExecute() = 0;

        void setCompany (Company::Company *c) {
            comp = c;
        }
    };

    class AddDeptCommand: public Command
    {
        std::string name;
    public:
        AddDeptCommand (const std::string &n): name(n) {}
        void Execute() {
            comp->newdp(name, new Company::Department(name));
        }
        void unExecute() {
            comp->deldp(name);
        }
    };

    class AddWorkerCommand: public Command
    {
        std::string dpt, wrk, ttl;
        unsigned sal;
    public:
        AddWorkerCommand (const std::string &d, 
                        const std::string &w,
                        const std::string &t,
                        const unsigned &s): dpt(d), wrk(w), ttl(t), sal(s) {}
        void Execute() {
            comp->newwrk(dpt, wrk, ttl, sal);
        }
        void unExecute() {
            comp->delwrk(dpt, wrk);
        }
    };

    class DeleteDeptCommand: public Command
    {
        std::string name;
        Company::Department cpy;
    public:
        DeleteDeptCommand (const std::string &n): name(n) {}
        void Execute() {
            Company::Department *x = (comp->find_dpt(name));
            if (x != nullptr) {
                cpy = *x;
            } else {
                throw std::logic_error("No department called " + name + " found.");
            }
            comp->deldp(name);
        }
        void unExecute() {
            comp->newdp(name, &cpy);
        }
    };

    class DeleteWorkerCommand: public Command
    {
        std::string dpt, wrk;
        std::pair<std::string, unsigned> info;
    public:
        DeleteWorkerCommand (const std::string &d, 
                            const std::string &w): dpt(d), wrk(w) {}
        void Execute() {              
            auto pr = comp->find_wrk(dpt, wrk);
            if (!pr.first.empty() && pr.second) {
                info = pr;
            } else {
                throw std::logic_error("Haven't find department or worker with such name.");
            }
            comp->delwrk(dpt, wrk);
        }
        void unExecute() {
            comp->newwrk(dpt, wrk, info.first, info.second);
        }
    };

    class ChangeNameDeptCommand: public Command
    {
        std::string dpt, newname;
    public:
        ChangeNameDeptCommand (const std::string &d, 
                            const std::string &n): dpt(d), newname(n) {}
        void Execute() {
            comp->chg_dpt(dpt, newname);
        }
        void unExecute() {
            comp->chg_dpt(newname, dpt);
        }
    };

    class EditDeptCommand: public Command
    {
        std::string dpt, wrk, newname;
        std::pair<std::string, unsigned> info, newinfo;
    public:
        EditDeptCommand (const std::string &d, 
                        const std::string &w,
                        const std::string &n,
                        const std::string &f,
                        const unsigned &s): dpt(d), wrk(w), newname(n), newinfo(f,s) {}
        void Execute() {
            comp->editwrk_elightability(dpt, wrk, newname);
            auto pr = comp->find_wrk(dpt, wrk);
            if (!pr.first.empty() && pr.second) {
                info = pr;
            } else {
                throw std::logic_error("Haven't find department or worker with such name.");
            }
            comp->editwrk(dpt, wrk, newname, newinfo.first, newinfo.second);
        }
        void unExecute() {
            comp->editwrk(dpt, newname, wrk, info.first, info.second);
        }
    };

    class SaveToFileCommand: public Command
    {
        std::string fileName;
        Company::Company prevstate;
        bool existed;
    public:
        SaveToFileCommand (const std::string &fname): fileName(fname) {
            if (FileExists(fname)) {
                prevstate = XML_reader(fileName).get_comp();
                existed = true;
            } else {
                existed = false;
            }
        }
        void Execute() {
            XML_writer(fileName, *comp);
        }
        void unExecute() {
            if (existed) {
                XML_writer(fileName, prevstate);
            } else {
                if (remove(fileName.c_str( ))) {
                    throw std::runtime_error("Remove operation failed");
                }
            }
        }
    };

    class LoadFromFileCommand: public Command
    {
        std::string fileName;
        Company::Company prevstate;
    public:
        LoadFromFileCommand (const std::string &fname): fileName(fname) {
            if (!FileExists(fname)) {
                throw std::runtime_error("File doesn't exist");
            }
        }
        void Execute() {
            prevstate = *comp;
            XML_reader x(fileName);
            comp->set_comp(x.get_comp());
            comp->show();
        }
        void unExecute() {
            comp->set_comp(prevstate);
        }
    };
}

class Invoker 
{
    std::vector <Commands::Command *> DoneCommands;
    std::vector <Commands::Command *> CanceledCommands;
    Company::Company com;
    Commands::Command *command;
public:
    void addwrk (const std::string &d, 
                 const std::string &w,
                 const std::string &t,
                 const unsigned &s ) {
        CanceledCommands.clear();
        command = new Commands::AddWorkerCommand(d, w, t, s);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void adddpt (const std::string &d) {
        CanceledCommands.clear();
        command = new Commands::AddDeptCommand(d);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void chgnmdpt (const std::string &name, const std::string &newname) {
        CanceledCommands.clear();
        command = new Commands::ChangeNameDeptCommand(name, newname);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void delwrk (const std::string &d, const std::string &w) {
        CanceledCommands.clear();
        command = new Commands::DeleteWorkerCommand(d, w);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void deldpt (const std::string &d) {
        CanceledCommands.clear();
        command = new Commands::DeleteDeptCommand(d);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void editdpt (const std::string &d, 
                  const std::string &w, 
                  const std::string &n,
                  const std::string &f,
                  const unsigned & s) {
        CanceledCommands.clear();
        command = new Commands::EditDeptCommand(d, w, n, f, s);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void savetofile (const std::string &fname) {
        CanceledCommands.clear();
        command = new Commands::SaveToFileCommand(fname);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void loadfromfile (const std::string &fname) {
        CanceledCommands.clear();
        command = new Commands::LoadFromFileCommand(fname);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void Undo() {
        if (DoneCommands.size() == 0) {
            std::cerr << "There's nothing to undo" << std::endl;
        } else {
            command = DoneCommands.back();
            DoneCommands.pop_back();
            command->unExecute();
            CanceledCommands.push_back(command);
        }
    }
    void Redo() {
        if (CanceledCommands.size() == 0) {
            std::cerr << "There's nothing to redo" << std::endl;
        } else {
            command = CanceledCommands.back();
            CanceledCommands.pop_back();
            command->Execute();
            DoneCommands.push_back(command);
        }
    }
    void Show() {
        com.show();
    }
};

void print_commands () {
    std::cout << "Commands:" << std::endl;
    std::cout << "0) add_dpt - adds department" << std::endl;
    std::cout << "1) add_wrk - adds worker" << std::endl;
    std::cout << "2) undo - performs undo, if possible" << std::endl;
    std::cout << "3) redo - performs redo, if possible" << std::endl;
    std::cout << "4) chgname_dpt - changes dpt name" << std::endl;
    std::cout << "5) edit_dpt - edits info about worker" << std::endl;
    std::cout << "6) del_dpt - deletes department" << std::endl;
    std::cout << "7) del_wrk - deletes worker from department" << std::endl;
    std::cout << "8) exit - exits the programm" << std::endl;
    std::cout << "9) commands - prints this guide" << std::endl;
    std::cout << "a) load - loads company from file" << std::endl;
    std::cout << "b) save - saves company to file" << std::endl;
}

int main()
{
    try {
        SetConsoleCP(1251);
	    SetConsoleOutputCP(65001);
        //system ("chcp 65001");
    } catch (...) {
        std::cout << "Console input/output in Russian may be uninformative" << std::endl;
    }
    std::string com;
    Invoker inv;
    print_commands();
    std::getline(std::cin, com);
    while (com != EXIT) {
        try {
            if (!(SYSTEM(com))) {
                if (com == ADD_WORKER) {
                    std::string dpt, wrk, ttl;
                    unsigned sal;
                    std::cout << "Department: ";
                    std::getline(std::cin, dpt);
                    std::cout << "Full name: ";
                    std::getline(std::cin, wrk);
                    std::cout << "Function: ";
                    std::getline(std::cin, ttl);
                    std::cout << "Salary: ";
                    std::string val;
                    std::getline(std::cin, val);
                    sal = std::stoul(val);
                    inv.addwrk(dpt, wrk, ttl, sal);
                } else if (com == ADD_DEPT) {
                    std::string dpt;
                    std::cout << "Department: ";
                    std::getline(std::cin, dpt);
                    inv.adddpt(dpt);
                } else if (com == CHANGE_DEPT_NAME) {
                    std::string dpt, ndpt;
                    std::cout << "Department: ";
                    std::getline(std::cin, dpt);
                    std::cout << "Department new name: ";
                    std::getline(std::cin, ndpt);
                    inv.chgnmdpt(dpt, ndpt);
                } else if (com == DELETE_WORKER) {
                    std::string dpt, wrk;
                    std::cout << "Department: ";
                    std::getline(std::cin, dpt);
                    std::cout << "Full name: ";
                    std::getline(std::cin, wrk);
                    inv.delwrk(dpt, wrk);
                } else if (com == EDIT_DEPT_CONTAINS) {
                    std::string dpt, wrk, newname;
                    std::cout << "Department: ";
                    std::getline(std::cin, dpt);
                    std::cout << "Full name: ";
                    std::getline(std::cin, wrk);
                    std::cout << "New full name: ";
                    std::getline(std::cin, newname);
                    std::string ttl;
                    unsigned sal;
                    std::cout << "New function: ";
                    std::getline(std::cin, ttl);
                    std::cout << "New salary: ";
                    std::string val;
                    std::getline(std::cin, val);
                    sal = std::stoul(val);
                    inv.editdpt(dpt, wrk, newname, ttl, sal);
                } else if (com == DELETE_DEPT) {
                    std::string dpt, wrk;
                    std::cout << "Department: ";
                    std::getline(std::cin, dpt);
                    inv.deldpt(dpt);
                } else if (com == LOAD) {
                    std::string fname;
                    std::cout << "File name: ";
                    std::getline(std::cin, fname);
                    inv.loadfromfile(fname);
                } else if (com == SAVE) {
                    std::string fname;
                    std::cout << "File name: ";
                    std::getline(std::cin, fname);
                    inv.savetofile(fname);
                } else {
                    std::cerr << "Unknown command" << std::endl;
                }
            } else {
                if (com == UNDO) {
                    inv.Undo();
                } else if (com == REDO) {
                    inv.Redo();
                } else if (com == HELP) {
                    print_commands();
                } else {
                    inv.Show();
                }
            }
            std::getline(std::cin, com);
        } catch (std::out_of_range outr) {
            std::cout << "Went out of range in " << outr.what() << std::endl;
        } catch (std::invalid_argument ia) {
            std::cout << "Got an invalid argument in " << ia.what() << std::endl;
        } catch (std::logic_error lg) {
            std::cout << lg.what() << std::endl;
        } catch (std::runtime_error rt) {
            std::cout << rt.what() << std::endl;
        } catch (...) {
            std::cout << "Unknown error occured";
        }
    }
}
