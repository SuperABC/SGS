#include <iostream>
#include <fstream>
#include "machine.h"

//#define ENTER_OMIT

using std::cin;
using std::cout;
using std::endl;

int main() {
	Lexeme l = Lexeme();
	Syntax s = Syntax();
	Machine m = Machine();

	string input;

	std::ifstream fin("test.sgs");

#ifndef  ENTER_OMIT 
	string next = string("$");
	while (1) {
		if (fin.eof()) {
			input = next;
			break;
		}

		if (next == string("$")) {
			getline(fin, input);
			if (fin.eof())break;
			getline(fin, next);
			if (next[0] == ' ' || next[0] == '\t') {
				while (next[0] == ' ' || next[0] == '\t') {
					input += next;
					if (fin.eof())Syntax::error("", SE_EXPOSE);
					getline(fin, next);
				}
				input += next;
				if (fin.eof())break;
				getline(fin, next);
			}
		}
		else {
			input = next;
			getline(fin, next);
			if (next[0] == ' ' || next[0] == '\t') {
				while (next[0] == ' ' || next[0] == '\t') {
					input += next;
					if (fin.eof())Syntax::error("", SE_EXPOSE);
					getline(fin, next);
				}
				input += next;
				if (fin.eof())break;
				getline(fin, next);
			}
		}
		try {
			l.input(input.c_str())->parse();
			s.input(l.strId, l.output)->parse();
			m.input(s.output, s.globeClassType, s.globeFunc, s.globeVar)->execute();
		}
		catch (LexemeException *le) {
			cout << l.get() << endl;
			cout << le->message() << endl;
		}
		catch (SyntaxException *se) {
			cout << l.get() << endl;
			cout << se->message() << endl;
		}
	}
	if (input != string("$")) {
		try {
			l.input(input.c_str())->parse();
			s.input(l.strId, l.output)->parse();
			m.input(s.output, s.globeClassType, s.globeFunc, s.globeVar)->execute();
		}
		catch (LexemeException *le) {
			cout << l.get() << endl;
			cout << le->message() << endl;
		}
		catch (SyntaxException *se) {
			cout << l.get() << endl;
			cout << se->message() << endl;
		}
	}

#else
	string tmp;
	while (!fin.eof()) {
		getline(fin, tmp);
		input += tmp;
	}
	try {
		l.input(input.c_str())->parse();
		s.input(l.strId, l.output)->parse();
		m.input(s.output, s.globeClassType, s.globeFunc, s.globeVar)->execute();
	}
	catch (LexemeException *le) {
		cout << l.get() << endl;
		cout << le->message() << endl;
	}
	catch (SyntaxException *se) {
		cout << l.get() << endl;
		cout << se->message() << endl;
	}
#endif

	system("pause");
}