//
// Created by bruce on 12/4/2022.
//

#include <iostream>
#include <fstream>
#include <functional>

#include "common/root_dir.h"

typedef std::pair<int, int> elf_range;
typedef std::pair<elf_range, elf_range> elf_pair_ranges;

std::ostream &operator <<(std::ostream &o, const elf_pair_ranges &elf_pair)
{
    return o << "[" << elf_pair.first.first << ", " << elf_pair.first.second << "] and ["
        << elf_pair.second.first << ", " << elf_pair.second.second << "]";
}

void ParseRange(const std::string &assignment, elf_range &range, const size_t start, const size_t end) {
    size_t separator_position = assignment.find_first_of('-', start);
    range.first = stoi(assignment.substr(start, separator_position - start));
    range.second = stoi(assignment.substr(separator_position + 1, end - (separator_position + 1)));
}

void ParseRanges(const std::string &assignment, elf_pair_ranges &pair_ranges) {
    size_t separator_position = assignment.find_first_of(',');
    ParseRange(assignment, pair_ranges.first, 0, separator_position);
    ParseRange(assignment, pair_ranges.second, separator_position + 1, assignment.size());
}

void LargestRangeFirst(elf_pair_ranges &pair_ranges) {
    if (pair_ranges.second.second - pair_ranges.second.first > pair_ranges.first.second - pair_ranges.first.first) {
        static elf_range tmp_range;
        tmp_range = pair_ranges.first;
        pair_ranges.first = pair_ranges.second;
        pair_ranges.second = tmp_range;
    }
}

bool FullContain(const elf_pair_ranges &pair_ranges) {
    return pair_ranges.first.first <= pair_ranges.second.first &&
        pair_ranges.first.second >= pair_ranges.second.second;
}

void SmallestStartFirst(elf_pair_ranges &pair_ranges) {
    if (pair_ranges.second.first < pair_ranges.first.first) {
        static elf_range tmp_range;
        tmp_range = pair_ranges.first;
        pair_ranges.first = pair_ranges.second;
        pair_ranges.second = tmp_range;
    }
}

bool Overlapping(const elf_pair_ranges &pair_ranges) {
    return pair_ranges.second.first <= pair_ranges.first.second;
}

int ElfOverlappingPairs(
    const std::string &cleanup_assignments,
    const std::function<void(elf_pair_ranges &)> &swap_logic,
    const std::function<bool(const elf_pair_ranges &)> &contains_logic
) {
    std::cout << "Parsing File: " << cleanup_assignments << std::endl;
    std::string absolute_path = Common::get_data_file_path(cleanup_assignments);
    std::ifstream assignments;
    assignments.open(absolute_path);
    if (assignments.fail()) {
        std::cout << "Failed to open file: data/" << cleanup_assignments << std::endl;
        return -1;
    }
    std::string pair_assignment;
    bool is_contain = false;
    long contain = 0;
    elf_pair_ranges pair_ranges;
    while (std::getline(assignments, pair_assignment)) {
        ParseRanges(pair_assignment, pair_ranges);
        swap_logic(pair_ranges);
        is_contain = contains_logic(pair_ranges);
#ifdef DEBUG
        std::cout << pair_ranges << ": " << (is_contain ? "CONTAIN" : "no contain...") << std::endl;
#endif
        if (is_contain) {
            contain += 1;
        }
    }
    return contain;
}


int main() {

    long solution_1 = ElfOverlappingPairs(
        "cleanup_assignments.data", LargestRangeFirst, FullContain
    );
    if (solution_1 == -1) {
        return -1;
    }
    std::cout << "Full containing Subsections: " << solution_1 << std::endl;

    long solution_2 = ElfOverlappingPairs(
        "cleanup_assignments.data", SmallestStartFirst, Overlapping
    );
    if (solution_2 == -1) {
        return -1;
    }
    std::cout << "Overlapping Subsections: " << solution_2 << std::endl;

    return 0;
}