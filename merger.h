#include "commons.h"
#include <math.h>
namespace merger{
    template<typename Reg, typename Item>
    FORCEINLINE void scalarMerge(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C)
    {
		Item* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
        Item a0, a1, a2, a3;

        a0 = a[0];
        a1 = a[1];

        a2 = b[0];
        a3 = b[1];

        a += 2;
        b += 2;
        Item* loadFrom = a;
        Item* opposite = b;

        while(loadFrom != endA && loadFrom != endB)
        {
            bool first = *loadFrom <= *opposite;

            Item* temp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom ;
            loadFrom = temp;    

            SWAP2(0, 2);
            SWAP2(1, 3);
            SWAP2(1, 2);

            c[0] = a0;
            c[1] = a1;
            a0 = loadFrom[0];
            a1 = loadFrom[1];
            c = c  + 2;
            loadFrom = loadFrom + 2;
        }
        SWAP2(0, 2);
        SWAP2(1, 3);
        SWAP2(1, 2);
        c[0] = a0;
        c[1] = a1;
        c = c + 2;
		Item* endOp = (loadFrom == endA) ? endB : endA;
        while (opposite != endOp)
        {
            a0 = opposite[0];
            a1 = opposite[1];
            SWAP2(0, 2); SWAP2(1, 3); SWAP2(1, 2);
			c[0] = a0; c[1] = a1;
            c += 2;
            opposite +=2;
        }
        c[0] = a2;
        c[1] = a3;
        
    }

