#ifndef RANDOMIZEDQUEUE_HPP
#define RANDOMIZEDQUEUE_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

template <class etype>
class RandomizedQueue {
public:
    template <bool is_const>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = std::conditional_t<is_const, const etype, etype>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = value_type *;
        using reference         = value_type &;

        basic_iterator() = default;

        basic_iterator &operator++() {
            current_index_++;
            if (permutation_.size() == current_index_) {
                permutation_.clear();
                current_index_ = 0;
            }
            return *this;
        }

        basic_iterator operator++(int) {
            auto temp = *this;
            operator++();
            return temp;
        }

        reference operator*() const { return *permutation_[current_index_]; }

        pointer operator->() const { return permutation_[current_index_]; }

        basic_iterator &operator--() {
            current_index_--;
            return *this;
        }

        basic_iterator operator--(int) {
            auto temp = *this;
            operator--();
            return temp;
        }

        friend bool operator==(const basic_iterator &first, const basic_iterator &second) {
            return ((first.permutation_ == second.permutation_) && (first.current_index_ == second.current_index_));
        }

        friend bool operator!=(const basic_iterator &first, const basic_iterator &second) { return !(first == second); }

    private:
        size_t current_index_{0};
        std::vector<pointer> permutation_;
        friend class RandomizedQueue;
        basic_iterator(std::conditional_t<is_const, const RandomizedQueue, RandomizedQueue> &que)
            : permutation_(que.size()) {
            for (size_t i = 0; i < que.size(); i++) {
                permutation_[i] = &que.random_queue[i];
            }
            std::shuffle(permutation_.begin(), permutation_.end(), que.new_random);
        }
    };

public:
    using iterator       = basic_iterator<false>;
    using const_iterator = basic_iterator<true>;
    RandomizedQueue()    = default;  // constructor
    ~RandomizedQueue()   = default;  // destructor
    size_t size() const;
    bool empty() const;
    void enqueue(etype element);
    const etype &sample() const;
    etype dequeue();
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    std::vector<etype> random_queue;
    thread_local static inline std::mt19937 new_random = std::mt19937(std::random_device{}());
    size_t get_random_index() const;
};

template <class etype>
size_t RandomizedQueue<etype>::get_random_index() const {
    std::uniform_int_distribution<std::size_t> distribution(0, size() - 1);
    return distribution(new_random);
}

template <class etype>
size_t RandomizedQueue<etype>::size() const {
    return random_queue.size();
}

template <class etype>
bool RandomizedQueue<etype>::empty() const {
    return (size() == 0);
}

template <class etype>
void RandomizedQueue<etype>::enqueue(etype element) {
    random_queue.push_back(std::move(element));
}

template <class etype>
const etype &RandomizedQueue<etype>::sample() const {
    assert(!empty());
    return random_queue[get_random_index()];
}

template <class etype>
etype RandomizedQueue<etype>::dequeue() {
    assert(!empty());
    size_t index = get_random_index();
    std::swap(random_queue[index], random_queue.back());
    etype result = std::move(random_queue.back());
    random_queue.pop_back();
    return result;
}

template <class etype>
typename RandomizedQueue<etype>::iterator RandomizedQueue<etype>::begin() {
    return {*this};
}

template <class etype>
typename RandomizedQueue<etype>::iterator RandomizedQueue<etype>::end() {
    return {};
}

template <class etype>
typename RandomizedQueue<etype>::const_iterator RandomizedQueue<etype>::cbegin() const {
    return {*this};
}

template <class etype>
typename RandomizedQueue<etype>::const_iterator RandomizedQueue<etype>::cend() const {
    return {};
}

template <class etype>
typename RandomizedQueue<etype>::const_iterator RandomizedQueue<etype>::begin() const {
    return cbegin();
}

template <class etype>
typename RandomizedQueue<etype>::const_iterator RandomizedQueue<etype>::end() const {
    return cend();
}

#endif  // RANDOMIZEDQUEUE_HPP
