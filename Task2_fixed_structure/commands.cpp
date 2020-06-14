#include "commands.h"

namespace Commands {
    void Command::setCompany (Company::Company *ptr) {
        comp = ptr;
    }

    AddDeptCommand::AddDeptCommand (const std::string &n): name(n) {}
    void AddDeptCommand::Execute() {
        comp->add_department(name);
    }
    void AddDeptCommand::unExecute() {
        comp->delete_department(name);
    }

    AddWorkerCommand::AddWorkerCommand (const std::string &d,
                      const std::string &w,
                      const std::string &t,
                      const unsigned &s): dpt(d), wrk(w), ttl(t), sal(s) {}
    void AddWorkerCommand::Execute() {
        comp->add_worker(dpt, wrk, ttl, sal);
    }
    void AddWorkerCommand::unExecute() {
        comp->delete_worker(dpt, wrk);
    }

    DeleteDeptCommand::DeleteDeptCommand (const std::string &n): name(n) {}
    void DeleteDeptCommand::Execute() {
        auto x = comp->find_dpt(name);
        if (!x.empty()) {
            cpy = x;
        } else {
            throw std::logic_error("No department called " + name + " found.");
        }
        comp->delete_department(name);
    }
    void DeleteDeptCommand::unExecute() {
        comp->init_department(name, cpy);
    }

    DeleteWorkerCommand::DeleteWorkerCommand (const std::string &d,
                         const std::string &w): dpt(d), wrk(w) {}
    void DeleteWorkerCommand::Execute() {
        auto pr = comp->find_wrk(dpt, wrk);
        if (!pr.first.empty() && pr.second) {
            info = pr;
        } else {
            throw std::logic_error("Haven't find department or worker with such name.");
        }
        comp->delete_worker(dpt, wrk);
    }
    void DeleteWorkerCommand::unExecute() {
        comp->add_worker(dpt, wrk, info.first, info.second);
    }

    ChangeNameDeptCommand::ChangeNameDeptCommand (const std::string &d,
                           const std::string &n): dpt(d), newname(n) {}
    void ChangeNameDeptCommand::Execute() {
        comp->change_department_name(dpt, newname);
    }
    void ChangeNameDeptCommand::unExecute() {
        comp->change_department_name(newname, dpt);
    }

    EditDeptCommand::EditDeptCommand (const std::string &d,
                     const std::string &w,
                     const std::string &n,
                     const std::string &f,
                     const unsigned &s): dpt(d), wrk(w), newname(n), newinfo(f,s) {}
    void EditDeptCommand::Execute() {
        comp->edit_worker_check(dpt, wrk, newname);
        auto pr = comp->find_wrk(dpt, wrk);
        if (!pr.first.empty() && pr.second) {
            info = pr;
        } else {
            throw std::logic_error("Haven't find department or worker with such name.");
        }
        comp->edit_worker(dpt, wrk, newname, newinfo.first, newinfo.second);
    }
    void EditDeptCommand::unExecute() {
        comp->edit_worker(dpt, newname, wrk, info.first, info.second);
    }

    SaveToFileCommand::SaveToFileCommand (const std::string &fname): fileName(fname) {
        if (FileExists(fname)) {
            prevstate = XML_reader(fileName).get_comp();
            existed = true;
        } else {
            existed = false;
        }
    }
    void SaveToFileCommand::Execute() {
        XML_writer(fileName, *comp);
    }
    void SaveToFileCommand::unExecute() {
        if (existed) {
            XML_writer(fileName, prevstate);
        } else {
            if (remove(fileName.c_str( ))) {
                throw std::runtime_error("Remove operation failed");
            }
        }
    }

    LoadFromFileCommand::LoadFromFileCommand (const std::string &fname): fileName(fname) {
        if (!FileExists(fname)) {
            throw std::runtime_error("File doesn't exist");
        }
    }
    void LoadFromFileCommand::Execute() {
        prevstate = *comp;
        XML_reader x(fileName);
        comp->set_comp(x.get_comp());
        comp->show();
    }
    void LoadFromFileCommand::unExecute() {
        comp->set_comp(prevstate);
    }
}
