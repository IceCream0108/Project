#include"./heaptimer.h"

void HeapTimer::SwapNode_(size_t i, size_t j){
    assert(i>=0 && i<heap_.size());
    assert(j>=0 && j<heap_.size());
    swap(heap_[i], heap_[j]);
    ref_[heap_[i].id] = j;
    ref_[heap_[j].id] = i;
}

void HeapTimer::siftup_(size_t i){
    assert(i>=0 && i<heap_.size());
    size_t parent = (i-1)/2;
    while(parent>=0){
        if(heap_[parent] > heap_[i]){
            SwapNode_(i, parent);
            i = parent;
            parent = (i-1)/2;
        }
        else break;
    }
}

bool HeapTimer::siftdown_(size_t i, size_t n){
    assert(i >=0 && i<heap_.size());
    assert(n >=0 && n<heap_.size());
    auto index = i;
    auto child = 2*index+1;
    while (child < n){
        if(child+1 < n && heap_[child+1] < heap_[child])
        {
            child++;
        }
        if(heap_[child] < heap_[index]){
            SwapNode_(index, child);
            index = child;
            child = 2*child+1;
        }
        break;
    }
    return index>i;
}