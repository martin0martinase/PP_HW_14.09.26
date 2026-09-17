#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <future>

using data_t = std::vector<unsigned long long>;
using value_t = data_t::value_type;

class Clicker
{
public:
    Clicker():
        start_(std::chrono::high_resolution_clock::now())
    {}
    double millisec() const {
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        auto t = high_resolution_clock::now();
        return duration_cast<milliseconds>(t - start_).count();
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

std::vector<std::pair<data_t::const_iterator, data_t::const_iterator>>
split_ranges(const data_t& data, size_t threads_count)
{
    std::vector<std::pair<data_t::const_iterator, data_t::const_iterator>> ranges;
    size_t whole = data.size() / threads_count;
    size_t rem = data.size() % threads_count;

    auto current = data.begin();

    for (size_t i = 0; i < threads_count; i++) {
        size_t chunk_size = (i < rem) ? whole + 1 : whole;
        auto next = current + chunk_size;
        ranges.push_back({current, next});
        current = next;
    }
    return ranges;
}

value_t sum_range(data_t::const_iterator begin, data_t::const_iterator end)
{
    value_t sum = 0;
    for (auto it = begin; it != end; ++it) {
        sum += *it;
    }
    return sum;
}

value_t parallel_sum(const data_t& data, size_t threads_count)
{
    value_t total_sum = 0;
    auto ranges = split_ranges(data, threads_count);

    std::vector<std::future<value_t>> futures;
    for (const auto& range : ranges) {
        futures.push_back(std::async(std::launch::async, sum_range, range.first, range.second));
    }
    for (auto& f : futures) {
        total_sum += f.get();
    }
    return total_sum;
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <threads_count>\n";
        return 1;
    }

    size_t threads_count = std::stoul(argv[1]);

    constexpr size_t size = 1000000000;
    data_t values(size, 1);
    Clicker cl;

    value_t result = parallel_sum(values, threads_count);
    double elapsed_time = cl.millisec();
    std::cout << threads_count << "," << elapsed_time << "\n";
}