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

    class FileReader {
    public:
        static std::optional<FileReader> CreateFileReader(const std::string &data_file) {
            std::cout << "Reading File: " << data_file << std::endl;
            FileReader reader(data_file);
            if (reader.file_data.fail()) {
                std::cout << "Failed to open file: data/" << data_file << std::endl;
                return {};
            } else {
                return reader;
            }
        }

        bool GetLine(std::string &write_to_line) {
            if (!next_lines.empty()) {
                if (current_line) {
                    previous_lines.push(current_line.value());
                }
                write_to_line = next_lines.top();
                current_line = next_lines.top();
                next_lines.pop();
                return true;
            }
            std::getline(file_data, write_to_line);
            if (file_data.fail()) {
                return false;
            }
            if (current_line) {
                previous_lines.push(*current_line);
            }
            current_line = write_to_line;
            return true;
        }

        bool GetLastLine(std::string &write_to_line) {
            if (previous_lines.empty()) {
                return false;
            }
            if (current_line) {
                next_lines.push(current_line.value());
            }
            write_to_line = previous_lines.top();
            current_line = write_to_line;
            previous_lines.pop();
            return true;
        }

        void SeekToEnd(std::string &write_to_line) {
            if (current_line) {
                previous_lines.push(current_line.value());
            }
            while (!next_lines.empty()) {
                current_line = next_lines.top();
                previous_lines.push(current_line.value());
                next_lines.pop();
            }
            write_to_line = current_line.value();
        }

        void Close() {
            if (file_data.is_open()) {
                file_data.close();
            }
        }
    private:
        explicit FileReader(const std::string &data_file) {
            std::string absolute_path = Common::get_data_file_path(data_file);
            file_data = std::ifstream(absolute_path);
        }
        std::ifstream file_data;
        std::stack<std::string> previous_lines;
        std::optional<std::string> current_line;
        std::stack<std::string> next_lines;
    };
}

#endif //ADVENTOFCODE2022_ROOT_DR_H
