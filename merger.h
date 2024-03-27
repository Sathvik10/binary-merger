#include "commons.h"
#include <math.h>
namespace merger
{
    void print128i(__m128i var)
    {
        uint32_t *values = (uint32_t *)&var;
        for (int i = 0; i < 4; ++i)
        {
            std::cout << values[i] << " ";
        }
    }
    template <typename... Args>
    void print128(Args... args)
    {
        std::cout << "Values: ";
        (void)std::initializer_list<int>{((void)print128i(args), 0)...};
        std::cout << std::endl;
    }

    void print128i(__m128i var1, __m128i var2, __m128i var3, __m128i var4, __m128i var5, __m128i var6, __m128i var7, __m128i var8)
    {
        int32_t *values = (int32_t *)&var1;
        std::cout << "Values: ";
        for (int i = 0; i < 4; ++i)
        {
            std::cout << values[i] << " ";
        }

        values = (int32_t *)&var2;
        for (int i = 0; i < 4; ++i)
        {
            std::cout << values[i] << " ";
        }
        values = (int32_t *)&var3;
        for (int i = 0; i < 4; ++i)
        {
            std::cout << values[i] << " ";
        }
        values = (int32_t *)&var4;
        for (int i = 0; i < 4; ++i)
        {
            std::cout << values[i] << " ";
        }
        values = (int32_t *)&var5;
        for (int i = 0; i < 4; ++i)
        {
            std::cout << values[i] << " ";
        }
        values = (int32_t *)&var6;
        for (int i = 0; i < 4; ++i)
        {
            std::cout << values[i] << " ";
        }
        values = (int32_t *)&var7;
        for (int i = 0; i < 4; ++i)
        {
            std::cout << values[i] << " ";
        }
        values = (int32_t *)&var8;
        for (int i = 0; i < 4; ++i)
        {
            std::cout << values[i] << " ";
        }
        std::cout << std::endl;
    }

    inline void swap(ui &a, ui &b)
    {
        ui temp = std::min(a, b);
        b = std::max(a, b);
        a = temp;
    }

    inline void scalarMerge2Neg(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        ui *a = A, *b = B, *endA = (A + lenA), *endB = (B + lenB), *c = C;
        ui a0, a1, a2, a3;

        a0 = a[0];
        a1 = a[1];

        a2 = b[0];
        a3 = b[1];

        a += 2;
        b += 2;
        ui *loadFrom = a;
        ui *opposite = b;

        while (loadFrom != endA && loadFrom != endB)
        {
            bool first = *loadFrom <= *opposite;
            ui *temp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = temp;

            SWAP(0, 2);
            SWAP(1, 3);
            SWAP(1, 2);

            c[0] = a0;
            c[1] = a1;
            a0 = loadFrom[0];
            a1 = loadFrom[1];
            c = c + 2;
            loadFrom = loadFrom + 2;
        }
        SWAP(0, 2);
        SWAP(1, 3);
        SWAP(1, 2);
        c[0] = a0;
        c[1] = a1;
        c = c + 2;
        ui *endOp = (loadFrom == endA) ? endB : endA;
        while (opposite != endOp)
        {
            a0 = opposite[0];
            a1 = opposite[1];
            SWAP(0, 2);
            SWAP(1, 3);
            SWAP(1, 2);
            c[0] = a0;
            c[1] = a1;
            c += 2;
            opposite += 2;
        }
        c[0] = a2;
        c[1] = a3;
    }

    inline void scalarMerge1Neg(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        ui *a = A, *b = B, *endA = (A + lenA), *endB = (B + lenB), *c = C;

        while (a != endA && b != endB)
        {
            bool first = *a <= *b;
            c[0] = first ? a[0] : b[0];
            a += first ? 1 : 0;
            b += first ? 0 : 1;
            c += 1;
        }

        ui *endOp = (a == endA) ? endB : endA;
        ui *startOp = (a == endA) ? b : a;
        while (startOp != endOp)
        {
            c[0] = startOp[0];
            c += 1;
            startOp += 1;
        }
    }

    inline void nplusmplus1(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        ui *a = A, *b = B, *endA = (A + lenA), *endB = (B + lenB), *c = C;
        while (b != endB)
        {
            while (a != endA)
            {
                if (a[0] <= b[0])
                {
                    c[0] = a[0];
                    c++;
                    a++;
                }
                else
                {
                    break;
                }
            }
            c[0] = b[0];
            b++;
            c++;
        }
        while (a != endA)
        {
            c[0] = a[0];
            c++;
            a++;
        }
    }

    inline void addElements(ui *a, ui64 low, ui64 high, ui *c)
    {
        // std::cout << " Input to add element is low : " << low  << " high : " << high << std::endl;
        for (ui64 i = low; i < high; i++)
        {
            // std::cout << " Vlaue of a: " << a[i] << std::endl;
            c[0] = a[i];
            c = c + 1;
        }
        // std::cout << std::endl;
    }

    inline int binarySearch(ui *a, ui64 low, ui64 high, ui b, ui64 lenA)
    {
        // std::cout << "Binary Search low : " << a[low]  << " high : " << a[high]  << " b: " << b << std::endl;

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
            if (a[mid] > b)
            {
                high = mid;
            }
            else if (a[mid] < b)
            {
                low = mid + 1;
            }
            else
                return mid;
        }

        // std::cout << "Low: " << a[low] << std::endl;

