#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <clocale>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <cwchar>
#include <codecvt>

#include <set>
#include <map>

/* Boilerplate feature-test macros: */
#if _WIN32 || _WIN64
#  include <sdkddkver.h>
#else
#  define _XOPEN_SOURCE     700
#  define _POSIX_C_SOURCE   200809L
#endif

#ifndef MS_STDLIB_BUGS // Allow overriding the autodetection.
/* The Microsoft C and C++ runtime libraries that ship with Visual Studio, as
 * of 2017, have a bug that neither stdio, iostreams or wide iostreams can
 * handle Unicode input or output.  Windows needs some non-standard magic to
 * work around that.  This includes programs compiled with MinGW and Clang
 * for the win32 and win64 targets.
 *
 * NOTE TO USERS OF TDM-GCC: This code is known to break on tdm-gcc 4.9.2. As
 * a workaround, "-D MS_STDLIB_BUGS=0" will at least get it to compile, but
 * Unicode output will still not work.
 */
#  if ( _MSC_VER || __MINGW32__ || __MSVCRT__ )
    /* This code is being compiled either on MS Visual C++, or MinGW, or
     * clang++ in compatibility mode for either, or is being linked to the
     * msvcrt (Microsoft Visual C RunTime) library.
     */
#    define MS_STDLIB_BUGS 1
#  else
#    define MS_STDLIB_BUGS 0
#  endif
#endif

#if MS_STDLIB_BUGS
#  include <io.h>
#  include <fcntl.h>
#endif

void init_locale(void)
// Does magic so that wcout can work.
{
#if MS_STDLIB_BUGS
  // Windows needs a little non-standard magic.
  constexpr char cp_utf16le[] = ".1200";
  setlocale( LC_ALL, cp_utf16le );
  _setmode( _fileno(stdout), _O_WTEXT );
  _setmode( _fileno(stdin), _O_WTEXT );
#else
  // The correct locale name may vary by OS, e.g., "en_US.utf8".
  constexpr char locale_name[] = "";
  setlocale( LC_ALL, locale_name );
  std::locale::global(std::locale(locale_name));
  wcout.imbue(std::locale());
  wcin.imbue(std::locale());
#endif
}

#ifdef _WIN32
   #include <io.h> 
   #define access    _access_s
#else
   #include <unistd.h>
#endif

#define SYSTEM(comp) comp == L"show" || comp == L"undo" || comp == L"redo" || comp == L"commands"
#define ADD_DEPT L"add_dpt"
#define ADD_WORKER L"add_wrk"
#define UNDO L"undo"
#define REDO L"redo"
#define CHANGE_DEPT_NAME L"chgname_dpt"
#define EDIT_DEPT_CONTAINS L"edit_dpt"
#define DELETE_DEPT L"del_dpt"
#define DELETE_WORKER L"del_wrk"
#define EXIT L"exit"
#define HELP L"commands"
#define LOAD L"load"
#define SAVE L"save"

#define XOPEN(str) L"<" + str + L">"
#define XCLOS(str) L"</" + str + L">"
#define NAMEDVAL(name, val) XOPEN(name) + val + XCLOS(name)

