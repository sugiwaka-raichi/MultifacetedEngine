#include <iostream>
#include "LEX.h"

using namespace std;

#define SCRIPT "#START[f.func[v.i]]\n[v.i] = (100 + 100)\n�\���N�h�����t�J	��Ӂ`\n�肫���I\n\n����	���킢��\n#END"

int main(void) {
	LEX& lex = LEX::getInstance();

	lex.Analysis(SCRIPT);
	lex.LogOut();

	getchar();
	return 0;
}