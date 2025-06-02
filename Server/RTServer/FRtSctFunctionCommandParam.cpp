#include <iostream>
#include "FRtSctFunctionCommandParam.h"

void FRtSctFunctionCommandParam::Init()
{
	ParamType = ERtSctFunCmdParam::ENone;
	ParamString.clear();
	ParamInt = 0;
	Paramfloat = 0.0f;
	ParamStringList.clear();
}

vector<string> FRtSctFunctionCommandParam::split( string target, string delim )
{

	getline( cin, target );

	getline( cin, delim );

	vector< string > v;
	size_t x = target.find( delim );

	while( x != string::npos )
	{
		v.push_back( target.substr( 0, x ) );
		target = target.substr( x );
		x = target.find( delim );
	}
	return v;
}

bool FRtSctFunctionCommandParam::SetData( ERtSctFunCmdParam Type, string OriginData )
{
	Init();

	if( OriginData.length() > 0 )
	{
		switch( Type )
		{
			//EStringVariable, // 문자열 변수명.
			//	EStringGameState, // 게임 상태 문자열.
			//	EStringID, // 객체 인스턴스 ID.
			//	EStringMenuSetID, // 메뉴 세트 ID
			//	EStringMessage, // 문자열 메시지.
			//	EStringAniName, // 애니메이션 이름.
			//	EStringAniNameList, // 애니메이션 이름 리스트.
			//	EStringVideoFile, // mp4, avi 비디오 파일명.
			//	EStringFormat, // 포맷 문자열.
			//	EStringFileName, // 파일 이름.
			//	EStringEventID, // 이벤트 ID.
			//	EStringKeyValue, // 입력 키 값.

		case ERtSctFunCmdParam::EStringVariable:
		case ERtSctFunCmdParam::EStringGameState:
		case ERtSctFunCmdParam::EStringID:
		case ERtSctFunCmdParam::EStringMenuSetID:
		case ERtSctFunCmdParam::EStringMessage:
		case ERtSctFunCmdParam::EStringAniName:
		case ERtSctFunCmdParam::EStringVideoFile:
		case ERtSctFunCmdParam::EStringFormat:
		case ERtSctFunCmdParam::EStringFileName:
		case ERtSctFunCmdParam::EStringEventID:
		case ERtSctFunCmdParam::EStringKeyValue:
		{
			ParamString = OriginData;
			string fine_str="\"";
			string replace_str="";
			ParamString = ParamString.replace( ParamString.find( fine_str ), fine_str.length(), replace_str );
		}
		break;
		case ERtSctFunCmdParam::EIntValue:
			ParamInt = atoi( OriginData.c_str() );
			break;
		case ERtSctFunCmdParam::EFloatValue:
			Paramfloat = atof( OriginData.c_str() );
			break;
		case ERtSctFunCmdParam::EStringList:
		case ERtSctFunCmdParam::EStringAniNameList:
		{
			ParamString = OriginData;

			string fine_str = "\"";
			string replace_str = "";
			ParamString = ParamString.replace( ParamString.find( fine_str ), fine_str.length(), replace_str );
			
			string fine_str1 = "[";
			
			ParamString = ParamString.replace( ParamString.find( fine_str1 ), fine_str1.length(), replace_str );

			string fine_str2 = "]";
			
			ParamString = ParamString.replace( ParamString.find( fine_str2 ), fine_str2.length(), replace_str );

			

			//ParamString.ParseIntoArray( ParamStringList, TEXT( "," ), true );
			ParamStringList = split( ParamString, "," );

			ParamString = "";
		}
		break;
		default:
			return false;
			break;
		}

		ParamType = Type;

		return true;
	}

	return false;
}
