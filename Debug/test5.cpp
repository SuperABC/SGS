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

string x;
int main() {
    x = "123";
    break;
    x[0] = (x[0] + 1);
    print_a_str(x);
    return 0;
}