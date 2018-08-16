#include <iostream>
#include <random>

/** normal random device */
void test1() {
  std::random_device rd;
  std::cout << "max: " << rd.max() << " min: " << rd.min() << "\n";
  for(int n=0; n<10; ++n)
    std::cout << rd() << std::endl;
}

/** random number engine
 *  mersenne_twister_engine */
void test2() {
  std::random_device rd;
  std::mt19937 mt(rd());
  for(int n = 0; n < 10; n++)
    std::cout << mt() << std::endl;
}

/** random number distributions */
void test3() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 6);
  for (int n = 0; n < 10; ++n)
      std::cout << dis(gen) << ' ';
  std::cout << '\n';
}

int main(int argc, char const *argv[])
{
  /* code */
  // test1();
  test3();
  return 0;
}
