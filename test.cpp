#include "re.h"
#include <iostream>
#include <assert.h>

using namespace std;

int test(const string& re_exp, const string& str) {
    static int i = 1;
    bool result = match(re_exp, str, 0, 0);
    cout << "Case: " << i++ << endl;
    cout << "re_exp:" << re_exp << "   str:" << str << endl;
    cout << "result:" << result << endl << endl;
    return result;
}

int main() {
    assert(test("123+", "123333333") == 1); // 匹配
    assert(test("123+", "123") == 0); // 不匹配
    assert(test("123*", "12333333") == 1);
    assert(test("1234*", "123") == 1); // 匹配，因为4可以为0次
    assert(test("12.4", "1234") == 1); // 匹配,因为.匹配任意字符
    assert(test("12.*", "12345678990") == 1); // 通过, .*匹配任意
    assert(test("\\w\\d234\\w", "A12345") == 1); // 支持\w \d
    assert(test("[a-cA-C]bc[acef]", "Bbcf") == 1); // 支持简单的分组
    assert(test("[a-cA-C]+bc[acef]", "BCCAbcf") == 1); // 分组支持+号，该表达式测试通过
    assert(test("[a-cA-C]*bc[acef]", "bcc") == 1); //*支持0次，表达式测试通过
    assert(test("[a-cA-C]+bc[acef]", "bcc") == 0); //该表达式测试不通过，+至少出现一次
    assert(test("[a-cA-C]?bc[acef]", "bbcc") == 1); //通过
    assert(test("[a-cA-C]?bc[acef]", "bcc") == 1); // 通过，忽略 [a-cA-C]?  这个分组（即匹配0次）
    cout << "Accept all cases!" << endl;
}