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
#include "utils.h"
#include "writer.h"
#include "merger.h"
#include <pthread.h>
#include <err.h>
#include <errno.h>
 #include <sched.h>
#include <stdio.h>
#include <stdlib.h>

template <typename Reg, bool stream, typename Item = ui, ui mergeType = 1, ui NREG = 1, ui UNROLL = 1>
void bmerge(ui writer_type = 1) {


	ui64 init_sz = stream ? MB(128) : (L2_BYTES >> 1); 
	ui Itemsize = sizeof(Item);
	ui64 n = init_sz / Itemsize;
	
	constexpr int repeat = stream ? 100 : 1e3;

	// create chunks, align boundary
	ui64 chunk = (n / (UNROLL << 1));
	chunk = (chunk / (sizeof(Reg) * NREG / Itemsize) * (sizeof(Reg) * NREG / Itemsize));
	n = chunk * (UNROLL << 1);

	ui64 sz = n * Itemsize;
	Item* A = (Item*)VALLOC(sz);
	Item* C = (Item*)VALLOC(sz);

	datagen::Writer<Item>  writer;
	writer.generate(A, n, writer_type);
	memset(C, 0, sz);
	sort_every(A, n, chunk);	

	printf("Reg: %lu, Key: %u, NREG: %u, UNROLL: %u\n", sizeof(Reg) << 3, Itemsize << 3, NREG, UNROLL);
	printf("Merging ... ");
	hrc::time_point st = hrc::now();
	FOR(i, repeat, 1) {
		if constexpr (std::is_same<Item, Reg>::value) {
			if constexpr (mergeType == 0)
				merger::nplusmplus1<Reg, Item>(A, chunk, A + chunk, chunk, C);
			else if constexpr (mergeType == 1)
				merger::binaryMerge<Reg, Item>(A, chunk, A + chunk, chunk, C);
			else if constexpr (mergeType == 2)
				merger::scalarOddEvenMerge<Reg, Item>(A, chunk, A + chunk, chunk, C);
			else merger::scalarMerge<Reg, Item>(A, chunk, A + chunk, chunk, C);

		}else{
			merger::vectorizedOddEvenMerge<Reg, Item>(A, chunk, A + chunk, chunk, C);
		}
	}

	hrc::time_point en = hrc::now();
	double el = ELAPSED_MS(st, en);
	double sp = n * repeat / el / 1e3;
	printf("done, elapsed: %.2f ms, Speed: %.2f M/s\n", el, sp);		

	printf("Checking correcntess ... ");
	FOR(i, UNROLL, 1)
		sort_correctness_checker(C + i * (chunk << 1), (chunk << 1));

	printf("done\n");

	VFREE(A, sz);
	VFREE(C, sz);
	PRINT_DASH(50);
}

// Function to print __m128i variable values
void print128i(__m128i var) {
    int32_t *values = (int32_t *)&var;
    std::cout << "Values: ";
    for (int i = 0; i < 4; ++i) {
        std::cout << values[i] << " ";
    }
    std::cout << std::endl;
}

int main() {

	bmerge<Regtype, false, Itemtype, MERGE_TYPE, 1, 1>();
	system("pause");
	return 0;
}