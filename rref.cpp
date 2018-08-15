#include <iostream>
#include <string>

/**@Ref hcb
 * @Modified Hogason
 */

using namespace std;

class TestClass
{
  private:
    static int ct;
    string message;

  public:
    TestClass();
    explicit TestClass(const string & newmessage) ;
    TestClass(TestClass && f);
    TestClass(const TestClass &f) = delete;
    TestClass operator+(const TestClass &f) const;
    TestClass & operator=(const TestClass &f) = delete; 
    TestClass & operator=(TestClass && f) ;

    void NotoUse() = delete;
    void showObject() const;
    ~TestClass();
};

//静态变量必须先初始化，不然就会出现未定义的引用，且类的静态数据成员的初始化必须在类外进行
int TestClass::ct = 0;

/*void TestClass::NotoUse(){
    cout<<"do not use me !"<< endl;
}*/

TestClass::TestClass()
    : message("hello world")
{
    ++ct;
}

TestClass::TestClass(const string & newmessage) 
    : message(newmessage)
{
    ++ct;
    cout << "string constructor called;number of objects:" << ct << endl;
}

// 使用默认的复制构造函数
// TestClass::TestClass(const TestClass & f){
//     ++ct;
//     cout << "copy constructor called;number of objects:" << ct << endl;
//     this->message = f.message;
// }

TestClass TestClass::operator+(const TestClass & f) const{
    cout << "Enter the operator +:"<<endl;
    /**@Modified */
    TestClass temp(this->message + f.message);
    cout << &temp << endl;
    return temp;
}

// 使用默认的复制赋值运算符 
// TestClass & TestClass::operator=(const TestClass & f) {
//     cout<< "Enter the operator = which is copy"<<endl;
//     if (this == &f){
//         return *this;
//     }
//     return *this;
// }

TestClass & TestClass::operator=(TestClass && f){
    cout<<"Enter the operator = which is move"<<endl;
    if (this == &f){
        return *this;
    }
    this->message = std::move(f.message);
    return *this;
}

void TestClass::showObject() const {
    cout << "This message is : " << message << endl;
    /**@Modified */
    cout << "Date address is : " << reinterpret_cast<const void*>(message.c_str()) << endl;
}

TestClass::~TestClass(){
    cout << "destructor called;objects left:"<< --ct << endl;
}

TestClass::TestClass(TestClass && f) 
    : message(std::move(f.message))
{
    ++ct;
    cout << "move constructor called;number of objects:" << ct << endl;

}

int main()
{
    namespace hogason = std;
    /**
     * 移动语义以及右值引用
     **/
    TestClass test1("hello world version 1");
    test1.showObject();
    hogason::cout<<hogason::endl;

    /**@Modified */
    TestClass test2(hogason::move(test1));
    test2.showObject();
    hogason::cout<<hogason::endl;

    TestClass test3; //假如在初始化的时候使用赋值运算符，那么调用的将会是构造函数而非是赋值运算符 如 TestClass test3 = test1；
    // test3 = test1; //test1是左值，调用的是复制赋值运算符 
    // test3.showObject();
    test3 = hogason::move(test2); //把左值强制转换为右值，这时调用的是移动赋值运算符
    test3.showObject();
    hogason::cout<<hogason::endl;

    /** -------case 1-------*/
    // TestClass test4; test4 = test1+test2;
    /** -------case 2-------*/
    TestClass test4 = test1+test2;

    hogason::cout << &test4 << hogason::endl;
    test4.showObject();
    hogason::cout<<hogason::endl;
    return 0;
}