        return low; // Return low index if not found
    }

    inline void binaryMerge(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        ui *a = A, *b = B, *endA = (A + lenA), *endB = (B + lenB), *c = C;
        ui64 j = 0;
        ui64 n = lenB;
        ui64 m = lenA;
        for (ui64 i = 1; i <= n; i++)
        {
            ui64 zbi = i - 1;
            double val = ((m - j) / (n - i + 1.0));

            // Finding Log
            int64_t *num = (int64_t *)&val;
            int64_t exponent = ((*num) >> 52) & 0x7ff;
            int r = (exponent - 1023);

            // double r = floor(log2(val));
            int s = 2 << r; // pow(2.0, r);
            int l = (m - j) / s;

            bool continueFlow = true;
            // std::cout << "m:" << m << " n " << n  << "  j  " << j << " val:";
            // std::cout << val <<" R:" << r << " S:" << s <<  " L:" <<  l << "\n";

            ui64 temp = 0;
            for (ui64 k = 1; k <= l; k++)
            {
                ui64 temp = (ui64)(j + k * s);
                ui64 zbtemp = temp - 1;
                if (b[zbi] < a[zbtemp])
                {
                    continueFlow = false;
                    ui64 newJ = binarySearch(a, temp - s, temp - 1, b[zbi], lenA);
                    // std::cout << "BF[i]: " << b[zbi] << " j:" << newJ << " a[newJ]:" << a[newJ] <<"\n";

                    memcpy(c, a + j, newJ - j);
                    // addElements(a, j , newJ, c);
                    c += newJ - j;
                    j = newJ;
                    c[0] = b[zbi];
                    c++;
                    break;
                }
            }
            if (continueFlow)
            {
                ui64 newJ = binarySearch(a, temp + 1, m, b[zbi], lenA);
                // std::cout << "BC[i] :" << b[zbi] << " j :" << newJ << "\n";

                memcpy(c, a + j, newJ - j);
                // addElements(a, j , newJ, c);
                c += newJ - j;
                j = newJ;
                c[0] = b[zbi];
                c++;
            }
            if (j >= m)
            {
                memcpy(c, b + i, n - i);
                c += n - i;
                // addElements(b, i , n, c);
                break;
            }
            // memcpy(c, a + j, m - j);
            addElements(a, j, m, c);
            // std::cout << "Value of j:" << j  << '\n';
        }
    }

    inline void scalarOddEvenMerge(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        ui *a = A, *b = B, *endA = (A + lenA), *endB = (B + lenB), *c = C;
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
        ui *loadFrom = a;
        ui *opposite = b;

        while (loadFrom != endA && loadFrom != endB)
        {
            bool first = *loadFrom <= *opposite;

            ui *temp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = temp;

            SWAP(0, 4);
            SWAP(1, 5);
            SWAP(2, 6);
            SWAP(3, 7);
            SWAP(2, 4);
            SWAP(3, 5);
            SWAP(1, 2);
            SWAP(3, 4); // SWAP(5,6);
            c[0] = a0;
            c[1] = a1;
            c[2] = a2;
            c[3] = a3;

            a0 = loadFrom[0];
            a1 = loadFrom[1];
            a2 = loadFrom[2];
            a3 = loadFrom[3];

            c = c + 4;
            loadFrom += 4;
        }

        SWAP(0, 4);
        SWAP(1, 5);
        SWAP(2, 6);
        SWAP(3, 7);
        SWAP(2, 4);
        SWAP(3, 5);
        SWAP(1, 2);
        SWAP(3, 4); // SWAP(5,6);
        c[0] = a0;
        c[1] = a1;
        c[2] = a2;
        c[3] = a3;
        c += 4;

        ui *endOp = (loadFrom == endA) ? endB : endA;
        while (opposite != endOp)
        {

            a0 = opposite[0];
            a1 = opposite[1];
            a2 = opposite[2];
            a3 = opposite[3];
            SWAP(0, 4);
            SWAP(1, 5);
            SWAP(2, 6);
            SWAP(3, 7);
            SWAP(2, 4);
            SWAP(3, 5);
            SWAP(1, 2);
            SWAP(3, 4); // SWAP(5,6);
            c[0] = a0;
            c[1] = a1;
            c[2] = a2;
            c[3] = a3;

            opposite += 4;
            c += 4;
        }

        SWAP(4, 6);
        SWAP(5, 7);
        SWAP(4, 5);
        SWAP(6, 7);
        SWAP(5, 6);
        c[0] = a4;
        c[1] = a5;
        c[2] = a6;
        c[3] = a7;
    }

    inline void scalarOddEvenMerge(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *D, ui64 lenD, ui *E, ui64 lenE, ui *C, ui *F)
    {
        ui *a = A, *b = B, *endA = (A + lenA), *endB = (B + lenB), *c = C;
        ui *d = D, *e = E, *endD = (D + lenD), *endE = (E + lenE), *f = F;

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

        ui *loadFrom1 = a;
        ui *opposite1 = b;

        ui *loadFrom2 = d;
        ui *opposite2 = e;

        while (loadFrom1 != endA && loadFrom1 != endB && loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *loadFrom1 <= *opposite1;

            ui *temp = first ? loadFrom1 : opposite1;
            opposite1 = first ? opposite1 : loadFrom1;
            loadFrom1 = temp;

            first = *loadFrom2 <= *opposite2;

            temp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = temp;

            SWAP(0, 4);
            SWAP(1, 5);
            SWAP(2, 6);
            SWAP(3, 7);
            SWAP(2, 4);
            SWAP(3, 5);
            SWAP(1, 2);
            SWAP(3, 4); // SWAP(5,6);
            c[0] = a0;
            c[1] = a1;
            c[2] = a2;
            c[3] = a3;

            SWAP(8, 12);
            SWAP(9, 13);
            SWAP(10, 14);
            SWAP(11, 15);
            SWAP(10, 12);
            SWAP(11, 13);
            SWAP(9, 10);
            SWAP(11, 12); // SWAP(13, 14);
            f[0] = a8;
            f[1] = a9;
            f[2] = a10;
            f[3] = a11;

            a0 = loadFrom1[0];
            a1 = loadFrom1[1];
            a2 = loadFrom1[2];
            a3 = loadFrom1[3];
            a8 = loadFrom2[0];
            a9 = loadFrom2[1];
            a10 = loadFrom2[2];
            a11 = loadFrom2[3];

            c = c + 4;
            loadFrom1 += 4;
            f = f + 4;
            loadFrom2 += 4;
        }

        while (loadFrom1 != endA && loadFrom1 != endB)
        {
            bool first = *loadFrom1 <= *opposite1;

            ui *temp = first ? loadFrom1 : opposite1;
            opposite1 = first ? opposite1 : loadFrom1;
            loadFrom1 = temp;

            SWAP(0, 4);
            SWAP(1, 5);
            SWAP(2, 6);
            SWAP(3, 7);
            SWAP(2, 4);
            SWAP(3, 5);
            SWAP(1, 2);
            SWAP(3, 4); // SWAP(5,6);
            c[0] = a0;
            c[1] = a1;
            c[2] = a2;
            c[3] = a3;

            a0 = loadFrom1[0];
            a1 = loadFrom1[1];
            a2 = loadFrom1[2];
            a3 = loadFrom1[3];

            c = c + 4;
            loadFrom1 += 4;
        }

        while (loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *loadFrom2 <= *opposite2;

            ui *temp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = temp;

            SWAP(8, 12);
            SWAP(9, 13);
            SWAP(10, 14);
            SWAP(11, 15);
            SWAP(10, 12);
            SWAP(11, 13);
            SWAP(9, 10);
            SWAP(11, 12); // SWAP(13, 14);
            f[0] = a8;
            f[1] = a9;
            f[2] = a10;
            f[3] = a11;

            a8 = loadFrom2[0];
            a9 = loadFrom2[1];
            a10 = loadFrom2[2];
            a11 = loadFrom2[3];

            f = f + 4;
            loadFrom2 += 4;
        }

        SWAP(0, 4);
        SWAP(1, 5);
        SWAP(2, 6);
        SWAP(3, 7);
        SWAP(2, 4);
        SWAP(3, 5);
        SWAP(1, 2);
        SWAP(3, 4);
        SWAP(5, 6);
        c[0] = a0;
        c[1] = a1;
        c[2] = a2;
        c[3] = a3;
        c = c + 4;

        SWAP(8, 12);
        SWAP(9, 13);
        SWAP(10, 14);
        SWAP(11, 15);
        SWAP(10, 12);
        SWAP(11, 13);
        SWAP(9, 10);
        SWAP(11, 12);
        SWAP(13, 14);
        f[0] = a8;
        f[1] = a9;
        f[2] = a10;
        f[3] = a11;
        f = f + 4;

        ui *endOp = (loadFrom1 == endA) ? endB : endA;
        while (opposite1 != endOp)
        {

            a0 = opposite1[0];
            a1 = opposite1[1];
            a2 = opposite1[2];
            a3 = opposite1[3];
            SWAP(0, 4);
            SWAP(1, 5);
            SWAP(2, 6);
            SWAP(3, 7);
            SWAP(2, 4);
            SWAP(3, 5);
            SWAP(1, 2);
            SWAP(3, 4); // SWAP(5,6);
            c[0] = a0;
            c[1] = a1;
            c[2] = a2;
            c[3] = a3;

            opposite1 += 4;
            c += 4;
        }

        SWAP(4, 6);
        SWAP(5, 7);
        SWAP(4, 5);
        SWAP(6, 7);
        SWAP(5, 6);
        c[0] = a4;
        c[1] = a5;
        c[2] = a6;
        c[3] = a7;

        endOp = (loadFrom2 == endD) ? endE : endD;
        while (opposite2 != endOp)
        {
            a8 = opposite2[0];
            a9 = opposite2[1];
            a10 = opposite2[2];
            a11 = opposite2[3];
            SWAP(8, 12);
            SWAP(9, 13);
            SWAP(10, 14);
            SWAP(11, 15);
            SWAP(10, 12);
            SWAP(11, 13);
            SWAP(9, 10);
            SWAP(11, 12); // SWAP(13, 14);
            f[0] = a8;
            f[1] = a9;
            f[2] = a10;
            f[3] = a11;

            opposite2 += 4;
            f += 4;
        }

        SWAP(12, 14);
        SWAP(13, 15);
        SWAP(12, 13);
        SWAP(14, 15);
        SWAP(13, 14);
        f[0] = a12;
        f[1] = a13;
        f[2] = a14;
        f[3] = a15;
    }

    inline void load(sse &a0, sse *ptr)
    {
        a0 = _mm_load_si128(ptr);
    }

    inline void store(sse &a0, sse *ptr)
    {
        _mm_stream_si128(ptr, a0);
        // print128i(a0);
    }

    inline void compareAndSort(sse &a0, sse &a1)
    {
        sse vTmp = _mm_min_epu32(a0, a1);
        a1 = _mm_max_epu32(a0, a1);
        a0 = vTmp;
    }

    inline void loadLowerHalf(sse &var1, sse &var2)
    {
        sse lowerHalfVar1 = _mm_unpacklo_epi64(var1, var2);
        var2 = _mm_unpackhi_epi64(var1, var2);
        var1 = lowerHalfVar1;
    }

    // Bitonic Merge
    inline void vectorSort2(sse &x1, sse &x2)
    {
        x2 = _mm_shuffle_epi32(x2, _MM_SHUFFLE(0, 1, 2, 3));
        __m128i l1l = _mm_min_epu32(x1, x2);
        __m128i l1h = _mm_max_epu32(x1, x2);

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

    inline void minMaxSwap(sse &x1, sse &x2)
    {
        sse temp = _mm_min_epu32(x1, x2);
        x2 = _mm_max_epu32(x1, x2);
        x1 = temp;
    }

    inline void bitonicMerge(sse &x1, sse &x2)
    {
        // Level 1
        x2 = _mm_shuffle_epi32(x2, _MM_SHUFFLE(0, 1, 2, 3));
        __m128i l1l = _mm_min_epu32(x1, x2);
        __m128i l1h = _mm_max_epu32(x1, x2);

        // Level 2: Diaginal Exchange
        //_mm_alignr_epi8(a0, a0, 4);
        // __m128i temp = _mm_shuffle_ps(l1l, l1h, 68);
        // x2 = _mm_shuffle_ps(l1l, l1h, 238);
        // x1 = temp;

        __m128i temp = _mm_blendv_epi8(l1l, l1h, _mm_set_epi32(-1, -1, 0, 0));
        x2 = _mm_blendv_epi8(l1h, l1l, _mm_set_epi32(-1, -1, 0, 0));
        x1 = _mm_alignr_epi8(temp, temp, 8);

        minMaxSwap(x1, x2);

        temp = _mm_blendv_epi8(x1, x2, _mm_set_epi32(-1, 0, -1, 0));
        x2 = _mm_blendv_epi8(x2, x1, _mm_set_epi32(-1, 0, -1, 0));
        x1 = _mm_shuffle_epi32(temp, _MM_SHUFFLE(2, 3, 0, 1));

        minMaxSwap(x1, x2);

        x1 = _mm_alignr_epi8(x1, x1, 8);
        temp = _mm_blendv_epi8(x1, x2, _mm_set_epi32(-1, -1, 0, 0));
        x2 = _mm_blendv_epi8(x2, x1, _mm_set_epi32(-1, -1, 0, 0));

        x1 = _mm_shuffle_epi32(temp, _MM_SHUFFLE(2, 0, 3, 1));
        x2 = _mm_shuffle_epi32(x2, _MM_SHUFFLE(0, 2, 1, 3));
    }

    inline void oddEvenMerge24v24(sse &x1, sse &x2, sse &x3, sse &x4, sse &x5, sse &x6, sse &x7, sse &x8, sse &x9, sse &x10, sse &x11, sse &x12)
    {
        // Step 1
        minMaxSwap(x1, x7);
        minMaxSwap(x2, x8);
        minMaxSwap(x3, x9);
        minMaxSwap(x4, x10);
        minMaxSwap(x5, x11);
        minMaxSwap(x6, x12);

        // Step 2
        minMaxSwap(x7, x4);
        minMaxSwap(x8, x5);
        minMaxSwap(x9, x6);

        // Step 3
        sse t = _mm_alignr_epi8(x7, _mm_set_epi32(0, 0, 0, 0), 8);
        x7 = _mm_alignr_epi8(x8, x7, 8);
        x8 = _mm_alignr_epi8(x9, x8, 8);
        x9 = _mm_alignr_epi8(x10, x9, 8);
        x10 = _mm_alignr_epi8(x11, x10, 8);
        x11 = _mm_alignr_epi8(x12, x11, 8);
        x12 = _mm_alignr_epi8(x12, x12, 8);

        minMaxSwap(t, x2);
        minMaxSwap(x7, x3);
        minMaxSwap(x8, x4);
        minMaxSwap(x9, x5);
        minMaxSwap(x10, x6);

        x12 = _mm_alignr_epi8(x12, x12, 12);
        x11 = _mm_alignr_epi8(x11, x10, 12);
        x10 = _mm_alignr_epi8(x10, x9, 12);
        x9 = _mm_alignr_epi8(x9, x8, 12);
        x8 = _mm_alignr_epi8(x8, x7, 12);
        x7 = _mm_alignr_epi8(x7, t, 12);
        t = _mm_alignr_epi8(t, _mm_set_epi32(0, 0, 0, 0), 12);

        minMaxSwap(t, x1);
        minMaxSwap(x7, x2);
        minMaxSwap(x8, x3);
        minMaxSwap(x9, x4);
        minMaxSwap(x10, x5);
        minMaxSwap(x11, x6);

        t = _mm_alignr_epi8(x7, t, 4);
        x7 = _mm_alignr_epi8(x8, x7, 4);
        x8 = _mm_alignr_epi8(x9, x8, 4);
        x9 = _mm_alignr_epi8(x10, x9, 4);
        x10 = _mm_alignr_epi8(x11, x10, 4);
        x11 = _mm_alignr_epi8(x12, x11, 4);
        x12 = _mm_alignr_epi8(x12, x12, 4);

        minMaxSwap(t, x1);
        minMaxSwap(x7, x2);
        minMaxSwap(x8, x3);
        minMaxSwap(x9, x4);
        minMaxSwap(x10, x5);
        minMaxSwap(x11, x6);

        t = _mm_alignr_epi8(x7, t, 4);
        x7 = _mm_alignr_epi8(x8, x7, 4);
        x8 = _mm_alignr_epi8(x9, x8, 4);
        x9 = _mm_alignr_epi8(x10, x9, 4);
        x10 = _mm_alignr_epi8(x11, x10, 4);
        x11 = _mm_alignr_epi8(x12, x11, 4);
        x12 = _mm_alignr_epi8(x12, x12, 4);

        minMaxSwap(t, x1);
        minMaxSwap(x7, x2);
        minMaxSwap(x8, x3);
        minMaxSwap(x9, x4);
        minMaxSwap(x10, x5);
        minMaxSwap(x11, x6);

        t = _mm_alignr_epi8(x7, t, 4);
        x7 = _mm_alignr_epi8(x8, x7, 4);
        x8 = _mm_alignr_epi8(x9, x8, 4);
        x9 = _mm_alignr_epi8(x10, x9, 4);
        x10 = _mm_alignr_epi8(x11, x10, 4);
        x11 = _mm_alignr_epi8(x12, x11, 4);

        x12 = _mm_unpackhi_epi32(x6, x11);
        x11 = _mm_unpacklo_epi32(x6, x11);
        sse t_x9 = _mm_unpacklo_epi32(x5, x10);
        x10 = _mm_unpackhi_epi32(x5, x10);

        sse t_x8 = _mm_unpackhi_epi32(x4, x9);
        sse t_x7 = _mm_unpacklo_epi32(x4, x9);

        x6 = _mm_unpackhi_epi32(x3, x8);
        x5 = _mm_unpacklo_epi32(x3, x8);

        x4 = _mm_unpackhi_epi32(x2, x7);
        x3 = _mm_unpacklo_epi32(x2, x7);

        x2 = _mm_unpackhi_epi32(x1, t);
        x1 = _mm_unpacklo_epi32(x1, t);
        x8 = t_x8;
        x7 = t_x7;
        x9 = t_x9;
    }

    // 20 shuffles, 3 blends and 17 swaps
    inline void batcherMerge(sse &x1, sse &x2, sse &x3, sse &x4, sse &x5, sse &x6, sse &x7, sse &x8)
    {
        // Step 1
        minMaxSwap(x1, x5);
        minMaxSwap(x2, x6);
        minMaxSwap(x3, x7);
        minMaxSwap(x4, x8);

        // Step 2
        minMaxSwap(x5, x3);
        minMaxSwap(x6, x4);

        // Step 3
        minMaxSwap(x5, x2);
        minMaxSwap(x6, x3);
        minMaxSwap(x7, x4);

        sse carry = _mm_alignr_epi8(x5, x8, 8);
        sse t1 = _mm_blend_epi16(carry, x1, 15);
        x5 = _mm_alignr_epi8(x6, x5, 8);
        x6 = _mm_alignr_epi8(x7, x6, 8);
        x7 = _mm_alignr_epi8(x8, x7, 8);

        minMaxSwap(t1, x1);
        minMaxSwap(x5, x2);
        minMaxSwap(x6, x3);
        minMaxSwap(x7, x4);

        sse carry2 = _mm_blend_epi16(carry, t1, 0xF0);
        carry2 = _mm_alignr_epi8(x5, carry2, 4);
        t1 = _mm_blend_epi16(carry2, x1, 3);
        x5 = _mm_alignr_epi8(x6, x5, 4);
        x6 = _mm_alignr_epi8(x7, x6, 4);
        x7 = _mm_alignr_epi8(carry, x7, 4);

        minMaxSwap(t1, x1);
        minMaxSwap(x5, x2);
        minMaxSwap(x6, x3);
        minMaxSwap(x7, x4);

        t1 = _mm_alignr_epi8(x5, t1, 4);
        x5 = _mm_alignr_epi8(x6, x5, 4);
        x6 = _mm_alignr_epi8(x7, x6, 4);
        x7 = _mm_alignr_epi8(carry2, x7, 4);

        x8 = _mm_unpackhi_epi32(x4, x7);
        x7 = _mm_unpacklo_epi32(x4, x7);
        sse temp = _mm_unpacklo_epi32(x3, x6);
        x6 = _mm_unpackhi_epi32(x3, x6);
        x4 = _mm_unpackhi_epi32(x2, x5);
        x3 = _mm_unpacklo_epi32(x2, x5);
        x5 = temp;
        x2 = _mm_unpackhi_epi32(x1, t1);
        x1 = _mm_unpacklo_epi32(x1, t1);
    }

    // 17 swaps, 22 shuffles
    inline void batcherMerge1722(sse &x1, sse &x2, sse &x3, sse &x4, sse &x5, sse &x6, sse &x7, sse &x8)
    {
        minMaxSwap(x1, x5);
        minMaxSwap(x2, x6);
        minMaxSwap(x3, x7);
        minMaxSwap(x4, x8);

        minMaxSwap(x5, x3);
        minMaxSwap(x6, x4);

        minMaxSwap(x5, x2);
        minMaxSwap(x6, x3);
        minMaxSwap(x7, x4);

        sse t = _mm_alignr_epi8(x5, _mm_set_epi32(0, 0, 0, 0), 8);
        x5 = _mm_alignr_epi8(x6, x5, 8);
        x6 = _mm_alignr_epi8(x7, x6, 8);
        x7 = _mm_alignr_epi8(x8, x7, 8);
        x8 = _mm_alignr_epi8(x8, x8, 8);

        minMaxSwap(t, x1);
        minMaxSwap(x5, x2);
        minMaxSwap(x6, x3);
        minMaxSwap(x7, x4);

        t = _mm_alignr_epi8(x5, t, 4);
        x5 = _mm_alignr_epi8(x6, x5, 4);
        x6 = _mm_alignr_epi8(x7, x6, 4);
        x7 = _mm_alignr_epi8(x8, x7, 4);
        x8 = _mm_alignr_epi8(x8, x8, 4);

        minMaxSwap(t, x1);
        minMaxSwap(x5, x2);
        minMaxSwap(x6, x3);
        minMaxSwap(x7, x4);

        t = _mm_alignr_epi8(x5, t, 4);
        x5 = _mm_alignr_epi8(x6, x5, 4);
        x6 = _mm_alignr_epi8(x7, x6, 4);
        x7 = _mm_alignr_epi8(x8, x7, 4);

        x8 = _mm_unpackhi_epi32(x4, x7);
        x7 = _mm_unpacklo_epi32(x4, x7);
        sse temp = _mm_unpacklo_epi32(x3, x6);
        x6 = _mm_unpackhi_epi32(x3, x6);
        x4 = _mm_unpackhi_epi32(x2, x5);
        x3 = _mm_unpacklo_epi32(x2, x5);
        x5 = temp;
        x2 = _mm_unpackhi_epi32(x1, t);
        x1 = _mm_unpacklo_epi32(x1, t);
    }

    inline void oddEvenMerge32v32(sse &x1, sse &x2, sse &x3, sse &x4, sse &x5, sse &x6, sse &x7, sse &x8, sse &x9, sse &x10, sse &x11, sse &x12, sse &x13, sse &x14, sse &x15, sse &x16)
    {
        // Step 1
        minMaxSwap(x1, x9);
        minMaxSwap(x2, x10);
        minMaxSwap(x3, x11);
        minMaxSwap(x4, x12);
        minMaxSwap(x5, x13);
        minMaxSwap(x6, x14);
        minMaxSwap(x7, x15);
        minMaxSwap(x8, x16);

        // Step 2
        minMaxSwap(x9, x5);
        minMaxSwap(x10, x6);
        minMaxSwap(x11, x7);
        minMaxSwap(x12, x8);

        // Step 3
        minMaxSwap(x9, x3);
        minMaxSwap(x10, x4);
        minMaxSwap(x11, x5);
        minMaxSwap(x12, x6);
        minMaxSwap(x13, x7);
        minMaxSwap(x14, x8);

        // Step 4
        minMaxSwap(x9, x2);
        minMaxSwap(x10, x3);
        minMaxSwap(x11, x4);
        minMaxSwap(x12, x5);
        minMaxSwap(x13, x6);
        minMaxSwap(x14, x7);
        minMaxSwap(x15, x8);

        // Step 5
        sse t = _mm_alignr_epi8(x9, _mm_set_epi32(0, 0, 0, 0), 8);
        x9 = _mm_alignr_epi8(x10, x9, 8);
        x10 = _mm_alignr_epi8(x11, x10, 8);
        x11 = _mm_alignr_epi8(x12, x11, 8);
        x12 = _mm_alignr_epi8(x13, x12, 8);
        x13 = _mm_alignr_epi8(x14, x13, 8);
        x14 = _mm_alignr_epi8(x15, x14, 8);
        x15 = _mm_alignr_epi8(x16, x15, 8);
        x16 = _mm_alignr_epi8(x16, x16, 8);

        minMaxSwap(t, x1);
        minMaxSwap(x9, x2);
        minMaxSwap(x10, x3);
        minMaxSwap(x11, x4);
        minMaxSwap(x12, x5);
        minMaxSwap(x13, x6);
        minMaxSwap(x14, x7);
        minMaxSwap(x15, x8);

        t = _mm_alignr_epi8(x9, t, 4);
        x9 = _mm_alignr_epi8(x10, x9, 4);
        x10 = _mm_alignr_epi8(x11, x10, 4);
        x11 = _mm_alignr_epi8(x12, x11, 4);
        x12 = _mm_alignr_epi8(x13, x12, 4);
        x13 = _mm_alignr_epi8(x14, x13, 4);
        x14 = _mm_alignr_epi8(x15, x14, 4);
        x15 = _mm_alignr_epi8(x16, x15, 4);
        x16 = _mm_alignr_epi8(x16, x16, 4);

        minMaxSwap(t, x1);
        minMaxSwap(x9, x2);
        minMaxSwap(x10, x3);
        minMaxSwap(x11, x4);
        minMaxSwap(x12, x5);
        minMaxSwap(x13, x6);
        minMaxSwap(x14, x7);
        minMaxSwap(x15, x8);

        t = _mm_alignr_epi8(x9, t, 4);
        x9 = _mm_alignr_epi8(x10, x9, 4);
        x10 = _mm_alignr_epi8(x11, x10, 4);
        x11 = _mm_alignr_epi8(x12, x11, 4);
        x12 = _mm_alignr_epi8(x13, x12, 4);
        x13 = _mm_alignr_epi8(x14, x13, 4);
        x14 = _mm_alignr_epi8(x15, x14, 4);
        x15 = _mm_alignr_epi8(x16, x15, 4);

        x16 = _mm_unpackhi_epi32(x8, x15);
        x15 = _mm_unpacklo_epi32(x8, x15);
        sse temp13 = _mm_unpacklo_epi32(x7, x14);
        x14 = _mm_unpackhi_epi32(x7, x14);
        sse temp11 = _mm_unpacklo_epi32(x6, x13);
        sse temp12 = _mm_unpackhi_epi32(x6, x13);
        sse temp10 = _mm_unpackhi_epi32(x5, x12);
        sse temp9 = _mm_unpacklo_epi32(x5, x12);

        x8 = _mm_unpackhi_epi32(x4, x11);
        x7 = _mm_unpacklo_epi32(x4, x11);
        x6 = _mm_unpackhi_epi32(x3, x10);
        x5 = _mm_unpacklo_epi32(x3, x10);
        x4 = _mm_unpackhi_epi32(x2, x9);
        x3 = _mm_unpacklo_epi32(x2, x9);
        x2 = _mm_unpackhi_epi32(x1, t);
        x1 = _mm_unpacklo_epi32(x1, t);
        x13 = temp13;
        x12 = temp12;
        x11 = temp11;
        x10 = temp10;
        x9 = temp9;
    }
    void optimizationCount4(sse *&opposite, sse *&c, sse &a4, sse &a5, sse &a6, sse &a7)
    {
        store(a4, c);
        store(a5, c + 1);
        store(a6, c + 2);
        store(a7, c + 3);

        load(a4, opposite);
        load(a5, opposite + 1);
        load(a6, opposite + 2);
        load(a7, opposite + 3);

        opposite += 4;
        c += 4;
    }

    void optimizationCount3(sse *&opposite, sse *&c, sse &a4, sse &a5, sse &a6, sse &a7)
    {
        store(a4, c);
        store(a5, c + 1);
        store(a6, c + 2);

        a4 = a7;
        load(a5, opposite);
        load(a6, opposite + 1);
        load(a7, opposite + 2);

        opposite += 3;
        c += 3;
    }
    void optimizationCount2(sse *&opposite, sse *&c, sse &a4, sse &a5, sse &a6, sse &a7)
    {
        store(a4, c);
        store(a5, c + 1);

        a4 = a6;
        a5 = a7;
        load(a6, opposite);
        load(a7, opposite + 1);

        opposite += 2;
        c += 2;
    }
    void optimizationCount1(sse *&opposite, sse *&c, sse &a4, sse &a5, sse &a6, sse &a7)
    {
        store(a4, c);
        a4 = a5;
        a5 = a6;
        a6 = a7;
        load(a7, opposite);

        opposite += 1;
        c += 1;
    }
    void optimizationCount0(sse *&opposite, sse *&c, sse &a4, sse &a5, sse &a6, sse &a7)
    {
    }

    inline void conditionalOptimization(sse *&loadFrom, sse *&opposite, sse *&c, sse &a4, sse &a5, sse &a6, sse &a7)
    {
        if (_mm_extract_epi32(a7, 3) <= *(ui *)loadFrom)
        {
            store(a4, c);
            store(a5, c + 1);
            store(a6, c + 2);
            store(a7, c + 3);

            load(a4, opposite);
            load(a5, opposite + 1);
            load(a6, opposite + 2);
            load(a7, opposite + 3);

            opposite += 4;
            c += 4;
            // log[4]++;
        }
        else if (_mm_extract_epi32(a6, 3) <= *(ui *)loadFrom)
        {
            store(a4, c);
            store(a5, c + 1);
            store(a6, c + 2);

            a4 = a7;
            load(a5, opposite);
            load(a6, opposite + 1);
            load(a7, opposite + 2);

            opposite += 3;
            c += 3;
            // log[3]++;
        }
        else if (_mm_extract_epi32(a5, 3) <= *(ui *)loadFrom)
        {
            store(a4, c);
            store(a5, c + 1);

            a4 = a6;
            a5 = a7;
            load(a6, opposite);
            load(a7, opposite + 1);

            opposite += 2;
            c += 2;
            // log[2]++;
        }
        else if (_mm_extract_epi32(a4, 3) <= *(ui *)loadFrom)
        {
            store(a4, c);

            a4 = a5;
            a5 = a6;
            a6 = a7;
            load(a7, opposite);

            opposite += 1;
            c += 1;
            // log[1]++;
        }
    }

    inline void addRemainingElementsFromEndStream(sse *&finishedStream, sse *&loadFrom, sse *&c, sse &a4, sse &a5, sse &a6, sse &a7)
    {
        int remainingElements = finishedStream - loadFrom;
        if (remainingElements > 0)
        {
            // Adding the remaining elements from finished stream.
            sse top[4];
            sse bottom[4];
            bottom[0] = a4;
            bottom[1] = a5;
            bottom[2] = a6;
            bottom[3] = a7;
            int zeros = 4 - remainingElements;

            for (int i = 0; i < zeros; i++)
            {
                top[i] = _mm_set_epi32(0, 0, 0, 0);
            }
            for (int i = zeros; i < 4; i++)
            {
                sse ax;
                load(ax, loadFrom + i - zeros);
                top[i] = ax;
            }
            batcherMerge(top[0], top[1], top[2], top[3], a4, a5, a6, a7);
            for (int i = zeros; i < 4; i++)
            {
                sse ax = top[i];
                store(ax, c);
                c += 1;
            }
        }
    }

    inline void addRemainingElementsFromEndStream(sse *&finishedStream, sse *&loadFrom, sse *&c, sse &a4, sse &a5, sse &a6, sse &a7, sse &a8, sse &a9, sse &a10, sse &a11)
    {
        int remainingElements = finishedStream - loadFrom;
        if (remainingElements > 0)
        {
            // Adding the remaining elements from finished stream.
            sse top[8];
            int zeros = 8 - remainingElements;

            for (int i = 0; i < zeros; i++)
            {
                top[i] = _mm_set_epi32(0, 0, 0, 0);
            }
            for (int i = zeros; i < 8; i++)
            {
                sse ax;
                load(ax, loadFrom + i - zeros);
                top[i] = ax;
            }
            oddEvenMerge32v32(top[0], top[1], top[2], top[3], top[4], top[5], top[6], top[7], a4, a5, a6, a7, a8, a9, a10, a11);
            for (int i = zeros; i < 8; i++)
            {
                sse ax = top[i];
                store(ax, c);
                c += 1;
            }
        }
    }

    inline void vectorBatcherMergeOptimizedv2(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        const int reg = 4;

        sse *a = (sse *)A, *b = (sse *)B, *endA = (sse *)(A + lenA), *endB = (sse *)(B + lenB), *c = (sse *)C;
        sse *realA = endA;
        sse *realB = endB;

        endA -= 4;
        endB -= 4;
        sse a0, a1, a2, a3, a4, a5, a6, a7;
        load(a0, a);
        load(a1, a + 1);
        load(a2, a + 2);
        load(a3, a + 3);

        load(a4, b);
        load(a5, b + 1);
        load(a6, b + 2);
        load(a7, b + 3);

        a += reg;
        b += reg;
        sse *loadFrom = a, *opposite = b;

        while ((a <= loadFrom && loadFrom <= endA) || (b <= loadFrom && loadFrom <= endB))
        {

            bool first = *(ui *)loadFrom < *(ui *)opposite;
            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            c += reg;
            int count = 0;
            count += _mm_extract_epi32(a4, 3) <= *(ui *)loadFrom;
            count += _mm_extract_epi32(a5, 3) <= *(ui *)loadFrom;
            count += _mm_extract_epi32(a6, 3) <= *(ui *)loadFrom;
            count += _mm_extract_epi32(a7, 3) <= *(ui *)loadFrom;

            store(a4, c);
            store(a5, c + 1);
            store(a6, c + 2);
            store(a7, c + 3);
            // std::cout << "count " << count << " \n";

            memcpy(c + 4, opposite, count * 4 * sizeof(ui));

            c += count;

            load(a4, c);
            load(a5, c + 1);
            load(a6, c + 2);
            load(a7, c + 3);

            opposite += count;

            load(a0, loadFrom);
            load(a1, loadFrom + 1);
            load(a2, loadFrom + 2);
            load(a3, loadFrom + 3);
            loadFrom += reg;
        }

        batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);
        store(a0, c);
        store(a1, c + 1);
        store(a2, c + 2);
        store(a3, c + 3);

        c += reg;
        bool firstStream = (a <= loadFrom && loadFrom <= realA);
        sse *finishedStream = firstStream ? realA : realB;

        int remainingElements = finishedStream - loadFrom;
        if (remainingElements > 0)
        {
            // Adding the remaining elements from finished stream.
            sse top[4];
            sse bottom[4];
            bottom[0] = a4;
            bottom[1] = a5;
            bottom[2] = a6;
            bottom[3] = a7;
            int zeros = 4 - remainingElements;

            for (int i = 0; i < zeros; i++)
            {
                top[i] = _mm_set_epi32(0, 0, 0, 0);
            }
            for (int i = zeros; i < 4; i++)
            {
                sse ax;
                load(ax, loadFrom + i - zeros);
                top[i] = ax;
            }
            batcherMerge(top[0], top[1], top[2], top[3], a4, a5, a6, a7);
            for (int i = zeros; i < 4; i++)
            {
                sse ax = top[i];
                store(ax, c);
                c += 1;
            }
        }

        sse *endOp = firstStream ? endB : endA;
        finishedStream = firstStream ? realB : realA;

        while (opposite <= endOp)
        {
            load(a0, opposite);
            load(a1, opposite + 1);
            load(a2, opposite + 2);
            load(a3, opposite + 3);

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            opposite += reg;
            c += reg;
        }

        remainingElements = finishedStream - opposite;
        if (remainingElements > 0)
        {
            // Adding the remaining elements from finished stream.
            sse top[4];
            int zeros = 4 - remainingElements;

            for (int i = 0; i < zeros; i++)
            {
                top[i] = _mm_set_epi32(0, 0, 0, 0);
            }
            for (int i = zeros; i < 4; i++)
            {
                sse ax;
                load(ax, opposite + i - zeros);
                top[i] = ax;
            }

            batcherMerge(top[0], top[1], top[2], top[3], a4, a5, a6, a7);

            for (int i = zeros; i < 4; i++)
            {
                sse ax = top[i];
                store(ax, c);
                c += 1;
            }
        }

        store(a4, c);
        store(a5, c + 1);
        store(a6, c + 2);
        store(a7, c + 3);
    }

    inline void vectorBatcherMergeOptimizedWithSplit(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *D, ui64 lenD, ui *E, ui64 lenE, ui *C, ui *F)
    {
        const int reg = 4;
        sse *a = (sse *)A, *b = (sse *)B, *endA = (sse *)(A + lenA), *endB = (sse *)(B + lenB), *c = (sse *)C;
        sse *d = (sse *)D, *e = (sse *)E, *endD = (sse *)(D + lenD), *endE = (sse *)(E + lenE), *f = (sse *)F;

        sse *Aend = endA;
        sse *Bend = endB;
        sse *Dend = endD;
        sse *Eend = endE;

        endA -= 4;
        endB -= 4;
        endD -= 4;
        endE -= 4;

        sse a0, a1, a2, a3, a4, a5, a6, a7;
        sse a8, a9, a10, a11, a12, a13, a14, a15;
        load(a0, a);
        load(a1, a + 1);
        load(a2, a + 2);
        load(a3, a + 3);

        load(a4, b);
        load(a5, b + 1);
        load(a6, b + 2);
        load(a7, b + 3);

        load(a8, d);
        load(a9, d + 1);
        load(a10, d + 2);
        load(a11, d + 3);

        load(a12, e);
        load(a13, e + 1);
        load(a14, e + 2);
        load(a15, e + 3);

        a += reg;
        b += reg;
        d += reg;
        e += reg;

        sse *loadFrom = a, *opposite = b;
        sse *loadFrom2 = d, *opposite2 = e;

        bool first = *(ui *)loadFrom < *(ui *)opposite;
        sse *tmp = first ? loadFrom : opposite;
        opposite = first ? opposite : loadFrom;
        loadFrom = tmp;
        batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

        first = *(ui *)loadFrom2 < *(ui *)opposite2;
        tmp = first ? loadFrom2 : opposite2;
        opposite2 = first ? opposite2 : loadFrom2;
        loadFrom2 = tmp;
        batcherMerge(a8, a9, a10, a11, a12, a13, a14, a15);

        while (((a <= loadFrom && loadFrom <= endA) || (b <= loadFrom && loadFrom <= endB)) && ((d <= loadFrom2 && loadFrom2 <= endD) || (e <= loadFrom2 && loadFrom2 <= endE)))
        {
            first = *(ui *)loadFrom < *(ui *)opposite;
            tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            first = *(ui *)loadFrom2 < *(ui *)opposite2;
            tmp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = tmp;

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);
            batcherMerge(a8, a9, a10, a11, a12, a13, a14, a15);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);
            store(a8, f);
            store(a9, f + 1);
            store(a10, f + 2);
            store(a11, f + 3);

            c += reg;
            f += reg;

            conditionalOptimization(loadFrom, opposite, c, a4, a5, a6, a7);
            conditionalOptimization(loadFrom2, opposite2, f, a12, a13, a14, a15);

            load(a0, loadFrom);
            load(a1, loadFrom + 1);
            load(a2, loadFrom + 2);
            load(a3, loadFrom + 3);

            load(a8, loadFrom2);
            load(a9, loadFrom2 + 1);
            load(a10, loadFrom2 + 2);
            load(a11, loadFrom2 + 3);

            loadFrom += reg;
            loadFrom2 += reg;

            _mm_prefetch((char *)(loadFrom + 64), _MM_HINT_T2);
            _mm_prefetch((char *)(loadFrom2 + 64), _MM_HINT_T2);
        }

        while ((a <= loadFrom && loadFrom <= endA) || (b <= loadFrom && loadFrom <= endB))
        {
            first = *(ui *)loadFrom < *(ui *)opposite;
            tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);
            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            c += reg;
            conditionalOptimization(loadFrom, opposite, c, a4, a5, a6, a7);

            load(a0, loadFrom);
            load(a1, loadFrom + 1);
            load(a2, loadFrom + 2);
            load(a3, loadFrom + 3);

            loadFrom += reg;
            _mm_prefetch((char *)(loadFrom + 64), _MM_HINT_T2);
        }

        while ((d <= loadFrom2 && loadFrom2 <= endD) || (e <= loadFrom2 && loadFrom2 <= endE))
        {

            first = *(ui *)loadFrom2 < *(ui *)opposite2;
            tmp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = tmp;

            batcherMerge(a8, a9, a10, a11, a12, a13, a14, a15);
            store(a8, f);
            store(a9, f + 1);
            store(a10, f + 2);
            store(a11, f + 3);

            f += reg;
            conditionalOptimization(loadFrom2, opposite2, f, a12, a13, a14, a15);

            load(a8, loadFrom2);
            load(a9, loadFrom2 + 1);
            load(a10, loadFrom2 + 2);
            load(a11, loadFrom2 + 3);
            loadFrom2 += reg;

            _mm_prefetch((char *)(loadFrom2 + 64), _MM_HINT_T2);
        }

        batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);
        batcherMerge(a8, a9, a10, a11, a12, a13, a14, a15);

        store(a0, c);
        store(a1, c + 1);
        store(a2, c + 2);
        store(a3, c + 3);
        store(a8, f);
        store(a9, f + 1);
        store(a10, f + 2);
        store(a11, f + 3);

        c += reg;
        f += reg;

        bool firstStream = (a <= loadFrom && loadFrom <= Aend);
        sse *finishedStream = firstStream ? Aend : Bend;
        addRemainingElementsFromEndStream(finishedStream, loadFrom, c, a4, a5, a6, a7);

        sse *endOp = firstStream ? endB : endA;
        finishedStream = firstStream ? Bend : Aend;

        while (opposite <= endOp)
        {
            load(a0, opposite);
            load(a1, opposite + 1);
            load(a2, opposite + 2);
            load(a3, opposite + 3);

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            opposite += reg;
            c += reg;
        }
        addRemainingElementsFromEndStream(finishedStream, opposite, c, a4, a5, a6, a7);

        store(a4, c);
        store(a5, c + 1);
        store(a6, c + 2);
        store(a7, c + 3);

        firstStream = (d <= loadFrom2 && loadFrom2 <= Dend);
        finishedStream = firstStream ? Dend : Eend;
        addRemainingElementsFromEndStream(finishedStream, loadFrom2, f, a12, a13, a14, a15);

        endOp = firstStream ? endE : endD;
        finishedStream = firstStream ? Eend : Dend;

        while (opposite2 <= endOp)
        {
            load(a8, opposite2);
            load(a9, opposite2 + 1);
            load(a10, opposite2 + 2);
            load(a11, opposite2 + 3);

            batcherMerge(a8, a9, a10, a11, a12, a13, a14, a15);

            store(a8, f);
            store(a9, f + 1);
            store(a10, f + 2);
            store(a11, f + 3);

            opposite2 += reg;
            f += reg;
        }

        addRemainingElementsFromEndStream(finishedStream, opposite2, f, a12, a13, a14, a15);
        store(a12, f);
        store(a13, f + 1);
        store(a14, f + 2);
        store(a15, f + 3);
    }

    template <ui reg = 4>
    inline void vectorBatcherMergeOptimized(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        sse *a = (sse *)A, *b = (sse *)B, *endA = (sse *)(A + lenA), *endB = (sse *)(B + lenB), *c = (sse *)C;
        sse *realA = endA;
        sse *realB = endB;
        void (*func[5])(sse *&, sse *&, sse &, sse &, sse &, sse &) = {
            optimizationCount0,
            optimizationCount1,
            optimizationCount2,
            optimizationCount3,
            optimizationCount4};
        constexpr int nreg = reg;

        endA -= nreg;
        endB -= nreg;
        sse a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
        load(a0, a);
        load(a1, a + 1);
        load(a2, a + 2);
        load(a3, a + 3);

        load(a4, b);
        load(a5, b + 1);
        load(a6, b + 2);
        load(a7, b + 3);

        a += nreg;
        b += nreg;
        sse *loadFrom = a, *opposite = b;

        while ((a <= loadFrom && loadFrom <= endA) || (b <= loadFrom && loadFrom <= endB))
        {
            bool first = *(ui *)loadFrom < *(ui *)opposite;
            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            c += nreg;

            conditionalOptimization(loadFrom, opposite, c, a4, a5, a6, a7);
            load(a0, loadFrom);
            load(a1, loadFrom + 1);
            load(a2, loadFrom + 2);
            load(a3, loadFrom + 3);

            loadFrom += nreg;
        }

        batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

        store(a0, c);
        store(a1, c + 1);
        store(a2, c + 2);
        store(a3, c + 3);

        c += nreg;

        bool firstStream = (a <= loadFrom && loadFrom <= realA);
        sse *finishedStream = firstStream ? realA : realB;

        addRemainingElementsFromEndStream(finishedStream, loadFrom, c, a4, a5, a6, a7);

        sse *endOp = firstStream ? endB : endA;
        finishedStream = firstStream ? realB : realA;

        while (opposite <= endOp)
        {
            load(a0, opposite);
            load(a1, opposite + 1);
            load(a2, opposite + 2);
            load(a3, opposite + 3);

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            opposite += nreg;
            c += nreg;
        }

        addRemainingElementsFromEndStream(finishedStream, opposite, c, a4, a5, a6, a7);
        store(a4, c);
        store(a5, c + 1);
        store(a6, c + 2);
        store(a7, c + 3);
    }

    inline void vectorBatcherMergeOptimized32v32(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        constexpr int reg = 8;
        sse *a = (sse *)A, *b = (sse *)B, *endA = (sse *)(A + lenA), *endB = (sse *)(B + lenB), *c = (sse *)C;
        sse *realA = endA;
        sse *realB = endB;

        constexpr int nreg = reg;

        endA -= nreg;
        endB -= nreg;
        sse a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;

        {
            load(a0, a);
            load(a1, a + 1);
            load(a2, a + 2);
            load(a3, a + 3);
            load(a4, a + 4);
            load(a5, a + 5);
            load(a6, a + 6);
            load(a7, a + 7);

            load(a8, b);
            load(a9, b + 1);
            load(a10, b + 2);
            load(a11, b + 3);
            load(a12, b + 4);
            load(a13, b + 5);
            load(a14, b + 6);
            load(a15, b + 7);
        }

        a += nreg;
        b += nreg;
        sse *loadFrom = a, *opposite = b;

        while ((a <= loadFrom && loadFrom <= endA) || (b <= loadFrom && loadFrom <= endB))
        {
            bool first = *(ui *)loadFrom < *(ui *)opposite;
            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            oddEvenMerge32v32(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            store(a4, c + 4);
            store(a5, c + 5);
            store(a6, c + 6);
            store(a7, c + 7);

            c += nreg;

            load(a0, loadFrom);
            load(a1, loadFrom + 1);
            load(a2, loadFrom + 2);
            load(a3, loadFrom + 3);

            load(a4, loadFrom + 4);
            load(a5, loadFrom + 5);
            load(a6, loadFrom + 6);
            load(a7, loadFrom + 7);

            loadFrom += nreg;
        }
        oddEvenMerge32v32(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

        store(a0, c);
        store(a1, c + 1);
        store(a2, c + 2);
        store(a3, c + 3);

        store(a4, c + 4);
        store(a5, c + 5);
        store(a6, c + 6);
        store(a7, c + 7);

        c += nreg;

        bool firstStream = (a <= loadFrom && loadFrom <= realA);
        sse *finishedStream = firstStream ? realA : realB;

        addRemainingElementsFromEndStream(finishedStream, loadFrom, c, a8, a9, a10, a11, a12, a13, a14, a15);

        sse *endOp = firstStream ? endB : endA;
        finishedStream = firstStream ? realB : realA;

        while (opposite <= endOp)
        {
            load(a0, opposite);
            load(a1, opposite + 1);
            load(a2, opposite + 2);
            load(a3, opposite + 3);

            load(a4, opposite + 4);
            load(a5, opposite + 5);
            load(a6, opposite + 6);
            load(a7, opposite + 7);

            oddEvenMerge32v32(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            store(a4, c + 4);
            store(a5, c + 5);
            store(a6, c + 6);
            store(a7, c + 7);

            opposite += nreg;
            c += nreg;
        }

        store(a8, c);
        store(a9, c + 1);
        store(a10, c + 2);
        store(a11, c + 3);
        store(a12, c + 4);
        store(a13, c + 5);
        store(a14, c + 6);
        store(a15, c + 7);
    }

    inline void vectorBatcherMerge(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        const int reg = 4;
        sse *a = (sse *)A, *b = (sse *)B, *endA = (sse *)(A + lenA), *endB = (sse *)(B + lenB), *c = (sse *)C;
        sse a0, a1, a2, a3, a4, a5, a6, a7;
        load(a0, a);
        load(a1, a + 1);
        load(a2, a + 2);
        load(a3, a + 3);

        load(a4, b);
        load(a5, b + 1);
        load(a6, b + 2);
        load(a7, b + 3);

        a += reg;
        b += reg;
        sse *loadFrom = a, *opposite = b;

        while (loadFrom != endA && loadFrom != endB)
        {
            bool first = *(ui *)loadFrom < *(ui *)opposite;

            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            load(a0, loadFrom);
            load(a1, loadFrom + 1);
            load(a2, loadFrom + 2);
            load(a3, loadFrom + 3);

            c += reg;
            loadFrom += reg;
        }

        batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);
        store(a0, c);
        store(a1, c + 1);
        store(a2, c + 2);
        store(a3, c + 3);

        c += reg;
        sse *endOp = (loadFrom == endA) ? endB : endA;

        while (opposite != endOp)
        {
            load(a0, opposite);
            load(a1, opposite + 1);
            load(a2, opposite + 2);
            load(a3, opposite + 3);

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            opposite += reg;
            c += reg;
        }
        store(a4, c);
        store(a5, c + 1);
        store(a6, c + 2);
        store(a7, c + 3);
    }

    inline void vectorBatcherMergeWithSplit(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *D, ui64 lenD, ui *E, ui64 lenE, ui *C, ui *F)
    {
        const int reg = 4;
        sse *a = (sse *)A, *b = (sse *)B, *endA = (sse *)(A + lenA), *endB = (sse *)(B + lenB), *c = (sse *)C;
        sse *d = (sse *)D, *e = (sse *)E, *endD = (sse *)(D + lenD), *endE = (sse *)(E + lenE), *f = (sse *)F;
        sse a0, a1, a2, a3, a4, a5, a6, a7;
        sse a8, a9, a10, a11, a12, a13, a14, a15;
        load(a0, a);
        load(a1, a + 1);
        load(a2, a + 2);
        load(a3, a + 3);

        load(a4, b);
        load(a5, b + 1);
        load(a6, b + 2);
        load(a7, b + 3);

        load(a8, d);
        load(a9, d + 1);
        load(a10, d + 2);
        load(a11, d + 3);

        load(a12, e);
        load(a13, e + 1);
        load(a14, e + 2);
        load(a15, e + 3);

        a += reg;
        b += reg;
        d += reg;
        e += reg;

        sse *loadFrom = a, *opposite = b;
        sse *loadFrom2 = d, *opposite2 = e;

        while (loadFrom != endA && loadFrom != endB && loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *(ui *)loadFrom < *(ui *)opposite;
            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            first = *(ui *)loadFrom2 < *(ui *)opposite2;
            tmp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = tmp;

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);
            batcherMerge(a8, a9, a10, a11, a12, a13, a14, a15);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);
            store(a8, f);
            store(a9, f + 1);
            store(a10, f + 2);
            store(a11, f + 3);

            load(a0, loadFrom);
            load(a1, loadFrom + 1);
            load(a2, loadFrom + 2);
            load(a3, loadFrom + 3);

            load(a8, loadFrom2);
            load(a9, loadFrom2 + 1);
            load(a10, loadFrom2 + 2);
            load(a11, loadFrom2 + 3);

            c += reg;
            f += reg;
            loadFrom += reg;
            loadFrom2 += reg;
        }

        while (loadFrom != endA && loadFrom != endB)
        {
            bool first = *(ui *)loadFrom < *(ui *)opposite;

            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            load(a0, loadFrom);
            load(a1, loadFrom + 1);
            load(a2, loadFrom + 2);
            load(a3, loadFrom + 3);

            c += reg;
            loadFrom += reg;
        }

        while (loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *(ui *)loadFrom2 < *(ui *)opposite2;
            sse *tmp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = tmp;

            batcherMerge(a8, a9, a10, a11, a12, a13, a14, a15);

            store(a8, f);
            store(a9, f + 1);
            store(a10, f + 2);
            store(a11, f + 3);
            load(a8, loadFrom2);
            load(a9, loadFrom2 + 1);
            load(a10, loadFrom2 + 2);
            load(a11, loadFrom2 + 3);

            f += reg;
            loadFrom2 += reg;
        }

        batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);
        store(a0, c);
        store(a1, c + 1);
        store(a2, c + 2);
        store(a3, c + 3);

        c += reg;
        sse *endOp = (loadFrom == endA) ? endB : endA;

        while (opposite != endOp)
        {
            load(a0, opposite);
            load(a1, opposite + 1);
            load(a2, opposite + 2);
            load(a3, opposite + 3);

            batcherMerge(a0, a1, a2, a3, a4, a5, a6, a7);

            store(a0, c);
            store(a1, c + 1);
            store(a2, c + 2);
            store(a3, c + 3);

            opposite += reg;
            c += reg;
        }
        store(a4, c);
        store(a5, c + 1);
        store(a6, c + 2);
        store(a7, c + 3);

        batcherMerge(a8, a9, a10, a11, a12, a13, a14, a15);
        store(a8, f);
        store(a9, f + 1);
        store(a10, f + 2);
        store(a11, f + 3);

        f += reg;
        endOp = (loadFrom2 == endD) ? endE : endD;

        while (opposite2 != endOp)
        {
            load(a8, opposite2);
            load(a9, opposite2 + 1);
            load(a10, opposite2 + 2);
            load(a11, opposite2 + 3);

            batcherMerge(a8, a9, a10, a11, a12, a13, a14, a15);

            store(a8, f);
            store(a9, f + 1);
            store(a10, f + 2);
            store(a11, f + 3);

            opposite2 += reg;
            f += reg;
        }
        store(a12, f);
        store(a13, f + 1);
        store(a14, f + 2);
        store(a15, f + 3);
    }

    inline void vectorSortRotateAndSwap(sse &x1, sse &x2)
    {
        minMaxSwap(x1, x2);
        x1 = _mm_alignr_epi8(x1, x1, 4);
        minMaxSwap(x1, x2);
        x1 = _mm_alignr_epi8(x1, x1, 4);
        minMaxSwap(x1, x2);
        x1 = _mm_alignr_epi8(x1, x1, 4);
        minMaxSwap(x1, x2);
        x1 = _mm_alignr_epi8(x1, x1, 4);
    }

    inline void vectorSort(sse &x1, sse &x2)
    {
        compareAndSort(x1, x2);

        x1 = _mm_alignr_epi8(x1, x1, 8);
        compareAndSort(x1, x2);

        x1 = _mm_alignr_epi8(x1, x1, 8);
        loadLowerHalf(x1, x2);

        x1 = _mm_alignr_epi8(x1, x1, 4);
        compareAndSort(x1, x2);

        x2 = _mm_alignr_epi8(x2, x2, 8);
        sse temp = _mm_blend_epi16(x1, x2, 0x30);
        // sse temp = _mm_blend_ps(x1, x2, 4);
        // x2  = _mm_blend_epi16(x2, x1, 0x30);
        x2 = _mm_blend_ps(x2, x1, 4);
        x2 = _mm_shuffle_epi32(x2, _MM_SHUFFLE(1, 0, 2, 3));
        x1 = _mm_shuffle_epi32(temp, _MM_SHUFFLE(1, 2, 0, 3));
    }

    template <int algorithm = 1>
    inline void vectorizedOddEvenMerge(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        sse *a = (sse *)A, *b = (sse *)B, *endA = (sse *)(A + lenA), *endB = (sse *)(B + lenB), *c = (sse *)C;
        sse a0, a1;
        load(a0, a);
        load(a1, b);

        a += 1;
        b += 1;
        sse *loadFrom = a, *opposite = b;

        while (loadFrom != endA && loadFrom != endB)
        {
            bool first = *(ui *)loadFrom < *(ui *)opposite;

            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else if constexpr (algorithm == 2)
                bitonicMerge(a0, a1);
            else
                vectorSortRotateAndSwap(a0, a1);

            store(a0, c);
            load(a0, loadFrom);

            c += 1;
            loadFrom += 1;
        }

        if constexpr (algorithm == 1)
            vectorSort(a0, a1);
        else if constexpr (algorithm == 2)
            bitonicMerge(a0, a1);
        else
            vectorSortRotateAndSwap(a0, a1);
        store(a0, c);

        c += 1;
        sse *endOp = (loadFrom == endA) ? endB : endA;

        while (opposite != endOp)
        {
            load(a0, opposite);
            if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else if constexpr (algorithm == 2)
                bitonicMerge(a0, a1);
            else
                vectorSortRotateAndSwap(a0, a1);
            store(a0, c);

            opposite += 1;
            c += 1;
        }
        store(a1, c);
    }

    template <int algorithm = 1>
    inline void vectorizedOddEvenMergeWithSplit(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *D, ui64 lenD, ui *E, ui64 lenE, ui *C, ui *F)
    {
        sse *a = (sse *)A, *b = (sse *)B, *endA = (sse *)(A + lenA), *endB = (sse *)(B + lenB), *c = (sse *)C;
        sse *d = (sse *)D, *e = (sse *)E, *endD = (sse *)(D + lenD), *endE = (sse *)(E + lenE), *f = (sse *)F;

        sse a0, a1, a2, a3;

        load(a0, a);
        load(a1, b);
        load(a2, d);
        load(a3, e);

        a += 1;
        b += 1;
        d += 1;
        e += 1;

        sse *loadFrom = a, *opposite = b;
        sse *loadFrom2 = d, *opposite2 = e;

        while (loadFrom != endA && loadFrom != endB && loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *(ui *)loadFrom < *(ui *)opposite;
            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            first = *(ui *)loadFrom2 < *(ui *)opposite2;
            tmp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = tmp;

            if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else
                vectorSort2(a0, a1);
            store(a0, c);
            load(a0, loadFrom);

            c += 1;
            loadFrom += 1;

            if constexpr (algorithm == 1)
                vectorSort(a2, a3);
            else
                vectorSort2(a2, a3);
            store(a2, f);
            load(a2, loadFrom2);

            f += 1;
            loadFrom2 += 1;
        }

        while (loadFrom != endA && loadFrom != endB)
        {
            bool first = *(ui *)loadFrom < *(ui *)opposite;

            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else
                vectorSort2(a0, a1);
            store(a0, c);
            load(a0, loadFrom);

            c += 1;
            loadFrom += 1;
        }

        while (loadFrom2 != endD && loadFrom2 != endE)
        {
            bool first = *(ui *)loadFrom2 < *(ui *)opposite2;
            sse *tmp = first ? loadFrom2 : opposite2;
            opposite2 = first ? opposite2 : loadFrom2;
            loadFrom2 = tmp;

            if constexpr (algorithm == 1)
                vectorSort(a2, a3);
            else
                vectorSort2(a2, a3);
            store(a2, f);
            load(a2, loadFrom2);

            f += 1;
            loadFrom2 += 1;
        }

        // Sort the remaining items of the first split
        if constexpr (algorithm == 1)
            vectorSort(a0, a1);
        else
            vectorSort2(a0, a1);
        store(a0, c);

        c += 1;
        sse *endOp = (loadFrom == endA) ? endB : endA;

        while (opposite != endOp)
        {
            load(a0, opposite);
            if constexpr (algorithm == 1)
                vectorSort(a0, a1);
            else
                vectorSort2(a0, a1);
            store(a0, c);

            opposite += 1;
            c += 1;
        }
        store(a1, c);

        // Sort the remaining items of the second split
        if constexpr (algorithm == 1)
            vectorSort(a2, a3);
        else
            vectorSort2(a2, a3);

        store(a2, f);

        f += 1;
        endOp = (loadFrom2 == endD) ? endE : endD;

        while (opposite2 != endOp)
        {
            load(a2, opposite2);
            if constexpr (algorithm == 1)
                vectorSort(a2, a3);
            else
                vectorSort2(a2, a3);
            store(a2, f);

            opposite2 += 1;
            f += 1;
        }
        store(a3, f);
    }

    inline void rotate(sse &x1, sse &x2)
    {
        sse temp = _mm_alignr_epi8(x1, x2, 4);
        x1 = _mm_alignr_epi8(x2, x1, 4);
        x2 = temp;
    }

    inline void rotateAndSwap(sse &x1, sse &x2, sse &x3, sse &x4)
    {
        minMaxSwap(x1, x3);
        minMaxSwap(x2, x4);
        rotate(x1, x2);

        minMaxSwap(x1, x3);
        minMaxSwap(x2, x4);
        rotate(x1, x2);

        minMaxSwap(x1, x3);
        minMaxSwap(x2, x4);
        rotate(x1, x2);

        minMaxSwap(x1, x3);
        minMaxSwap(x2, x4);
        rotate(x1, x2);

        minMaxSwap(x1, x3);
        // minMaxSwap(x2, x4);
        rotate(x1, x2);

        minMaxSwap(x1, x3);
        // minMaxSwap(x2, x4);
        rotate(x1, x2);

        minMaxSwap(x1, x3);
        // minMaxSwap(x2, x4);
        rotate(x1, x2);

        minMaxSwap(x1, x3);
        // minMaxSwap(x2, x4);
        rotate(x1, x2);
    }

    // inline void rotateAndSwap(sse& x1, sse& x2, sse& x3, sse& x4)
    // {
    // 	minMaxSwap(x1, x3);
    // 	minMaxSwap(x2, x4);
    // 	rotate(x1, x2);
    // 			minMaxSwap(x1, x3);
    // 	minMaxSwap(x2, x4);
    // 	rotate(x1, x2);
    // 			minMaxSwap(x1, x3);
    // 	minMaxSwap(x2, x4);
    // 	rotate(x1, x2);
    // 			minMaxSwap(x1, x3);
    // 	minMaxSwap(x2, x4);
    // 	rotate(x1, x2);
    // 			minMaxSwap(x1, x3);
    // 	minMaxSwap(x2, x4);
    // 	rotate(x1, x2);
    // 			minMaxSwap(x1, x3);
    // 	minMaxSwap(x2, x4);
    // 	rotate(x1, x2);
    // 			minMaxSwap(x1, x3);
    // 	minMaxSwap(x2, x4);
    // 	rotate(x1, x2);
    // 			minMaxSwap(x1, x3);
    // 	minMaxSwap(x2, x4);
    // 	rotate(x1, x2);
    // 			minMaxSwap(x1, x3);
    // 	minMaxSwap(x2, x4);
    // 	//rotate(x1, x2);

    // }

    template <int algorithm = 1>
    inline void rotateAndSwap8(ui *A, ui64 lenA, ui *B, ui64 lenB, ui *C)
    {
        sse *a = (sse *)A, *b = (sse *)B, *endA = (sse *)(A + lenA), *endB = (sse *)(B + lenB), *c = (sse *)C;
        sse a0, a1, b0, b1;
        load(a0, a);
        a += 1;
        load(a1, a);
        a += 1;

        load(b0, b);
        b += 1;
        load(b1, b);
        b += 1;

        sse *loadFrom = a, *opposite = b;

        while (loadFrom != endA && loadFrom != endB)
        {
            bool first = *(ui *)loadFrom < *(ui *)opposite;

            sse *tmp = first ? loadFrom : opposite;
            opposite = first ? opposite : loadFrom;
            loadFrom = tmp;

            rotateAndSwap(a0, a1, b0, b1);

            store(a0, c);
            c += 1;
            store(a1, c);
            c += 1;

            load(a0, loadFrom);
            loadFrom += 1;
            load(a1, loadFrom);
            loadFrom += 1;
        }

        rotateAndSwap(a0, a1, b0, b1);
        store(a0, c);
        c += 1;
        store(a1, c);
        c += 1;

        sse *endOp = (loadFrom == endA) ? endB : endA;

        while (opposite != endOp)
        {
            load(a0, opposite);
            opposite += 1;
            load(a1, opposite);
            opposite += 1;

            rotateAndSwap(a0, a1, b0, b1);
            store(a0, c);
            c += 1;
            store(a1, c);
            c += 1;
        }
        store(b0, c);
        c += 1;
        store(b1, c);
        c += 1;
    }
}