#ifndef STATE_H
#define STATE_H

#include "Board.h"

class State
{
public:
	Board board;
	State *parent;
	int g, h, f;

	State(char b[7][7], State *p, int g, int h, int f) : board(b), parent(p), g(g), h(h), f(f) {}

	friend bool operator<(const State &s1, const State &s2);
};

inline bool operator<(const State &s1, const State &s2)
{
	return s1.board < s2.board;
}

#endif //STATE_H