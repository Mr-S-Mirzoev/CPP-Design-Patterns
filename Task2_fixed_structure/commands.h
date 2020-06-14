#pragma once

#ifndef INCLUDED_COMMANDS_14062020
#define INCLUDED_COMMANDS_14062020
    #include "fileinfo.h"
    #include "xmlhandler.h"
    #include "company.h"
    namespace Commands {
        class Command
        {
        protected:
            Company::Company *comp;
        public:
            virtual ~Command() {}
            virtual void Execute() = 0;
            virtual void unExecute() = 0;
            
            void setCompany (Company::Company *ptr);
        };
        
        class AddDeptCommand: public Command
        {
            std::string name;
        public:
            AddDeptCommand (const std::string &n);
            void Execute();
            void unExecute();
        };
        
        class AddWorkerCommand: public Command
        {
            std::string dpt, wrk, ttl;
            unsigned sal;
        public:
            AddWorkerCommand (const std::string &d,
                              const std::string &w,
                              const std::string &t,
                              const unsigned &s);
            void Execute();
            void unExecute();
        };
        
        class DeleteDeptCommand: public Command
        {
            std::string name;
            Company::Department cpy;
        public:
            DeleteDeptCommand (const std::string &n);
            void Execute();
            void unExecute();
        };
        
        class DeleteWorkerCommand: public Command
        {
            std::string dpt, wrk;
            std::pair<std::string, unsigned> info;
        public:
            DeleteWorkerCommand (const std::string &d,
                                 const std::string &w);
            void Execute();
            void unExecute();
        };
        
        class ChangeNameDeptCommand: public Command
        {
            std::string dpt, newname;
        public:
            ChangeNameDeptCommand (const std::string &d,
                                   const std::string &n);
            void Execute();
            void unExecute();
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
                             const unsigned &s);
            void Execute();
            void unExecute();
        };
        
        class SaveToFileCommand: public Command
        {
            std::string fileName;
            Company::Company prevstate;
            bool existed;
        public:
            SaveToFileCommand (const std::string &fname);
            void Execute();
            void unExecute();
        };
        
        class LoadFromFileCommand: public Command
        {
            std::string fileName;
            Company::Company prevstate;
        public:
            LoadFromFileCommand (const std::string &fname);
            void Execute();
            void unExecute();
        };
    }
#endif
