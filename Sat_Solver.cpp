#include "Sat_Solver.h"


Sat_Solver::Sat_Solver()
{
}

Sat_Solver::~Sat_Solver()
{
	function.clear();
	variables.clear();
	changes.clear();
}

bool Sat_Solver::all_clauses_satisfied(){
	int i = 0;
	while (i < function.size() && function[i].is_satisfied == 1)
		i++;
	if (i == function.size())
		return true;
	else
		return false;
}

int Sat_Solver::check_clause(clause* object){
	int count = 0;
	for (int i = 0; i < object->negative_literals.size(); i++)
	if (!variables[object->negative_literals[i]].value || !variables[object->negative_literals[i]].is_define)
	{
		count += 1;
		/*if (!variables[object->negative_literals[i]].value && variables[object->positive_literals[i]].is_define)
		{
			object->is_satisfied = 1;
			local_changes.push_back(object);
		}*/
	}
	for (int i = 0; i < object->positive_literals.size(); i++)
	if (variables[object->positive_literals[i]].value || !variables[object->positive_literals[i]].is_define)
	{
		count += 1;
	/*	if (variables[object->positive_literals[i]].value && variables[object->positive_literals[i]].is_define)
		{
			object->is_satisfied = 1;
			local_changes.push_back(object);
		}*/
	}
	return count;
}

bool Sat_Solver::pick_next(clause* object, x& next_var){
	object->is_satisfied = 1;
	local_changes.push_back(object);
	for (int i = 0; i < object->negative_literals.size(); i++)
	if (!variables[object->negative_literals[i]].is_define)
	{
		next_var = &variables[object->negative_literals[i]];
		next_var->is_define = true;
		next_var->value = false;
		return true;
	}
	for (int i = 0; i < object->positive_literals.size(); i++)
	if (!variables[object->positive_literals[i]].is_define)
	{
		next_var = &variables[object->positive_literals[i]];
		next_var->is_define = true;
		next_var->value = true;
		return true;
	}
	return false;
}

void Sat_Solver::deinit_var(x& new_variable){
	new_variable->is_define = false;
	new_variable = NULL;
}

bool Sat_Solver::unit_propagation(x new_variable){
	if (new_variable == NULL)
		return true;
	if (new_variable->value)
	{
		for (int i = 0; i < new_variable->negative_list.size(); i++)
		if (new_variable->negative_list[i]->is_satisfied != 1)
		{
			int a = check_clause(new_variable->negative_list[i]);
			if (a == 0)
				return false;
			else
			if (a == 1)
			{
				x next_variable = NULL;
				if (pick_next(new_variable->negative_list[i], next_variable))
				{
					local_var_changes.push_back(next_variable);
					check_function(next_variable);
					if (!unit_propagation(next_variable))
					{
						//deinit_var(next_variable);
						return false;
					}
				}
				else
					return false;
			}
		}
	}
	else
	{
		for (int i = 0; i < new_variable->positive_list.size(); i++)
		if (new_variable->positive_list[i]->is_satisfied != 1)
		{
			int a = check_clause(new_variable->positive_list[i]);
			if (a == 0)
				return false;
			else
			if (a == 1)
			{
				x next_variable = NULL;
				if (pick_next(new_variable->positive_list[i], next_variable))
				{
					local_var_changes.push_back(next_variable);
					check_function(next_variable);
					if (!unit_propagation(next_variable))
					{
						//deinit_var(next_variable);
						return false;
					}
				}
				else
					return false;
			}
		}
	}

	return true;
}

bool Sat_Solver::choose_x(x& new_variable){
	int most_included = 0, counter = 0;
	for (int j = 0; j < variables.size(); j++)
	if (!variables[j].is_define)
	if (variables[j].clauses.size()>counter)
	{
		counter = variables[j].clauses.size();
		most_included = j;
	}
	if (counter == 0 && most_included == 0)
		return false;

	new_variable = &variables[most_included];

	return true;
}

void Sat_Solver::init_var(x new_variable){
	int a = new_variable->negative_list.size();
	int b = new_variable->positive_list.size();
	if (b >= a)
		new_variable->value = true;
	else
		new_variable->value = false;
	new_variable->is_define = true;
}

void Sat_Solver::reinit_var(x new_variable){
	if (new_variable->value)
		new_variable->value = false;
	else
		new_variable->value = true;
}

