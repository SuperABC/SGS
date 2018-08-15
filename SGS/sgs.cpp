#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

void print_int(int content);
void print_number(float content);
void print_ch(char content);
void print_str(string content);
int input_int();
float input_number();
char input_ch();
string input_str();
string input_line();
void skip_line();
int open_file(string path);
string read_fp(int fp, int length);

class file {
public:
    string path;
    int fp;
    int index;
};

string read_file(file content, int length);

int rounds;
int sum;
int main() {
    rounds = 10000;
    sum = 0;
    while((rounds > 0)) {
        sum = (sum + rounds);
        rounds = (rounds - 1);
    }

    print_int(sum);
    return 0;
}