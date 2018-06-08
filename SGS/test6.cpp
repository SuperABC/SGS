#include <iostream>
#include <string>
#include <cstdio>
using std::string;

void print_an_int(int a){
    printf("%d\n", a);
}

void print_a_number(double a){
    printf("%lf\n", a);
}

void print_a_bool(bool a){
    if(a == true)
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
}

void print_a_str(std::string a){
    std::cout << a << std::endl;
}

float intToFloat(int a) {
    return a;
}

int floatToInt(float a) {
    return a;
}

class list{
public:
    int name;
    int list[10];
};
list one;
list lists[10];
int main() {
    one.list[0] = 1;
    print_an_int(one.list[0]);
    lists[0].list[0] = 2;
    print_an_int(lists[0].list[0]);
    return 0;
}