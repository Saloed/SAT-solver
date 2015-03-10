#include"Sat_Solver.h"

int main(int argc, char *argv[]){
	if (argc != 2)
	return 2;
	ifstream file(argv[1]);
	if (!file.is_open())
		return 1;
	bool solved = false;

	Sat_Solver solution;

	solution.read_function(file);
	if (solution.solver())
		solved = true;
	solution.write_variables(cout, solved);
	return 0;
}