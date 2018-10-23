#include <iostream>       // std::cout
#include <string>         // std::string
#include <bitset>         // std::bitset

/**
bitset存储二进制数位。

bitset就像一个bool类型的数组一样，但是有空间优化——bitset中的一个元素一般只占1 bit，相当于一个char元素所占空间的八分之一。

bitset中的每个元素都能单独被访问，例如对于一个叫做foo的bitset，表达式foo[3]访问了它的第4个元素，就像数组一样。

bitset有一个特性：整数类型和布尔数组都能转化成bitset。

bitset的大小在编译时就需要确定。如果你想要不确定长度的bitset，请使用（奇葩的）vector<bool>。
*/

/**
bitset的运算就像一个普通的整数一样，可以进行与(&)、或(|)、异或(^)、左移(<<)、右移(>>)等操作。
*/

int main ()
{
  std::bitset<4> foo (std::string("1001"));
  std::bitset<4> bar (std::string("0011"));

  std::cout << (foo^=bar) << '\n';       // 1010 (XOR,assign)
  std::cout << (foo&=bar) << '\n';       // 0010 (AND,assign)
  std::cout << (foo|=bar) << '\n';       // 0011 (OR,assign)

  std::cout << (foo<<=2) << '\n';        // 1100 (SHL,assign)
  std::cout << (foo>>=1) << '\n';        // 0110 (SHR,assign)

  std::cout << (~bar) << '\n';           // 1100 (NOT)
  std::cout << (bar<<1) << '\n';         // 0110 (SHL)
  std::cout << (bar>>1) << '\n';         // 0001 (SHR)

  std::cout << (foo==bar) << '\n';       // false (0110==0011)
  std::cout << (foo!=bar) << '\n';       // true  (0110!=0011)

  std::cout << (foo&bar) << '\n';        // 0010
  std::cout << (foo|bar) << '\n';        // 0111
  std::cout << (foo^bar) << '\n';        // 0101

  return 0;
}