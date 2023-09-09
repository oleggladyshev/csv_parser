#pragma once
#include <vector>
#include <algorithm>

template <class Value, size_t c_size = 100>
class VectorSort {
public:
    using ValueType = std::vector<Value>;

    VectorSort () {
        m_vec.reserve(c_size);
    }

    void push(const Value& r) {
        if (r > m_vec[c_size - 1]) return;

        auto pos = std::upper_bound(m_vec.begin(), m_vec.end(), r);
        if (pos != m_vec.end()) {
            m_vec.pop_back();
            m_vec.insert(pos, r);
        }
    }

    void push_initial(const Value& r) {
        auto pos = std::upper_bound(m_vec.begin(), m_vec.end(), r);
        if (pos != m_vec.end()) {
            m_vec.insert(pos, r);
        }
        else {
            m_vec.push_back(r);
        }
    }

    const ValueType& Get() const {
        return m_vec;
    }

    const size_t Moves() const { return moves; }

    static constexpr size_t size()  { return c_size; }

private:
    ValueType m_vec; 
    size_t moves = 0;
};
