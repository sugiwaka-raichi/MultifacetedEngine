#include <iostream>
#include "LEX.h"

using namespace std;

#define SCRIPT "#START[f.func[v.i]]\n[v.i] = (100 + 100)\n能美クドリャフカ	わふ～\nりきぃ！\n\n理樹	かわいい\n#END"

int main(void) {
	LEX& lex = LEX::getInstance();

	lex.Analysis(SCRIPT);
	lex.LogOut();

	getchar();
	return 0;
}