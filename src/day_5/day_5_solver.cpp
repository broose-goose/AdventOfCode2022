//
// Created by bruce on 12/4/2022.
//

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <regex>
#include <optional>

#include "common/root_dir.h"

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

typedef std::stack<char> crate_stack;

typedef std::vector<crate_stack> supply_crates;

std::string CrateTopsString(const supply_crates &crates) {
    std::string s_out;
    for (auto &s : crates) {
        s_out += s.top();
    }
    return s_out;
}

std::ostream &operator <<(std::ostream &o, const supply_crates &crates)
{
    for (auto &s : crates) {
        if (!s.empty()) {
            o << "[" << s.top() << "] ";
        }
    }
    return o;
}

int DetermineCrateStacks(const std::string &stack_numbers) {
    std::regex get_last_crate_number(R"((\d+)\s?+$)");
    std::smatch match;
    if (std::regex_search(stack_numbers, match, get_last_crate_number)) {
        return stoi(match[0]);
    } else {
        // didn't find the stack string; return -1 and continue until we do
        return -1;
    }
}

void ParseStackLine(supply_crates &crates, const std::string &plan_line) {
    const size_t number_crates = crates.size();
    for (int i = 0; i < number_crates; i++) {
        const int search_index = i * 4 + 1;
        if (search_index >= plan_line.size()) {
            return;
        }
        const char c = plan_line.at(i * 4 + 1);
        if (c != ' ') {
            crates[i].push(c);
        }
    }
}

class Operation {
public:
    static std::optional<Operation> CreateOperation(const std::string &op_string) {
        Operation op(op_string);
        if (op.crates_to_move == -1) {
            return {};
        } else {
            return op;
        }
    }
    void apply_operation(supply_crates &supply, bool is_9001_unit) {
        crate_stack &from_stack = supply.at(from_stack_index);
        crate_stack &to_stack = supply.at(to_stack_index);
        if (!is_9001_unit) {
            // 9000 unit 1 at a time
            for (int i = 0; i < crates_to_move; i++) {
                to_stack.push(from_stack.top());
                from_stack.pop();
            }
        } else {
            // super annoying, but we can create a tmp stack to
            // preserve order
            static crate_stack tmp_stack;
            for (int i = 0; i < crates_to_move; i++) {
                tmp_stack.push(from_stack.top());
                from_stack.pop();
            }
            for (int i = 0; i < crates_to_move; i++) {
                to_stack.push(tmp_stack.top());
                tmp_stack.pop();
            }
        }

    }
private:
    explicit Operation(const std::string &op) {
        // if this doesn't get changed, we know it failed
        crates_to_move = -1;
        std::smatch matches;
        if (std::regex_search(op, matches, regex_str)) {
            if (matches.size() != 4) {
                std::cout << "Operation string didn't have 4 matches? but " << matches.size() << ": " << op << std::endl;
                return;
            }
            // crates are indexed at 1; change it to 0
            crates_to_move = stoi(matches[1]);
            from_stack_index = stoi(matches[2]) - 1;
            to_stack_index = stoi(matches[3]) - 1;
        } else {
            std::cout << "Improper operation string: " << op << std::endl;
        }
    }
    static const std::regex regex_str;
    int crates_to_move;
    int from_stack_index;
    int to_stack_index;
};

const std::regex Operation::regex_str(R"(move (\d+) from (\d+) to (\d+))");


std::string RunElvenCrane(const std::string &crate_stack_plan, bool is_9001_unit) {
    std::cout << "Running Elven Crane Simulation" << std::endl;
    std::string absolute_path = Common::get_data_file_path(crate_stack_plan);
    std::optional<FileReader> op_reader = FileReader::CreateFileReader(crate_stack_plan);
    if (!op_reader) {
        return "";
    }
    FileReader &reader = *op_reader;

    std::string plan_line;
    int stack_index_line = 0;
    int number_of_stacks = -1;
    std::cout << "Finding stack line" << std::endl;
    while (reader.GetLine(plan_line)) {
        number_of_stacks = DetermineCrateStacks(plan_line);
        if (number_of_stacks != -1) {
            break;
        }
        stack_index_line++;
    }
    if (number_of_stacks == -1) {
        std::cout << "Failed to determine the number of stacks" << std::endl;
        return "";
    }

    std::cout << "Creating " << number_of_stacks << " stacks" << std::endl;
    supply_crates crates;
    crates.resize(number_of_stacks, crate_stack());
    while (reader.GetLastLine(plan_line)) {
        ParseStackLine(crates, plan_line);
    }

    std::cout << "Created stacks with tops: " << crates << std::endl;
    reader.SeekToEnd(plan_line);
    // blank line for some reason
    reader.GetLine(plan_line);

    std::optional<Operation> op;
    while(reader.GetLine(plan_line)) {
        op = Operation::CreateOperation(plan_line);
        if (!op) {
            return "";
        }
        (*op).apply_operation(crates, is_9001_unit);
    }

    return CrateTopsString(crates);
}

int main() {

    std::string solution_1 = RunElvenCrane("crate_stacks.data", false);
    if (solution_1.empty()) {
        return -1;
    }
    std::cout << "Top stacks 9000: " << solution_1 << std::endl;

    std::string solution_2 = RunElvenCrane("crate_stacks.data", true);
    if (solution_2.empty()) {
        return -1;
    }
    std::cout << "Top stacks 9001: " << solution_2 << std::endl;
    return 0;
}