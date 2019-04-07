#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include<QString>
#include"MinHeap.h"

template<class T>
struct HuffmanNode
{
    T data;//权
    char huf;//值
    HuffmanNode*parent, *leftChild, *rightChild;
    HuffmanNode() :parent(0), leftChild(0), rightChild(0) {}
    HuffmanNode(T elem, HuffmanNode*pa = 0, HuffmanNode*left = 0, HuffmanNode*right = 0) :data(elem), parent(pa), leftChild(left), rightChild(right) {}
    bool operator<(HuffmanNode<T>&R) { return data < R.data; }
    void operator=(const HuffmanNode<T>&R) {
        data = R.data;
        leftChild = R.leftChild;
        rightChild = R.rightChild;
        parent = R.parent;
        huf = R.huf;
    }
};

template<class T>
class HuffmanTree {
public:
    HuffmanNode<T>*root;
    HuffmanTree(T w[], int n);
    ~HuffmanTree() { deleteTree(root); }
    void output(HuffmanNode<T>*t, QString*q,char*s,int&n,QString qs);
protected:
    void deleteTree(HuffmanNode<T>*t);
    void mergeTree(HuffmanNode<T>&bt1, HuffmanNode<T>&bt2, HuffmanNode<T>*&parent);
};

template<class T>
HuffmanTree<T>::HuffmanTree(T w[], int n)
{
    MinHeap<HuffmanNode<T>> hp;
    HuffmanNode<T>*parent = 0, *first = 0, *second = 0, *work = 0;
    first = new HuffmanNode<T>;
    second = new HuffmanNode<T>;
    work = new HuffmanNode<T>;
    for (int i = 0; i < 256; i++) {
        if (w[i] != 0) {//当某个编码出现过
            work->data = w[i];//编码出现的次数
            work->huf=i;
            work->leftChild = work->rightChild = work->parent = 0;
            hp.Insert(*work);
        }
    }
    for (int i = 0; i < n - 1; i++) {
        hp.RemoveMin(*first);
        hp.RemoveMin(*second);
        mergeTree(*first, *second, parent);
        hp.Insert(*parent);
    }
    root = parent;
}

template<class T>
void HuffmanTree<T>::deleteTree(HuffmanNode<T>* t)
{
    if (t != 0) {
        deleteTree(t->leftChild);
        deleteTree(t->rightChild);
        delete t;
    }
}

template<class T>
inline void HuffmanTree<T>::mergeTree(HuffmanNode<T>& bt1, HuffmanNode<T>& bt2, HuffmanNode<T>*& parent)
{
    HuffmanNode<T>*p = new HuffmanNode<T>;
    p->leftChild= new HuffmanNode<T>;
    p->rightChild = new HuffmanNode<T>;
    *(p->leftChild) = bt1;
    *(p->rightChild) = bt2;
    p->data = bt1.data + bt2.data;
    parent = bt1.parent = bt2.parent = p;
}

template<class T>
void HuffmanTree<T>::output(HuffmanNode<T>*t, QString*qt,char*s, int&n,QString qs)
{
    if(t->leftChild!=0||t->rightChild!=0){//当t不为叶节点
        QString st=qs;
        st+="0";
        output(t->leftChild,qt,s,n,st);
        st=qs;
        st+="1";
        output(t->rightChild,qt,s,n,st);
    }
    else{
        s[n]=t->huf;
        qt[n++]=qs;
    }
}



#endif // HUFFMANTREE_H
