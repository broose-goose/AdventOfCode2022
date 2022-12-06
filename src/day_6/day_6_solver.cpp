//
// Created by bruce on 12/4/2022.
//

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <optional>
#include <algorithm>

#include "common/fs.h"

std::vector<std::tuple<std::string, int, int>> example_packets = {
        { "mjqjpqmgbljsphdztnvjfqwrcgsmlb", 7, 19 },
        { "bvwbjplbgvbhsrlpgdmjqwftvncz", 5, 23 },
        { "nppdvjthqldpwncqszvftbrmjlhg", 6, 23 },
        { "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 10, 29 },
        { "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 11, 26 }
};

class CorruptPacket {
public:
    CorruptPacket(const std::string &p): raw_packet(p) {}

    int GetEndOfHeader() {
        if (end_of_header == -1) {
            findElement(header_size, 0, end_of_header);
        }
        return end_of_header;
    }
    int GetStartOfMessage() {
        if (start_of_message == -1) {
            findElement(message_size, 0, start_of_message);
        }
        return start_of_message;
    }
private:
    static const int header_size;
    static const int message_size;

    const std::string &raw_packet;
    int end_of_header = -1;
    int start_of_message = -1;

    std::deque<char> sliding_window;
    std::map<char, int> window_counts;

    void findElement(const int &window_size, const int &start_position, int &position) {
        sliding_window.clear();
        window_counts.clear();
        for (auto it = raw_packet.begin() + start_position; it != raw_packet.end(); it++) {
            const char &c = *it;
            // add to sliding window; increment the value frequency
            sliding_window.push_back(c);
            if (!window_counts.contains(c)) {
                window_counts[c] = 1;
            } else {
                window_counts[c]++;
            }
            if (sliding_window.size() < window_size) {
                // haven't parsed enough characters for a full header
                continue;
            } else if (sliding_window.size() > window_size) {
                // if sliding window exceeds known header size, remove the corrupt
                // character
                const char cc = sliding_window.front();
                window_counts[cc]--;
                if (window_counts[cc] == 0) {
                    window_counts.erase(cc);
                }
                sliding_window.pop_front();
            }
            // cpp WEEEEEEE
            if (std::max_element(
                    window_counts.begin(), window_counts.end(),
                    [](const std::pair<char,int>& a, const std::pair<char,int>& b) -> bool {
                        return a.second < b.second;
                    }
            )->second == 1) {
                // offset by 1 b/c elves
                position = std::distance(raw_packet.begin(), it) + 1;
                return;
            }
        }
    }
};

const int CorruptPacket::header_size = 4;
const int CorruptPacket::message_size = 14;

int TestPartOne() {
    std::cout << "Running Tests 1..." << std::endl;

    for (auto &[packet, header_end, _] : example_packets) {
        CorruptPacket p(packet);
        auto end_of_header = p.GetEndOfHeader();
        if (end_of_header != header_end) {
            std::cout << "Incorrect end of header; Expects: " << header_end
                << "; Got: " << end_of_header << std::endl;
            return -1;
        } else {
            std::cout << "Correctly parsed packet: " << packet << "; header: " << end_of_header << std::endl;
        }
    }
    std::cout << "Tests 1 finished successfully :D" << std::endl;
    return 0;
};

int RunPartOne() {
    std::cout << "Finding packet header end" << std::endl;
    std::optional<Common::FileReader> op_reader = Common::FileReader::CreateFileReader("broken_packet.data");
    if (!op_reader) {
        return -1;
    }
    Common::FileReader &reader = *op_reader;
    std::string raw_packet;
    if (!reader.GetLine(raw_packet)) {
        std::cout << "Failed to read packet" << std::endl;
        return -1;
    }

    std::cout << "Parsing packet" << std::endl;
    CorruptPacket p(raw_packet);
    return p.GetEndOfHeader();
}

int TestPartTwo() {
    std::cout << "Running Tests 2..." << std::endl;

    for (auto &[packet, _, data_start] : example_packets) {
        CorruptPacket p(packet);
        auto start_of_data = p.GetStartOfMessage();
        if (start_of_data != data_start) {
            std::cout << "Incorrect start of message; Expects: " << data_start
                      << "; Got: " << start_of_data << std::endl;
            return -1;
        } else {
            std::cout << "Correctly parsed packet: " << packet << "; Message: " << start_of_data << std::endl;
        }
    }
    std::cout << "Tests 2 finished successfully :D" << std::endl;
    return 0;
};

int RunPartTwo() {
    std::cout << "Finding packet data offset" << std::endl;
    std::optional<Common::FileReader> op_reader = Common::FileReader::CreateFileReader("broken_packet.data");
    if (!op_reader) {
        return -1;
    }
    Common::FileReader &reader = *op_reader;
    std::string raw_packet;
    if (!reader.GetLine(raw_packet)) {
        std::cout << "Failed to read packet" << std::endl;
        return -1;
    }

    std::cout << "Parsing packet" << std::endl;
    CorruptPacket p(raw_packet);
    return p.GetStartOfMessage();
}

int main() {
    int test_1 = TestPartOne();
    if (test_1 == -1) {
        return -1;
    }

    int solution_1 = RunPartOne();
    if (solution_1 == -1) {
        return -1;
    }
    std::cout << "Packet header ends at: " << solution_1 << std::endl;

    int test_2 = TestPartTwo();
    if (test_2 == -1) {
        return -1;
    }

    int solution_2 = RunPartTwo();
    if (solution_2 == -1) {
        return -1;
    }
    std::cout << "Packet message starts at: " << solution_2 << std::endl;

    return 0;
}