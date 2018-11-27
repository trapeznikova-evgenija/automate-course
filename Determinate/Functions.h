#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <string>
#include <vector>
#include <queue>

struct AutomateInfo
{
	int signalsNumber;
	int statesNumber;
	int finishStatesNumber;
	std::vector<int> finalyStatesArr;
};

typedef std::set<int> Cell;
typedef std::vector<std::vector<Cell>> FlexMatrix;

AutomateInfo ReadAutomateInfo(std::ifstream &fIn);
void InitializeTable(FlexMatrix &table, const AutomateInfo info, std::ifstream &fIn);
void PrintMatrixInFile(FlexMatrix &table, std::ofstream &fOut);
void PrintAutomateInfo(FlexMatrix &table, std::ofstream &fOut, const AutomateInfo info);
std::queue<Cell> InitializeQueue();
std::vector<Cell> GetStates(FlexMatrix &table, Cell currentStatel);
