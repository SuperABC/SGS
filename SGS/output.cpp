#include <iostream>
#include <string>
using std::string;
void print_an_int(int value);
void print_a_number(float value);
void print_a_boolean(bool value);
void print_a_str(string value);
int current_time();
int add(int a, int b);
int add(int a, int b){
<<<<<<< HEAD
    int result;
    result = (a + b);
    return result;
}
class point{
public:
    int x;
    int y;
};
int main(){
    int a;
    a = 10;
    print_an_int(a);
    int test_i;
    test_i = 10;
    print_an_int(test_i);
    float test_f;
    test_f = 0;
    print_a_number(test_f);
    bool test_b;
    test_b = true;
    print_a_boolean(test_b);
    string test_s;
    test_s = "hello world";
    print_a_str(test_s);
    int rounds;
    rounds = 10;
    while((rounds > 0)){
        if((rounds > 5)){
            rounds = (rounds - 2);
        }
        else{
            rounds = (rounds - 1);
        }
        print_an_int(rounds);
    }
    print_an_int(add(1, 2));
    string str_list[10];
    str_list[0] = "abc";
    str_list[1] = "def";
    str_list[2] = "ghi";
    int iterator;
    iterator = 0;
    while((iterator < 3)){
        print_a_str(str_list[iterator]);
        iterator = (iterator + 1);
    }
    point left_top;
    left_top = (0, 0);
    point right_bottom;
    right_bottom = (10, 10);
    print_a_number(left_top.x);
    print_a_number(right_bottom.y);
    point test_point;
    test_point = (1, 1);
    print_a_number(((2 * 2) + ((7 & (3 > (2 + (test_point.x * test_point.y)))) / 2)));
    print_a_boolean((true && (false || (true + false))));
    return 0;
=======
int result;
result = (a + b);
return result;
}
class point{
public:
int x;
int y;
}
class point{
public:
int x;
int y;
}
int main(){
int a;
a = 10;
print_an_int(a);
int test_i;
test_i = 10;
print_an_int(test_i);
float test_f;
test_f = 0;
print_a_number(test_f);
bool test_b;
test_b = true;
print_a_boolean(test_b);
string test_s;
test_s = "hello world";
print_a_str(test_s);
int rounds;
rounds = 10;
while((rounds > 0)){
if((rounds > 5)){
rounds = (rounds - 2);
}
else{
rounds = (rounds - 1);
}
print_an_int(rounds);
}
print_an_int(add(1, 2));
string str_list[10];
str_list[0] = "abc";
str_list[1] = "def";
str_list[2] = "ghi";
int iterator;
iterator = 0;
while((iterator < 3)){
print_a_str(str_list[iterator]);
iterator = (iterator + 1);
}
point left_top;
left_top = (0, 0);
point right_bottom;
right_bottom = (10, 10);
print_a_number(left_top.x);
print_a_number(right_bottom.y);
point test_point;
test_point = (1, 1);
print_a_number(((2 * 2) + ((7 & (3 > (2 + (test_point.x * test_point.y)))) / 2)));
print_a_boolean((true && (false || (true + false))));
return 0;
>>>>>>> parent of dd0fa55... Add code gen.
}