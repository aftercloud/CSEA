#ifndef GRAPH_H
#define GRAPH_H
#include"Stack.h"
#include"Queue.h"
#include<QFile>
#include<QTextStream>

const int maxSize = 30;
template<class T>
struct Vertex {
    T data;
    int dest;
    int cost;
    Vertex<T>*next;//边
    Vertex<T>*link;//顶点
    Vertex() { next = 0; link = 0; dest = 0; cost=0;}
};

template<class T=char>
class Graph {
public:
    Graph(int max = maxSize);
    ~Graph();
    void deleteG();
    int GetNum() { return VertexNum; }//返回顶点数
    void InsertV(T item);//插入顶点
    void InsertE(int n,int m,int cost=0);//连接索引为n和m的顶点
    void DeleteE(int n,int m);
    //从索引为i的节点开始遍历
    void Di(int begin,int end,QString&s,int edge);
    void Reset(int n,int m,T*a,int*b,int*c,int*d);
    void DFS(int i, T*x);//深度优先遍历
    void BFS(int i, T*x);//广度优先遍历
    bool search();
    void output(QString&s);
    Vertex<T>*NodeTable;
    int VertexNum;//顶点数
    Vertex<T>**Address;//记录每个顶点的地址
    int maxA;//溢出已做处理
};
template<class T>
inline Graph<T>::Graph(int max)
{
    NodeTable = 0;
    VertexNum = 0;
    Address = new Vertex<T>*[max];
    maxA = max;
    for (int i = 0; i < maxA; i++)
    {
        Address[i] = 0;
    }
}
template<class T>
Graph<T>::~Graph()
{
    deleteG();
}
template<class T>
void Graph<T>::deleteG()
{
    Vertex<T>*p1 = 0, *p2 = 0;
    for (int i = VertexNum - 1; i > -1; i--)
    {
        p1 = Address[i];
        p2 = p1->next;
        while (1)
        {
            delete p1;
            p1 = p2;
            if (p2 == 0)break;
            p2 = p1->next;
        }
    }
    delete[]Address;
}
template<class T>
void Graph<T>::InsertV(T item)
{
    if (VertexNum == maxA)//顶点存储达到最大后扩容
    {
        Vertex<T>**temp = new Vertex<T>*[maxA];//temp储存原来Address中的地址
        for (int i = 0; i < maxA; i++)
            temp[i] = Address[i];
        delete[]Address;
        maxA *= 2;
        Address = new Vertex<T>*[maxA];
        for (int i = 0; i < maxA / 2; i++)//将Address重新初始化
            Address[i] = temp[i];
        for (int i = maxA / 2; i < maxA; i++)
            Address[i] = 0;
    }
    if (NodeTable == 0)
    {
        NodeTable = new Vertex<T>;
        NodeTable->data = item;
        NodeTable->dest = VertexNum;
        Address[VertexNum] = NodeTable;
    }
    else {
        Vertex<T>*p = NodeTable;
        while (p->link != 0)
            p = p->link;
        p->link = new Vertex<T>;
        p->link->data = item;
        p->link->dest = VertexNum;
        Address[VertexNum] = p->link;
    }
    VertexNum++;
}

template<class T>
void Graph<T>::InsertE(int n,int m,int cost)
{
    if (VertexNum < n + 1)return;
    Vertex<T>*p = NodeTable;
    for (int i = 0; i < n; i++)
        p = p->link;
    while (p->next != 0)
        p = p->next;
    p->next = new Vertex<T>;
    Vertex<T>*pt = Address[m];
    p->next->cost=cost;
    p->next->dest = pt->dest;
    p = NodeTable;
    for (int i = 0; i < m; i++)
        p = p->link;
    while (p->next != 0)
        p = p->next;
    p->next = new Vertex<T>;
    pt = Address[n];
    p->next->cost=cost;
    p->next->dest = pt->dest;
}

template<class T>
void Graph<T>::Reset(int n, int m, T * a, int * b, int * c,int*d)
{
    deleteG();//清空
    NodeTable = 0;
    VertexNum = 0;
    Address = new Vertex<T>*[n];
    maxA = n;
    for (int i = 0; i < maxA; i++)
    {
        Address[i] = 0;
    }
    for (int i = 0; i < n; i++)
        InsertV(a[i]);
    for (int i = 0; i < m; i++)
        InsertE(b[i],c[i],d[i]);
}

