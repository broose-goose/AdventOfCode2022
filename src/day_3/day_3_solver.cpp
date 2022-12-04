
#include <iostream>
#include <fstream>
#include <set>
#include <map>

#include "common/root_dir.h"

char GetCommonValue(std::string &rucksack_contents) {
    std::set<char> found_contents;
    for (int i = 0; i < rucksack_contents.size() / 2; i++) {
        found_contents.emplace(rucksack_contents.at(i));
    }
    for (int i = rucksack_contents.size() / 2; i < rucksack_contents.size(); i++) {
        if (found_contents.contains(rucksack_contents.at(i))) {
            return rucksack_contents.at(i);
        }
    }
    // if we don't find it just panic i guess
    exit(-1);
}

int GetCharValue(const char &c) {
    if (c >= 'a') {
        return c - 'a' + 1;
    }  else {
        return c - 'A' + 27;
    }
}



int ElfRucksackFinangler(const std::string &rucksack_log) {
    std::cout << "Parsing File: " << rucksack_log << std::endl;
    std::string absolute_path = Common::get_data_file_path(rucksack_log);
    std::ifstream content_log;
    content_log.open(absolute_path);
    if (content_log.fail()) {
        std::cout << "Failed to open file: data/" << rucksack_log << std::endl;
        return -1;
    }

    std::string current_rucksack;
    int priorities_common = 0, char_value;
    char common_char;
    while (std::getline(content_log, current_rucksack)) {
        common_char = GetCommonValue(current_rucksack);
        char_value = GetCharValue(common_char);
        priorities_common += char_value;
#ifdef DEBUG
        std::cout << common_char << ": " << char_value << "; total: " << priorities_common << std::endl;
#endif
    }
    return priorities_common;
}

int ElfRucksackBadgeIdentifier(const std::string &rucksack_log) {
    std::cout << "Parsing File: " << rucksack_log << std::endl;
    std::string absolute_path = Common::get_data_file_path(rucksack_log);
    std::ifstream content_log;
    content_log.open(absolute_path);
    if (content_log.fail()) {
        std::cout << "Failed to open file: data/" << rucksack_log << std::endl;
        return -1;
    }
    int elf_number = 0, char_value, priorities_total = 0;
    std::string current_rucksack;
    char common_char;
    std::set<char> tracker_1, tracker_2;
    while (std::getline(content_log, current_rucksack)) {
        if (elf_number == 0) {
            for (const char &c : current_rucksack) {
                tracker_1.insert(c);
            }
        } else if (elf_number == 1) {
            for (const char &c : current_rucksack) {
                if (tracker_1.contains(c)) {
                    tracker_2.insert(c);
                }
            }
        } else {
            for (const char &c : current_rucksack) {
                if (tracker_2.contains(c)) {
                    common_char = c;
                    break;
                }
            }
        }
        elf_number = (elf_number + 1) % 3;
        if (elf_number % 3 == 0) {
            char_value = GetCharValue(common_char);
            priorities_total += char_value;
            tracker_1.clear();
            tracker_2.clear();
        }
    }
    return priorities_total;
}

int main() {

    long solution_1 = ElfRucksackFinangler("rucksack_contents.data");
    if (solution_1 == -1) {
        return -1;
    }
    std::cout << "Sum of priorities, common items: " << solution_1 << std::endl;

    long solution_2 = ElfRucksackBadgeIdentifier("rucksack_contents.data");
    if (solution_2 == -1) {
        return -1;
    }
    std::cout << "Sum of priorities, badge groups: " << solution_2 << std::endl;

    return 0;
}
