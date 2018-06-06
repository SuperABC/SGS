#include <iostream>
#include <string>
using std::string;
int main() {
    char x[4];
    x = "123";
    x[0] = (x[0] + 1);
    print_a_str(x);
    return 0;
}