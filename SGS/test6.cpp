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
class list{
public:
    string name;
    int list[]list;
};
list one;
