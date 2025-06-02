#pragma once


#include <string>
#include <vector>
#include "MyEnum.h"
#include "MyStruct.h"
#include "Thread.h"

using namespace std;


struct FRtScenarioEventCommand
{


	string EventID;


	string OwnerObjectID;


	string TargetObjectID;

	int EventDataIndex; // FRtScenarioData::EventList  index


	vector<FRtSctFunctionCommand> ConditionCommandList;


	vector<FRtSctFunctionCommand> ProcessCommandList;
};


class CPlayScenario : public CThread
{
public:
	string m_SCTFileName;
	wchar_t m_WFileName[64];

	FRtScenarioData m_SctData;

	map< string, FRtScenarioEventCommand > m_MapCmd;
	vector< FRtScenarioEventCommand > m_ArrCmd;

public:
	CPlayScenario();
	~CPlayScenario();

	void LoadSCT();
	void ParseSCT();


	int GetSctPlayerCount();
	bool IsSctObjectID( char* id );

	void PlayStart();

	unsigned int ThreadProc();
	vector<string> split( string s, string delimiter );
};

