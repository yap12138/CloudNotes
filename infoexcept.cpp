#include <iostream>
#include <string>
#include <stdexcept>

/** @Hogason 
 * A marco to record file，func，line etc. 
 * info to construct a exception.
*/
#define EXCEPTINFO(msg) \
    std::string("") + __FILE__ + \
     ": " + __FUNCTION__ + \
     ": " + std::to_string(__LINE__) + \
     ": " + msg

class my_exception : public std::logic_error
{
public:
    explicit my_exception(const std::string& _msg) : std::logic_error(_msg) {
        std::cout<<"construct\n";
    }
    ~my_exception() { std::cout<<"destruct\n"; }
    my_exception(const my_exception& except) : std::logic_error(except) {
        std::cout<<"cp construct\n";
    }
    my_exception(my_exception&& except) : std::logic_error(std::move(except)) {
        std::cout<<"mv construct\n";
    }
};

void throw_except() {
    // More effective c++ 中的throw必定产生一次复制构造
    // 在C++11后不存在
    throw my_exception(EXCEPTINFO("this is my test exception."));
}

int main(int argc, char const *argv[])
{
    try {
        throw_except();
    } catch(const std::logic_error& err) {
        std::cout<< err.what() <<std::endl;
        throw;  // C++11后， 不会产生临时对象
    }

    return 0;
}
