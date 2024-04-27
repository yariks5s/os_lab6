#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

void multiplyRowByColumn(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C, int row, int col, int m)
{
    int sum = 0;
    for (int i = 0; i < m; ++i)
        sum += A[row][i] * B[i][col];

    C[row][col] = sum;

    // Вивід результату обчислення з допомогою м'ютексу для уникнення гонки
    static std::mutex io_mutex;
    std::lock_guard<std::mutex> lock(io_mutex);
    std::cout << "[" << row << "," << col << "]=" << sum << std::endl;
}

void parallelMatrixMultiplication(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C, int n, int m, int k)
{
    std::vector<std::thread> threads;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < k; ++j)
            threads.emplace_back(multiplyRowByColumn, std::cref(A), std::cref(B), std::ref(C), i, j, m);
    }
    for (auto& thread : threads)
        thread.join();
}

int main()
{
    int n = 2, m = 3, k = 2;
    std::vector<std::vector<int>> A = {{1, 2, 3}, {4, 5, 6}};
    std::vector<std::vector<int>> B = {{7, 8}, {9, 10}, {11, 12}};
    std::vector<std::vector<int>> C(n, std::vector<int>(k, 0));

    auto start = std::chrono::steady_clock::now();
    parallelMatrixMultiplication(A, B, C, n, m, k);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Total time: " << duration << " ns\n";
    return 0;
}
