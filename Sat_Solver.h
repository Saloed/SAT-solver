#pragma once
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;

struct clause{
	vector<int> positive_literals;
	vector<int> negative_literals;
	int is_satisfied; //1-satisfied, 0-unsat, -1 - unknown
	//int counter;  //number of non-false literals
};

typedef clause* watch;

struct variable{
	bool value;
	bool is_define;
	vector<watch> clauses;
	vector<watch> negative_list;
	vector<watch> positive_list;
};

typedef variable* x;

class Sat_Solver
{
public:
	Sat_Solver();
	~Sat_Solver();

	//проверяет корректность ввода
	bool solver();
	//основная функция
	//ищет решение
	bool solve(x new_variable);
	//отменяет изменения значений множителей на текущем шаге
	void undo_change(vector<watch> local_changes);
	//проверяет какие скобки стали верными на текущем шаге
	void check_function(x new_variable);
	//присваивают значения переменной
	void reinit_var(x new_variable);
	void init_var(x new_variable);
	//выбор самой встречающейся не инициализированной переменной
	bool choose_x(x& new_variable);
	//
	bool unit_propagation(x new_variable);
	//отменяет назначение переменной
	void deinit_var(x& new_variable);
	//выбор переменной для следующего unit propagation
	bool pick_next(clause* object, x& next_var);
	//считает колличество переменных "non-false"
	int check_clause(clause* object);
	//проверяет основное условие
	//если решение найдено все множители станут верны
	bool all_clauses_satisfied();
	//чтение из файла в dimacs формате
	ifstream& read_function(ifstream& file);
	//вывод результата
	ostream& write_variables(ostream& stream, bool solved);
	//отменяет все инициализации сделанные на текущем шаге
	void undo_var_change(vector<x> local_var_changes);
	//инициализирует чистые переменные
	void pure_literals();

	//хранит функцию как массив мнжителей
	vector<clause> function;
	//хранит переменные
	vector<variable> variables;
	//запоминают сделанные изменения
	vector<vector<watch>> changes;
	vector<watch> local_changes;
	vector<x> local_var_changes;
	vector<vector<x>> var_changes;

};

