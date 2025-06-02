#pragma once

#include <string>
#include <vector>
#include "MyEnum.h"

using namespace std;

class FRtSctFunctionCommandParam
{
public:
	FRtSctFunctionCommandParam()
	{
		Init();
	}

	FRtSctFunctionCommandParam( ERtSctFunCmdParam Type, string OriginData )
	{
		SetData( Type, OriginData );
	}

	ERtSctFunCmdParam ParamType;

	string ParamString;
	vector<string> ParamStringList;

	int ParamInt;
	float Paramfloat;

	void Init();

	vector<string> split( string target, string delim );

	bool SetData( ERtSctFunCmdParam Type, string OriginData );

	bool IsParamString() {
		return ( ERtSctFunCmdParam::ENone < ParamType && ParamType < ERtSctFunCmdParam::EIntValue );
	}

	bool IsParamInt() {
		return ParamType == ERtSctFunCmdParam::EIntValue;
	}

	bool IsParamFloat() {
		return ParamType == ERtSctFunCmdParam::EFloatValue;
	}

	bool IsParamStringList() {
		return ParamType == ERtSctFunCmdParam::EStringList;
	}
};