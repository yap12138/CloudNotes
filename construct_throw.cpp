#include <stdexcept>
#include <iostream>


class cthrow
{
public:
    cthrow() noexcept(true) {
        if (max >= 1) 
            throw_something();
        ++max;
        std::cout << "now count: " << max << std::endl;
    }

    void throw_something() noexcept(false) {
        throw std::logic_error("throw in cthrow's constructor.");
    }

private:
    static int max;
};

int cthrow::max = 0;

int main(int argc, char const *argv[])
{
    cthrow c1;
    
    try
    {
        cthrow c2;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << "exit.\n";
    return 0;
}
