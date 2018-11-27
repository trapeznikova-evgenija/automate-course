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

		if (doneStates.find(currentState) != doneStates.end())
		{
			continue;
		}

		complexStateAlias.push_back(currentState);
		doneStates.insert(currentState);
		vector<Cell> states = GetStates(transitionTable, currentState);
		determinateMatrix.push_back(states);

		for (Cell &statesSet : states)
		{
			if (!statesSet.empty() && doneStates.find(statesSet) == doneStates.end())
			{
				statesQueue.push(statesSet);
			}
		}
	}

	PrintAutomateInfo(determinateMatrix, fOut, info, complexStateAlias);
	PrintMatrixInFile(determinateMatrix, fOut);

    return 0;
}

