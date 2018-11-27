#include "stdafx.h"
#include "Functions.h"

using namespace std;

AutomateInfo ReadAutomateInfo(ifstream &fIn)
{
	AutomateInfo info;

	fIn >> info.signalsNumber;
	fIn >> info.statesNumber;
	fIn >> info.finishStatesNumber;

	int tempState = 0;
	for (int i = 0; i < info.finishStatesNumber; ++i)
	{
		fIn >> tempState;
		info.finalyStatesArr.push_back(tempState);
	}

	return info;
}

void PrintMatrixInFile(FlexMatrix &table, ofstream &fOut)
{
	Cell::iterator it;

	for (int i = 0; i < table.size(); ++i)
	{
		for (int j = 0; j < table[i].size(); ++j)
		{
			for (it = table[i][j].begin(); it != table[i][j].end(); ++it)
			{
				cout << "!" << *it << "!  ";
			}
		}
		cout << endl;
	}
}

void PrintAutomateInfo(FlexMatrix &table, ofstream &fOut, const AutomateInfo info)
{
	fOut << info.signalsNumber;
	fOut << table.size();
	fOut << info.finishStatesNumber;
	
	for (int i = 0; i < info.finishStatesNumber; i++)
	{
		fOut << info.finalyStatesArr[i];
	}


}

void InitializeTable(FlexMatrix &table, const AutomateInfo info, ifstream &fIn)
{
	table.resize(info.statesNumber, vector<Cell>(info.signalsNumber));

	stringstream strstream;
	string initialStr = "";
	string tempStr = "";

	for (int j = 0; j < info.statesNumber + 1; j++)
	{
		getline(fIn, initialStr);
		strstream << initialStr;

		while (strstream >> tempStr)
		{
			int t_state = stoi(tempStr);
			strstream >> tempStr;
			int t_signal = stoi(tempStr);

			table[j-1][t_signal].insert(t_state);
		}

		strstream.clear();
	} 
}

queue<Cell> InitializeQueue()
{
	Cell element;
	queue<Cell> queue;

	element.insert(0);
	queue.push(element);

	return queue;
}

vector<Cell> GetStates(FlexMatrix &table, Cell currentState)
{
	Cell::iterator it;
	//функция возвращает вектор состояний (сливает состояния, если в currentState > 1 элемента, и 
	vector <Cell> resultStates;
	vector <Cell> tempStates;

	if (currentState.size() == 1)
	{
		int state = *currentState.begin();
		resultStates = table[state];
	} else
	{
		bool itFirstState = true;
		//склеиваем состояния
		for (it = currentState.begin(); it != currentState.end(); ++it)
		{
			if (itFirstState)
			{
				resultStates = table[*it];
				itFirstState = false;
			} else
			{
				tempStates = table[*it];

				for (size_t i = 0; i < resultStates.size(); i++)
				{
					//идем по каждому из множеств в tempStates и смотрим, есть ли элемент в нем
					Cell oneSet = resultStates[i];
					Cell twoSet = tempStates[i];
					Cell resultSet;
					
					resultSet.insert(oneSet.begin(), oneSet.end());
					resultSet.insert(twoSet.begin(), twoSet.end());

					resultStates[i] = resultSet;

				}

			}
		}
	}
	
	return resultStates;
}