template<class T>
void Graph<T>::DFS(int i, T * x)
{
    int complete = 0;//complete=VertexNum时循环结束
    bool*mark = new bool[VertexNum];//记录顶点是否被遍历过
    for (int j = 0; j < VertexNum; j++)
        mark[j] = false;
    Stack<int>stack; Vertex<T>*t1 = Address[i], *t2 = 0;
    int num = 0, tp1 = 0, tp2 = 0; char temp = 0;
    stack.Push(i);//将开始顶点压入队列
    while (complete < VertexNum) {
        tp1 = stack.Data();
        t1 = Address[tp1];//当前
        if (mark[t1->dest] == false) {
            x[complete++] = t1->data;
            mark[t1->dest] = true;
        }
        while (t1->next!=0) {//进
            if (mark[t1->next->dest]==false) {//t1->next没有被遍历过
                if (t1->next->next != 0)stack.Push(t1->next->next->dest);
                tp1 = t1->next->dest;
                t1 = Address[tp1];
                mark[tp1] = true;
                x[complete++] = t1->data;//将当前栈顶元素输入数组
            }
            else { t1 = t1->next; }//跳过一个节点
        }
    }
    delete[]mark;
}

template<class T>
void Graph<T>::BFS(int i, T * x)
{
    int complete = 0;//complete=VertexNum时循环结束
    bool*mark = new bool[VertexNum];//记录顶点是否被遍历过
    for (int j = 0; j < VertexNum; j++)
        mark[j] = false;
    mark[i] = true;
    LinkedQueue<int>queue;
    int num = 0; char temp = 0;
    queue.EnQueue(i);//将开始顶点压入队列
    Vertex<T>*t1 = 0, *t2 = 0;
    while (!queue.IsEmpty()) {
        queue.DeQueue(num);
        t1 = Address[num];
        temp = t1->data;
        x[complete++]=temp;
        while (t1->next != 0) {
            if (mark[t1->next->dest] == false) {
                mark[t1->next->dest] = true;
                queue.EnQueue(t1->next->dest);
            }
            t1 = t1->next;
        }
    }
    delete[]mark;
}

template<class T>
void Graph<T>::output(QString &s)
{
    s="";Vertex<T>*p=0;
    for(int i=0;i<VertexNum;i++){
        p=Address[i];
        while(p!=0){
            s+=QString::number(p->dest);
            s+="->";p=p->next;
        }
        s+="NULL\r\n";
    }
}

template<class T>
void Graph<T>::DeleteE(int n, int m)
{
    if (VertexNum < n + 1)return;

    Vertex<T>*p = NodeTable;
    for (int i = 0; i < n; i++)
        p = p->link;
    while (p->next->dest != m)
        p = p->next;
    Vertex<T>*pt = p->next->next;
    delete p->next;
    p->next=pt;

    p = NodeTable;
    for (int i = 0; i < m; i++)
        p = p->link;
    while (p->next->dest != n)
        p = p->next;
    pt = p->next->next;
    delete p->next;
    p->next=pt;
}

template<class T>
bool Graph<T>::search()//图能连通则返回true
{
    int complete = 0;//complete=VertexNum时循环结束
    bool*mark = new bool[VertexNum];//记录顶点是否被遍历过
    for (int j = 0; j < VertexNum; j++)
        mark[j] = false;
    mark[0] = true;
    LinkedQueue<int>queue;
    int num = 0; char temp = 0;
    queue.EnQueue(0);//将开始顶点压入队列
    Vertex<T>*t1 = 0, *t2 = 0;
    while (!queue.IsEmpty()) {
        queue.DeQueue(num);
        t1 = Address[num];
        temp = t1->data;
        complete++;
        while (t1->next != 0) {
            if (mark[t1->next->dest] == false) {
                mark[t1->next->dest] = true;
                queue.EnQueue(t1->next->dest);
            }
            t1 = t1->next;
        }
    }
    delete[]mark;
    return complete==VertexNum?true:false;
}

template<class T>
void Graph<T>::Di(int begin, int end, QString&s,int edge)
{
    Vertex<T>*p=Address[begin];
    int*distance=new int[edge];//记录begin到各点的距离
    QString*road=new QString[edge];
    for(int i=0;i<edge;i++)
    {
        distance[i]=418000000;
        road[i]="";
    }
    Stack<int>ret;int stack=0;
    road[begin]+=QChar(p->data);
    distance[begin]=0;
    bool*mark = new bool[VertexNum];//记录顶点是否被遍历过
    for (int j = 0; j < VertexNum; j++)
        mark[j] = false;
    mark[begin] = true;
    int min=2140000000;int lom=-1;
    for (int i=0;i<edge-1;i++) {
        p = Address[begin];
        mark[begin]=true;
        ret.Push(begin);
        while (p->next != 0) {//获取相对于begin的短程线
            if (mark[p->next->dest] == false) {
                if(distance[p->next->dest]>distance[begin]+p->next->cost)
                {
                    distance[p->next->dest]=distance[begin]+p->next->cost;
                        road[p->next->dest]=road[begin];
                        road[p->next->dest]+=QChar(p->next->dest+65);
                }
            }
            p = p->next;
        }
        for (int j=0;j<edge;j++) {//获取相对于begin的短程线
                if(distance[j]<min&&!mark[j])
                {
                    min=distance[j];
                    lom=j;
                }
        }
            begin=lom;
            min=2140000000;
    }
    s=road[end];
    delete[]road;
    delete[]distance;
    delete[]mark;
}

#endif // GRAPH_H