    template<typename Reg, typename Item>
    FORCEINLINE void nplusmplus1(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C)
    {
		Item* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
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

    template< typename Item>
    FORCEINLINE void addElements(Item* a, ui64 low, ui64 high, Item* c)
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

    template<typename Reg, typename Item>
    FORCEINLINE int binarySearch(Item* a, ui64 low, ui64 high, Item b, ui64 lenA) {
        //std::cout << " Binary Search low : " << low  << " high : " << high  << " b: " << b << std::endl;

        while (low < high) {
            int mid = low + (high - low) / 2;

            bool lh = a[mid] <= b;
            high = lh ? high : mid;
            low = lh ? mid + 1: low;
            bool next = lh && a[mid + 1] > b;
            
            low = next ? mid : low;
            high = next ? low - 1 : high;
        }
        
        return low; // Return low index if not found
    }

    template<typename Reg, typename Item>
    FORCEINLINE void binaryMerge(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C)
    {
        Item* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
        ui64 j = 0;
        ui64 n = lenB;
        ui64 m = lenA;
        for(ui64 i = 1; i <= n ; i++)
        {
            ui64 zbi = i - 1;
            double val = ((m-j)/(n-i+1.0));
            double r = floor(log2(val));
            double s = pow(2.0, r);
            double l = floor((m - j)/s);

            bool continueFlow = true;
            // std::cout << "m " << m << " n " << n  << "  j  " << j << "  ";
            // std::cout <<val <<"_R : " << r << " S : " << s <<  " L : " <<  l << "\n";
    
            ui64 temp = 0;
            for(ui64 k = 1 ; k <= l ; k++)
            {
                ui64 temp = (ui64)(j + k * s);
                ui64 zbtemp = temp - 1;
                if(b[zbi] < a[zbtemp])
                {
                    continueFlow = false;
                    ui64 newJ = binarySearch<Reg, Item>(a, temp - s, temp - 1, b[i], lenA);
                    // std::cout << "B[i] : " << b[zbi] << " j : " << newJ << "\n"; 

                    addElements<Item>(a, j , newJ, c);
                    c += newJ - j; 
                    j = newJ;
                    c[0] = b[zbi];
                    c++;
                    break;
                }
            }
            if(continueFlow)
            {
                ui64 newJ = binarySearch<Reg, Item>(a, temp + 1, m , b[zbi], lenA);
                // std::cout << "B[i] : " << b[zbi] << " j : " << newJ << "\n"; 
                
                addElements<Item>(a, j , newJ, c);
                c += newJ - j; 
                j = newJ;
                c[0]= b[zbi];
                c++;
            }
            if(j >= m)
            {
                addElements<Item>(b, i , n, c);
                break;
            }
            addElements<Item>(a, j , m, c);
            // std::cout << "Value of j : " << j  << '\n'; 
        }
    }

    template<typename Reg, typename Item>
    FORCEINLINE void scalarOddEvenMerge(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C)
    {
        Item* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
        Item a0, a1, a2, a3, a4, a5, a6, a7;

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
        Item* loadFrom = a;
        Item* opposite = b;

        while(loadFrom != endA && loadFrom != endB)
        {
            bool first = *loadFrom <= *opposite;

            Item* temp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom ;
            loadFrom = temp;   

            SWAP2(0,4); SWAP2(1,5); SWAP2(2,6); SWAP2(3,7);
            SWAP2(2,4); SWAP2(3,5);
            SWAP2(1,2); SWAP2(3,4); SWAP2(5,6);
			c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;

			a0 = loadFrom[0]; a1 = loadFrom[1]; a2 = loadFrom[2]; a3 = loadFrom[3];

            c = c + 4;
            loadFrom += 4;
        }

        SWAP2(0,4); SWAP2(1,5); SWAP2(2,6); SWAP2(3,7);
        SWAP2(2,4); SWAP2(3,5);
        SWAP2(1,2); SWAP2(3,4); SWAP2(5,6);
        c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;

        Item* endOp = (loadFrom == endA) ? endB : endA;
		while (opposite != endOp) {

            a0 = opposite[0]; a1 = opposite[1];	a2 = opposite[2]; a3 = opposite[3];
            SWAP2(0,4); SWAP2(1,5); SWAP2(2,6); SWAP2(3,7);
            SWAP2(2,4); SWAP2(3,5);
            SWAP2(1,2); SWAP2(3,4); SWAP2(5,6);
            c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;

            opposite += 4;
			c += 4;
        }

        SWAP2(0,4); SWAP2(1,5); SWAP2(2,6); SWAP2(3,7);
        SWAP2(2,4); SWAP2(3,5);
        SWAP2(1,2); SWAP2(3,4); SWAP2(5,6);
        c[0] = a4; c[1] = a5; c[2] = a6; c[3] = a7;
		c += 4;
    }

    template<typename Reg>
    FORCEINLINE void load(Reg& a0, Reg* ptr)
    {
        a0 = _mm_load_si128(ptr);
    }

    template<typename Reg>
    FORCEINLINE void store(Reg& a0, Reg* ptr)
    {
        _mm_stream_si128(ptr, a0);
    }

    template<typename Reg>
    FORCEINLINE void compareAndSort(Reg& a0 , Reg& a1){
        Reg vTmp = _mm_min_epu32(a0, a1);
        a1 = _mm_max_epu32(a0, a1);
	    a0 = vTmp;
    }

    template<typename Reg>
    FORCEINLINE void loadLowerHalf(Reg& var1 , Reg& var2){
        Reg lowerHalfVar1 = _mm_unpacklo_epi64(var1, var2);
        var2 = _mm_unpackhi_epi64(var1, var2);
        var1 = lowerHalfVar1;
    }
    
    template<typename Reg>
    FORCEINLINE void vectorSort(Reg& x1, Reg& x2)
    {
        compareAndSort<Reg>(x1, x2);
            
        x1 = _mm_alignr_epi8(x1, x1, 8);
        compareAndSort(x1, x2);

        x1 = _mm_alignr_epi8(x1, x1, 8);
        loadLowerHalf(x1, x2);

        x1 = _mm_alignr_epi8(x1, x1, 4);
        compareAndSort(x1, x2);

        x2 = _mm_alignr_epi8(x2, x2, 8);
        __m128i temp  = _mm_blend_epi16(x1, x2, 0x30);
        x2  = _mm_blend_epi16(x2, x1, 0x30);
        x2 = _mm_shuffle_epi32(x2, _MM_SHUFFLE(1,0,2,3));
        x1 = _mm_shuffle_epi32(temp, _MM_SHUFFLE(1,2,0,3));
    }

    void print128i(__m128i var) {
        int32_t *values = (int32_t *)&var;
        std::cout << "Values: ";
        for (int i = 0; i < 4; ++i) {
            std::cout << values[i] << " ";
        }
        std::cout << std::endl;
    }

    template<typename Reg, typename Item>
    FORCEINLINE void vectorizedOddEvenMerge(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C)
    {
		Reg* a = (Reg*)A, * b = (Reg*)B, * endA = (Reg*)(A + lenA), * endB = (Reg*)(B + lenB), * c = (Reg*)C;
		Reg a0, a1;

        // a0 = _mm_setzero_si128();
        // a1 = _mm_setzero_si128();

        load(a0, a);
        load(a1, b);

        a += 1;
        b += 1;
        Reg* loadFrom = a, * opposite = b;

		while (loadFrom != endA && loadFrom != endB) {
			bool first = *(Item*)loadFrom < *(Item*)opposite;

			Reg* tmp = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp;

            vectorSort<Reg>(a0, a1);
            store(a0, c);
            load(a0, loadFrom);

            c += 1;
            loadFrom += 1;
        }

        vectorSort<Reg>(a0, a1);
        store(a0, c);

        c += 1;
		Reg* endOp = (loadFrom == endA) ? endB : endA;

        while (opposite != endOp) {
            load<Reg>(a0, opposite);
            vectorSort<Reg>(a0, a1);
            store<Reg>(a0, c);

            opposite += 1;
			c += 1;
        }
        store<Reg>(a1, c);
    }
} 