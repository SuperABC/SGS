#include <iostream>
#include <string>
using std::string;
int main() {
    int a[10];
    a[0] = 0;
    a[1] = (a[0] + 5);
    print_an_int(a[1]);
    return 0;
}