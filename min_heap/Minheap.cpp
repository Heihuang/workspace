#include "Minheap.h"
namespace lxh{
Minheap::Minheap(/* args */)
{
}

Minheap::~Minheap()
{
}

int Minheap::Push(int e){
    int a [] = {1,20,3,14,5,6};
    Adjust(a, 6);
    for(size_t i = 0; i < 6; ++i){
        std::cout << a[i] << ",";
    }
    return 0;
}
int Minheap::Pop(){
    
    return 0;
}
int Minheap::Top(){
    return 0;
}

int Minheap::Adjust(int * heap, int len){
    int temp = 0;
    for(int i = len/2 - 1; i >= 0; i--){
        
        int li = getLI(i);
        int ri = getRI(i);
        if(SatisfyLT(heap, i, li, len)){
            Swap(heap[i], heap[li]);
            if(SatisfyLT(heap, li, getLI(li), len) || SatisfyRT(heap, li, getRI(li), len)){
                Adjust(heap, len);
            }
        }
        if(SatisfyRT(heap, i, ri, len)){
            Swap(heap[i], heap[ri]);
            if(SatisfyLT(heap, ri, getLI(ri), len) || SatisfyRT(heap, ri, getRI(ri), len)){
               Adjust(heap, len); 
            }
        }
        
    }
    return 0;
}

}