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
    int arr[10];
    writeGArray(arr, 1, 2);
    print_a_number(visitGArray(1));
    return 0;
}