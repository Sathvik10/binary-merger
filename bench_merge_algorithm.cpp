#include "commons.h"
#include "writer.h"
#include "merger.h"
#include "sse_utils.h"

template <int mergeType = 0, bool loadStore = false, long long repeatTimes = (long)1e9>
void bench_merge_algo()
{

    constexpr long repeat = repeatTimes;
    long long n = 0;
    sse a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
    ui *A = (ui *)malloc(64 * sizeof(ui));
    ui *C = (ui *)malloc(64 * sizeof(ui));

    double cycles = 4.8;

    datagen::Writer<ui> writer;
    writer.generate(A, 32, 1);
    sse *a = (sse *)A;
    sse *c = (sse *)C;

    std::cout << "Benchmarking merge type=";
    if constexpr (mergeType == 1)
    {
        n = 16;
        std::cout << " BatcherMerge(16,16) ";
        std::sort(A, A + 16);
        std::sort(A + 16, A + 32);
        origami_merger::load(a0, a);
        origami_merger::load(a1, a + 1);
        origami_merger::load(a2, a + 2);
        origami_merger::load(a3, a + 3);
        origami_merger::load(a4, a + 4);
        origami_merger::load(a5, a + 5);
        origami_merger::load(a6, a + 6);
        origami_merger::load(a7, a + 7);
    }
    else if constexpr (mergeType == 2)
    {
        n = 4;
        std::cout << " Bitonic Merge(4,4) ";
        std::sort(A, A + 4);
        std::sort(A + 4, A + 8);
        origami_merger::load(a0, a);
        origami_merger::load(a1, a + 1);
    }
    else if (mergeType == 3)
    {
        n = 4;
        std::cout << " Rswap Merge(4,4) ";
        std::sort(A, A + 4);
        std::sort(A + 4, A + 8);
        origami_merger::load(a0, a);
        origami_merger::load(a1, a + 1);
    }
    else if (mergeType == 4)
    {
        n = 64;
        std::cout << " Odd Even Merge(32, 32) ";
        std::sort(A, A + 32);
        std::sort(A + 32, A + 64);
        origami_merger::load(a0, a);
        origami_merger::load(a1, a + 1);
        origami_merger::load(a2, a + 2);
        origami_merger::load(a3, a + 3);
        origami_merger::load(a4, a + 4);
        origami_merger::load(a5, a + 5);
        origami_merger::load(a6, a + 6);
        origami_merger::load(a7, a + 7);
        a += 8;
        origami_merger::load(a8, a);
        origami_merger::load(a9, a + 1);
        origami_merger::load(a10, a + 2);
        origami_merger::load(a11, a + 3);
        origami_merger::load(a12, a + 4);
        origami_merger::load(a13, a + 5);
        origami_merger::load(a14, a + 6);
        origami_merger::load(a15, a + 7);
        a -= 8;
    }
    else if constexpr (mergeType == 5)
    {
        n = 48;
        std::cout << " Odd Even Merge(24,24) ";
        std::sort(A, A + 24);
        std::sort(A + 24, A + 48);
        origami_merger::load(a0, a);
        origami_merger::load(a1, a + 1);
        origami_merger::load(a2, a + 2);
        origami_merger::load(a3, a + 3);
        origami_merger::load(a4, a + 4);
        origami_merger::load(a5, a + 5);
        origami_merger::load(a6, a + 6);
        origami_merger::load(a7, a + 7);
        origami_merger::load(a8, a + 8);
        origami_merger::load(a9, a + 9);
        origami_merger::load(a10, a + 10);
        origami_merger::load(a11, a + 11);
    }
    else
    {
        ReportError("Unsupported Algo type\n");
    }
    std::cout << " Repeat: " << repeat << "\n";
    int am = 0;
    hrc::time_point st = hrc::now();
    for (long i = 0; i < repeatTimes; i++)
    {
        if constexpr (mergeType == 1)
        {
            origami_merger::batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);
            if (loadStore)
            {
                origami_merger::store(a0, c);
                origami_merger::store(a1, c + 1);
                origami_merger::store(a2, c + 2);
                origami_merger::store(a3, c + 3);
                origami_merger::store(a4, c + 4);
                origami_merger::store(a5, c + 5);
                origami_merger::store(a6, c + 6);
                origami_merger::store(a7, c + 7);

                origami_merger::load(a0, a);
                origami_merger::load(a1, a + 1);
                origami_merger::load(a2, a + 2);
                origami_merger::load(a3, a + 3);
                origami_merger::load(a4, a + 4);
                origami_merger::load(a5, a + 5);
                origami_merger::load(a6, a + 6);
                origami_merger::load(a7, a + 7);
            }
        }
        else if constexpr (mergeType == 2)
        {
            origami_merger::bitonicMerge(a0, a1);
        }
        else if constexpr (mergeType == 3)
        {
            sse_utils::rswap<ui, sse>(a0, a1);
        }
        else if constexpr (mergeType == 4)
        {
            origami_merger::oddEvenMerge32v32(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
            if constexpr (loadStore)
            {
                origami_merger::store(a0, c);
                origami_merger::store(a1, c + 1);
                origami_merger::store(a2, c + 2);
                origami_merger::store(a3, c + 3);
                origami_merger::store(a4, c + 4);
                origami_merger::store(a5, c + 5);
                origami_merger::store(a6, c + 6);
                origami_merger::store(a7, c + 7);
                c += 8;
                origami_merger::store(a8, c);
                origami_merger::store(a9, c + 1);
                origami_merger::store(a10, c + 2);
                origami_merger::store(a11, c + 3);
                origami_merger::store(a12, c + 4);
                origami_merger::store(a13, c + 5);
                origami_merger::store(a14, c + 6);
                origami_merger::store(a15, c + 7);
                c -= 8;

                origami_merger::load(a0, a);
                origami_merger::load(a1, a + 1);
                origami_merger::load(a2, a + 2);
                origami_merger::load(a3, a + 3);
                origami_merger::load(a4, a + 4);
                origami_merger::load(a5, a + 5);
                origami_merger::load(a6, a + 6);
                origami_merger::load(a7, a + 7);
                a += 8;
                origami_merger::load(a8, a);
                origami_merger::load(a9, a + 1);
                origami_merger::load(a10, a + 2);
                origami_merger::load(a11, a + 3);
                origami_merger::load(a12, a + 4);
                origami_merger::load(a13, a + 5);
                origami_merger::load(a14, a + 6);
                origami_merger::load(a15, a + 7);
                a -= 8;
            }
        }
        else if constexpr (mergeType == 5)
        {
            origami_merger::oddEvenMerge24v24(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
            if (loadStore)
            {
                origami_merger::store(a0, c);
                origami_merger::store(a1, c + 1);
                origami_merger::store(a2, c + 2);
                origami_merger::store(a3, c + 3);
                origami_merger::store(a4, c + 4);
                origami_merger::store(a5, c + 5);
                origami_merger::store(a6, c + 6);
                origami_merger::store(a7, c + 7);
                origami_merger::store(a8, c + 8);
                origami_merger::store(a9, c + 9);
                origami_merger::store(a10, c + 10);
                origami_merger::store(a11, c + 11);

                origami_merger::load(a0, a);
                origami_merger::load(a1, a + 1);
                origami_merger::load(a2, a + 2);
                origami_merger::load(a3, a + 3);
                origami_merger::load(a4, a + 4);
                origami_merger::load(a5, a + 5);
                origami_merger::load(a6, a + 6);
                origami_merger::load(a7, a + 7);
                origami_merger::load(a8, a + 8);
                origami_merger::load(a9, a + 9);
                origami_merger::load(a10, a + 10);
                origami_merger::load(a11, a + 11);
            }
        }
    }
    hrc::time_point en = hrc::now();
    double el = ELAPSED_MS(st, en);
    double sp = (n * repeat) / el / 1e3;
    double noc = (cycles / (sp / 1000)) * n;
    printf("done, elapsed: % .2f ms, Speed : % f M / s, Number of Cycles : % f \n", el, sp, noc);

    if constexpr (mergeType == 1)
    {
        origami_merger::store(a0, a);
        origami_merger::store(a1, a + 1);
        origami_merger::store(a2, a + 2);
        origami_merger::store(a3, a + 3);
        origami_merger::store(a4, a + 4);
        origami_merger::store(a5, a + 5);
        origami_merger::store(a6, a + 6);
        origami_merger::store(a7, a + 7);
        for (int i = 1; i < 32; i++)
        {
            if (A[i - 1] > A[i])
            {
                std::cout << "The output is out of order in batcher merge at index" << i << "\n";
                return;
            }
        }
        std::cout << "Verification Done\n";
    }
    else if constexpr (mergeType == 2 || mergeType == 3)
    {
        origami_merger::store(a0, a);
        origami_merger::store(a1, a + 1);
        for (int i = 1; i < 8; i++)
        {
            if (A[i - 1] > A[i])
            {
                std::cout << "The output is out of order in batcher merge at index" << i << "\n";
                return;
            }
        }
        std::cout << "Verification Done\n";
    }
    std::cout << "\n";
}
int main()
{
    bench_merge_algo<5, false>();
    bench_merge_algo<5, true>();
    return 0;
}