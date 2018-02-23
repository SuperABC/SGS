#include <iostream>
#include <fstream>
#include "machine.h"

#define ENTER_OMIT

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
	while (1) {
		if (fin.eof())break;
		getline(fin, input);
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