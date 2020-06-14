#include "xmlhandler.h"

XML_reader::XML_reader (const std::string &filename) {
    std::ifstream in(filename);
    std::string line;
    std::string lastdpt, name, ttl;
    unsigned salary;
    
    // Reading file line by line
    while (std::getline(in, line)) {
        if (line[2] == '?') {
            continue;
        }
        if (string_manip::trim_copy(line) == "<departments>") {
            continue;
        }
        std::string s_trimmed((string_manip::trim_copy(line)));
        if (s_trimmed.substr(1, 10) == "department") {
            lastdpt = s_trimmed.substr(18, s_trimmed.length() - 20);
            comp.add_department(lastdpt);
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
            comp.add_worker(lastdpt, name, ttl, salary);
        } else {
            continue;
        }
    }
}
Company::Company XML_reader::get_comp () const {
    return comp;
}
Company::Company& XML_reader::get_comp_lnk () {
    return comp;
}
void XML_reader::show () const {
    comp.show();
}

XML_writer::XML_writer (const std::string &filename, const Company::Company &c) {
    std::ofstream testFile;
    testFile.open(filename, std::ios::out | std::ios::binary);
    testFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    auto lines = c.xml_repr();
    for (auto &ln : lines) {
        testFile << ln << std::endl;
    }
    testFile.close();
}
