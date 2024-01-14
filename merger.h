#include "commons.h"
#include <math.h>
namespace merger{

    inline void swap(ui &a, ui &b)
    {
        ui temp = std::min(a, b);
        b = std::max(a , b);
        a = temp;
    }

    inline void scalarMerge2Neg(ui* A, ui64 lenA, ui* B, ui64 lenB, ui* C)
    {
		ui* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
        ui a0, a1, a2, a3;

        a0 = a[0];
        a1 = a[1];

        a2 = b[0];
        a3 = b[1];

        a += 2;
        b += 2;
        ui* loadFrom = a;
        ui* opposite = b;

        while(loadFrom != endA && loadFrom != endB)
        {
            bool first = *loadFrom <= *opposite;
            ui* temp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom ;
            loadFrom = temp;  
  
            SWAP(0, 2);
            SWAP(1, 3);
            SWAP(1, 2);

            c[0] = a0;
            c[1] = a1;            
            a0 = loadFrom[0];
            a1 = loadFrom[1];
            c = c  + 2;
            loadFrom = loadFrom + 2;
        }
        SWAP(0, 2);
        SWAP(1, 3);
        SWAP(1, 2);
        c[0] = a0;
        c[1] = a1;
        c = c + 2;
		ui* endOp = (loadFrom == endA) ? endB : endA;
        while (opposite != endOp)
        {
            a0 = opposite[0];
            a1 = opposite[1];
            SWAP(0, 2); SWAP(1, 3); SWAP(1, 2);
			c[0] = a0; c[1] = a1;
            c += 2;
            opposite +=2;
        }
        c[0] = a2;
        c[1] = a3;
        
    }

    inline void scalarMerge1Neg(ui* A, ui64 lenA, ui* B, ui64 lenB, ui* C)
    {
		ui* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;

        while(a != endA && b != endB)
        {
            bool first = *a <= *b;
            c[0] = first ? a[0] : b[0];
            a += first ? 1 : 0;
            b += first ? 0 : 1;
            c += 1;
        }

        ui* endOp = (a == endA) ? endB : endA;
        ui* startOp = (a == endA) ? b : a;
        while (startOp != endOp)
        {
            c[0] = startOp[0];
            c += 1;
            startOp += 1;
        }
    }


    inline void nplusmplus1(ui* A, ui64 lenA, ui* B, ui64 lenB, ui* C)
    {
		ui* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
        while(b != endB)
        {
            while(a != endA)
            {
                if(a[0] <= b[0])
                {
                    c[0] = a[0];
                    c++;
                    a++;
                }
                else{
                    break;
                }
            }
            c[0] = b[0];
            b++;
            c++;
        }
        while( a != endA)
        {
            c[0] = a[0];
            c++;
            a++;
        }
    }

    inline void addElements(ui* a, ui64 low, ui64 high, ui* c)
    {
         // std::cout << " Input to add element is low : " << low  << " high : " << high << std::endl;
        for(ui64 i = low; i < high; i++)
        {
        // std::cout << " Vlaue of a: " << a[i] << std::endl;
            c[0] = a[i];
            c = c + 1;

        }
        // std::cout << std::endl;
    }

    inline int binarySearch(ui* a, ui64 low, ui64 high, ui b, ui64 lenA) {
        //std::cout << "Binary Search low : " << a[low]  << " high : " << a[high]  << " b: " << b << std::endl;

        // while (low < high) {
        //     int mid = low + (high - low) / 2;

        //     bool lh = a[mid] <= b;
        //     high = lh ? high : mid;
        //     low = lh ? mid + 1: low;
        //     bool next = lh && a[mid + 1] > b;
            
        //     low = next ? mid : low;
        //     high = next ? low - 1 : high;
        // }

        while (low < high)
        {
            ui64 mid = low + (high - low) / 2;
            if(a[mid] > b)
            {
                high = mid;
            }
            else if (a[mid] < b)
            {
                low = mid + 1;
            }
            else return mid;
        }  

        //std::cout << "Low: " << a[low] << std::endl;  
        
        return low; // Return low index if not found
    }


