#include "invoker.h"

void Invoker::addwrk (const std::string &d,
             const std::string &w,
             const std::string &t,
             const unsigned &s ) {
    CanceledCommands.clear();
    command = new Commands::AddWorkerCommand(d, w, t, s);
    command->setCompany(&com);
    command->Execute();
    DoneCommands.push_back(command);
}
void Invoker::adddpt (const std::string &d) {
    CanceledCommands.clear();
    command = new Commands::AddDeptCommand(d);
    command->setCompany(&com);
    command->Execute();
    DoneCommands.push_back(command);
}
void Invoker::chgnmdpt (const std::string &name, const std::string &newname) {
    CanceledCommands.clear();
    command = new Commands::ChangeNameDeptCommand(name, newname);
    command->setCompany(&com);
    command->Execute();
    DoneCommands.push_back(command);
}
void Invoker::delwrk (const std::string &d, const std::string &w) {
    CanceledCommands.clear();
    command = new Commands::DeleteWorkerCommand(d, w);
    command->setCompany(&com);
    command->Execute();
    DoneCommands.push_back(command);
}
void Invoker::deldpt (const std::string &d) {
    CanceledCommands.clear();
    command = new Commands::DeleteDeptCommand(d);
    command->setCompany(&com);
    command->Execute();
    DoneCommands.push_back(command);
}
void Invoker::editdpt (const std::string &d,
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
void Invoker::savetofile (const std::string &fname) {
    CanceledCommands.clear();
    command = new Commands::SaveToFileCommand(fname);
    command->setCompany(&com);
    command->Execute();
    DoneCommands.push_back(command);
}
void Invoker::loadfromfile (const std::string &fname) {
    CanceledCommands.clear();
    command = new Commands::LoadFromFileCommand(fname);
    command->setCompany(&com);
    command->Execute();
    DoneCommands.push_back(command);
}
void Invoker::Undo() {
    if (DoneCommands.size() == 0) {
        std::cerr << "There's nothing to undo" << std::endl;
    } else {
        command = DoneCommands.back();
        DoneCommands.pop_back();
        command->unExecute();
        CanceledCommands.push_back(command);
    }
}
void Invoker::Redo() {
    if (CanceledCommands.size() == 0) {
        std::cerr << "There's nothing to redo" << std::endl;
    } else {
        command = CanceledCommands.back();
        CanceledCommands.pop_back();
        command->Execute();
        DoneCommands.push_back(command);
    }
}
void Invoker::Show() {
    com.show();
}
Invoker::~Invoker () {
    for (auto x : DoneCommands) {
        delete x;
    }
    for (auto x : CanceledCommands) {
        delete x;
    }
}
