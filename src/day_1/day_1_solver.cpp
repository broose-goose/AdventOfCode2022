//
// Created by bruce on 12/3/2022.
//

#include <iostream>
#include <fstream>
#include <filesystem>

#include "common/priority_queue.h"
#include "common/root_dir.h"



long ElfCalorieMaximum(const std::string &elf_log_file, const unsigned int elves_to_track) {
    std::cout << "Parsing File: " << elf_log_file << " for top " << elves_to_track << " elves" << std::endl;

    // get data dir directory
    std::string absolute_path = Common::get_data_file_path(elf_log_file);

    std::ifstream elf_log;
    elf_log.open(absolute_path);
    if (elf_log.fail()) {
        std::cout << "Failed to open file: data/" << elf_log_file << std::endl;
        return -1;
    }
    Common::long_min_heap max_n_elf_calories;
    long current_elf_calories = 0;
    std::string current_calories_str;
    while (std::getline(elf_log, current_calories_str)) {
        if (current_calories_str.empty()) {
            Common::PushNToQueue(max_n_elf_calories, elves_to_track, current_elf_calories);
#ifdef DEBUG
            std::cout << "Max " << elves_to_track << " Calories: ";
            Common::print_queue(max_n_elf_calories);
            std::cout << std::endl;
#endif
            current_elf_calories = 0;
        } else {
            current_elf_calories += std::stoi(current_calories_str);
        }
    }
    // one more time for last elf
    Common::PushNToQueue(max_n_elf_calories, elves_to_track, current_elf_calories);
    return Common::sum_queue(max_n_elf_calories);
}

int main() {
    // relative to data dir
    long solution_1 = ElfCalorieMaximum("elf_calories_log_1.data", 1);
    if (solution_1 == -1) {
        return -1;
    }
    std::cout << "Max Elf Calories: " << solution_1 << std::endl;

    long solution_2 = ElfCalorieMaximum("elf_calories_log_1.data", 3);
    if (solution_2 == -1) {
        return -1;
    }
    std::cout << "Max Elf Calories for top 3 elves: " << solution_2 << std::endl;
    return 0;
}
