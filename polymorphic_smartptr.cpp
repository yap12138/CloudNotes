#include <iostream>

template<typename T>
class smart_ptr
{
public:
    smart_ptr(T* raw_ptr) : _ptr(raw_ptr) {}
    ~smart_ptr() {
        if (_ptr != nullptr) {
            delete _ptr;
        }
    }

    smart_ptr(const smart_ptr<T>& other) = default;

    template<typename U>
    smart_ptr(smart_ptr<U>& other) : _ptr(other.get()) { other.reset(); }

    T* get() const { return _ptr; }

    void reset(T* raw_ptr = nullptr) { _ptr = raw_ptr; }

    T* operator->() { return _ptr; }

private:
    T* _ptr;
};

class ABC 
{
public:
    virtual ~ABC() = 0; // 纯虚析构函数

    virtual void say() const = 0;
};

// 包含纯虚析构函数，则类必须给此函数定义函数体。
ABC::~ABC() {}

class Base : public ABC
{
public:
    ~Base() override {}

    void say() const override {
        std::cout << "Base\n";
    }
};

class Derive : public Base
{
public:
    void say() const override {
        std::cout << "Derive\n";
    }
};

void use_smart_ptr(smart_ptr<ABC> sp) {
    std::cout << "argument: sp<ABC>\n";
    sp->say();
}

void use_smart_ptr(smart_ptr<Base> sp) {
    std::cout << "argument: sp<Base>\n";
    sp->say();
}

int main(int argc, char const *argv[])
{
    smart_ptr<Derive> ptr(new Derive);
    // error: call of overloaded 'use_smart_ptr(smart_ptr<Derive>&)' is ambiguous
    // use_smart_ptr(ptr);
    use_smart_ptr(static_cast<smart_ptr<ABC>>(ptr));
    return 0;
}
