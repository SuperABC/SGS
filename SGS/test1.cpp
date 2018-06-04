#include <iostream>
#include <string>
using std::string;
int main() {
    int a;
    a = 0;
    int b;
    b = 1;
    bool c;
    c = false;
    b = (b + a);
    c = (b > a);
    if(c){
        b = 5;
    }
    else{
        b = 3;
    }
    print_an_int(b);
    return 0;
}