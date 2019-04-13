#include <fstream>
#include <iostream>
#include <memory>

extern FILE* yyin;
extern int yyparse();
extern std::string fcode;


int main(int argc, char* args[])
{

	std::cout << "I am in main." << std::endl;
	FILE* input;
	input = fopen(args[1], "r");
	if (input == nullptr) {
		std::cout << "Invalid file path." << std::endl;
		return EXIT_FAILURE;
	}
	auto out = args[2];
	if (!out) {
		out = "out.asm";
	}
	yyin = input;
	//initializeProgram();
	yyparse();
	auto val = fcode;
	std::ofstream fout(out);
	fout << val;
	
};
