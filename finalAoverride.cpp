#include <iostream>
 
// 标示符含义：
// override，表示此虚函数必定“重写”了基类中的对应虚函数。
// final，(1)作用在虚函数：表示此虚函数已处在“最终”状态，后代类必定不能重写这个虚函数。
//        (2)作用在类：表示此类必定不能被继承
// 编译器将帮你检查是否“必定”
 
class B1 final {}; // 此类不能被继承
// class D1: public B1 {}; // error!
 
class B
{
public:
// 	virtual void func() override // error! 指定了重写但实际并没重写,没有基类
// 	{ 
// 		std::cout << __FUNCTION__ << std::endl;
// 	}
	virtual void f() const 
	{ 
		std::cout << __FUNCTION__ << std::endl;
	}
	virtual void fun() 
	{ 
		std::cout << __FUNCTION__ << std::endl; 
	}
};
 
class D : public B
{
public:
	virtual void f(int)		 // ok! 隐藏,由于没有重写同名函数B::f,在D中变为不可见
	{ 
		std::cout << "hiding: " <<__FUNCTION__ << std::endl; 
	} 
// 	virtual void f() override	// error! 指定了重写但实际并没重写,类型声明不完全相同
// 	{ 
// 		std::cout << __FUNCTION__ << std::endl; 
// 	} 
	virtual void fun() override final // ok! 指定了重写实际上也重写了，同时，指定为最终，后代类中不能再重写此虚函数
	{ 
		std::cout << __FUNCTION__ << std::endl; 
	} 
};
 
class D2 : public D
{
public:
	virtual void f() const      // ok! 重写B::f(),同时,由于没有重写D::f(int),在D2中变不可见
	{ 
		std::cout << __FUNCTION__ << std::endl;
	}
// 	virtual void fun() // error! 基类的此虚函数被指定为最终,不能被重写,虽然没有显示指定"override"
// 	{
// 		std::cout << __FUNCTION__ << std::endl;
// 	}
// 	virtual void fun() override // error! 基类的此虚函数被指定为最终，不能被重写
// 	{
// 		std::cout << __FUNCTION__ << std::endl;
// 	} 
};
 
int main(int argc, char* argv[])
{
	D2 d2;
	D d1;
	B *b = &d1;
	b->f();		// B::f
	b->fun();	// D::fun
//	d1.f();		// error! B::f在D中不可见
	d1.f(10);	// hiding: D::f
//	d2.f(10);	// error! D::f(int)在D2中不可见
	d2.f();		// D2::f
	d2.fun();	// D::fun
 
	return 0;
}