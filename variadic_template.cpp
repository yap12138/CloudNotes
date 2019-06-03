#include <iostream>
#include <memory>

using namespace std;

template<typename ...Args>
string generate(const char* fmt, const Args& ...args);

int main()
{
    cout << generate("name = %s and id = %d ", "123", 321) << endl;
    return 0;
}

template<typename ...Args>
string generate(const char* where, const Args& ...args)
{
    // string sfmt = fmt;
    // auto size = snprintf(nullptr, 0, sfmt.c_str(), args...);
    // cout << size << endl;
    // //unique_ptr<char[]> buf( new char[size] );
    // sfmt.resize(size);
    // size = snprintf(const_cast<char*>( sfmt.data() ), size, fmt, args...);
    string sql = "select name,db,timestamp,dims,fields,pk,partition,status,geo from task where ";
    {
        auto size = snprintf(nullptr, 0, where, args...);
        unique_ptr<char[]> buf( new char[size] );
        snprintf(buf.get(), size, where, args...);
        sql += buf.get();
    }
    return sql;
}