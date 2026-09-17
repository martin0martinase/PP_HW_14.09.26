#include<iostream>
#include <chrono>

using data_t = std::vector<unsigned long long>;
using value_t = data_t::value_type;

void howDivVec(int** vec,int* size, int* threads_count){
    int whole = 0;
    int rem = 0;
    whole = *size / *threads_count;
    rem = *size % *threads_count;

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
    std::cout << sum_range(v.begin(), v.begin() + 2) << "\n";
}