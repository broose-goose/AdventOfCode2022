//
// Created by bruce on 12/4/2022.
//

#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <map>
#include <regex>
#include <deque>

#include "common/fs.h"

struct File {
    std::string name;
    int size;
};

struct Folder {
    int size = 0;
    std::string name;
    std::shared_ptr<Folder> parent = nullptr;
    std::map<std::string, File> files;
    std::map<std::string, std::shared_ptr<Folder>> children;

    void CreateFile(std::string f_name, int f_size) {
        File f;
        f.name = std::move(f_name);
        f.size = f_size;
        files.insert({ name, f });
        size += f_size;
        auto ancestor = parent;
        while (ancestor != nullptr) {
            ancestor->size += f_size;
            ancestor = ancestor->parent;
        }
    }
    std::string DetailsString() {
        std::string out = name ;
        auto ancestor = parent;
        while (ancestor != nullptr) {
            out.insert(0, "/");
            out.insert(0, name);
            ancestor = ancestor->parent;
        }
        out.insert(0, "(");
        out.append(" ");
        out.append(std::to_string(size));
        out.append(")");
        return out;
    }
};

class Session {
public:
    bool ParseLine(const std::string &console_line) {
        if (console_line.empty()) {
            std::cout << "Nothing to read from console..." << std::endl;
            return false;
        }
        if (console_line.at(0) == '$') {
            return parseCommand(console_line);
        }
        if (is_parsing_ls) {
            return parseLsLine(console_line);
        }
        std::cout << "Didn't ls but don't see a command? damn elves" << std::endl;
        return false;
    }
    void PrintState() {
        std::cout << "Root: " << (!root ? "N/A" : root->name)
            << "CWD: " << (!cwd ? "N/A" : cwd->name)
            << "is_lsing?: " << is_parsing_ls
            << std::endl;
    }
    void Tree() {
        if (root == nullptr) {
            std::cout << "EMPTY" << std::endl;
            return;
        }
        printFolder(root, 0);
    }
    int SumDirectoriesSmallerThan(int smaller_than) {
        int total = 0;
        std::deque<std::shared_ptr<Folder>> folder_dq = { root };
        while (!folder_dq.empty()) {
            for (auto &folder_pair : folder_dq.front()->children) {
                folder_dq.push_back(folder_pair.second);
            }
            if (folder_dq.front()->size <= smaller_than) {
#ifdef DEBUG
                std::cout << "Small folder found: " << folder_dq.front()->DetailsString() << std::endl;
#endif
                total += folder_dq.front()->size;
            }
            folder_dq.pop_front();
        }
        return total;
    }
    std::shared_ptr<Folder> GetSmallestDeletableFolder() {
        const int available_space = 70000000 - root->size;
        const int needed_space = 30000000 - available_space;
        std::shared_ptr<Folder> smallest_deletable_folder = root;
        std::deque<std::shared_ptr<Folder>> folder_dq = { root };
        while (!folder_dq.empty()) {
            for (auto &folder_pair: folder_dq.front()->children) {
                folder_dq.push_back(folder_pair.second);
            }
            if (folder_dq.front()->size >= needed_space) {
                if (smallest_deletable_folder == nullptr || folder_dq.front()->size < smallest_deletable_folder->size) {
#ifdef DEBUG
                    std::cout << "Small folder found: " << folder_dq.front()->DetailsString() << std::endl;
#endif
                    smallest_deletable_folder = folder_dq.front();
                }
            }
            folder_dq.pop_front();
        }
        return smallest_deletable_folder;
    }
private:
    std::shared_ptr<Folder> root = nullptr;
    std::shared_ptr<Folder> cwd = nullptr;
    bool is_parsing_ls = false;
    static const std::regex cd_regex;
    static const std::regex dir_regex;
    static const std::regex file_regex;

