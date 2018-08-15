#include <iostream>

using std::cout;
using std::endl;

void _new_handler()
{
    cout<<"new handler custom."<<endl;
    std::set_new_handler(nullptr);
}

template<typename T>
class NewHandleSupport
{
    class AutoResume final
    {
    public:
        AutoResume(std::new_handler _hd) : _holdee(_hd) {}
        ~AutoResume() { std::set_new_handler(_holdee); }
    private:
        std::new_handler _holdee;
    };

private:
    static std::new_handler _handle;
public:
    NewHandleSupport() = default;

    void* operator new(std::size_t _size) {
        cout<<"base new"<<endl;
        AutoResume resume(std::set_new_handler(_handle));
        return ::operator new(_size);
    }

    void set_new_handler(std::new_handler handler) { _handle = handler; }
};

template<typename T>
std::new_handler NewHandleSupport<T>::_handle = _new_handler;

class Base : public NewHandleSupport<Base>
{
private:
    /* data */
public:
    Base(/* args */) { }

    /*void* operator new(std::size_t _size) 
    {
        cout<<"Base assign"<<endl;
        return ::operator new(_size);
    }*/
protected:
    ~Base() { }
};

int main(int argc, char const *argv[])
{
    Base* b1 = new Base;
    return 0;
}
