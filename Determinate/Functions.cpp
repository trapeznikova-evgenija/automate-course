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

	for (size_t i = 0; i < table[i].size(); ++i)
	{
		for (size_t j = 0; j < table.size(); ++j)
		{
			if (table[j][i].empty())
			{
				fOut << -1 << " ";
			} else
			{
				for (it = table[j][i].begin(); it != table[j][i].end(); ++it)
				{
					fOut << *it << " ";
				}
			}
		}
		fOut << endl;
	}
}

vector<Cell> GetNewFinishStates(const AutomateInfo info, vector<Cell> complexStateAlias)
{
	vector<Cell> resultFinishStates;

	for (size_t i = 0; i < complexStateAlias.size(); i++)
	{
		for (const int &info : info.finalyStatesArr)
		{
			if (complexStateAlias[i].find(info) != complexStateAlias[i].end())
			{
				//если есть это конечное состояние в сете
				if (find(resultFinishStates.begin(), resultFinishStates.end(), complexStateAlias[i]) == resultFinishStates.end())
				{
					resultFinishStates.push_back(complexStateAlias[i]);
				}
			}
		}
	}

	return resultFinishStates;
}

void PrintAutomateInfo(FlexMatrix &table, ofstream &fOut, const AutomateInfo info, vector<Cell> complexStateAlias)
{
	fOut << info.signalsNumber << endl;
	fOut << table.size() << endl;
	vector<Cell> newFinishStates = GetNewFinishStates(info, complexStateAlias);
	fOut << newFinishStates.size() << endl;

	Cell::iterator it;
	for (Cell &state : newFinishStates)
	{
		for (it = state.begin(); it != state.end(); ++it)
		{
			fOut << *it << " ";
		}
		fOut << " ";
	} 

	fOut << endl;
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
	vector <Cell> resultStates;
	vector <Cell> tempStates;

	if (currentState.size() == 1)
	{
		int state = *currentState.begin();
		resultStates = table[state];
	} else
	{
		bool itFirstState = true;

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

void PrintSetElement(ofstream &dotFileName, Cell complexStateAlias)
{
	Cell::iterator it;

	int counter = 0;
	dotFileName << "\"";
	for (it = complexStateAlias.begin(); it != complexStateAlias.end(); ++it)
	{
		counter++;
		dotFileName << *it;
		if (complexStateAlias.size() > 1 && counter != complexStateAlias.size())
		{
			dotFileName << ", ";
		}
	}
	dotFileName << "\"";
}

void MarkEndState(ofstream &dotFileName, Cell state, Cell finishState)
{
	Cell intersectionState;
	set_intersection(state.begin(), state.end(), finishState.begin(), finishState.end(), std::inserter(intersectionState, intersectionState.begin()));
	
	if (intersectionState.begin() != intersectionState.end())
	{
		dotFileName << " [shape=box]";
	}
}

Cell CreateSetFromArr(const AutomateInfo info)
{
	Cell finishState;

	for (size_t i = 0; i < info.finalyStatesArr.size(); i++)
	{
		finishState.insert(info.finalyStatesArr[i]);
	}

	return finishState;
}

void CreateVisualizationDotFile(ofstream &dotFileName, FlexMatrix determinateMatrix, vector<Cell> complexStateAlias, const AutomateInfo info)
{
	Cell finishStateSet = CreateSetFromArr(info);

	dotFileName << "digraph DetermAutomat {" << endl;

	for (Cell &state : complexStateAlias)
	{
		PrintSetElement(dotFileName, state);
		MarkEndState(dotFileName, state, finishStateSet);
		dotFileName << endl;
	}

	for (size_t i = 0; i < determinateMatrix.size(); i++)
	{
		for (size_t j = 0; j < determinateMatrix[i].size(); j++)
		{
			if (determinateMatrix[i][j].begin() != determinateMatrix[i][j].end())
			{
				dotFileName << "	";
				PrintSetElement(dotFileName, complexStateAlias[i]);
				dotFileName << "->";
				PrintSetElement(dotFileName, determinateMatrix[i][j]);
				dotFileName << " [label=" << j << ']' << endl;

			}
		}
	} 
	dotFileName << "}";
	dotFileName.close();
}