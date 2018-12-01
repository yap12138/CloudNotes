#include <regex>
#include <iostream>

using namespace std;

/**
 * 记录有关正则表达式的用法
 */

#define TEST(types, case) void Test##types##_##case()

/**
 * 使用regex_match，显示是否全文匹配规则
 * @param str 主串
 * @param reg 模式串 
 */
template<typename Str1, typename Str2>
void show_match(Str1&& str, Str2&& reg)
{
    string src(std::forward<Str1>(str));
    regex pattern(std::forward<Str2>(reg));
    smatch result;
    if (regex_match(src, result, pattern)) {
        for (int i = 0; i < result.size(); ++i) {
            cout << "[" << i << "]: " << result[i] << endl;
        }
    }
}

/**
 * 使用regex_regex，显示是否存在匹配规则的子串
 * @param str 主串
 * @param reg 模式串 
 */
template<typename Str1, typename Str2>
void show_search(Str1&& str, Str2&& reg)
{
    string src(std::forward<Str1>(str));
    regex pattern(std::forward<Str2>(reg));
    smatch result;
    
    auto iterStart = std::cbegin(src);
    auto iterEnd = std::cend(src);
    auto cnt = 0;
    while (regex_search(iterStart, iterEnd, result, pattern)) {
        cout << "match case<" << ++cnt << ">\n";
        for (int i = 0; i < result.size(); ++i) {
            cout << "[" << i << "]: " << result[i] << endl;
        }
        iterStart = result[0].second;
    }
}

TEST(Regex, Url)
{
    cout << "{" << __func__ << "}\n"; 
    show_match("http://www.runoob.com:80/html/html-tutorial.html", "(\\w+)://([^/:]+)(:\\d*)?([^# ]*)");
    // C++中正则表达式转义字符需要双反斜线，即"\\"
    // 一个()代表一个子表达式，匹配的规则的串存放于smatch(cmatch)下标为0的位置，每个子表达式按顺序往后递推
}

TEST(Regex, Constraint)
{
    cout << "{" << __func__ << "}\n"; 
    show_match("<H1> Chapter 1 </H1>", "(<.*?>).*((\\bCha\\w*) ([1-9][0-9]?))\\s*(</.*?>)");
    // 嵌套()的表达式存放父表达式后，仍按顺序地推
}

TEST(Regex, Cookie)
{
    cout << "{" << __func__ << "}\n"; 
    show_search("pgv_pvi=9676536832; RK=rVIcGVh1Ek; ptcz=fdaf941595f8b87404c43f57b12ccbdaa0b8a8cc9936b1d01140697c94404385; "
        "webp=1; tvfe_boss_uuid=77819c835d09153b; pgv_pvid=3319213808; o_cookie=438773897; ptui_loginuin=438773897@qq.com; "
        "pt2gguin=o0438773897; edition=mail.qq.com; CCSHOW=000001; ptisp=cm; pgv_si=s221295616", "([^=]+)=([^;]+);?\\s*");
    // [^=]中的^表示不接受，即此表达式表示接受除[]中(此处为=)字符的串
    // 匹配cookie中，第一部分取在“=”之前的，即 ([^=]+)
    // \s 匹配任何空白字符，包括空格、制表符、换页符
}

int main(int argc, char const *argv[])
{
    // TestRegex_Url();
    // TestRegex_Constraint();
    TestRegex_Cookie();
    return 0;
}
