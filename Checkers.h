#ifndef CHECKERS_H
#define CHECKERS_H

#include <queue>
#include <vector>
#include <set>
#include "State.h"

struct stateCompare
{
	bool operator()(State *s1, State *s2)
	{
		return s1->f > s2->f;
	}
};

class Checkers
{
private:
	State *start;
	int count_of_states, count_of_checked_states;

	State* moveRight(State *parent, int k, int l)
	{
		char b[7][7];

		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				b[i][j] = parent->board(i, j);
		b[k][l] = '.';
		b[k][l + 1] = '.';
		b[k][l + 2] = 'o';

		return new State(b, parent, 0, 0, 0);
	}

	State* moveDown(State *parent, int k, int l)
	{
		char b[7][7];

		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				b[i][j] = parent->board(i, j);
		b[k][l] = '.';
		b[k + 1][l] = '.';
		b[k + 2][l] = 'o';

		return new State(b, parent, 0, 0, 0);
	}

	State* moveLeft(State *parent, int k, int l)
	{
		char b[7][7];

		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				b[i][j] = parent->board(i, j);
		b[k][l] = '.';
		b[k][l - 1] = '.';
		b[k][l - 2] = 'o';

		return new State(b, parent, 0, 0, 0);
	}

	State* moveUp(State *parent, int k, int l)
	{
		char b[7][7];

		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				b[i][j] = parent->board(i, j);
		b[k][l] = '.';
		b[k - 1][l] = '.';
		b[k - 2][l] = 'o';

		return new State(b, parent, 0, 0, 0);
	}

	vector<State*> getAdjacentStates(State *parent)
	{
		vector<State*> res;

		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				if (parent->board(i, j) == 'o')
				{
					if (parent->board.canMoveDown(i, j))
						res.push_back(moveDown(parent, i, j));
					if (parent->board.canMoveUp(i, j))
						res.push_back(moveUp(parent, i, j));
					if (parent->board.canMoveRight(i, j))
						res.push_back(moveRight(parent, i, j));
					if (parent->board.canMoveLeft(i, j))
						res.push_back(moveLeft(parent, i, j));
				}

		return res;
	}

	int distance(Board &b1, Board &b2)
	{
		return (b1.heuristicEstimateOfDistance() - b2.heuristicEstimateOfDistance());
	}

	State* find(priority_queue<State*, vector<State*>, stateCompare> &states, State *s)
	{
		priority_queue<State*, vector<State*>, stateCompare> pq = states;
		
		while (!pq.empty())
		{
			State *ss = pq.top();

			if (ss->board == s->board)
				return ss;

			pq.pop();
		}

		return nullptr;
	}

	bool isGoal(State *s)
	{
		return s->board.isGoal();
	}

	void outputSolution(State *s)
	{
		while (s != 0)
		{
			cout << "_______________" << endl;
			cout << s->board;
			s = s->parent;
		}
	}

public:
	Checkers()
	{
		char b[7][7];

		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
			{
				if (i == 0 || i == 1 || i == 5 || i == 6)
					if (j == 0 || j == 1 || j == 5 || j == 6)
						b[i][j] = ' ';
					else
						b[i][j] = 'o';
				else if (i == 3 && j == 3)
					b[i][j] = '.';
				else
					b[i][j] = 'o';
			}

		start = new State(b, nullptr, 0, 0, 0);
		count_of_states = count_of_checked_states = 0;
	}

	bool aStar()
	{
		// очередь состояний для рассмотрения
		priority_queue<State*, vector<State*>, stateCompare> states_to_check;
		// множество рассмотренных состояний
		set<State> checked_states;
		// смежные с текущим состояния
		vector<State*> adjacent_states;

		states_to_check.push(start);
		count_of_states += 1;

		// стоимость пути от начала до текущего состояния
		start->g = 0;
		// эвристическая оценка расстояния от текущего до целевого состояния
		start->h = start->board.heuristicEstimateOfDistance();
		// эвристическая функция
		start->f = start->g + start->h;

		while (!states_to_check.empty())
		{
			State *current_state = states_to_check.top();
			count_of_checked_states += 1;

			if (current_state->board.isGoal())
			{
				outputSolution(current_state);
				adjacent_states.clear();
				return true;
			}

			states_to_check.pop();
			checked_states.insert(*current_state);

			if (current_state->board.isDeadlock())
				continue;

			if (!adjacent_states.empty())
				adjacent_states.clear();

			adjacent_states = getAdjacentStates(current_state);

			for (auto &cur_adj_st : adjacent_states)
			{
				State *adj_st;
				count_of_states += 1;
				
				int tentativeG = current_state->g + distance(current_state->board, cur_adj_st->board);
				set<State>::iterator it = checked_states.find(*cur_adj_st);

				if (it != checked_states.end())
				{
					if (it->g <= tentativeG)
						continue;
					else
					{
						cur_adj_st->g = tentativeG;
						cur_adj_st->h = it->h;
						cur_adj_st->f = cur_adj_st->g + cur_adj_st->h;

						checked_states.erase(it);
						states_to_check.push(cur_adj_st);
					}
				}
				else
				{
					adj_st = find(states_to_check, cur_adj_st);

					if (adj_st == nullptr)
					{
						cur_adj_st->g = tentativeG;
						cur_adj_st->h = cur_adj_st->board.heuristicEstimateOfDistance();
						cur_adj_st->f = cur_adj_st->g + cur_adj_st->h;

						states_to_check.push(cur_adj_st);
					}
					else if (adj_st->g > tentativeG)
					{
						adj_st->g = tentativeG;
						adj_st->f = adj_st->g + adj_st->h;
						adj_st->parent = current_state;
					}
				}
			}
		}

		adjacent_states.clear();
		return false;
	}

	int getCountOfStates()
	{
		return count_of_states;
	}

	int getCountOfCheckedStates()
	{
		return count_of_checked_states;
	}
};

#endif //CHECKERS_H