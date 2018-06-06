#include <iostream>
#include <string>
using std::string;
int visitGArray(int i);
int visitGArray(int i){
    int result;
    result = arr[i];
}
int writeGArray(int b[], int i, int v);
int writeGArray(int b[], int i, int v){
    int result;
    b[i] = v;
}
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
    int i;
    i = 1;
    float fact;
    fact = 1;
    float res;
    res = 1;
    while((i < 11)){
        res = (res + (1 / fact));
        fact = (fact * intToFloat(i));
        i = (i + 1);
    }
    print_a_number(res);
    int a[10];
    a[0] = 0;
    a[1] = (a[0] + 5);
    print_a_number(a);
    int a;
    int arr[10];
    writeGArray(arr, 1, 2);
    print_a_number(visitGArray(1));
    char x[4];
    x = "123";
    x[0] = (x[0] + 1);
    print_a_str(x);
    return 0;
}