#pragma once
#ifndef INCLUDED_STRINGMANIPULATION_14062020
#define INCLUDED_STRINGMANIPULATION_14062020
    #include <string>
    #include <vector>
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
#endif