void Sat_Solver::check_function(x new_variable){
	if (new_variable == NULL)
		return;
	if (new_variable->value)
	{
		for (int i = 0; i < new_variable->positive_list.size(); i++)
		if (new_variable->positive_list[i]->is_satisfied != 1)
		{
			new_variable->positive_list[i]->is_satisfied = 1;
			local_changes.push_back(new_variable->positive_list[i]);
		}
	}
	else
	{
		for (int i = 0; i < new_variable->negative_list.size(); i++)
		if (new_variable->negative_list[i]->is_satisfied != 1)
		{
			new_variable->negative_list[i]->is_satisfied = 1;
			local_changes.push_back(new_variable->negative_list[i]);
		}
	}
}

void Sat_Solver::undo_change(vector<watch> local_changes){
	for (int i = 0; i < local_changes.size(); i++)
		local_changes[i]->is_satisfied = -1;
}

void Sat_Solver::undo_var_change(vector<x> local_var_changes){
	for (int i = 0; i < local_var_changes.size(); i++)
		local_var_changes[i]->is_define = false;
}

bool Sat_Solver::solve(x new_variable){
	check_function(new_variable);
	if (all_clauses_satisfied())
		return true;
	if (!unit_propagation(new_variable))
	{
		undo_var_change(local_var_changes);
			undo_change(local_changes);
			local_var_changes.clear();
			local_changes.clear();
		return false;
	}
	var_changes.push_back(local_var_changes);
	local_var_changes.clear();
	changes.push_back(local_changes);
	local_changes.clear();
	if (!choose_x(new_variable))
		new_variable = NULL;
	else
	init_var(new_variable);
	if (solve(new_variable))
		return true;
	reinit_var(new_variable);
	if (solve(new_variable))
		return true;
	deinit_var(new_variable);
	undo_var_change(var_changes[var_changes.size() - 1]);
	undo_change(changes[changes.size() - 1]);
	var_changes.pop_back();
	changes.pop_back();
	return false;
}

void Sat_Solver::pure_literals(){
	for (int i = 0; i < variables.size(); i++)
	{
		if (variables[i].positive_list.size() == 0)
		{
			variables[i].is_define = true;
			variables[i].value = false;
			check_function(&variables[i]);
		}
		if (variables[i].negative_list.size() == 0)
		{
			variables[i].is_define = true;
			variables[i].value = true;
			check_function(&variables[i]);
		}
	}
}

bool Sat_Solver::solver(){
	if (function.size() == 0)
		return true;
	if (variables.size() == 0)
		return true;
	pure_literals();
	if (solve(NULL))
		return true;
	else
		return false;
}

ifstream& Sat_Solver::read_function(ifstream& file){


	int per = 0, mnoj = 0, a;
	char b;
	string start;
	while (!file.eof() && (start != "p cnf"))
	{
		file.get(b);
		start.push_back(b);
		if (b == '\n')
			start.clear();
	}

	file >> per >> mnoj;
	variables.resize(per);
	for (int j = 0; j < per; j++)
		variables[j].is_define = false;
	function.resize(mnoj);
	for (int j = 0; j < mnoj; j++)
	{
		function[j].is_satisfied = -1;
	}

	int i = 0;
	watch supp = NULL;

	while (file >> a)
	{
		if (a != 0)
		{
			if (a > 0)
			{
				function[i].positive_literals.push_back(a - 1);
				supp = &function[i];
				variables[a - 1].positive_list.push_back(supp);
				variables[a - 1].clauses.push_back(supp);
			}
			else
			{
				a *= (-1);
				function[i].negative_literals.push_back(a - 1);
				supp = &function[i];
				variables[a - 1].negative_list.push_back(supp);
				variables[a - 1].clauses.push_back(supp);
			}
		}
		else
			i += 1;
	}
	return file;
}

ostream& Sat_Solver::write_variables(ostream& stream, bool solved){

	if (solved)
	{
		stream << "s SATISFIABLE" << endl;
		stream << "v ";
		for (int i = 0; i < variables.size(); i++)
		if (variables[i].is_define)
		{
			if (variables[i].value)
				stream << i + 1 << " ";
			else
				stream << (i + 1)*(-1) << " ";
		}
		stream << "0";
	}
	else
		stream << "s UNSATISFIABLE";

	return stream;
}