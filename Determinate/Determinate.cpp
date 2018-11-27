// Determinate.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Functions.h"
#include <Windows.h>

using namespace std;

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	ifstream fIn("input.txt");
	ofstream fOut("output.txt");

	AutomateInfo info;
	info = ReadAutomateInfo(fIn);

	FlexMatrix transitionTable;
	InitializeTable(transitionTable, info, fIn);
	queue<Cell> statesQueue;
	set<Cell> doneStates;
	vector<Cell> complexStateAlias;
	FlexMatrix determinateMatrix;
	
	statesQueue = InitializeQueue();

	while (!statesQueue.empty())
	{
		Cell currentState;
		currentState = statesQueue.back();
		statesQueue.pop();
		complexStateAlias.push_back(currentState);
		doneStates.insert(currentState);
		vector<Cell> states = GetStates(transitionTable, currentState);
		determinateMatrix.push_back(states);

		for (Cell &statesSet : states)
		{
			if (doneStates.find(statesSet) == doneStates.end())
			{
				statesQueue.push(statesSet);
			}
		}
		cout << "!!! " << endl;
	}

	Cell::iterator it;

	/*cout << "кол-во строк " << determinateMatrix.size() << endl;
	

	for (int i = 0; i < determinateMatrix.size(); ++i)
	{
		cout << "кол-во столбцов " << determinateMatrix.size() << endl;
		for (int j = 0; j < determinateMatrix[i].size(); ++j)
		{
			for (it = determinateMatrix[i][j].begin(); it != determinateMatrix[i][j].end(); ++it)
			{
				cout << *it << "  ";
			}
		}
		cout << endl;
	} */

    return 0;
}

