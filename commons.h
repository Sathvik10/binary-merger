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

#pragma once

// #include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "sse2neon.h"
// #include <intrin.h>		// MMX
// #include <mmintrin.h>		// MMX
// #include <xmmintrin.h>	// SSE
// #include <emminlstrin.h>	// SSE2
// #include <smmintrin.h>	// SSE4
// #include <immintrin.h>	// AVX
// #include <zmmintrin.h>	// AVX
#include <algorithm>
#include <random> // std::default_random_engine
#include <chrono> // std::chrono::system_clock
#include <assert.h>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <thread>
#include <omp.h>
#include <sys/mman.h>
#include <queue>

using namespace std::chrono;
using hrc = high_resolution_clock;

// typedef key
typedef int64_t i64;
typedef uint64_t ui64;
typedef uint32_t ui;

#define L2_BYTES MB(1)
#define REPEAT 10000

// typedef simd stuff
typedef __m128i sse;
typedef __m128 ssef;
typedef __m128d ssed;

// #define MAX(x, y) ((x)<(y)?(y):(x))
#define FOR(i, n, k) for (ui64(i) = 0; (i) < (n); (i) += (k))
#define MB(x) (x << 20)
#define VALLOC(sz) (mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0))

#define SWAP(x, y)                                        \
	{                                                     \
		{                                                 \
			ui tmp = ((a##x) < (a##y) ? (a##x) : (a##y)); \
			a##y = ((a##x) < (a##y) ? (a##y) : (a##x));   \
			a##x = tmp;                                   \
		}                                                 \
	}
