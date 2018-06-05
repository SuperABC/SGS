#include <iostream>
#include <string>
using std::string;
int main() {
    int i;
    i = 1;
    float fact;
    fact = 1;
    float res;
    res = 0;
    while((i < 11)){
        res = (res + (1 / fact));
        fact = (fact * intToFloat(i));
        i = (i + 1);
    }
    print_a_number(res);
    return 0;
}