    inline void binaryMerge(ui* A, ui64 lenA, ui* B, ui64 lenB, ui* C)
    {
        ui* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
        ui64 j = 0;
        ui64 n = lenB;
        ui64 m = lenA;
        for(ui64 i = 1; i <= n ; i++)
        {
            ui64 zbi = i - 1;
            double val = ((m-j)/(n-i+1.0));

            // Finding Log
            int64_t* num = (int64_t*)&val;
            int64_t exponent = ((*num) >> 52) & 0x7ff;
            int r = (exponent - 1023);

            //double r = floor(log2(val));
            int s = 2 << r; //pow(2.0, r);
            int l = (m - j)/s;

            bool continueFlow = true;
            // std::cout << "m:" << m << " n " << n  << "  j  " << j << " val:";
            // std::cout << val <<" R:" << r << " S:" << s <<  " L:" <<  l << "\n";
    
            ui64 temp = 0;
            for(ui64 k = 1 ; k <= l ; k++)
            {
                ui64 temp = (ui64)(j + k * s);
                ui64 zbtemp = temp - 1;
                if(b[zbi] < a[zbtemp])
                {
                    continueFlow = false;
                    ui64 newJ = binarySearch(a, temp - s, temp - 1, b[zbi], lenA);
                    //std::cout << "BF[i]: " << b[zbi] << " j:" << newJ << " a[newJ]:" << a[newJ] <<"\n"; 

                    memcpy(c, a + j, newJ - j);
                    //addElements(a, j , newJ, c);
                    c += newJ - j; 
                    j = newJ;
                    c[0] = b[zbi];
                    c++;
                    break;
                }
            }
            if(continueFlow)
            {
                ui64 newJ = binarySearch(a, temp + 1, m , b[zbi], lenA);
                //std::cout << "BC[i] :" << b[zbi] << " j :" << newJ << "\n"; 
                
                memcpy(c, a + j, newJ - j);
                //addElements(a, j , newJ, c);
                c += newJ - j; 
                j = newJ;
                c[0]= b[zbi];
                c++;
            }
            if(j >= m)
            {
                memcpy(c, b + i, n - i);
                c += n - i;
                //addElements(b, i , n, c);
                break;
            }
            //memcpy(c, a + j, m - j);
            addElements(a, j , m, c);
            //std::cout << "Value of j:" << j  << '\n'; 
        }
    }

    inline void scalarOddEvenMerge(ui* A, ui64 lenA, ui* B, ui64 lenB, ui* C)
    {
        ui* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
        ui a0, a1, a2, a3, a4, a5, a6, a7;

        a0 = a[0];
        a1 = a[1];

        a2 = a[2];
        a3 = a[3];

        a4 = b[0];
        a5 = b[1]; 
        a6 = b[2];
        a7 = b[3];

        a += 4;
        b += 4;
        ui* loadFrom = a;
        ui* opposite = b;

        while(loadFrom != endA && loadFrom != endB)
        {
            bool first = *loadFrom <= *opposite;

            ui* temp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom ;
            loadFrom = temp;   

            SWAP(0,4); SWAP(1,5); SWAP(2,6); SWAP(3,7);
            SWAP(2,4); SWAP(3,5);
            SWAP(1,2); SWAP(3,4); //SWAP(5,6);
			c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;

			a0 = loadFrom[0]; a1 = loadFrom[1]; a2 = loadFrom[2]; a3 = loadFrom[3];

            c = c + 4;
            loadFrom += 4;
        }

        SWAP(0,4); SWAP(1,5); SWAP(2,6); SWAP(3,7);
        SWAP(2,4); SWAP(3,5);
        SWAP(1,2); SWAP(3,4); //SWAP(5,6);
        c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;
        c += 4;

        ui* endOp = (loadFrom == endA) ? endB : endA;
		while (opposite != endOp) {

            a0 = opposite[0]; a1 = opposite[1];	a2 = opposite[2]; a3 = opposite[3];
            SWAP(0,4); SWAP(1,5); SWAP(2,6); SWAP(3,7);
            SWAP(2,4); SWAP(3,5);
            SWAP(1,2); SWAP(3,4); //SWAP(5,6);
            c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;

            opposite += 4;
			c += 4;
        }

		SWAP(4, 6); SWAP(5, 7); SWAP(4, 5); SWAP(6, 7); SWAP(5, 6);
        c[0] = a4; c[1] = a5; c[2] = a6; c[3] = a7;

    }

