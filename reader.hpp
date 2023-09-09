#pragma once

#include <cstdio>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <tuple>

template <class Value>
class CsvReader {
public:
    CsvReader(char* data, size_t data_size) :
        m_data(data),
        m_str(data),
        m_data_size(data_size) {
    }

    Value Read()
    {
        Value res;
        res.descr = m_str;
        while (*m_str != ',') m_str++;
        *m_str = 0;
        m_str++;

        res.score = 0;
        while (*m_str == ' ') m_str++;
        do {
            res.score = res.score * 10 + static_cast<uint32_t>(*m_str - '0');
        } while (*(++m_str) != '\n');
            
        m_str++;
        return res;
    }
        
    bool eof() const {
        return m_str - m_data >= m_data_size;
    }

private:
    size_t m_pos, m_data_size;
    char* m_data;
    char* m_str;
};
