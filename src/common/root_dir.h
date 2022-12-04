//
// Created by bruce on 12/3/2022.
//

#ifndef ADVENTOFCODE2022_ROOT_DR_H
#define ADVENTOFCODE2022_ROOT_DR_H

#include <string>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define ROOT_PATH TOSTRING(PROJECT_DIR)

const char kPathSeparator =
#ifdef _WIN32
                            '\\';
#else
                            '/';
#endif

namespace Common {
    std::string get_data_file_path(const std::string &f_name) {
        return std::string(ROOT_PATH) +  kPathSeparator + "data" + kPathSeparator + f_name;
    }
}

#endif //ADVENTOFCODE2022_ROOT_DR_H
