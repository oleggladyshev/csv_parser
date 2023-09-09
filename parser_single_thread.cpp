//Select desr, score from CSV ORDER by score desc, descr ASC limit 100

#include "heap_sort.hpp"
#include "vector_sort.hpp"
#include "reader.hpp"
#include "record.hpp"
#include "utils.hpp"

constexpr size_t c_size = 100;

using Container = VectorSort<Record, c_size>;
// using Container = HeapSort<Record, c_size + 1>;

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
    Container cont;

    auto t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < cont.size(); i++)
    {
        auto r = reader.Read();
        cont.push_initial(r);
    }

    while (!reader.eof())
    {
        auto r = reader.Read();
        cont.push(r);
    }

    auto t2 = std::chrono::steady_clock::now();

    print_container(cont.Get());

    auto t3 = std::chrono::steady_clock::now();


    std::cout << std::endl;
    print_timediff(t1 - t0, "File reading: ");
    print_timediff(t2 - t1, "Read and sort: ");
    print_timediff(t3 - t2, "Output: ");
    print_timediff(t3 - t0, "Total: ");

    return 0;
}
