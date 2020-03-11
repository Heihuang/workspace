/**
 * 
 */

#ifndef MINHEAP_H_
#define MINHEAP_H_
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;
namespace lxh{
class Minheap
{
private:
    /* data */
    vector<int> heap_;
public:
    Minheap(/* args */);
    ~Minheap();
    int Push(int e);
    int Pop();
    int Top();
private:
    inline int getNI(int len){
        return len / 2 - 1;
    }

    inline int getLI(const int i){
        return (2*i+1);
    }
    inline int getRI(const int i){
        return (2*i+2);
    }
    inline int getLNode(const int i){
        int r = getLI(i);
        return heap_[r];
    }
    inline int getRNode(const int i){
        int r = getRI(i);
        return heap_[r];
    }
    inline int isHasLChd(int li, int len){
        return li < len ? 1 : 0;
    }
    inline int isHasRChd(int ri, int len){
        return ri < len ? 1 : 0;
    }
    inline int isGtLN(int r, int ln){
        return r > ln ? 1 : 0;
    }
    inline int isGtRN(int r, int rn){
        return r > rn ? 1 : 0;
    }
    inline int SatisfyLT(int *heap, int i, int li, int len){
        if(isHasLChd(li, len) && isGtLN(heap[i], heap[li])){
            return 1;
        }
        return 0;
    }
    inline int SatisfyRT(int *heap, int i, int ri, int len){
        if(isHasRChd(ri, len) && isGtRN(heap[i], heap[ri])){
            return 1;
        }
        return 0;
    }
    inline int Swap(int &r, int &n)
    {
        int temp(std::move(r));
        r = std::move(n);
        n = std::move(temp);
    }
    int Adjust(int * heap, int len);
};

}
#endif