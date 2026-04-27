#include <thread>
#include <ctime>
#include <iostream>
#include <vector>
#include <cmath>

double **matrix;

void by_row(int i, int m) {
    for(int j = 0; j < m; ++j){
        matrix[i][j] += std::log(std::log(matrix[i][j])) * std::exp2(matrix[i][j]);
    }
}

void by_column(int j, int n) {
    for(int i = 0; i < n; ++i){
        matrix[i][j] += std::log(std::log(matrix[i][j])) * std::exp2(matrix[i][j]);
    }
}

void test(const int n, const int m, void (*fn) (int i, int m)) {

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            matrix[i][j] = i * j + 1;
        }
     }

    std::vector<std::thread> threads;

    for (int i = 0; i < n; ++i) {
        threads.push_back(std::thread(fn, i, m));
    }
    threads.resize(n);

    for (auto& thread: threads) {
        thread.join();
    }
}

int main(int argc, char* argv[]) {
    int n, m, c;

    if (argc < 4) {
        std::cout << "invalid arguments" << std::endl;
        return -1;
    }
    n = std::atoi(argv[1]);
    m = std::atoi(argv[2]);
    c = std::atoi(argv[3]);

    matrix = new double*[n];
    for(int i = 0; i < n; ++i) {
        matrix[i] = new double[m];
    }

    std::clock_t start = std::clock(), dur;

    if (c == 0) {
        test(n, m, by_row);
    } else {
        test(n, m, by_column);
    }

    dur = std::clock() - start;

    std::cout << dur << std::endl;

    for(int i = 0; i < n; ++i) {
        delete []matrix[i];
    }
    delete []matrix;
    return 0;
}
