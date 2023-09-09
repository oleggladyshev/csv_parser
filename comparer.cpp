#include "heap_sort.hpp"
#include "vector_sort.hpp"
#include "reader.hpp"
#include "record.hpp"
#include "utils.hpp"
#include <chrono>
#include <vector>

using namespace std::chrono;

constexpr size_t c_size = 100;

using CV = VectorSort<Record, c_size>;
using CH = HeapSort<Record, c_size + 1>;

template <class Cont>
steady_clock::duration Measure(
    const std::vector<Record>& data_parsed) {

    auto t1 = steady_clock::now();
    Cont cont_arr;
    auto it = data_parsed.cbegin();
    for (int i = 0; i < cont_arr.size(); i++)
    {
        cont_arr.push_initial(*it);
        ++it;
    }

    while (it != data_parsed.cend())
    {
        cont_arr.push(*it);
        ++it;
    }

    return steady_clock::now() - t1;
}


int main () {
    static const char* fname = "data.csv";

    auto t0 = std::chrono::steady_clock::now();

    FILE* f = fopen(fname, "r");
    fseek(f, 0, SEEK_END);
    const auto fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* data = new char[fsize];
    std::ignore = fread(data, 1, fsize, f);         

    CsvReader<Record> reader(data, fsize);

    std::vector<Record> data_parsed;
    while (!reader.eof())
    {
        auto r = reader.Read();
        data_parsed.push_back(r);
    }

    auto tv = Measure<CV>(data_parsed);
    auto th = Measure<CH>(data_parsed);

    std::cout << std::endl;
    print_timediff(tv, "Vector: ");
    print_timediff(th, "Heap: ");

    return 0;
}