namespace string_manip {
    // trim from start (in place)
    static inline void ltrim(std::wstring &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::wstring &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::wstring &s) {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    static inline std::wstring ltrim_copy(std::wstring s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    static inline std::wstring rtrim_copy(std::wstring s) {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    static inline std::wstring trim_copy(std::wstring s) {
        trim(s);
        return s;
    }

    //split string
    static inline std::vector<std::wstring> split (const std::wstring &s, const std::wstring &delim) {
        std::vector<std::wstring> results;
        auto start = 0U;
        auto end = s.find(delim);
        while (end != std::wstring::npos)
        {
            results.push_back(s.substr(start, end - start));
            start = end + delim.length();
            end = s.find(delim, start);
        }
        results.push_back(s.substr(start, end));
        return results;
    }

    std::wstring StrToWide (const std::string &narrow_utf8_source_string) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        //std::string narrow = converter.to_bytes(wide_utf16_source_string);
        std::wstring wide = converter.from_bytes(narrow_utf8_source_string);
        return wide;
    }

    std::string WideToStr (const std::wstring &wide_utf16_source_string) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::string narrow = converter.to_bytes(wide_utf16_source_string);
        return narrow;
    }
}

bool FileExists( const std::wstring &Filename ) {
    return access(string_manip::WideToStr(Filename).c_str(), 0) == 0;
}

namespace Company {
    class WorkerInfo 
    {
        std::wstring title;
        unsigned salary;
    public:
        WorkerInfo () {};
        WorkerInfo (const std::wstring &t, const unsigned &s): title(t), salary(s) {}
        std::wstring repr () const {
            return L"        ---->>Function: " + title + L"\n" + 
                    L"        ---->>Salary: " + std::to_wstring(salary) + L"\n";
        }
        std::pair <std::wstring, unsigned> GetData () const {
            return std::pair<std::wstring, unsigned> (title, salary);
        }
        std::vector <std::wstring> xml_repr () const {
            std::wstring indent4 = L"            ";
            std::vector <std::wstring> lines;
            lines.push_back(indent4 + NAMEDVAL(L"function", title));
            lines.push_back(indent4 + NAMEDVAL(L"salary", std::to_wstring(salary)));
            return lines;
        }
    };

    struct StrPtrComp 
    {
        bool operator() (const std::wstring *lhs, const std::wstring *rhs) const {
            return ((*lhs) < (*rhs));
        }
    };

