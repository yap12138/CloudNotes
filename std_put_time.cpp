#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

/**
 * @brief C++ 时间格式化函数put_time的使用
 */

// DAY, DD MM YYYY HH:MM:SS GMT
void show_time_format()
{
    //打印到标准输出
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    // std::getlocaltime 获取当地时间, std::gmtime获取格林制时间
    auto tm = *std::gmtime(&t); 
	std::cout  << std::put_time(&tm, "%a, %d %m %Y %H:%M:%S GMT") << std::endl;
 
	//转为字符串
	std::stringstream ss;
	ss << std::put_time(&tm, "%F %T");
    // 在MinGW64上 %F 和 %T等均不可用，其他编译器未测试
	std::string str = ss.str();
    std::cout << str << std::endl;
}

void canonical_example()
{
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    // std::wcout << L"zh_CN: " << std::put_time<wchar_t>(&tm, L"%c %Z") << '\n';
    std::cout << "zh_CN: " << std::put_time(&tm, "%c %z") << '\n';
}

int main(int argc, char const *argv[])
{
    show_time_format();
    canonical_example();
    return 0;
}
