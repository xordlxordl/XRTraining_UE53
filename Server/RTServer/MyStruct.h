// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "framework.h"

#include "MyEnum.h"
#include "FRtSctFunctionCommandParam.h"

struct FVector2D
{
	float x;
	float y;
};


struct FRtSctFunctionCommand
{
	ERtSctFunCmd FunctionCommand;
	vector<FRtSctFunctionCommandParam> Params;

	FRtSctFunctionCommand()
	{
		FunctionCommand = ERtSctFunCmd::ENone;
	}
};


struct FRtUIMenuElement
{
	string menuNameTID;
	string menuName; 
	string actionEventNameID;

	JS_OBJECT( JS_MEMBER( menuNameTID ),
			   JS_MEMBER( menuName ),
			   JS_MEMBER( actionEventNameID )
	);

};

struct FRtNpcPatternElement
{
	string PatternID;
	int Weight;

	FRtNpcPatternElement()
	{
		Weight = 0;
	}

};


struct FRtNpcScriptElement
{
	string ScriptSoundFilePath;
	string ScriptTextTID; 
	string ScriptText; 
};


struct FRtProcessTimer
{
	string TimerID; 
	float TimerValue = 0;
};

struct FRtProcessUI
{
	ERtSctEventProcessDataUI UIType = ERtSctEventProcessDataUI::ENone;

	vector< FRtUIMenuElement > UIMenuElementList;
	
};

struct FRtProcessNPC
{
	vector<FVector2D> NpcMoveToList;
	vector<FRtNpcPatternElement> NpcPatternList;
	vector<FRtNpcScriptElement> NpcScriptList;
};


struct FRtEventProcess
{
	//ERtSctEventProcess ProcessType = ERtSctEventProcess::ENone;

	//ERtSctEventProcessCategory ProcessCategory = ERtSctEventProcessCategory::ENone;

	//ERtSctEventProcessCategorySub ProcessCategorySub = ERtSctEventProcessCategorySub::ENone;

	string processValue;

	//string ProcessSubValue;

	
	//vector<FRtProcessUI> UIList;
	
	
	//vector<FRtProcessNPC> NpcList;

	
	//vector<FRtProcessTimer> TimerList;

	JS_OBJECT( JS_MEMBER( processValue ) );
};

struct FRtScenarioCondition
{

	//ERtSctCondition conditionType = ERtSctCondition::ENone;

	//ERtSctTarget target1Type = ERtSctTarget::ENone;

	//ERtSctTargetState target1State = ERtSctTargetState::ENone;

	string target1Value;
	
	//ERtSctTarget target2Type = ERtSctTarget::ENone;
	
	//string target2Value;

	JS_OBJECT( JS_MEMBER( target1Value ) );
	

};


struct FRtScenarioEvent
{
	string eventNameTId;
	string eventName; 
	string eventCategory; 

	ERtEventHandleType eventHandleType;

	
	string ownerId; 
	string targetId; 

	
	vector< FRtScenarioCondition > eventOccurConditionList;
	vector< FRtEventProcess > eventProcessList;

	
	string preconditionEventNameTIDs;

	JS_OBJECT( JS_MEMBER( eventNameTId ),
			   JS_MEMBER( eventName ),
			   JS_MEMBER( eventCategory ),
			   JS_MEMBER( eventHandleType ),
			   JS_MEMBER( ownerId ),
			   JS_MEMBER( targetId ),
			   JS_MEMBER( preconditionEventNameTIDs ),
			   JS_MEMBER( eventOccurConditionList ),
			   JS_MEMBER( eventProcessList ) );
};

struct FRtScenarioScript
{
	ERtSctFunCmd FunctionCommand = ERtSctFunCmd::ENone; // �Լ� ���.

	vector<string> FunctionParamList; // �Ķ���� ����Ʈ.
	

};


struct FRtScenarioMission
{
	string MissionNameTID; // �̼� �̸� TID
	
	string MissionSummaryTID; // �̼� ���� TID
	
	string MissionName; // �̼� �̸�
	
	string MissionSummary; // �̼� ����
	
	vector< FRtScenarioCondition > MissionStartConditionList;
	
	vector< FRtScenarioCondition > MissionGoalConditionList;
	
	
};


struct FRtScenarioPlayer
{
	string playerObjectId; // mct ���� ������ �÷��̾� ��ü ID
	
	string playerName; // �÷��̾� �̸�
	
	string gameReadyPlaceObjectId; // ���� �غ� ��� ��ü ID
	
	string equipObjectId; // ��� ���� ��ü ID

	int isGroupLeader; // �׷��� ���� ����. (1: ����, 0: ���)