    class Department 
    {
        std::wstring name;
        std::map<const std::wstring*, WorkerInfo *, StrPtrComp> workers;
    public:
        Department (const std::wstring &n): name(n) {}
        Department (const Department &rhs): name(rhs.name), workers(rhs.workers) {}
        Department () {}
        void newwrk (const std::wstring &wrk, 
                    const std::wstring &title, 
                    const unsigned &salary) {
            std::wstring *x = new std::wstring (wrk);
            auto search = workers.find(x);
            if (search == workers.end()) {
                workers[x] = new WorkerInfo(title, salary);
            } else {
                throw std::logic_error("There is already a worker with such name");
            }
        } 
        void editwrk_elightability (const std::wstring &wrk, const std::wstring &newname) {
            auto search = workers.find(&wrk);
            if (search != workers.end()) {
                if (wrk != newname) {
                    auto newsearch = workers.find(&newname);
                    if (newsearch != workers.end()) {
                        throw std::logic_error("There is no workers with such name");
                    }
                }
            } else {
                throw std::logic_error("There is no workers with such name");
            }
        } 
        void editwrk (const std::wstring &wrk,
                    const std::wstring &nwrk,
                    const std::wstring &nttl,
                    const unsigned &nsal) {
            auto search = workers.find(&wrk);
            if (wrk != nwrk) {
                workers[&nwrk] = new WorkerInfo(nttl, nsal);
                workers.erase(search);
            } else {
                workers[&wrk] = new WorkerInfo(nttl, nsal);
            }
        }
        void newname (const std::wstring &x) {
            name = x;
        }
        void delwrk (const std::wstring &wrk) {
            auto search = workers.find(&wrk);
            if (search != workers.end()) {
                workers.erase(search);
            } else {
                throw std::logic_error("There is no worker with such name");
            }
        }
        bool operator< (const Department & w) const {
            return (name < w.name);
        }
        std::pair <std::wstring, unsigned> find_wrk (const std::wstring &wrk) const {
            auto search = workers.find(&wrk);
            if (search != workers.end()) {
                return (*search).second->GetData();
            } else {
                return std::make_pair(std::wstring(), unsigned());
            }
        }
        std::wstring repr () const {
            std::wstring s;
            s = L"    ====" + name + L"\n";
            for (auto &wrk : workers) {
                s += L"        ----Name: " + *(wrk.first) + L"\n" + wrk.second->repr();
            }
            return s;
        }
        std::vector <std::wstring> xml_repr () const {
            std::wstring indent = L"   ";
            std::wstring indent2 = L"      ";
            std::wstring indent3 = L"         ";
            std::wstring indent4 = L"            ";
            std::vector <std::wstring> lines;
            lines.push_back(indent + L"<department name=\"" + name + L"\">");
            lines.push_back(indent2 + L"<employments>");
            for (auto &wrk : workers) {
                lines.push_back(indent3 + L"<employment>");
                auto x = string_manip::split(*wrk.first, L" ");
                lines.push_back(indent4 + NAMEDVAL(L"surname", x[0]));
                lines.push_back(indent4 + NAMEDVAL(L"name", x[1]));
                lines.push_back(indent4 + NAMEDVAL(L"middleName", x[2]));
                for (auto &lns : (wrk.second)->xml_repr()) {
                    lines.push_back(lns);
                }
                lines.push_back(indent3 + L"</employment>");
            }
            lines.push_back(indent2 + L"</employments>");
            lines.push_back(indent + L"</department>");
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
            std::wstring s;
            s = L">>>>Company:\n";
            for (auto &dpt : dpts) {
                s += dpt->repr();
            }
            std::wcout << s;
        }
        void newdp (const std::wstring &dp, Department *ptr) {
            auto search = dpts.find(ptr);
            if (search == dpts.end()) {
                dpts.insert(ptr);
            } else {
                throw std::logic_error("There is a dept with the same name");
            }
        }
        void deldp (const std::wstring &dp) {
            auto search = dpts.find(new Department(dp));
            if (search != dpts.end()) {
                dpts.erase(search);
            } else {
                throw std::logic_error("There was no dept with name");
            }
        }
        void editwrk_elightability (
                    const std::wstring &dpt,
                    const std::wstring &wrk,
                    const std::wstring &newname) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                (*search)->editwrk_elightability(wrk, newname);
            } else {
                throw std::logic_error("There was no dept with such name");
            }
        }
        void editwrk (const std::wstring &dpt,
                    const std::wstring &wrk,
                    const std::wstring &nwrk,
                    const std::wstring &nttl,
                    const unsigned &nsal) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                (*search)->editwrk(wrk, nwrk, nttl, nsal);
            } else {
                throw std::logic_error("There was no dept with such name");
            }
        }
        void newwrk (const std::wstring &dpt, 
                    const std::wstring &wrk, 
                    const std::wstring &title, 
                    const unsigned &salary) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                (*search)->newwrk(wrk, title, salary);
            } else {
                throw std::logic_error("There was no dept with such name");
            }
        }
        void delwrk (const std::wstring &dpt, const std::wstring &wrk) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                (*search)->delwrk(wrk);
            } else {
                throw std::logic_error("There was no dept with such name");
            }
        }
        std::pair <std::wstring, unsigned> find_wrk (const std::wstring &dpt, const std::wstring &wrk) {
            auto search = dpts.find(new Department(dpt));
            if (search != dpts.end()) {
                return (*search)->find_wrk(wrk);
            } else {
                return std::make_pair(std::wstring(), unsigned());
            }
        }
        Department* find_dpt (const std::wstring &name) {
            auto search = dpts.find(new Department(name));
            if (search != dpts.end()) {
                return (*search);
            } else {
                return nullptr;
            }
        }
        void chg_dpt(const std::wstring &ex, const std::wstring &n) {
            Department *x = find_dpt(ex);
            if (x != nullptr) {
                x->newname(n);
            } else {
                throw std::logic_error("No department called this way found.");
            }
        }
        std::vector <std::wstring> xml_repr () const {
            std::vector <std::wstring> lines;
            lines.push_back(L"<departments>");
            for (auto &dp : dpts) {
                for (auto &lns : dp->xml_repr()) {
                    lines.push_back(lns);
                }
            }
            lines.push_back(L"</departments>");
            return lines;
        }
    };
}

