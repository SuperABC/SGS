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
        std::cout << "true" << std:endl;
    else
        std::cout << "false" << std:endl;
}
void print_a_string(std::string a){
    std::cout << a << std::endl;
}
int a;
int arr[10];
int visitGArray(int i){
    int result;
    result = arr[i];
    return result;
}
int writeGArray(int b[], int i, int v){
    int result;
    b[i] = v;
    return result;
}
int main() {
    writeGArray(arr, 1, 2);
    print_an_int(visitGArray(1));
    return 0;
}