    inline void scalarOddEvenMerge(ui* A, ui64 lenA, ui* B, ui64 lenB, ui* D, ui64 lenD, ui* E, ui64 lenE ,ui* C, ui* F)
    {
        ui* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
        ui* d = D, * e = E, * endD = (D + lenD), * endE = (E + lenE), * f = F;
        
        ui a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;

        a0 = a[0];
        a1 = a[1];
        a2 = a[2];
        a3 = a[3];

        a4 = b[0];
        a5 = b[1]; 
        a6 = b[2];
        a7 = b[3];

        a8 = d[0];
        a9 = d[1];
        a10 = d[2];
        a11 = d[3];

        a12 = e[0];
        a13 = e[1]; 
        a14 = e[2];
        a15 = e[3];

        a += 4;
        b += 4;
        d += 4;
        e += 4;

        ui* loadFrom1 = a;
        ui* opposite1 = b;

        ui* loadFrom2 = d;
        ui* opposite2 = e;


        while(loadFrom1 != endA && loadFrom1 != endB && loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *loadFrom1 <= *opposite1;

            ui* temp = first ? loadFrom1 : opposite1;
            opposite1 = first ? opposite1 : loadFrom1 ;
            loadFrom1 = temp;   

            first = *loadFrom2 <= *opposite2;

            temp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2 ;
            loadFrom2 = temp;

            SWAP(0,4); SWAP(1,5); SWAP(2,6); SWAP(3,7);
            SWAP(2,4); SWAP(3,5);
            SWAP(1,2); SWAP(3,4); //SWAP(5,6);
			c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;

            SWAP(8,12); SWAP(9, 13); SWAP(10, 14); SWAP(11 ,15);
            SWAP(10 ,12); SWAP(11, 13);
            SWAP(9, 10); SWAP(11, 12); //SWAP(13, 14);
			f[0] = a8; f[1] = a9; f[2] = a10; f[3] = a11;

			a0 = loadFrom1[0]; a1 = loadFrom1[1]; a2 = loadFrom1[2]; a3 = loadFrom1[3];
			a8 = loadFrom2[0]; a9 = loadFrom2[1]; a10 = loadFrom2[2]; a11 = loadFrom2[3];

            c = c + 4;
            loadFrom1 += 4;
            f = f + 4;
            loadFrom2 += 4;
        }

        while(loadFrom1 != endA && loadFrom1 != endB)
        {
            bool first = *loadFrom1 <= *opposite1;

            ui* temp = first ? loadFrom1 : opposite1;
            opposite1 = first ? opposite1 : loadFrom1 ;
            loadFrom1 = temp;   

            SWAP(0,4); SWAP(1,5); SWAP(2,6); SWAP(3,7);
            SWAP(2,4); SWAP(3,5);
            SWAP(1,2); SWAP(3,4); //SWAP(5,6);
			c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;

			a0 = loadFrom1[0]; a1 = loadFrom1[1]; a2 = loadFrom1[2]; a3 = loadFrom1[3];

            c = c + 4;
            loadFrom1 += 4;
        }


        while(loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *loadFrom2 <= *opposite2;

            ui* temp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2 ;
            loadFrom2 = temp;

            SWAP(8,12); SWAP(9, 13); SWAP(10, 14); SWAP(11 ,15);
            SWAP(10 ,12); SWAP(11, 13);
            SWAP(9, 10); SWAP(11, 12); //SWAP(13, 14);
			f[0] = a8; f[1] = a9; f[2] = a10; f[3] = a11;

			a8 = loadFrom2[0]; a9 = loadFrom2[1]; a10 = loadFrom2[2]; a11 = loadFrom2[3];

            f = f + 4;
            loadFrom2 += 4;
        }

        SWAP(0,4); SWAP(1,5); SWAP(2,6); SWAP(3,7);
        SWAP(2,4); SWAP(3,5);
        SWAP(1,2); SWAP(3,4); SWAP(5,6);
        c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;
        c = c + 4;

        SWAP(8,12); SWAP(9, 13); SWAP(10, 14); SWAP(11 ,15);
        SWAP(10 ,12); SWAP(11, 13);
        SWAP(9, 10); SWAP(11, 12); SWAP(13, 14);
        f[0] = a8; f[1] = a9; f[2] = a10; f[3] = a11;
        f = f + 4;

        ui* endOp = (loadFrom1 == endA) ? endB : endA;
		while (opposite1 != endOp) {

            a0 = opposite1[0]; a1 = opposite1[1];	a2 = opposite1[2]; a3 = opposite1[3];
            SWAP(0,4); SWAP(1,5); SWAP(2,6); SWAP(3,7);
            SWAP(2,4); SWAP(3,5);
            SWAP(1,2); SWAP(3,4); //SWAP(5,6);
            c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;

            opposite1 += 4;
			c += 4;
        }

		SWAP(4, 6); SWAP(5, 7); SWAP(4, 5); SWAP(6, 7); SWAP(5, 6);
        c[0] = a4; c[1] = a5; c[2] = a6; c[3] = a7;

        endOp = (loadFrom2 == endD) ? endE : endD;
		while (opposite2 != endOp) {
            a8 = opposite2[0]; a9 = opposite2[1];	a10 = opposite2[2]; a11 = opposite2[3];
            SWAP(8,12); SWAP(9, 13); SWAP(10, 14); SWAP(11 ,15);
            SWAP(10 ,12); SWAP(11, 13);
            SWAP(9, 10); SWAP(11, 12); //SWAP(13, 14);
            f[0] = a8; f[1] = a9; f[2] = a10; f[3] = a11;

            opposite2 += 4;
			f += 4;
        }

		SWAP(12, 14); SWAP(13, 15); SWAP(12, 13); SWAP(14, 15); SWAP(13, 14);
        f[0] = a12; f[1] = a13; f[2] = a14; f[3] = a15;

    }

