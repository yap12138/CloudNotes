#include <iostream>

using std::cout;

class RVO
{
    int _val;
public:
    RVO(int _non) : _val(_non) { cout<<"construct\n"; }
    ~RVO() { cout<<"destruct\n"; }
    RVO(const RVO& _rvo) { cout<<"cp construct\n"; }
    RVO(RVO&& _rvo) { cout<<"mv construct\n"; }
    const RVO operator++(int) { 
        cout<<"suffix inc\n";
        RVO tmp(this->_val++);
        return  tmp;
    }
};

void transform(RVO& _val) {

}

int main(int argc, char const *argv[])
{
    RVO rvo(77);
    cout<<"---start---\n";
    // transform(88);
    RVO ret = rvo++;

    cout<<"---end---\n";
    return 0;
}
