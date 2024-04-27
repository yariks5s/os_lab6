#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

void multiplyPart(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C, int start, int end, int m, int k)
{
    for (int i = start; i < end; ++i)
    {
        for (int j = 0; j < k; ++j)
        {
            int sum = 0;
            for (int t = 0; t < m; ++t)
                sum += A[i][t] * B[t][j];

            C[i][j] = sum;
        }
    }
}

void parallelMatrixMultiplication(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C, int n, int m, int k, int num_threads)
{
    std::vector<std::thread> threads;
    int rows_per_thread = n / num_threads;
    int extra_rows = n % num_threads;

    int start_row = 0;
    for (int i = 0; i < num_threads; ++i)
    {
        int rows_to_handle = rows_per_thread + (i < extra_rows ? 1 : 0);
        threads.emplace_back(multiplyPart, std::cref(A), std::cref(B), std::ref(C), start_row, start_row + rows_to_handle, m, k);
        start_row += rows_to_handle;
    }

    for (auto& thread : threads)
        thread.join();
}

int main()
{
    int n = 100, m = 100, k = 100;
    std::vector<std::vector<int>> A(n, std::vector<int>(m, 1));
    std::vector<std::vector<int>> B(m, std::vector<int>(k, 1));
    std::vector<std::vector<int>> C(n, std::vector<int>(k, 0));

    for (int num_threads = 1; num_threads <= 16; num_threads++) /// Тестування для різної кількості потоків
    {
        auto start = std::chrono::steady_clock::now();
        parallelMatrixMultiplication(A, B, C, n, m, k, num_threads);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Threads: " << num_threads << " Time: " << duration << " µs\n";
    }

    return 0;
}
