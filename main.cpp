#include <iostream>
#include "machine.h"

using std::cin;
using std::cout;
using std::endl;

int main() {
	Lexeme l = Lexeme();
	Syntax s = Syntax();
	Machine m = Machine();

	string input;

	while (1) {
		getline(cin, input);
		try {
			l.input(input.c_str())->parse();
			s.input(l.strId, l.output)->parse();
			m.input(s.output, s.globeClassType, s.globeFunc, s.globeVar)->execute();
		}
		catch (LexemeException *le) {
			cout << le->message() << endl;
		}
		catch (SyntaxException *se) {
			cout << se->message() << endl;
		}
	}
}