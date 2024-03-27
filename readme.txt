The Objective of the repository is to implement an optimized External Memory merge sort. The focus is on improving the merge operation. Multiple merge algorithms are experimented such as Bitonic Merge, Batcher's Odd Even Merge and Rotate and Swap. Some of the operations are implemented both in scalar and vector forms. The comparisons of vector results are shown below.

——————————————— Benchmark ———————————————
Generating data with : Mersenne - Twister ... done
Benchmarking merge type = BatcherMerge(16, 16)  Repeat : 1000000000
done, elapsed : 5450.92 ms, Speed : 2935.284919 M / s
Verification Done

Generating data with : Mersenne - Twister ... done
Benchmarking merge type = Bitonic Merge(4, 4)  Repeat : 1000000000
done, elapsed : 2717.50 ms, Speed : 1471.943585 M / s
Verification Done

Generating data with : Mersenne - Twister ... done
Benchmarking merge type = Rswap Merge(4, 4)  Repeat : 1000000000
done, elapsed : 1807.56 ms, Speed : 2212.930453 M / s
Verification Done
—————————————————————————————————————————————

——————————————— Batcher Odd even Merge optimized ———————————————
Generating data with : Mersenne - Twister ... done
Reg : 128, Key : 32, NREG : 1, UNROLL : 1 Size : 32768
Merging ...
done, elapsed : 811.11 ms, Speed : 4039.89 M / s
Checking correcntess ... done
———————————————— For different forms of data input —————
All Same - 5668.96
Sorted Seq - 2869.96
Reverse Sorted - 2887.14
Almost Sorted - 4425.43
Pareto Keys - 5084.56
Pareto-b2b - 2760.61
Pareto-Shuffled - 5063.42
Fibonacci Sequence - 5312.75
Fibonacci Sequence2 - 5585.33
—————————————————————————————————————————————


Speeds are in M/S. Below are the results with respect to Batcher Odd even Merge
——————————————— Data Size ———————————————
256 KB - 4039.89
1 MB - 2500.85
10 MB - 2279.33
20 MB - 2282.82
30 MB - 2289.49
40 MB - 2279.34
50 MB - 2268.45
60 Mb - 2266.94
————————————————————————————————————



——————————————— Batcher Merge optimized Unroll ———————————————
Generating data with : Mersenne - Twister ... done
Reg : 128, Key : 32, NREG : 1, UNROLL : 2 Size : 32768 (256 KB)
Merging ...
done, elapsed : 868.35 ms, Speed : 3773.60 M / s
Checking correcntess ... done
——————————————————————————————————————————	

1 MB - 2722.24 M/s
20 MB - 2161.64 M/S

---------------------------------------------------

Number of cycles for each operation:

32v32 gave 85 cycles. (Just the merge operation)

16v16 gave 26 cycles. (Just the merge operation)

32v32 along with load and store gave 315 cycles. [includes 16 load and 16 store]

16v16 along with load and store gave 114 cycles. [includes 8 loads and 8 stores].

24v24 gave 68 cycles. (Just the merge operation)

24v24 along with load and store gave 120 cycles. [includes 12 loads and 12 stores].