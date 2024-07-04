#include "Subset.hpp"

#include "RandomizedQueue.hpp"
void subset(unsigned long k, std::istream& in, std::ostream& out) {
    RandomizedQueue<std::string> q;
    std::string line;
    while (k > 0 && std::getline(in, line)) {
        q.enqueue(line);
        --k;
    }
    while (!q.empty()) {
        out << q.dequeue() << '\n';
    }
}
