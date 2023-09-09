#pragma once
#include <cmath>
#include <cstdint>

struct Record {
    static const int c_descr_size = 15;
    char* descr;
    uint64_t score;

    bool operator > (const Record& rhs) const {
        if (score < rhs.score) return false;
        if (score > rhs.score) return true;
        return descr_less(rhs.descr);
    }
    bool operator < (const Record& rhs) const {
        return !(*this > rhs);
    }
    bool descr_less(const char* rhs) const {
        size_t pos = 0;
        while (pos < c_descr_size)
        {
            if (descr[pos] == 0) return true;
            if (rhs[pos] == 0) return false;
            if (descr[pos] < rhs[pos]) return true;
            if (descr[pos] > rhs[pos]) return false;
            pos++;
        }
        return false;
    }
};
