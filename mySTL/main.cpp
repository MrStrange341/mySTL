#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "algorithm.h"

std::vector<int> init(int n) {
    std::mt19937 gen;
    std::vector<int> res(n);
    for (int i = 0; i < n; ++i) {
        res[i] = gen() % 1000000;
    }
    return res;
}

void sorttest(const int& times, std::vector<int> a) {
    std::vector<double> avg;

    for (int i = 0; i < times; ++i) {

        auto start = std::chrono::high_resolution_clock::now();

        // 执行函数
        mystl::sort(a.begin(), a.end());

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;

        avg.push_back(duration.count());
    }

    double avr = 0;

    for (auto& n : avg) {
        avr += n;
    }

    avr /= times;

    std::cout << "mystl::sort took " << avr * 1000 << " ms." << std::endl;
}

void stabletest(const int& times, std::vector<int> a) {
    std::vector<double> avg;

    for (int i = 0; i < times; ++i) {

        auto start = std::chrono::high_resolution_clock::now();

        // 执行函数
        mystl::stable_sort(a.begin(), a.end());

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;

        avg.push_back(duration.count());
    }

    double avr = 0;

    for (auto& n : avg) {
        avr += n;
    }

    avr /= times;

    std::cout << "mystl::stable_sort took " << avr * 1000 << " ms." << std::endl;
}

void ssorttest(const int& times, std::vector<int> a) {
    std::vector<double> avg;

    for (int i = 0; i < times; ++i) {

        auto start = std::chrono::high_resolution_clock::now();

        // 执行函数
        std::sort(a.begin(), a.end());

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;

        avg.push_back(duration.count());
    }

    double avr = 0;

    for (auto& n : avg) {
        avr += n;
    }

    avr /= times;

    std::cout << "std::sort took " << avr * 1000 << " ms." << std::endl;
}

void sstabletest(const int& times, std::vector<int> a) {
    std::vector<double> avg;

    for (int i = 0; i < times; ++i) {

        auto start = std::chrono::high_resolution_clock::now();

        // 执行函数
        std::stable_sort(a.begin(), a.end());

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;

        avg.push_back(duration.count());
    }

    double avr = 0;

    for (auto& n : avg) {
        avr += n;
    }

    avr /= times;

    std::cout << "std::stable_sort took " << avr * 1000 << " ms." << std::endl;
}

int main()
{
    std::vector<int> v = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9 };

    std::vector<int> x = { 1, 2, 3, 0, 0, 0, 0, 0 };

    std::vector<int> v0 = { 4, 2, 5, 1, 3 };

    //std::vector<int> v1{ 11, 12, 13 };
    //std::vector<int> v2{ 10, 11, 12, 13, 14, 15, 16 };
    std::cout << mystl::max({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    //std::vector<int> a = init(100000000);
    //std::vector<int> b = a;
    //std::vector<int> c = a;
    //std::vector<int> d = a;

    //std::cout << mystl::includes(v2.begin(), v2.end(), v1.begin(), v1.end());


    return 0;
}