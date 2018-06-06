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

void print_a_string(std::string a){
    std::cout << a << std::endl;
}

float intToFloat(int a) {
    return a;
}

int floatToInt(float a) {
    return a;
}

int i;
float fact;
float res;
int main() {
    i = 1;
    fact = 1;
    res = 0;
    while((i < 11)){
        res = (res + (1 / fact));
        fact = (fact * intToFloat(i));
        i = (i + 1);
    }
    print_a_number(res);
    return 0;
}