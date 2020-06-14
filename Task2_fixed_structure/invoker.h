#pragma once

#ifndef INCLUDED_INVOKER_14062020
#define INCLUDED_INVOKER_14062020
    #include "commands.h"
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
                     const unsigned &s );
        void adddpt (const std::string &d);
        void chgnmdpt (const std::string &name, const std::string &newname);
        void delwrk (const std::string &d, const std::string &w);
        void deldpt (const std::string &d);
        void editdpt (const std::string &d,
                      const std::string &w,
                      const std::string &n,
                      const std::string &f,
                      const unsigned & s);
        void savetofile (const std::string &fname);
        void loadfromfile (const std::string &fname);
        void Undo();
        void Redo();
        void Show();
        ~Invoker ();
    };
#endif
