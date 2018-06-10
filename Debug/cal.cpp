#include <iostream>
#include <string>
#include <cstdio>
using std::string;

void print_an_int(int a){
    printf("%d\n", a);
}

int newline() {puts("");}

void print_a_number(double a){
    printf("%lf\n", a);
}

void print_a_bool(bool a){
    if(a == true)
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
}

void print_a_str(std::string a){
    std::cout << a << std::endl;
}

float intToFloat(int a) {
    return a;
}

int floatToInt(float a) {
    return a;
}

int fail;
int readNum();
int readNum(){
    int result = 0;
    char c;
    c = getchar();
    while(((c < 48) || (c > 57))){
        c = getchar();
    }
    while(((c >= 48) || (c <= 57))){
        result = (((result * 10) + c) - 48);
        c = getchar();
    }
    return result;
}
string str;
int length;
int position;
char current();
char current(){
    char result = '\0';
    if((position <= length)){
        result = str[position];
        return result;
    }
    result = 0;
    return result;
}
void match();
void match(){
    position = (position + 1);
}
int parseNum();
int parseNum(){
    int result = 0;
    char c;
    c = current();
    result = 0;
    while(((c >= 48) && (c <= 57))){
        result = (((result * 10) + c) - 48);
        match();
        c = current();
    }
    return result;
}
int parseFactor();
int parseTerm();
int parseTerm(){
    int result = 0;
    result = parseFactor();
    char c;
    c = current();
    while(((c == 42) || (c == 47))){
        if((c == 42)){
            match();
            result = (result * parseFactor());
        }
        else{
            match();
            result = (result / parseFactor());
        }
        c = current();
    }
    return result;
}
int parseExpr();
int parseExpr(){
    int result = 0;
    result = parseTerm();
    char c;
    c = current();
    while(((c == 43) || (c == 45))){
        if((c == 43)){
            match();
            result = (result + parseTerm());
        }
        else{
            match();
            result = (result - parseTerm());
        }
        c = current();
    }
    return result;
}
int parseFactor(){
    int result = 0;
    char c;
    c = current();
    if((c == 40)){
        match();
        int temp;
        temp = parseExpr();
        match();
        result = temp;
        return result;
    }
    result = parseNum();
    return result;
}
int main() {
    fail = 0;
    while(true){
        length = 0;
        position = 0;
        fail = 0;
        char c;
        c = getchar();
        while((c != 10)){
            if((c == 32)){
                c = getchar();
                continue;
            }
            str[length] = c;
            length = (length + 1);
            c = getchar();
        }
        int res;
        res = parseExpr();
        print_an_int(res);
        newline();
    }
    return 0;
}