	JS_OBJECT( JS_MEMBER( playerObjectId ),
			   JS_MEMBER( playerName ),
			   JS_MEMBER( gameReadyPlaceObjectId ),
			   JS_MEMBER( equipObjectId ),
			   JS_MEMBER( isGroupLeader ) );
			   
};


struct FRtScenarioPlayerGroup
{
	string playerGroupId; // �÷��̾� �׷� ID
	
	string playerGroupName; // �÷��̾� �׷� �̸�
	
	ERtPlayerType playerType = ERtPlayerType::ENone;; // �÷��̾� Ÿ��. PC or NPC
	
	string groupInfoText; // PC �÷��̾� �׷� �»�� ǥ�� �ؽ�Ʈ, NPC�� ��� ����
	
	vector< FRtScenarioPlayer > playerList;  // �÷��̾� ����Ʈ
	
	JS_OBJECT( JS_MEMBER( playerGroupId ),
			   JS_MEMBER( playerGroupName ),
			   JS_MEMBER( playerType ),
			   JS_MEMBER( groupInfoText ),
			   JS_MEMBER( playerList ) );
};

struct FRtMenuSet
{
	string menuSetID;
	
	string menuSetName;
	
	ERtSctEventUIType uIType = ERtSctEventUIType::ENone;
	
	vector< FRtUIMenuElement > uIMenuElementList;

	JS_OBJECT( JS_MEMBER( menuSetID ),
			   JS_MEMBER( menuSetName ),
			   JS_MEMBER( uIType ),
			   JS_MEMBER( uIMenuElementList ) 
	);

};


struct FRtVariable
{
	string variableNameID;

	string variableName;

	int initValue;

	int isUnexpectedVariable = 0; // ���� ���� ����. (1: ���� ����, 0: �Ϲ� ����)

	JS_OBJECT( JS_MEMBER( variableNameID ),
			   JS_MEMBER( variableName ),
			   JS_MEMBER( initValue ),
			   JS_MEMBER( isUnexpectedVariable )
	);
};

struct FRtScenarioData
{
	string scenarioNameTId; // �ó����� �̸� TID
	
	string scenarioSummaryTId; // �ó����� ���� TID
	
	string scenarioGoalTId; // �ó����� ��ǥ TID
	
	string scenarioName; // �ó����� �̸� (TID�� ������� �ؽ�Ʈ�� ������ �����Ѵ�)
	
	string scenarioSummary; // �ó����� ���� (TID�� ������� �ؽ�Ʈ�� ������ �����Ѵ�)
	
	string scenarioGoal; // �ó����� ��ǥ (TID�� ������� �ؽ�Ʈ�� ������ �����Ѵ�)
	
	float trainingTime = 0; // �Ʒ� �ð�(�ʴ���)
	
	int playerCount = 0; // �÷��̾� �ο���
	
	string mctFileName; // Mct ���� �̸�
	
	string mctFilePath; // Mct ���� ���
	
	vector< FRtScenarioMission > missionList;
	
	vector< FRtScenarioEvent > eventList;
	
	vector< FRtScenarioPlayerGroup > playerGroupList;

	vector< FRtMenuSet > menuSetList; // �޴� ��Ʈ ����Ʈ..
	vector< FRtVariable > varialbeList; // ���� ����Ʈ.

	JS_OBJECT( JS_MEMBER( scenarioNameTId ),
			   JS_MEMBER( scenarioSummaryTId ),
			   JS_MEMBER( scenarioGoalTId ),
			   JS_MEMBER( scenarioName ),
			   JS_MEMBER( scenarioSummary ),
			   JS_MEMBER( scenarioGoal ),
			   JS_MEMBER( trainingTime ),
			   JS_MEMBER( playerCount ),
			   JS_MEMBER( mctFileName ),
			   JS_MEMBER( mctFilePath ),
			   JS_MEMBER( eventList ),
			   JS_MEMBER( playerGroupList ),
			   JS_MEMBER( menuSetList ),
			   JS_MEMBER( varialbeList )
	);
};


struct FRtScenarioEventScriptCommandData
{
	string OwnerObjectID;
	
	string TargetObjectID;
	
	vector< FRtSctFunctionCommand > ConditionCommandList;
	
	vector< FRtSctFunctionCommand > ProcessCommandList;
};


struct FRtMissionInfo
{
	bool MissionStartConditionOk;
	bool MissionGoalConditionOk;
	int MissionArrayIndex;
};


struct JsonFuncion
{
	vector< ERtSctFunCmd > fnlist;
};


struct staaa
{
	int ���_�ѱ�;
	string �񶧸���;
	staaa()
	{
		�񶧸���="�񶧸���";
		���_�ѱ� = 0;
	}
};