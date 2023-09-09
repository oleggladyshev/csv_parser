#include "heap_sort.hpp"
#include "vector_sort.hpp"
#include "reader.hpp"
#include "record.hpp"
#include "utils.hpp"
#include <chrono>
#include <thread>

using namespace std::chrono;

constexpr size_t c_size = 100;

using Container = VectorSort<Record, c_size>;
// using Container = HeapSort<Record, c_size + 1>;

void join(const Container::ValueType& a1, const Container::ValueType& a2, Container::ValueType& res)
{
    auto ir = res.begin();
    auto i1 = a1.cbegin();
    auto i2 = a2.cbegin();
    while (ir != res.end())
    {
        if (*i1 < *i2) {
            *ir = *i1;
            ++i1;
        } else {
            *ir = *i2;
            ++i2;
        }
        ++ir;
    }
}

struct ThreadContext {
    ThreadContext(char* data, size_t data_size, size_t ofs1, size_t ofs2) : 
    reader(data, data_size),
    ofs_s(ofs1), ofs_e(ofs2) {}

    CsvReader<Record> reader;
    Container cont;
    steady_clock::time_point ts, te;
    size_t ofs_s, ofs_e;
};


int main () {
    auto t0 = steady_clock::now();
    static const char* fname = "data.csv";
    FILE* f(fopen(fname, "r"));
    fseek(f, 0, SEEK_END);
    const auto data_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* full_data = new char[data_size];
    std::ignore = fread(full_data, 1, data_size, f);

    auto t1 = steady_clock::now();

    const auto TN = std::thread::hardware_concurrency();
    std::vector<ThreadContext> context;
    context.reserve(TN);

    size_t start_pos = 0;
    for (int i = 0; i < TN; i++) {
        auto end_pos = data_size * (i + 1) / TN;
        while (end_pos < data_size && full_data[end_pos] != '\n')
            end_pos++;
        context.emplace_back(full_data + start_pos, end_pos - start_pos, start_pos, end_pos);
        start_pos = end_pos + 1;
    }

    auto proc = [](ThreadContext& ctx) {
        ctx.ts = steady_clock::now();
        for (int i = 0; i < ctx.cont.size(); i++)
        {
            auto r = ctx.reader.Read();
            ctx.cont.push_initial(r);
        }

        while (!ctx.reader.eof())
        {
            auto r = ctx.reader.Read();
            ctx.cont.push(r);
        }
        ctx.te = steady_clock::now();
    };

    std::vector<std::thread> threads;
    threads.reserve(TN);
    for (int i = 0; i < TN; i++)
    {
        threads.emplace_back(proc, std::ref(context[i]));
    }

    for (auto &t : threads)
    {
        t.join();
    }

    std::vector<Container::ValueType> results;
    results.resize(TN);
    for (auto &r : results)
        r.resize(Container::size());
    size_t n = TN;
    for (int i = 0; i < TN/2; i++) {
        join(context[i].cont.Get(), context[i + TN / 2].cont.Get(), results[i]);
    }
    n = n / 2;
    size_t p = 0;
    while (n > 1) {
        for (int i = 0; i < n / 2; i++) {
            join(results[p + i], results[p + i + n / 2], results[p + n + i]);
        }
        p += n / 2 * 2;
        n = (n + 1) / 2;
    }

    auto t2 = steady_clock::now();

    print_container(results[p]);

    auto t3 = steady_clock::now();

    std::cout << std::endl;
    print_timediff(t1 - t0, "File reading: ");
    print_timediff(t2 - t1, "Read and sort: ");
    print_timediff(t3 - t2, "Output: ");
    print_timediff(t3 - t0, "Total: ");
    std::cout << TN << " threads" << std::endl;
    for (auto &c : context) {
        print_timediff(c.te - c.ts, "Thread time: ");
    }

    return 0;
}
