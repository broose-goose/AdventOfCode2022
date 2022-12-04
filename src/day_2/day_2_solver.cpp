//
// Created by bruce on 12/3/2022.
//

#include <iostream>
#include <fstream>
#include <functional>

#include "common/root_dir.h"

enum Trinary {
    Zero,
    One,
    Two
};

void OpponentAction(Trinary &op_arg, const std::string &raw) {
    op_arg = static_cast<Trinary>(raw.at(0) - 'A');
}

void Response(Trinary &my_arg, const std::string &raw) {
    my_arg = static_cast<Trinary>(raw.at(2) - 'X');
}

template<typename T>
T pythonic_modulo(const T &a, const T &b) {
    return (b + (a % b)) % b;
}

void ResolveIfMove(long &score, const Trinary &op_move, const Trinary &my_move) {
    // you get 1 point for rock, 2 for paper, 3 for scissors
    score += my_move + 1;
    if (op_move == my_move) {
        // draw is 3 points
        score += 3;
        return;
    }
    static uint8_t sum_score;
    sum_score = op_move + my_move;
    if (my_move > op_move && sum_score != 2 || my_move < op_move && sum_score == 2) {
        // in this case we win as the only 2 is Rock(0) + Scissors(2) = 2
        score += 6;
    }
    // don't do anything if we lose
}

void ResolveIfOutcome(long &score, const Trinary &op_move, const Trinary &my_outcome) {
    // you get 0 points for loss, 3 points for draw, 6 points for win
    score += my_outcome * 3;
    switch (my_outcome) {
        // loss
        case Trinary::Zero:
            // rotate backward on the trinary, offset by one
            score += pythonic_modulo(op_move - 1, 3) + 1;
            break;
        // tie
        case Trinary::One:
            score += op_move + 1;
            break;
        case Trinary::Two:
            // rotate forward on the trinary, offset by one
            score += pythonic_modulo(op_move + 1, 3) + 1;
            break;
    }
}


long ElfRpcSimulation(
    const std::string &elf_cheat_sheet,
    const std::function<void(long &, const Trinary &, const Trinary &)> &ResolveRound
) {
    std::cout << "Parsing File: " << elf_cheat_sheet << std::endl;

    // get data dir directory
    std::string absolute_path = Common::get_data_file_path(elf_cheat_sheet);

    std::ifstream cheat_sheet;
    cheat_sheet.open(absolute_path);
    if (cheat_sheet.fail()) {
        std::cout << "Failed to open file: data/" << elf_cheat_sheet << std::endl;
        return -1;
    }
    Trinary op_move, my_arg;
    std::string current_round;
    long score = 0;
    while (std::getline(cheat_sheet, current_round)) {
        OpponentAction(op_move, current_round);
        Response(my_arg, current_round);
        ResolveRound(score, op_move, my_arg);
    }
    return score;
}

int main() {

    long solution_1 = ElfRpcSimulation("rpc_strategy_guide.data", ResolveIfMove);
    if (solution_1 == -1) {
        return -1;
    }
    std::cout << "Total RPC score if second column is move: " << solution_1 << std::endl;

    long solution_2 = ElfRpcSimulation("rpc_strategy_guide.data", ResolveIfOutcome);
    if (solution_2 == -1) {
        return -1;
    }
    std::cout << "Total RPC score if second column is outcome: " << solution_2 << std::endl;

    return 0;
}
