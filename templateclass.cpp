#include <iostream>
#include <string>

template<typename T>
class smart_ptr
{
public:
    template<typename U>
    explicit smart_ptr(smart_ptr<U> & smptr) : _ptr(smptr.get()) {}

    explicit smart_ptr(T* instance) : _ptr(instance) {}

    ~smart_ptr() { 
        if (_ptr != nullptr) {
            delete _ptr;
        }
    }

    T* get() const { return _ptr; };
private:
    T* _ptr;
};

class B
{
    B(const B& other) = delete;
    B& operator=(const B& other) = delete;
public:
    B(int val = 777) : _id(val) {}
    B(B&& _rigth) = default;

    virtual int getIdentifier() const { return _id;}
private:
    int _id;
};

class D : public B
{
public:
    D(int val = 666, int offset = 0) : B(val), _offset(offset) {}
    virtual int getIdentifier() const override final { return B::getIdentifier() + _offset;}
private:
    int _offset;
};

int main(int argc, char const *argv[])
{
    using std::string;
    smart_ptr<string> ptr1(new string("hogason test template"));
    std::cout<<*(ptr1.get())<<std::endl;

    smart_ptr<D> ptrD(new D(123, 111));
    //smart_ptr<B> ptrB(ptr1); //error can't implicit cast from 'string' to 'B'
    smart_ptr<B> ptrB(ptrD); //ok. it can implicit cast from derive to base class.
    std::cout<<ptrB.get()->getIdentifier()<<std::endl;
    return 0;
}
