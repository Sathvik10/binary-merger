#include <iostream>
#include "commons.h"

// Assuming definitions of ui64 and inline
using ui64 = unsigned long long; // Change to the appropriate unsigned 64-bit integer type

// The inline macro (replace it with appropriate definition if available)
#define inline inline


template< typename Item>
inline void addElements(Item* a, ui64 low, ui64 high, Item* c)
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
inline int binarySearch(Item* a, ui64 low, ui64 high, Item b, ui64 lenA) {
    std::cout << " Binary Search low : " << low  << " high : " << high  << " b: " << b << std::endl;

    while (low < high) {
        int mid = low + (high - low) / 2;

        if (a[mid] <= b) {
            if (mid + 1 < lenA && a[mid + 1] > b) {
                return mid; // Found the index i
            } else {
                low = mid + 1; // Move to the right half
            }
        } else {
            high = mid; // Move to the left half
        }
    }
    std::cout << " Binary Search low : " << low  << " high : " << high  << " b: " << b << " low: " << low << std::endl;


    return low; // Return low index if not found
}

template<typename Reg, typename Item>
inline void binaryMerge(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C)
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

int main() {
    // Sample usage of binaryMerge function
    const ui64 lenA = 5;
    const ui64 lenB = 5;

	using Itemtype = ui;
	using Regtype = Itemtype;

    // Itemtype A[lenA] = {1, 3, 5, 7, 9};
    // Itemtype B[lenB] = {2, 4, 6, 8, 10};
    // Itemtype C[lenA + lenB]; // Resulting merged array

    ui64 sz = 5 *  sizeof(Itemtype);
	Itemtype* A = (Itemtype*)VALLOC(sz);
	Itemtype* B = (Itemtype*)VALLOC(sz);
	Itemtype* C = (Itemtype*)VALLOC(2 * sz);

    for(int i = 0 ; i < 5 ; i++)
    {
        A[i] = 3 * (i + 1);
        B[i] = 2 * (i + 1);
    }

    std::cout << "Values of A: ";
    for (ui64 i = 0; i < lenA; ++i) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Values of B: ";
    for (ui64 i = 0; i < lenA; ++i) {
        std::cout << B[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Before Merging array: ";
    for (ui64 i = 0; i < lenA + lenB; ++i) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;

    // Call to binaryMerge function
    binaryMerge<Regtype, Itemtype>(A, lenA, B, lenB, C);

    // Displaying the merged array C (this part is optional)
    std::cout << "Merged array: ";
    for (ui64 i = 0; i < lenA + lenB; ++i) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