    inline void load(sse& a0, sse* ptr)
    {
        a0 = _mm_load_si128(ptr);
    }

    inline void store(sse& a0, sse* ptr)
    {
        _mm_stream_si128(ptr, a0);
    }

    inline void compareAndSort(sse& a0 , sse& a1){
        sse vTmp = _mm_min_epu32(a0, a1);
        a1 = _mm_max_epu32(a0, a1);
	    a0 = vTmp;
    }

    inline void loadLowerHalf(sse& var1 , sse& var2){
        sse lowerHalfVar1 = _mm_unpacklo_epi64(var1, var2);
        var2 = _mm_unpackhi_epi64(var1, var2);
        var1 = lowerHalfVar1;
    }


    void print128i(__m128i var) {
        int32_t *values = (int32_t *)&var;
        std::cout << "Values: ";
        for (int i = 0; i < 4; ++i) {
            std::cout << values[i] << " ";
        }
        std::cout << std::endl;
    }

    // Bitonic Merge
    inline void vectorSort2(sse& x1, sse& x2)
    {   
        x2 = _mm_shuffle_epi32(x2, _MM_SHUFFLE(0, 1, 2, 3));
        __m128i l1l = _mm_min_epu32(x1, x2);
        __m128i l1h  = _mm_max_epu32(x1, x2);
        
        {	
            __m128i l2i = _mm_shuffle_epi32(l1l, _MM_SHUFFLE(1, 0, 3, 2));
            __m128i l2l = _mm_min_epu32(l1l, l2i);
            __m128i l2h = _mm_max_epu32(l1l, l2i);
            l2i = _mm_unpackhi_epi64(l2l, l2h);

            __m128i l3i = _mm_shuffle_epi32(l2i, _MM_SHUFFLE(2, 3, 0, 1));
            __m128i l3l = _mm_min_epu32(l2i, l3i);
            __m128i l3h = _mm_max_epu32(l2i, l3i);

            x1 = _mm_blend_ps(l3l, l3h, 10);
        }

        {
            __m128i l2i = _mm_shuffle_epi32(l1h, _MM_SHUFFLE(1, 0, 3, 2));
            __m128i l2l = _mm_min_epu32(l1h, l2i);
            __m128i l2h = _mm_max_epu32(l1h, l2i);
            l2i = _mm_unpackhi_epi64(l2l, l2h);

            __m128i l3i = _mm_shuffle_epi32(l2i, _MM_SHUFFLE(2, 3, 0, 1));
            __m128i l3l = _mm_min_epu32(l2i, l3i);
            __m128i l3h = _mm_max_epu32(l2i, l3i);

            x2 = _mm_blend_ps(l3l, l3h, 10);
        }
    }
    
    inline void vectorSort(sse& x1, sse& x2)
    {
        compareAndSort(x1, x2);
            
        x1 = _mm_alignr_epi8(x1, x1, 8);
        compareAndSort(x1, x2);

        x1 = _mm_alignr_epi8(x1, x1, 8);
        loadLowerHalf(x1, x2);

        x1 = _mm_alignr_epi8(x1, x1, 4);
        compareAndSort(x1, x2);

        x2 = _mm_alignr_epi8(x2, x2, 8);
        sse temp  = _mm_blend_epi16(x1, x2, 0x30);
        //sse temp = _mm_blend_ps(x1, x2, 4);
        //x2  = _mm_blend_epi16(x2, x1, 0x30);
        x2 = _mm_blend_ps(x2, x1, 4);
        x2 = _mm_shuffle_epi32(x2, _MM_SHUFFLE(1,0,2,3));
        x1 = _mm_shuffle_epi32(temp, _MM_SHUFFLE(1,2,0,3));
    }

