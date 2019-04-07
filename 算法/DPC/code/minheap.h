#ifndef MINHEAP_H
#define MINHEAP_H

const int DefaultSize = 256;

template<class E>
class MinHeap{
private:
    E*heap;//最小堆元素存储数组
    int currentSize;//最小堆当前元素个数
    int maxHeapSize;//最小堆最大容量
    void swap(int x, int y);//交换
    void siftUp(int locate);//向上调整算法
    void siftDown();//从根向下调整
public:
    MinHeap(int sz = DefaultSize);//缺省构造函数
    MinHeap(E arr[], int n);//带参构造函数
    ~MinHeap() { delete[]heap; }//析构函数
    bool Insert(const E&x);//将x插入堆中
    bool RemoveMin(E&x);//删除堆顶元素
    bool IsEmpty()const { return currentSize == 0; }
    bool IsFull()const { return currentSize == maxHeapSize; }
    void MakeEmtpy() { currentSize = 0; }//置空
    void OutPut(E x[]);//输出
};

template<class E>
inline void MinHeap<E>::swap(int x, int y)
{
    E temp = heap[x];
    heap[x] = heap[y];
    heap[y] = temp;
}

template<class E>
inline void MinHeap<E>::siftUp(int locate)
{
    int s = locate;//子
    int f = (s - 1) / 2;//父
    while (s > 0) {
        if (heap[s] < heap[f]) {
            swap(s, f);
            s = f;
            f = (s - 1) / 2;
        }
        else { break; }
    }
}

template<class E>
inline void MinHeap<E>::siftDown()
{
    int i = 0;
    int f = 0, s1 = 0, s2 = 0;
    while (i * 2 + 1 <= currentSize - 1) {
        s1 = i * 2 + 1;
        s2 = s1 + 1;
        f = i;
        if (heap[s1] < heap[f] || heap[s2] < heap[f]) {
            if (heap[s1] < heap[s2]) {
                swap(s1, i);
                i = s1;
            }
            else {
                swap(s2, i);
                i = s2;
            }
        }
        else break;
    }
}

template<class E>
MinHeap<E>::MinHeap(int sz) {
    maxHeapSize = (DefaultSize < sz) ? sz : DefaultSize;
    heap = new E[maxHeapSize];
    currentSize = 0;
}

template<class E>
MinHeap<E>::MinHeap(E arr[], int n)
{
    maxHeapSize = (DefaultSize < n) ? n : DefaultSize;
    heap = new E[maxHeapSize];
    for (int i = 0; i < n; i++)heap[i] = arr[i];
    currentSize = n;
    int currentPos = 1;
    while (currentPos < currentSize)
        siftUp(currentPos++);
}

template<class E>
inline bool MinHeap<E>::Insert(const E & x)
{
    if (IsFull())return false;
    heap[currentSize] = x;
    currentSize++;
    siftUp(currentSize - 1);
    return true;
}

template<class E>
inline bool MinHeap<E>::RemoveMin(E & x)
{
    if(IsEmpty())return false;
    x = heap[0];
    heap[0] = heap[currentSize-- - 1];
    siftDown(); return true;
}

template<class E>
inline void MinHeap<E>::OutPut(E x[])
{
    for (int i = 0; i < currentSize; i++) {
        x[i] = heap[i];
    }
}

#endif // MINHEAP_H
