#ifndef BOARD_H
#define BOARD_H

using namespace std;

class Board
{
private:
	char arr[7][7];
	int loc1;	// even(i) && even(j)
	int loc2;	// even(i) && odd(j)
	int loc3;	// odd(i) && even(j)
	int loc4;	// odd(i) && odd(j)
public:
	Board(char b[7][7])
	{
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				arr[i][j] = b[i][j];
		loc1 = loc2 = loc3 = loc4 = 0;
		countLocs();
	}

	void countLocs()
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
				if (arr[i][j] == 'o')
				{
					if ((i % 2 == 0) && (j % 2 == 0))
						loc1++;
					else if ((i % 2 == 0) && (j % 2 != 0))
						loc2++;
					else if ((i % 2 != 0) && (j % 2 == 0))
						loc3++;
					else
						loc4++;
				}
		}
	}

	bool canMoveRight(int i, int j)
	{
		return ((j + 2 < 7) && (arr[i][j + 1] == 'o') && (arr[i][j + 2] == '.'));
	}

	bool canMoveDown(int i, int j)
	{
		return ((i + 2 < 7) && (arr[i + 1][j] == 'o') && (arr[i + 2][j] == '.'));
	}

	bool canMoveLeft(int i, int j)
	{
		return ((j - 2 >= 0) && (arr[i][j - 1] == 'o') && (arr[i][j - 2] == '.'));
	}

	bool canMoveUp(int i, int j)
	{
		return ((i - 2 >= 0) && (arr[i - 1][j] == 'o') && (arr[i - 2][j] == '.'));
	}

	int heuristicEstimateOfDistance()
	{
		return loc1 + loc2 + loc3 + loc4 - 1;
	}

	bool isGoal()
	{
		return (loc1 + loc2 + loc3 + loc4) == 1;
	}

	bool isDeadlock()
	{
		return abs(loc1 - loc2 - loc3 + loc4) > 1;
	}

	char& operator()(int i, int j)
	{
		return arr[i][j];
	}

	friend bool operator==(const Board &b1, const Board &b2);
	friend bool operator<(const Board &b1, const Board &b2);
	friend ostream& operator<<(ostream &os, const Board &b);
};

inline bool operator==(const Board &b1, const Board &b2)
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (b1.arr[i][j] != b2.arr[i][j])
				return false;
	return true;
}

inline bool operator<(const Board &b1, const Board &b2)
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (b1.arr[i][j] == '.' && b2.arr[i][j] == 'o')
				return false;
			else if (b1.arr[i][j] == 'o' && b2.arr[i][j] == '.')
				return true;
	return false;
}

inline ostream& operator<<(ostream &os, const Board &b)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
			os << " " << b.arr[i][j];
		os << endl;
	}
	return os;
}

#endif //BOARD_H