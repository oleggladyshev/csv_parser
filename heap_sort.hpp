#pragma once
#include <vector>
#include <cstdlib>
#include <iostream>

template <class Value, size_t c_size = 100, bool print_debug = false>
class HeapSort {
public:
    using ValueType = std::vector<Value>;

    HeapSort() : first_pos(c_size) {
        m_vec.resize(c_size);
    }

    void push_initial(const Value& r) {
        first_pos--;
        m_vec[first_pos] = r;
        heapify();
        if (print_debug) {
            for (auto i = first_pos; i <  c_size; i++)
            {
                std::cout << m_vec[i].score << " ";
            }
            std::cout << std::endl;
        }
    }

    void push(const Value& r) {
        if (r > m_vec[0])
            return;
        m_vec[first_pos] = r;
        heapify();
    }

    void heapify() {
        size_t i = 0;
        do {
            auto left = i * 2 + 1;
            auto right = i * 2 + 2;
            auto max = i;
            if (left + first_pos < c_size &&
                m_vec[left + first_pos] > m_vec[max + first_pos])
                max = left;
            if (right + first_pos < c_size &&
                m_vec[right + first_pos] > m_vec[max + first_pos])
                max = right;
            if (max != i) {
                auto r = m_vec[max + first_pos];
                m_vec[max + first_pos] = m_vec[i + first_pos];
                m_vec[i + first_pos] = r;
                i = max;
            }
            else {
                break;
            }
        } while (1);
    }

    const ValueType& Get() const {
        return m_vec;
    }

    static constexpr size_t size()  { return c_size; }

private:
    ValueType m_vec;
    size_t first_pos;
};
