#include <iostream>                                

class Foo
{
	int* _i;
public:
	Foo() : _i(new int(77)) {}
	~Foo() { std::cout << "deconstruct\n"; delete _i; }

	void test() const {
		std::cout << __FUNCTION__ << std::endl;
	}
};

int main(int argc, char* argv[])
{
	Foo* p = new Foo();
	p->test();

	delete p;

	delete p;	//gcc 下调用两次析构函数， msvc下crash
	return 0;
}