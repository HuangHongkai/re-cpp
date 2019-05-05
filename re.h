#ifndef RE_H
#define RE_H

#include <string>
#include <set>
#include <stdexcept>

/**
 * match为正则表达式匹配测试函数，info存储上一个状态的部分信息
 * @param re_exp 正则表达式，例如123*
 * @param str 需要匹配的字符串
 * @param i  已经完成匹配的re_exp的位置
 * @param j  已经完成匹配的str的位置
 * @param last_info  上一个状态的信息
 * @return
 */
struct info {
    bool flag1; // 存储 + 标志位信息
};

bool match(const std::string& re_exp, const std::string& str, size_t i, size_t j, info* last_info = nullptr) {
    using namespace std;
    size_t len1 = re_exp.size(), len2 = str.size();
    if(i==len1 && j!=len2) return false;
    if(i==len1 && j==len2) return true;
    set<char> chars;
    size_t k = i;
    auto process1 = [&re_exp, &chars](size_t k) { // 处理斜线的情况，例如\w \d \+ \-
        if(re_exp[k] == '\\') {
            if(re_exp[k+1] == 'w') {
                // 英文和数字
                for(char c='a';c<='z';c++) chars.insert(c);
                for(char c='A';c<='Z';c++) chars.insert(c);
                for(char c='0';c<='9';c++) chars.insert(c);
            } else if(re_exp[k+1] == 'd') {
                // 数字
                for(char c='0';c<='9';c++) chars.insert(c);
            } else if(re_exp[k+1] == '+' || re_exp[k+1] == '-' || re_exp[k+1] == '*' ||
                      re_exp[k+1] == '[' || re_exp[k+1] == ']' ||
                      re_exp[k+1] == '{' || re_exp[k+1] == '}')
                chars.insert(re_exp[k+1]);
            else
                // 暂时其他认定为异常
                throw runtime_error("bad expression");
        }
    };
    if(re_exp[i] == '[' ) {
        // 做[]处理
        for(k=i+1;re_exp[k]!=']'&&k<len1;k++) {
            if (re_exp[k] == '\\') {
                process1(k);
                k++;
            } else if((k+2 < len1 &&
                       (isalpha(re_exp[k]) && re_exp[k+1] == '-' && isalpha(re_exp[k+2]))) ||
                      (isdigit(re_exp[k]) && re_exp[k+1] == '-' && isalpha(re_exp[k+2]))) {
                // a-z A-Z 0-9这种类型
                char k1 = re_exp[k], k2 = re_exp[k+2];
                if(k1 > k2) throw  runtime_error("k1 > k2");
                for(char c=k1;c<=k2;c++) chars.insert(c);
            } else {
                chars.insert(re_exp[k]);
            }
        }
    } else if(re_exp[i] == '\\') {
        process1(i);
        k = k+1;
    } else {
        chars.insert(re_exp[i]);
    }
    if(re_exp[k+1] == '*' || re_exp[k+1] == '+') {
        if(chars.find(str[j]) != chars.end() || (re_exp[k] == '.' && j != len2) ) {
            info s{true};
            bool res1 = match(re_exp, str, i, j+1, &s);
            if(re_exp[k+1] == '*')
                return res1 ||  match(re_exp, str, k+2, j+1) || match(re_exp, str, k+2, j);
            else {
                if(res1)
                    return res1;
                if(last_info != nullptr && last_info->flag1)
                    return match(re_exp, str, k+2, j+1);
                return false;
            }
        } else {
            if(re_exp[k+1] == '*')
                return match(re_exp, str, k+2, j); // 忽略*
            else
                return false;
        }
    } else if(re_exp[k+1] == '?') {
        if(chars.find(str[j]) != chars.end() || (re_exp[k] == '.' && j != len2) )
            return match(re_exp, str, k+2, j+1) || match(re_exp, str, k+2, j); // 匹配一次或不匹配
        else
            return match(re_exp, str, k+2, j); // 不匹配
    } // TODO 实现{0,1}这种限制匹配次数的表达式
    else {
        if(chars.find(str[j]) != chars.end() || (re_exp[k] == '.' && j != len2) ) {
            return match(re_exp, str, k+1, j+1);// 停止匹配
        } else {
            return false;
        }
    }
}

#endif //UNTITLED_RE_H
