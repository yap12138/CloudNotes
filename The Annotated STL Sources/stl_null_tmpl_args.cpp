#include <iostream>
#include <cstddef>

using namespace std;

/*
 * 实现所谓的bound friend templates，
 * 也就是说class template的某个实例与其friend function template的某个实例有一对一的关系
 */

class alloc {
};

template <class T, class Alloc = alloc, size_t BufSiz = 0>
class deque {
public:
    deque() { cout << "deque" << ' '; }
};

// 前置声明
template <class T, class Sequence>
class stack;

template <class T, class Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y);

template <class T, class Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y);


template <class T, class Sequence = deque<T> >
class stack {
    // 以下三组效果相同
    // friend bool operator== <T> (const stack<T>&, const stack<T>&);
    // friend bool operator< <T> (const stack<T>&, const stack<T>&);

    // friend bool operator== <T> (const stack&, const stack&);
    // friend bool operator< <T> (const stack&, const stack&);

    friend bool operator== <> (const stack&, const stack&);
    friend bool operator< <> (const stack&, const stack&);

    // 不可以这样写
    // friend bool operator== (const stack&, const stack&);
    // friend bool operator< (const stack&, const stack&);

public:
    stack() { cout << "stack" << endl; }
private:
    Sequence c;
};

template <class T, class Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    cout << "operator==" << '\t';
    return true;
}

template <class T, class Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    cout << "operator<" << '\t';
    return true;
}

int main(int argc, char const *argv[])
{
    stack<int> x;
    stack<int> y;

    cout << (x == y) << endl;
    cout << (x < y) << endl;

    stack<char> y1;
    // cout << (x == y1) << endl;   // error: no match for...
    // cout << (x < y1) << endl;
    return 0;
}
