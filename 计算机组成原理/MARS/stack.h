#ifndef STACK_H
#define STACK_H
template<typename T>
struct StackNode
{
    T data;
    StackNode<T>*link;
    StackNode() { link = 0; }
    StackNode(T x, StackNode<T>*p = 0) { data = x; link = p; }
};

template<typename T>
class Stack
{
    StackNode<T>*top;
public:
    Stack() { top = 0; }
    ~Stack() { MakeEmpty(); }
    void Push(T x);//进栈
    void Pop(T&x);//退栈
    T Data() { return top->data; }
    bool IsEmpty() { return top == 0; }
    void MakeEmpty();
};

template<typename T>
void Stack<T>::Push(T x)
{
    top = new StackNode<T>(x, top);
}

template<typename T>
void Stack<T>::Pop(T & x)
{
    if (IsEmpty() == true)return;
    StackNode<T>*p = top;
    top = top->link;
    x = p->data;
    delete p;
}

template<typename T>
void Stack<T>::MakeEmpty()
{
    StackNode<T>*p = 0;
    while (top != 0) {
        p = top;
        top = top->link;
        delete p;
    }
}
#endif // STACK_H
