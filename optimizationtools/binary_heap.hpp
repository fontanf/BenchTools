#pragma once

#include <vector>
#include <cstdint>
#include <functional>

namespace optimizationtools
{

template <typename Key>
class BinaryHeap
{

public:

    typedef int64_t Index;
    typedef int64_t Position;
    typedef std::function<Key (Index)> Function;

    BinaryHeap(Index element_number): positions_(element_number + 1, -1), heap_(1) { }
    BinaryHeap(Index element_number, Function get_key);
    virtual ~BinaryHeap() { }

    inline bool empty() const { return heap_.size() == 1; }
    inline Position size() const { return heap_.size() - 1; }
    inline std::pair<Index, Key> top() const { return heap_[1]; }
    inline void pop();
    inline void update_key(Index index, Key key);

private:

    std::vector<std::pair<Index, Key>> heap_;
    std::vector<Position> positions_;

    inline Key cost(Position element_pos) const { return heap_[element_pos].second; }
    inline void percolate_up(Position position);
    inline void percolate_down(Position position);

};

/******************************************************************************/

template <typename Key>
void BinaryHeap<Key>::percolate_up(Position position)
{
    while (position != 1 && cost(position) >= cost(position / 2)) {
        Position position_child = position / 2;
        positions_[heap_[position].first] = position_child;
        positions_[heap_[position_child].first] = position;
        auto tmp = heap_[position_child];
        heap_[position_child] = heap_[position];
        heap_[position] = tmp;
        position = position_child;
    }
}

template <typename Key>
void BinaryHeap<Key>::percolate_down(Position position)
{
    while (2 * position < (Position)heap_.size()) {
        Position position_child_1 = 2 * position;
        Position position_child_2 = 2 * position + 1;
        if (position_child_2 >= (Position)heap_.size()
                || cost(position_child_1) <= cost(position_child_2)) {
            if (cost(position) <= cost(position_child_1))
                return;
            positions_[heap_[position].first] = position_child_1;
            positions_[heap_[position_child_1].first] = position;
            auto tmp = heap_[position_child_1];
            heap_[position_child_1] = heap_[position];
            heap_[position] = tmp;
            position = position_child_1;
        } else {
            if (cost(position) <= cost(position_child_2))
                return;
            positions_[heap_[position].first] = position_child_2;
            positions_[heap_[position_child_2].first] = position;
            auto tmp = heap_[position_child_2];
            heap_[position_child_2] = heap_[position];
            heap_[position] = tmp;
            position = position_child_2;
        }
    }
}

template <typename Key>
BinaryHeap<Key>::BinaryHeap(Index element_number, Function get_key):
    heap_(element_number + 1),
    positions_(element_number + 1)
{
    for (Index index = 0; index < element_number; ++index) {
        heap_[index + 1] = {index, get_key(index)};
        positions_[index] = index + 1;
    }

    for (Position position = element_number; position >= 1; --position)
        percolate_down(position);
}

template <typename Key>
void BinaryHeap<Key>::update_key(Index index, Key key)
{
    Position position = positions_[index];

    if (position == -1) {
        positions_[index] = heap_.size();
        heap_.push_back({index, key});
        percolate_up(heap_.size() - 1);
    } else if (key > cost(position)) {
        heap_[position].second = key;
        percolate_down(position);
    } else if (key < cost(position)) {
        heap_[position].second = key;
        percolate_up(position);
    }
}

template <typename Key>
void BinaryHeap<Key>::pop()
{
    assert(size() > 0);
    positions_[heap_.back().first] = 1;
    heap_[1] = heap_.back();
    heap_.pop_back();
    percolate_down(1);
}

}
