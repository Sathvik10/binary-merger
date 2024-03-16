/*--------------------------------------------------------------------------------------------
 - Origami: A High-Performance Mergesort Framework											 -
 - Copyright(C) 2021 Arif Arman, Dmitri Loguinov											 -
 - Produced via research carried out by the Texas A&M Internet Research Lab                  -
 -                                                                                           -
 - This program is free software : you can redistribute it and/or modify                     -
 - it under the terms of the GNU General Public License as published by                      -
 - the Free Software Foundation, either version 3 of the License, or                         -
 - (at your option) any later version.                                                       -
 -                                                                                           -
 - This program is distributed in the hope that it will be useful,                           -
 - but WITHOUT ANY WARRANTY; without even the implied warranty of                            -
 - MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                               -
 - GNU General Public License for more details.                                              -
 -                                                                                           -
 - You should have received a copy of the GNU General Public License                         -
 - along with this program. If not, see < http://www.gnu.org/licenses/>.                     -
 --------------------------------------------------------------------------------------------*/

#include "commons.h"
#include "merger.h"
#include <pthread.h>
#include <err.h>
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

// Function to print __m128i variable values
void print128i(__m128i var)
{
	int32_t *values = (int32_t *)&var;
	std::cout << "Values: ";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << values[i] << " ";
	}
	std::cout << std::endl;
}

void randomNumberGenerator(ui *array, ui64 numberOfItems)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<ui> distribution;

	// Generate random numbers
	for (ui64 i = 0; i < numberOfItems; ++i)
	{
		array[i] = distribution(gen);
	}
}

void sort_splits(ui *src, ui64 size, ui ns)
{
	ui64 split_size = size / ns;
#pragma omp parallel for
	for (int i = 0; i < size; i += split_size)
	{
		std::sort(src + i, src + i + split_size);
	}
}

void correctnessChecked(ui *a, ui64 n)
{
	for (int i = 1; i < n; i++)
	{
		if (a[i - 1] > a[i])
		{
			std::cout << "Out of order. Index: " << i << " Size: " << n << std::endl;
			return;
		}
	}
}

template <bool scalar = true, int mergeType = 0, ui split = 1, ui64 size = L2_BYTES>
void merge()
{
	printf("--------------------------------------------------------------\n");
	ui itemSize = sizeof(ui);
	ui64 number_of_items = size / itemSize;

	ui number_of_splits = split << 1;

	ui *src = (ui *)VALLOC(size);
	ui *dest = (ui *)VALLOC(size);
	ui *copy = (ui *)VALLOC(size);

	// Generate the data
	randomNumberGenerator(src, number_of_items);

	memset(dest, 0, size);

	for (ui64 i = 0; i < number_of_items; i++)
	{
		copy[i] = src[i];
	}
	std::sort(copy, copy + number_of_items);

	// Start the Merge
	int repeat = REPEAT;
	double totalTime = 0.0;

	ui64 split_size = number_of_items / number_of_splits;

	// Sort the splits
	sort_splits(src, number_of_items, number_of_splits);

	printf("Config: Scalar %d : Merge Type %d : split %d\n", scalar, mergeType, split);

	for (int i = 0; i < repeat; i++)
	{
		hrc::time_point startTime = hrc::now();
		// Merge Algorithms
		if constexpr (scalar)
		{
			if constexpr (split == 1)
			{
				if constexpr (mergeType == 0)
					merger::nplusmplus1(src, split_size, src + split_size, split_size, dest);
				else if constexpr (mergeType == 1)
					merger::binaryMerge(src, split_size, src + split_size, split_size, dest);
				else if constexpr (mergeType == 2)
					merger::scalarOddEvenMerge(src, split_size, src + split_size, split_size, dest);
				else if constexpr (mergeType == 3)
					merger::scalarMerge1Neg(src, split_size, src + split_size, split_size, dest);
				else
					merger::scalarMerge2Neg(src, split_size, src + split_size, split_size, dest);
			}
			else if constexpr (split == 2)
			{
				if constexpr (mergeType == 2)
					merger::scalarOddEvenMerge(src, split_size, src + split_size, split_size, src + 2 * split_size,
											   split_size, src + 3 * split_size, split_size, dest, dest + 2 * split_size);
			}
		}
		else
		{
			if constexpr (split == 1)
			{
				if constexpr (mergeType == 4)
				{
					merger::rotateAndSwap8(src, split_size, src + split_size, split_size, dest);
				}
				else if constexpr (mergeType == 5)
				{
					merger::vectorBatcherMergeOptimized(src, split_size, src + split_size, split_size, dest);
				}
				else
				{
					merger::vectorizedOddEvenMerge<mergeType>(src, split_size, src + split_size, split_size, dest);
				}
			}
			else if constexpr (split == 2)
			{
				if constexpr (mergeType == 5)
				{
					merger::vectorBatcherMergeOptimizedWithSplit(src, split_size, src + split_size, split_size, src + 2 * split_size,
																 split_size, src + 3 * split_size, split_size, dest, dest + 2 * split_size);
				}
				else
				{
					merger::vectorizedOddEvenMergeWithSplit<mergeType>(src, split_size, src + split_size, split_size, src + 2 * split_size,
																	   split_size, src + 3 * split_size, split_size, dest, dest + 2 * split_size);
				}
			}
		}
		hrc::time_point endTime = hrc::now();

		totalTime += duration_cast<duration<double, std::milli>>(endTime - startTime).count();
	}
	for (int j = 0; j < split; j++)
		correctnessChecked(dest + j * 2 * split_size, 2 * split_size);

	// for (int i = 0 ; i < number_of_items ; i++)
	// {
	// 	printf("%u %u\n", *(dest + i), *(copy+i));
	// }

	double speed = number_of_items * repeat / totalTime / 1e3;
	printf("done, elapsed: %.2f ms, Speed: %.2f M/s\n", totalTime, speed);
	printf("--------------------------------------------------------------\n");

	// Check correctness
}

int main()
{

	// First Para: true for scalar
	// Second Para: Type of algorithm:
	// 0: nplusmplus1 1: binaryMerge 2: scalarOddEvenMerge 3: Optimised scalarMerge-1Reg (Origami) 4: scalarMerge-2Reg (Origami Merge)
	// Third Para: number of splits (1 or 2) supported

	// Vector
	// mergeType = 1: OddEven 2: Bitonic 3: Rotate and Swap 5: Batcher's Odd Even Merge
	// MergeType
	merge<false, 5, 1>();

	return 0;
}