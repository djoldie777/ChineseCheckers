#include <iostream>
#include <time.h>
#include "Checkers.h"

int main()
{
	Checkers checkers;

	time_t start = clock();

	if (checkers.aStar())
	{
		cout << "________________________________" << endl << endl;
		cout << " Count of states : " << checkers.getCountOfStates() << endl;
		cout << " Count of checked states : " << checkers.getCountOfCheckedStates() << endl;
		cout << " Working time : " << double(clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;
		cout << "________________________________" << endl << endl;
	}
	else
		cout << "Can't find the solution." << endl;

	system("pause");
	return 0;
}