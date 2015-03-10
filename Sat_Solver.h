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

	//��������� ������������ �����
	bool solver();
	//�������� �������
	//���� �������
	bool solve(x new_variable);
	//�������� ��������� �������� ���������� �� ������� ����
	void undo_change(vector<watch> local_changes);
	//��������� ����� ������ ����� ������� �� ������� ����
	void check_function(x new_variable);
	//����������� �������� ����������
	void reinit_var(x new_variable);
	void init_var(x new_variable);
	//����� ����� ������������� �� ������������������ ����������
	bool choose_x(x& new_variable);
	//
	bool unit_propagation(x new_variable);
	//�������� ���������� ����������
	void deinit_var(x& new_variable);
	//����� ���������� ��� ���������� unit propagation
	bool pick_next(clause* object, x& next_var);
	//������� ����������� ���������� "non-false"
	int check_clause(clause* object);
	//��������� �������� �������
	//���� ������� ������� ��� ��������� ������ �����
	bool all_clauses_satisfied();
	//������ �� ����� � dimacs �������
	ifstream& read_function(ifstream& file);
	//����� ����������
	ostream& write_variables(ostream& stream, bool solved);
	//�������� ��� ������������� ��������� �� ������� ����
	void undo_var_change(vector<x> local_var_changes);
	//�������������� ������ ����������
	void pure_literals();

	//������ ������� ��� ������ ���������
	vector<clause> function;
	//������ ����������
	vector<variable> variables;
	//���������� ��������� ���������
	vector<vector<watch>> changes;
	vector<watch> local_changes;
	vector<x> local_var_changes;
	vector<vector<x>> var_changes;

};

