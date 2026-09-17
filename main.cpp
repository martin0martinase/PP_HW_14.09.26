#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <future>

using data_t = std::vector<unsigned long long>;
using value_t = data_t::value_type;

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

value_t sum_range(data_t::const_iterator begin, data_t::const_iterator end)
{
    value_t sum = 0;
    for (auto it = begin; it != end; ++it) {
        sum += *it;
    }
    return sum;
}

int main(int argc, char* argv[]){
    data_t v = {1, 2, 3, 4, 5};
    std::cout << sum_range(v.begin(), v.end()) << "\n";
}