    template<int algorithm = 1>
    inline void vectorizedOddEvenMerge(ui* A, ui64 lenA, ui* B, ui64 lenB, ui* C)
    {
		sse* a = (sse*)A, * b = (sse*)B, * endA = (sse*)(A + lenA), * endB = (sse*)(B + lenB), * c = (sse*)C;
		sse a0, a1;

        load(a0, a);
        load(a1, b);

        a += 1;
        b += 1;
        sse* loadFrom = a, * opposite = b;

		while (loadFrom != endA && loadFrom != endB) {
			bool first = *(ui*)loadFrom < *(ui*)opposite;

			sse* tmp = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp;

            if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else vectorSort2(a0, a1);

            store(a0, c);
            load(a0, loadFrom);

            c += 1;
            loadFrom += 1;
        }

        if constexpr (algorithm == 1)
            vectorSort(a0, a1);
        else vectorSort2(a0, a1);
        store(a0, c);

        c += 1;
		sse* endOp = (loadFrom == endA) ? endB : endA;

        while (opposite != endOp) {
            load(a0, opposite);
            if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else vectorSort2(a0, a1);

            store(a0, c);

            opposite += 1;
			c += 1;
        }
        store(a1, c);
    }


    template<int algorithm = 1>
    inline void vectorizedOddEvenMergeWithSplit(ui* A, ui64 lenA, ui* B, ui64 lenB,ui* D,ui64 lenD, ui* E , ui64 lenE, ui* C, ui* F)
    {
		sse* a = (sse*)A, * b = (sse*)B, * endA = (sse*)(A + lenA), * endB = (sse*)(B + lenB), * c = (sse*)C;
		sse* d = (sse*)D, * e = (sse*)E, * endD = (sse*)(D + lenD), * endE = (sse*)(E + lenE), * f = (sse*)F;

		sse a0, a1, a2, a3;

        load(a0, a);
        load(a1, b);
        load(a2, d);
        load(a3, e);

        a += 1;
        b += 1;
        d += 1;
        e += 1;

        sse* loadFrom = a, * opposite = b;
        sse* loadFrom2 = d, * opposite2 = e;

        while(loadFrom != endA && loadFrom != endB && loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *(ui*)loadFrom < *(ui*)opposite;
			sse* tmp = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp; 

            first =  *(ui*)loadFrom2 < *(ui*)opposite2;
            tmp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = tmp;

             if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else vectorSort2(a0, a1);
            store(a0, c);
            load(a0, loadFrom);

            c += 1;
            loadFrom += 1;

            if constexpr (algorithm == 1)
                vectorSort(a2, a3);
            else vectorSort2(a2, a3);
            store(a2, f);
            load(a2, loadFrom2);

            f += 1;
            loadFrom2 += 1;
        }

		while (loadFrom != endA && loadFrom != endB) {
			bool first = *(ui*)loadFrom < *(ui*)opposite;

			sse* tmp = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp;

            if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else vectorSort2(a0, a1);
            store(a0, c);
            load(a0, loadFrom);

            c += 1;
            loadFrom += 1;
        }

        while(loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *(ui*)loadFrom2 < *(ui*)opposite2;
            sse* tmp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = tmp;

            if constexpr (algorithm == 1)
                vectorSort(a2, a3);
            else vectorSort2(a2, a3);
            store(a2, f);
            load(a2, loadFrom2);

            f += 1;
            loadFrom2 += 1;
        }
        
        // Sort the remaining items of the first split
        if constexpr (algorithm == 1)
            vectorSort(a0, a1);
        else vectorSort2(a0, a1);
        store(a0, c);

        c += 1;
		sse* endOp = (loadFrom == endA) ? endB : endA;

        while (opposite != endOp) {
            load(a0, opposite);
            if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else vectorSort2(a0, a1);
            store(a0, c);

            opposite += 1;
			c += 1;
        }
        store(a1, c);

        // Sort the remaining items of the second split
        if constexpr (algorithm == 1)
            vectorSort(a2, a3);
        else vectorSort2(a2, a3);

        store(a2, f);

        f += 1;
		endOp = (loadFrom2 == endD) ? endE : endD;

        while (opposite2 != endOp) {
            load(a2, opposite2);
            if constexpr (algorithm == 1)
                vectorSort(a2, a3);
            else vectorSort2(a2, a3);
            store(a2, f);

            opposite2 += 1;
			f += 1;
        }
        store(a3, f);
    }
} 