    bool parseCommand(const std::string &console_line) {
        is_parsing_ls = false;
        std::smatch matches;
        if (std::regex_search(console_line, matches, cd_regex)) {
            if (matches.size() != 2) {
                std::cout << "malformed cd string? " << matches.size() << "; " << console_line << std::endl;
                return false;
            } else if (root == cwd && matches[1] == "..") {
                std::cout << "trying to cd back out of root...?" << std::endl;
                return false;
            }
            if (root == nullptr) {
                root = std::make_shared<Folder>(Folder());
                root->name = matches[1];
                cwd = root;
            } else if (matches[1] == "..") {
                cwd = cwd->parent;
            } else if (cwd->children.contains(matches[1])) {
                cwd = cwd->children.at(matches[1]);
            } else {
                auto new_folder = createNewFolder(matches[1]);
                cwd = new_folder;
            }
        } else {
            // for now, lets just assume we only have ls and cd
            is_parsing_ls = true;
        }
        return true;
    }

    bool parseLsLine(const std::string &console_line) {
        if (cwd == nullptr) {
            std::cout << "don't even know how I got here..." << std::endl;
        }
        std::smatch matches;
        if (std::regex_search(console_line, matches, dir_regex)) {
            if (matches.size() != 2) {
                std::cout << "Really shouldn't be testing this much..." << std::endl;
                return false;
            }
            if (!cwd->children.contains(matches[1])) {
                std::make_shared<Folder>(Folder());
            }
        } else if (std::regex_search(console_line, matches, file_regex)) {
            if (matches.size() != 3) {
                std::cout << "But this is going to work first time so worth it?" << std::endl;
                return false;
            }
            if (!cwd->files.contains(matches[2])) {
                cwd->CreateFile(matches[2], std::stoi(matches[1]));
            }
        } else {
            std::cout << "Not dir or file; ls is failing, everything is failing D:" << std::endl;
            return false;
        }
        return true;
    }

    std::shared_ptr<Folder> createNewFolder(const std::string &folder_name) {
        auto new_folder = std::make_shared<Folder>(Folder());
        new_folder->name = folder_name;
        new_folder->parent = cwd;
        cwd->children.insert({ folder_name, new_folder });
        return new_folder;
    }

    static void printFolder(const std::shared_ptr<Folder>& wd, unsigned int indent) {
        std::cout << std::string(indent, ' ') << "- " << wd->name << " (dir)" << std::endl;
        for (auto &[_, file] : wd->files) {
            std::cout << std::string(indent + 2, ' ') << "- " << file.name
                << " (file, size=" << file.size << ")" << std::endl;
        }
        for (auto &[_, folder]: wd->children) {
            printFolder(folder, indent + 2);
        }
    }
};

const std::regex Session::cd_regex(R"(^\$ cd (.+?)$)");
const std::regex Session::dir_regex(R"(^dir (.+?)$)");
const std::regex Session::file_regex(R"(^(\d+?) (.+?)$)");

int FindSmallFiles() {
    std::cout << "Finding small files" << std::endl;
    std::optional<Common::FileReader> op_reader = Common::FileReader::CreateFileReader("elf_console.txt");
    if (!op_reader) {
        return -1;
    }
    Common::FileReader &reader = *op_reader;
    std::string console_line;
    Session session;
    while(reader.GetLine(console_line)) {
        auto success = session.ParseLine(console_line);
        if (!success) {
            return -1;
        }
    }

#ifdef DEBUG
    session.Tree();
#endif

    int solution_1 = session.SumDirectoriesSmallerThan(100000);
    if (solution_1 == -1) {
        return -1;
    }
    std::cout << "Directories with below 100000 (bytes?): " << solution_1 << std::endl;

    std::shared_ptr<Folder> solution_2 = session.GetSmallestDeletableFolder();
    if (solution_2 == nullptr) {
        return -1;
    }
    std::cout << "Smallest deletable directory: " << solution_2->DetailsString() << std::endl;

    return 1;
}


int main() {
    return FindSmallFiles();
}