class XML_reader {
    Company::Company comp;
public:
    XML_reader (const std::wstring &filename) {
        std::wifstream in(string_manip::WideToStr(filename));
        std::wstring line;
        std::wstring lastdpt, name, ttl;
        unsigned salary;

        // Читаем файл построчно
        while(std::getline(in, line)) {
            if (line[2] == L'?') {
                continue;
            }
            if (line == L"<departments>") {
                continue;
            }
            std::wstring s_trimmed((string_manip::trim_copy(line)));
            if (s_trimmed.substr(1, 10) == L"department") {
                lastdpt = s_trimmed.substr(18, s_trimmed.length() - 20);
                comp.newdp(lastdpt, new Company::Department(lastdpt));
            } else if (s_trimmed == L"<employments>") {
                continue;
            } else if (s_trimmed == L"<employment>") {
                name.erase();
                ttl.erase();
                salary = 0;
            } else if (s_trimmed.substr(1, 7) == L"surname") {
                size_t begin = s_trimmed.find(L'>', 0) + 1;
                size_t end = s_trimmed.find(L'<', 1);
                name = s_trimmed.substr(begin, end - begin);
            } else if (s_trimmed.substr(1, 4) == L"name") {
                size_t begin = s_trimmed.find(L'>', 0) + 1;
                size_t end = s_trimmed.find(L'<', 1);
                name += L" " + s_trimmed.substr(begin, end - begin);
            } else if (s_trimmed.substr(1, 10) == L"middleName") {
                size_t begin = s_trimmed.find(L'>', 0) + 1;
                size_t end = s_trimmed.find(L'<', 1);
                name += L" " + s_trimmed.substr(begin, end - begin);
            } else if (s_trimmed.substr(1, 8) == L"function") {
                size_t begin = s_trimmed.find(L'>', 0) + 1;
                size_t end = s_trimmed.find(L'<', 1);
                ttl = s_trimmed.substr(begin, end - begin);
            } else if (s_trimmed.substr(1, 6) == L"salary") {
                size_t begin = s_trimmed.find(L'>', 0) + 1;
                size_t end = s_trimmed.find(L'<', 1);
                salary = std::stoul(s_trimmed.substr(begin, end - begin));
            } else if (s_trimmed == L"</employment>") {
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
    XML_writer (const std::wstring &filename, const Company::Company &c) {
        std::wofstream testFile;
        testFile.open(string_manip::WideToStr(filename), std::ios::out | std::ios::binary);
        std::wcout << L"Writing..." << std::endl;
        testFile << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
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
        std::wstring name;
    public:
        AddDeptCommand (const std::wstring &n): name(n) {}
        void Execute() {
            comp->newdp(name, new Company::Department(name));
        }
        void unExecute() {
            comp->deldp(name);
        }
    };

    class AddWorkerCommand: public Command
    {
        std::wstring dpt, wrk, ttl;
        unsigned sal;
    public:
        AddWorkerCommand (const std::wstring &d, 
                        const std::wstring &w,
                        const std::wstring &t,
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
        std::wstring name;
        Company::Department cpy;
    public:
        DeleteDeptCommand (const std::wstring &n): name(n) {}
        void Execute() {
            Company::Department *x = (comp->find_dpt(name));
            if (x != nullptr) {
                cpy = *x;
            } else {
                throw std::logic_error("No department called this way found.");
            }
            comp->deldp(name);
        }
        void unExecute() {
            comp->newdp(name, &cpy);
        }
    };

    class DeleteWorkerCommand: public Command
    {
        std::wstring dpt, wrk;
        std::pair<std::wstring, unsigned> info;
    public:
        DeleteWorkerCommand (const std::wstring &d, 
                            const std::wstring &w): dpt(d), wrk(w) {}
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
        std::wstring dpt, newname;
    public:
        ChangeNameDeptCommand (const std::wstring &d, 
                            const std::wstring &n): dpt(d), newname(n) {}
        void Execute() {
            comp->chg_dpt(dpt, newname);
        }
        void unExecute() {
            comp->chg_dpt(newname, dpt);
        }
    };

    class EditDeptCommand: public Command
    {
        std::wstring dpt, wrk, newname;
        std::pair<std::wstring, unsigned> info, newinfo;
    public:
        EditDeptCommand (const std::wstring &d, 
                        const std::wstring &w,
                        const std::wstring &n,
                        const std::wstring &f,
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
        std::wstring fileName;
        Company::Company prevstate;
        bool existed;
    public:
        SaveToFileCommand (const std::wstring &fname): fileName(fname) {
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
                if (remove(string_manip::WideToStr(fileName).c_str())) {
                    throw std::runtime_error("Remove operation failed");
                }
            }
        }
    };

    class LoadFromFileCommand: public Command
    {
        std::wstring fileName;
        Company::Company prevstate;
    public:
        LoadFromFileCommand (const std::wstring &fname): fileName(fname) {
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
    void addwrk (const std::wstring &d, 
                 const std::wstring &w,
                 const std::wstring &t,
                 const unsigned &s ) {
        CanceledCommands.clear();
        command = new Commands::AddWorkerCommand(d, w, t, s);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void adddpt (const std::wstring &d) {
        CanceledCommands.clear();
        command = new Commands::AddDeptCommand(d);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void chgnmdpt (const std::wstring &name, const std::wstring &newname) {
        CanceledCommands.clear();
        command = new Commands::ChangeNameDeptCommand(name, newname);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void delwrk (const std::wstring &d, const std::wstring &w) {
        CanceledCommands.clear();
        command = new Commands::DeleteWorkerCommand(d, w);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void deldpt (const std::wstring &d) {
        CanceledCommands.clear();
        command = new Commands::DeleteDeptCommand(d);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void editdpt (const std::wstring &d, 
                  const std::wstring &w, 
                  const std::wstring &n,
                  const std::wstring &f,
                  const unsigned & s) {
        CanceledCommands.clear();
        command = new Commands::EditDeptCommand(d, w, n, f, s);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void savetofile (const std::wstring &fname) {
        CanceledCommands.clear();
        command = new Commands::SaveToFileCommand(fname);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void loadfromfile (const std::wstring &fname) {
        CanceledCommands.clear();
        command = new Commands::LoadFromFileCommand(fname);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void Undo() {
        if (DoneCommands.size() == 0) {
            std::wcerr << L"There's nothing to undo" << std::endl;
        } else {
            command = DoneCommands.back();
            DoneCommands.pop_back();
            command->unExecute();
            CanceledCommands.push_back(command);
        }
    }
    void Redo() {
        if (CanceledCommands.size() == 0) {
            std::wcerr << L"There's nothing to redo" << std::endl;
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
    std::wcout << L"Commands:" << std::endl;
    std::wcout << L"0) add_dpt - adds department" << std::endl;
    std::wcout << L"1) add_wrk - adds worker" << std::endl;
    std::wcout << L"2) undo - performs undo, if possible" << std::endl;
    std::wcout << L"3) redo - performs redo, if possible" << std::endl;
    std::wcout << L"4) chgname_dpt - changes dpt name" << std::endl;
    std::wcout << L"5) edit_dpt - edits info about worker" << std::endl;
    std::wcout << L"6) del_dpt - deletes department" << std::endl;
    std::wcout << L"7) del_wrk - deletes worker from department" << std::endl;
    std::wcout << L"8) exit - exits the programm" << std::endl;
    std::wcout << L"9) commands - prints this guide" << std::endl;
    std::wcout << L"a) load - loads company from file" << std::endl;
    std::wcout << L"b) save - saves company to file" << std::endl;
}

int main()
{
    try {
        init_locale();
    } catch (...) {
        std::wcout << L"Console input/output in Russian may be uninformative" << std::endl;
    }
    std::wstring com;
    Invoker inv;
    print_commands();
    std::getline(std::wcin, com);
    while (com != EXIT) {
        try {
            if (!(SYSTEM(com))) {
                if (com == ADD_WORKER) {
                    std::wstring dpt, wrk, ttl;
                    unsigned sal;
                    std::wcout << L"Department: ";
                    std::getline(std::wcin, dpt);
                    std::wcout << L"Full name: ";
                    std::getline(std::wcin, wrk);
                    std::wcout << L"Function: ";
                    std::getline(std::wcin, ttl);
                    std::wcout << L"Salary: ";
                    std::wstring val;
                    std::getline(std::wcin, val);
                    sal = std::stoul(val);
                    inv.addwrk(dpt, wrk, ttl, sal);
                } else if (com == ADD_DEPT) {
                    std::wstring dpt;
                    std::wcout << L"Department: ";
                    std::getline(std::wcin, dpt);
                    std::wcout << dpt << std::endl;
                    inv.adddpt(dpt);
                } else if (com == CHANGE_DEPT_NAME) {
                    std::wstring dpt, ndpt;
                    std::wcout << L"Department: ";
                    std::getline(std::wcin, dpt);
                    std::wcout << L"Department new name: ";
                    std::getline(std::wcin, ndpt);
                    inv.chgnmdpt(dpt, ndpt);
                } else if (com == DELETE_WORKER) {
                    std::wstring dpt, wrk;
                    std::wcout << L"Department: ";
                    std::getline(std::wcin, dpt);
                    std::wcout << L"Full name: ";
                    std::getline(std::wcin, wrk);
                    inv.delwrk(dpt, wrk);
                } else if (com == EDIT_DEPT_CONTAINS) {
                    std::wstring dpt, wrk, newname;
                    std::wcout << L"Department: ";
                    std::getline(std::wcin, dpt);
                    std::wcout << L"Full name: ";
                    std::getline(std::wcin, wrk);
                    std::wcout << L"New full name: ";
                    std::getline(std::wcin, newname);
                    std::wstring ttl;
                    unsigned sal;
                    std::wcout << L"New function: ";
                    std::getline(std::wcin, ttl);
                    std::wcout << L"New salary: ";
                    std::wstring val;
                    std::getline(std::wcin, val);
                    sal = std::stoul(val);
                    inv.editdpt(dpt, wrk, newname, ttl, sal);
                } else if (com == DELETE_DEPT) {
                    std::wstring dpt, wrk;
                    std::wcout << L"Department: ";
                    std::getline(std::wcin, dpt);
                    inv.deldpt(dpt);
                } else if (com == LOAD) {
                    std::wstring fname;
                    std::wcout << L"File name: ";
                    std::getline(std::wcin, fname);
                    inv.loadfromfile(fname);
                } else if (com == SAVE) {
                    std::wstring fname;
                    std::wcout << L"File name: ";
                    std::getline(std::wcin, fname);
                    inv.savetofile(fname);
                } else {
                    std::wcerr << L"Unknown command" << std::endl;
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
            std::getline(std::wcin, com);
        } catch (std::out_of_range outr) {
            std::wcout << L"Went out of range in " << string_manip::StrToWide(outr.what()) << std::endl;
        } catch (std::invalid_argument ia) {
            std::wcout << L"Got an invalid argument in " << string_manip::StrToWide(ia.what()) << std::endl;
        } catch (std::logic_error lg) {
            std::wcout << string_manip::StrToWide(lg.what()) << std::endl;
        } catch (std::runtime_error rt) {
            std::wcout << string_manip::StrToWide(rt.what()) << std::endl;
        } catch (...) {
            std::wcout << L"Unknown error occured";
        }
    }
}