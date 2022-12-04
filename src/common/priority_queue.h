//
// Created by bruce on 12/3/2022.
//

#ifndef ADVENTOFCODE2022_PRIORITY_QUEUE_H
#define ADVENTOFCODE2022_PRIORITY_QUEUE_H

#include <vector>
#include <queue>
#include <iostream>

namespace Common {
    typedef std::priority_queue <long, std::vector<long>, std::greater<>> long_min_heap;

    template<typename Q>
    void print_queue(Q q) {
        for (;!q.empty(); q.pop()) {
            std::cout << q.top() << ' ';
        }
    }

    template<typename Q>
    long sum_queue(Q q) {
        long s = 0;
        for (;!q.empty(); q.pop()) {
            s += q.top();
        }
        return s;
    }

    void PushNToQueue(long_min_heap &min_heap, const unsigned int entries, const long value) {
        min_heap.push(value);
        if (min_heap.size() > entries) {
            min_heap.pop();
        }
    }
}


#endif //ADVENTOFCODE2022_PRIORITY_QUEUE_H
