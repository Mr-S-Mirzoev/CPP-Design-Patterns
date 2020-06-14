#ifndef IMPORTED_INPUTHANDLER_14062020
#define IMPORTED_INPUTHANDLER_14062020
    #include <iostream>
    #include "invoker.h"

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

    static inline void print_commands () {
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

    static inline int handle_input() {
        Invoker inv;
        print_commands();
        std::string com = "START";
        while (!com.empty() && com != EXIT) {
            try {
                std::getline(std::cin, com);
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
                    } else if (com == EXIT) {
                        std::cout << "Thanks for using the programm!" << std::endl;
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
                return 1;
            }
        }
        return 0;
    }
#endif
