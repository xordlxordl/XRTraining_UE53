// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/RtScenarioModel.h"
#include "Serialization/JsonSerializer.h" //Json
#include "JsonObjectConverter.h" // JsonUtilities
#include "RtJsonFunctionLib.h"

DEFINE_LOG_CATEGORY(RtEvent);

#define KEY_CHECK(key) if(key.IsEmpty()) return false;

// Sets default values
ARtScenarioModel::ARtScenarioModel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARtScenarioModel::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARtScenarioModel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// SctData 에 .sct 파일만 로드 한다.
bool ARtScenarioModel::LoadScenarioDataStruct(FString sctfilePath)
{
	bool isSuccess = false;
	FString outMessage;

	TSharedPtr<FJsonObject> sctJsonObject = URtJsonFunctionLib::ReadJson(sctfilePath, isSuccess, outMessage);
	if (sctJsonObject.IsValid())
	{
		// Try to convert generic json object to the desired structure. Output goes in RetRtTemplateStruct
		if (!FJsonObjectConverter::JsonObjectToUStruct<FRtScenarioData>(sctJsonObject.ToSharedRef(), &SctData))
		{
			//bOutSuccess = false;
			//OutInfoMessage = FString::Printf(TEXT("Read Struct Json Failed - Was not able to convert the json object to your desired structure. Is it the rigth format / struct? - '%s'"), *JsonFilePath);
			//return FRtMCTTemplate();
			return false;
		}

		isSuccess = true;
	}

	return isSuccess;
}

bool ARtScenarioModel::LoadSCT(FString sctfilePath)
{
	if (!LoadScenarioDataStruct(sctfilePath))
	{
		return false;
	}

	// 선행 조건이 있는 이벤트는 뒤로 보낸다.
	//SctData.EventList.Sort(
	//	[](const FRtScenarioEvent& A, const FRtScenarioEvent& B) 
	//	{ 
	//		return A.PreconditionEventNameTIDs.IsEmpty(); 
	//	}
	//);
	//int32 counter = 0;

	if (URtResourceModelSubsystem::Get())
	{

#if WITH_EDITOR
		//URtResourceModelSubsystem::Get()->InitFunctionScriptInfoCollector();
#endif
		//for (const FRtScenarioSuddenEvent& item : SctData.SuddenEventList)
		//{
		//	for (const auto& proItem : item.EventProcessList)
		//	{
		//		FRtSctFunctionCommand Cmd;
		//		if (URtResourceModelSubsystem::Get()->ScriptParsing(proItem.ProcessValue, Cmd) == FRtScriptParsingResult::ESuccess)
		//		{
		//			ServerEventCommandMap.Add(item.EventNameTID, { &Cmd, TEXT("") });
		//		}
		//	}
		//}

		// 돌발 이벤트 데이터
		for (const FRtScenarioSuddenEvent& suddenData : SctData.SuddenEventList)
		{
			FRtScenarioSuddenEventCommand Cmds;
			SetEventCommand(Cmds, suddenData);

			SuddenEventCommandMap.Add(Cmds.EventID, Cmds);

			FRtScenarioSuddenEventCommand* eventCommand = SuddenEventCommandMap.Find(Cmds.EventID);
			ServerEventCommandMap.Add(Cmds.EventID, { eventCommand, TEXT("") });
			ReplayEventCommandMap.Add(Cmds.EventID, *eventCommand);

		}

		// 타겟 드롭박스 데이터
		for (const FRtDropbox& dropboxData : SctData.DropboxList)
		{
			FRtDropboxCommand CmdData;

			CmdData.DropboxID = dropboxData.DropboxID;
			CmdData.DropboxOwnerList = dropboxData.DropboxOwnerList;

			SetDropboxElemantCommands(CmdData, dropboxData);

			DropboxCommandMap.Add(CmdData.DropboxID, CmdData);

			if (const FRtDropboxCommand* dropboxCommand = DropboxCommandMap.Find(CmdData.DropboxID))
			{
				for (const FString& targetID : dropboxCommand->DropboxOwnerList)
				{
					if (TargetDropboxMap.Contains(targetID))
						TargetDropboxMap.Find(targetID)->IDs.Add(dropboxCommand->DropboxID);
					else
					{
						FRtIDList idlist({dropboxCommand->DropboxID});
						TargetDropboxMap.Add(targetID, idlist);
					}
				}

				AddServerEventCommandMap(*dropboxCommand);
			}
		}

		// 무전기 메뉴 데이터
		{
			const FRtRadioDropbox& dropboxData = SctData.RadioDropbox;

			RadioDropboxCommand.DropboxID = TEXT("__DEFAULT_RADIO_DROPBOX__")/*dropboxData.DropboxID*/;

			SetDropboxElemantCommands(RadioDropboxCommand, dropboxData);

			AddServerEventCommandMap(RadioDropboxCommand);
		}

		for (const FRtVariable& item : SctData.VarialbeList)
		{
			InitVarialbleMap.Add(item.VariableNameID, item.InitValue);
			VarialbleMap.Add(item.VariableNameID, item.InitValue);
			OldVarialbleMap.Add(item.VariableNameID, item.InitValue);
		}

		// 시나리오 이벤트 데이터
		//int eventCounter = 0;
		for (const FRtScenarioEvent& eventData : SctData.EventList)
		{
			FRtScenarioEventCommand CmdData;
			SetEventCommand(CmdData, eventData);

			//CmdData.EventDataIndex = eventCounter++;

			for (const auto& conItem : eventData.EventOccurConditionList)
			{
				FRtSctFunctionCommand Cmd;
				if (URtResourceModelSubsystem::Get()->ScriptParsing(conItem.ConditionScript, Cmd) == FRtScriptParsingResult::ESuccess)
				{
					CmdData.ConditionCommandList.Add(Cmd);

					if (Cmd.FunctionCommand == ERtSctFunCmd::IsTargetInteract &&
						Cmd.Params.Num() == 1 && Cmd.Params[0].IsParamString())
						TargetInteractObjects.Add(Cmd.Params[0].ParamString);
				}
			}

			for (auto& processItem : CmdData.ProcessCommandList) {
				// 시스템 명령어가 있는 경우..
				if (processItem.FunctionCommand == ERtSctFunCmd::EndTraining) {
					CmdData.bLocalProcessOnControl = true;
				}
			}

			EventCommandMap.Add(CmdData.EventID, CmdData);
			ReplayEventCommandMap.Add(CmdData.EventID, StaticCast<FRtEventCommand>(CmdData));

			//SctEventMap.Add(CmdData.EventID, counter);
			//if( item.EventOccurConditionList.Num() > 0 )
			//{
			//	SctHaveOccorConEventIdxList.Add(counter);
			//}

			//counter++;
		}

		EventIDLoopList.Empty();
		for (const auto& data : EventCommandMap)
		{
			InitEventIDLoopList.Add(data.Key);
			EventIDLoopList.Add(data.Key);
		}

		GameState = ERtSctGameState::EGameState_None;

#if WITH_EDITOR // 변경되는 스크립트 사항 쉽게 모아 정리 하기 위한 에디터 전용 코드.

		//TMap<FString, FString> mapData = URtResourceModelSubsystem::Get()->GetFuctionSpecCollectorMap();
		//for (const auto& data : mapData)
		//{
		//	APP_LOG("[Sct Function Spec Info] %s", *data.Value);
		//}

		//TArray<FString> arrData = URtResourceModelSubsystem::Get()->GetAllFunctionCommandList();
		//for (const auto& data : arrData)
		//{
		//	APP_LOG("[Sct Function List Info] %s", *data);
		//}

		//mapData.Num(); // 여기에 브레이크 걸고 내용을 확인하여 복사하면 된다. 
#endif

	}

	//counter = 0;
	//for (auto & item : SctData.MissionList)
	//{
	//	MissionInfoMap.Add(item.MissionNameTID, FRtMissionInfo(false, false, counter));
	//	counter ++;
	//}

	return true;
}

//const FRtScenarioEvent* ARtScenarioModel::FindEvent(FString eventID)
//{
//	FRtScenarioEventCommand* pEventCommand = EventCommandMap.Find(eventID);
//	if (pEventCommand)
//	{
//		return &SctData.EventList[pEventCommand->EventDataIndex];
//	}
//
//	//int32* pIndex = SctEventMap.Find(eventID);
//	//if (pIndex)
//	//{
//	//	return &SctData.EventList[*pIndex];
//	//}
//
//	return nullptr;
//}

bool ARtScenarioModel::RemoveRegistOccorConEvent(FString eventID)
{
	//int32* pIndex = SctEventMap.Find(eventID);
	//if (pIndex)
	//{
	//	return SctHaveOccorConEventIdxList.Remove(*pIndex) > 0;
	//}

	return false;
}

// 오우너 객체 이름을 기반으로 우선 소트를 하고 같은 이름에 대해서는 
// 이벤트(콘디션 + 프로세스) > 이벤트 처리(프로세스) > 콘디션 순으로 소트한다.
void ARtScenarioModel::SortEventList()
{
	SctData.EventList.Sort([](const FRtScenarioEvent& A, const FRtScenarioEvent& B) {
		bool sortResult = false;

		int32 AConditionNum = A.EventOccurConditionList.Num();
		int32 AProcessNum = A.EventProcessList.Num();
		int32 BConditionNum = B.EventOccurConditionList.Num();
		int32 BProcessNum = B.EventProcessList.Num();

		int32 AScore = 0;
		int32 BScore = 0;

		if (AConditionNum > 0 && AProcessNum > 0) {
			AScore = 3;
		}
		else if (AConditionNum > 0) {
			AScore = 1;
		}
		else if (AProcessNum > 0) {
			AScore = 2;
		}

		if (BConditionNum > 0 && BProcessNum > 0) {
			BScore = 3;
		}
		else if (BConditionNum > 0) {
			BScore = 1;
		}
		else if (BProcessNum > 0) {
			BScore = 2;
		}

		if (A.EventName.ToString() == B.EventName.ToString())
		{
			if (AScore == BScore)
			{
				return A.EventNameTID < B.EventNameTID;
			}
			else
			{
				return AScore > BScore;
			}
		}
		else
		{
			return A.EventName.ToString() < B.EventName.ToString();
		}

		//if (A.EventCategory == B.EventCategory)
		//{
		//	if( A.EventName.ToString() == B.EventName.ToString() )
		//	{
		//		if (A.OwnerID == B.OwnerID)
		//		{
		//			if (AScore == BScore)
		//			{
		//				return A.EventNameTID < B.EventNameTID;
		//			}
		//			else
		//			{
		//				return AScore > BScore;
		//			}
		//		}
		//		else
		//		{
		//			return A.OwnerID < B.OwnerID;
		//		}
		//	}
		//	else
		//	{
		//		return A.EventName.ToString() < B.EventName.ToString();
		//	}
		//}
		//else
		//{
		//	return A.EventCategory < B.EventCategory;
		//}
		}
	);
}

void ARtScenarioModel::SortPlayerGroupList()
{
	SctData.PlayerGroupList.Sort([](const FRtScenarioPlayerGroup& A, const FRtScenarioPlayerGroup& B) {
		if (A.PlayerType == B.PlayerType)
		{
			return A.PlayerGroupID < B.PlayerGroupID;
		}
		else
		{
			return A.PlayerType < B.PlayerType;
		}
		}
	);
}

void ARtScenarioModel::SortDropboxList()
{
	SctData.DropboxList.Sort([](const FRtDropbox& A, const FRtDropbox& B) {
		//if (A.UIType == B.UIType)
		//{
		return A.DropboxID < B.DropboxID;
		//}
		//else
		//{
		//	return A.UIType < B.UIType;
		//}
		}
	);
}

void ARtScenarioModel::SortVariableList()
{
	//SctData.VarialbeList.Sort([](const FRtVariable& A, const FRtVariable& B) {
	//		if (A.IsUnexpectedVariable == B.IsUnexpectedVariable)
	//		{
	//			return A.VariableNameID < B.VariableNameID;
	//		}
	//		else
	//		{
	//			return A.IsUnexpectedVariable < B.IsUnexpectedVariable;
	//		}
	//	}
	//);
}

bool ARtScenarioModel::FindEventItem(FString itemKey, FRtScenarioEvent& foundItem, int32& index)
{
	KEY_CHECK(itemKey)

		index = 0;
	for (auto& item : SctData.EventList)
	{
		if (item.EventNameTID == itemKey)
		{
			foundItem = item;
			return true;
		}
		index++;
	}

	return false;
}

bool ARtScenarioModel::FindDropboxItem(FString itemKey, FRtDropbox& foundItem, int32& index)
{
	KEY_CHECK(itemKey)

		index = 0;
	for (auto& item : SctData.DropboxList)
	{
		if (item.DropboxID == itemKey)
		{
			foundItem = item;
			return true;
		}
		index++;
	}

	return false;
}

bool ARtScenarioModel::FindSuddenEventItem(FString itemKey, FRtScenarioSuddenEvent& foundItem, int32& index)
{
	KEY_CHECK(itemKey)

		index = 0;
	for (auto& item : SctData.SuddenEventList)
	{
		if (item.EventNameTID == itemKey)
		{
			foundItem = item;
			return true;
		}
		index++;
	}

	return false;
}

bool ARtScenarioModel::FindPlayerGroupItem(FString itemKey, FRtScenarioPlayerGroup& foundItem, int32& index)
{
	KEY_CHECK(itemKey)

		index = 0;
	for (auto& item : SctData.PlayerGroupList)
	{
		if (item.PlayerGroupID == itemKey)
		{
			foundItem = item;
			return true;
		}
		index++;
	}

	return false;
}

bool ARtScenarioModel::FindVariableItem(FString itemKey, FRtVariable& foundItem, int32& index)
{
	KEY_CHECK(itemKey)

		index = 0;
	for (auto& item : SctData.VarialbeList)
	{
		if (item.VariableNameID == itemKey)
		{
			foundItem = item;
			return true;
		}
		index++;
	}

	return false;
}

bool ARtScenarioModel::FindSystemMenuElementItem(FString itemKey, FRtUIMenuElement& foundItem, int32& index)
{
	KEY_CHECK(itemKey)

		index = 0;
	for (auto& item : SctData.RadioDropbox.DropboxElementList)
	{
		if (item.EventNameTID == itemKey)
		{
			foundItem = item;
			return true;
		}
		index++;
	}

	return false;
}

int32 ARtScenarioModel::RemoveEventItem(FString itemKey)
{
	KEY_CHECK(itemKey)

		FRtScenarioEvent foundItem;
	int32 index;
	if (FindEventItem(itemKey, foundItem, index))
	{
		SctData.EventList.RemoveAt(index);
		return index;
	}

	return -1;
}

int32 ARtScenarioModel::RemoveDropboxItem(FString itemKey)
{
	KEY_CHECK(itemKey)

		FRtDropbox foundItem;
	int32 index;
	if (FindDropboxItem(itemKey, foundItem, index))
	{
		SctData.DropboxList.RemoveAt(index);
		return index;
	}

	return -1;
}

int32 ARtScenarioModel::RemoveSuddenEventItem(FString itemKey)
{
	KEY_CHECK(itemKey)

		FRtScenarioSuddenEvent foundItem;
	int32 index;
	if (FindSuddenEventItem(itemKey, foundItem, index))
	{
		SctData.SuddenEventList.RemoveAt(index);
		return index;
	}

	return -1;
}

int32 ARtScenarioModel::RemovePlayerGroupItem(FString itemKey)
{
	KEY_CHECK(itemKey)

		FRtScenarioPlayerGroup foundItem;
	int32 index;
	if (FindPlayerGroupItem(itemKey, foundItem, index))
	{
		SctData.PlayerGroupList.RemoveAt(index);
		return index;
	}

	return -1;
}

int32 ARtScenarioModel::RemoveVariableItem(FString itemKey)
{
	KEY_CHECK(itemKey)

		FRtVariable foundItem;
	int32 index;
	if (FindVariableItem(itemKey, foundItem, index))
	{
		SctData.VarialbeList.RemoveAt(index);
		return index;
	}

	return -1;
}

int32 ARtScenarioModel::RemoveSystemMenuElementItem(FString itemKey)
{
	KEY_CHECK(itemKey)

		FRtUIMenuElement foundItem;
	int32 index;
	if (FindSystemMenuElementItem(itemKey, foundItem, index))
	{
		SctData.RadioDropbox.DropboxElementList.RemoveAt(index);
		return index;
	}

	return -1;
}

bool ARtScenarioModel::DuplicateEventItem(FString itemKey, FString postFix)
{
	KEY_CHECK(itemKey)

		FRtScenarioEvent foundItem;
	int32 index;
	if (FindEventItem(itemKey, foundItem, index))
	{
		int32 counter = 1;
		do
		{
			foundItem.EventNameTID = FString::Printf(TEXT("%s%s%d"), *itemKey, *postFix, counter++);
		} while (!InsertEventItem(foundItem, (index + 1)));
	}

	return false;
}

bool ARtScenarioModel::DuplicateDropboxItem(FString itemKey, FString postFix)
{
	KEY_CHECK(itemKey)

		FRtDropbox foundItem;
	int32 index;
	if (FindDropboxItem(itemKey, foundItem, index))
	{
		int32 counter = 1;
		do
		{
			foundItem.DropboxID = FString::Printf(TEXT("%s%s%d"), *itemKey, *postFix, counter++);
		} while (!InsertDropboxItem(foundItem, (index + 1)));
	}

	return false;
}

bool ARtScenarioModel::DuplicateSuddenEventItem(FString itemKey, FString postFix)
{
	KEY_CHECK(itemKey)

		FRtScenarioSuddenEvent foundItem;
	int32 index;
	if (FindSuddenEventItem(itemKey, foundItem, index))
	{
		int32 counter = 1;
		do
		{
			foundItem.EventNameTID = FString::Printf(TEXT("%s%s%d"), *itemKey, *postFix, counter++);
		} while (!InsertSuddenEventItem(foundItem, (index + 1)));
	}

	return false;
}

bool ARtScenarioModel::DuplicatePlayerGroupItem(FString itemKey, FString postFix)
{
	KEY_CHECK(itemKey)

		FRtScenarioPlayerGroup foundItem;
	int32 index;
	if (FindPlayerGroupItem(itemKey, foundItem, index))
	{
		int32 counter = 1;
		do
		{
			foundItem.PlayerGroupID = FString::Printf(TEXT("%s%s%d"), *itemKey, *postFix, counter++);
		} while (!InsertPlayerGroupItem(foundItem, (index + 1)));
	}

	return false;
}

bool ARtScenarioModel::DuplicateVariableItem(FString itemKey, FString postFix)
{
	KEY_CHECK(itemKey)

		FRtVariable foundItem;
	int32 index;
	if (FindVariableItem(itemKey, foundItem, index))
	{
		int32 counter = 1;
		do
		{
			foundItem.VariableNameID = FString::Printf(TEXT("%s%s%d"), *itemKey, *postFix, counter++);
		} while (!InsertVariableItem(foundItem, (index + 1)));
	}

	return false;
}

bool ARtScenarioModel::DuplicateSystemMenuElementItem(FString itemKey, FString postFix)
{
	KEY_CHECK(itemKey)

		FRtUIMenuElement foundItem;
	int32 index;
	if (FindSystemMenuElementItem(itemKey, foundItem, index))
	{
		int32 counter = 1;
		do
		{
			foundItem.EventNameTID = FString::Printf(TEXT("%s%s%d"), *itemKey, *postFix, counter++);
		} while (!InsertSystemMenuElementItem(foundItem, (index + 1)));
	}

	return false;
}

bool ARtScenarioModel::AddEventItem(FRtScenarioEvent newItem)
{
	FRtScenarioEvent foundItem;
	int32 index;
	if (FindEventItem(newItem.EventNameTID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		SctData.EventList.Add(newItem);
		return true;
	}
}

bool ARtScenarioModel::AddDropboxItem(FRtDropbox newItem)
{
	FRtDropbox foundItem;
	int32 index;
	if (FindDropboxItem(newItem.DropboxID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		SctData.DropboxList.Add(newItem);
		return true;
	}
}

bool ARtScenarioModel::AddSuddenEventItem(FRtScenarioSuddenEvent newItem)
{
	FRtScenarioSuddenEvent foundItem;
	int32 index;
	if (FindSuddenEventItem(newItem.EventNameTID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		SctData.SuddenEventList.Add(newItem);
		return true;
	}
}

bool ARtScenarioModel::AddPlayerGroupItem(FRtScenarioPlayerGroup newItem)
{
	FRtScenarioPlayerGroup foundItem;
	int32 index;
	if (FindPlayerGroupItem(newItem.PlayerGroupID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		SctData.PlayerGroupList.Add(newItem);
		return true;
	}

}

bool ARtScenarioModel::AddVariableItem(FRtVariable newItem)
{
	FRtVariable foundItem;
	int32 index;
	if (FindVariableItem(newItem.VariableNameID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		SctData.VarialbeList.Add(newItem);
		return true;
	}
}

bool ARtScenarioModel::AddSystemMenuElementItem(FRtUIMenuElement newItem)
{
	FRtUIMenuElement foundItem;
	int32 index;
	if (FindSystemMenuElementItem(newItem.EventNameTID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		SctData.RadioDropbox.DropboxElementList.Add(newItem);
		return true;
	}
}

bool ARtScenarioModel::InsertEventItem(FRtScenarioEvent newItem, int32 insertIndex)
{
	FRtScenarioEvent foundItem;
	int32 index;
	if (FindEventItem(newItem.EventNameTID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		if (0 <= insertIndex && insertIndex < SctData.EventList.Num())
		{
			SctData.EventList.Insert(newItem, insertIndex);
		}
		else {
			SctData.EventList.Add(newItem);
		}
		return true;
	}

	return false;
}

bool ARtScenarioModel::InsertDropboxItem(FRtDropbox newItem, int32 insertIndex)
{
	FRtDropbox foundItem;
	int32 index;
	if (FindDropboxItem(newItem.DropboxID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		if (0 <= insertIndex && insertIndex < SctData.DropboxList.Num())
		{
			SctData.DropboxList.Insert(newItem, insertIndex);
		}
		else {
			SctData.DropboxList.Add(newItem);
		}
		return true;
	}

	return false;
}

bool ARtScenarioModel::InsertSuddenEventItem(FRtScenarioSuddenEvent newItem, int32 insertIndex)
{
	FRtScenarioSuddenEvent foundItem;
	int32 index;
	if (FindSuddenEventItem(newItem.EventNameTID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		if (0 <= insertIndex && insertIndex < SctData.SuddenEventList.Num())
		{
			SctData.SuddenEventList.Insert(newItem, insertIndex);
		}
		else {
			SctData.SuddenEventList.Add(newItem);
		}
		return true;
	}

	return false;
}

bool ARtScenarioModel::InsertPlayerGroupItem(FRtScenarioPlayerGroup newItem, int32 insertIndex)
{
	FRtScenarioPlayerGroup foundItem;
	int32 index;
	if (FindPlayerGroupItem(newItem.PlayerGroupID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		if (0 <= insertIndex && insertIndex < SctData.PlayerGroupList.Num())
		{
			SctData.PlayerGroupList.Insert(newItem, insertIndex);
		}
		else {
			SctData.PlayerGroupList.Add(newItem);
		}
		return true;
	}

	return false;
}

bool ARtScenarioModel::InsertVariableItem(FRtVariable newItem, int32 insertIndex)
{
	FRtVariable foundItem;
	int32 index;
	if (FindVariableItem(newItem.VariableNameID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		if (0 <= insertIndex && insertIndex < SctData.VarialbeList.Num())
		{
			SctData.VarialbeList.Insert(newItem, insertIndex);
		}
		else {
			SctData.VarialbeList.Add(newItem);
		}
		return true;
	}

	return false;
}

bool ARtScenarioModel::InsertSystemMenuElementItem(FRtUIMenuElement newItem, int32 insertIndex)
{
	FRtUIMenuElement foundItem;
	int32 index;
	if (FindSystemMenuElementItem(newItem.EventNameTID, foundItem, index)) {
		return false; // 같은 ID는 허용하지 않는다.
	}
	else {
		if (0 <= insertIndex && insertIndex < SctData.RadioDropbox.DropboxElementList.Num())
		{
			SctData.RadioDropbox.DropboxElementList.Insert(newItem, insertIndex);
		}
		else {
			SctData.RadioDropbox.DropboxElementList.Add(newItem);
		}
		return true;
	}

	return false;
}

bool ARtScenarioModel::SetEventItem(FString itemKey, FRtScenarioEvent newItem)
{
	KEY_CHECK(itemKey)

		FRtScenarioEvent foundItem;
	int32 index;
	if (FindEventItem(itemKey, foundItem, index))
	{
		SctData.EventList[index] = newItem;
		return true;
	}

	return false;
}

bool ARtScenarioModel::SetDropboxItem(FString itemKey, FRtDropbox newItem)
{
	KEY_CHECK(itemKey)

		FRtDropbox foundItem;
	int32 index;
	if (FindDropboxItem(itemKey, foundItem, index))
	{
		SctData.DropboxList[index] = newItem;
		return true;
	}

	return false;
}

bool ARtScenarioModel::SetSuddenEventItem(FString itemKey, FRtScenarioSuddenEvent newItem)
{
	KEY_CHECK(itemKey)

		FRtScenarioSuddenEvent foundItem;
	int32 index;
	if (FindSuddenEventItem(itemKey, foundItem, index))
	{
		SctData.SuddenEventList[index] = newItem;
		return true;
	}

	return false;
}

bool ARtScenarioModel::SetPlayerGroupItem(FString itemKey, FRtScenarioPlayerGroup newItem)
{
	KEY_CHECK(itemKey)

		FRtScenarioPlayerGroup foundItem;
	int32 index;
	if (FindPlayerGroupItem(itemKey, foundItem, index))
	{
		SctData.PlayerGroupList[index] = newItem;
		return true;
	}

	return false;
}

bool ARtScenarioModel::SetVariableItem(FString itemKey, FRtVariable newItem)
{
	KEY_CHECK(itemKey)

		FRtVariable foundItem;
	int32 index;
	if (FindVariableItem(itemKey, foundItem, index))
	{
		SctData.VarialbeList[index] = newItem;
		return true;
	}

	return false;
}

bool ARtScenarioModel::SetSystemMenuElementItem(FString itemKey, FRtUIMenuElement newItem)
{
	KEY_CHECK(itemKey)

		FRtUIMenuElement foundItem;
	int32 index;
	if (FindSystemMenuElementItem(itemKey, foundItem, index))
	{
		SctData.RadioDropbox.DropboxElementList[index] = newItem;
		return true;
	}

	return false;
}

void ARtScenarioModel::ChangeSystemMenuScreenPos()
{
	int32 data = (int32)SctData.RadioDropbox.ScreenPosInfo;
	data++;
	if (data > (int32)ERtSctSystemMenuScreenPos::ERightTop) {
		data = 1;
	}

	SctData.RadioDropbox.ScreenPosInfo = (ERtSctSystemMenuScreenPos)data;
}

// UStruct 객체를 Json 파일로 저장한다.
bool ARtScenarioModel::WriteSctDataToJsonFile(FString outputFilePath, FRtScenarioData sctData)
{
	// Try to convert struct to generic json object
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(sctData);
	if (JsonObject == nullptr)
	{
		return false;
	}

	FString newPath = outputFilePath; //FPaths::ProjectDir() + cctFilePath;

	bool bSuccess = false;
	FString OutInfoMessage;
	// Try to write json to file
	URtJsonFunctionLib::WriteJson(newPath, JsonObject, bSuccess, OutInfoMessage);

	return bSuccess;
}

void ARtScenarioModel::SetEventCommand(FRtEventCommand& cmdData, const FRtEvent& eventData)
{
	cmdData.EventID = eventData.EventNameTID;
	if (!eventData.EventName.IsEmpty())
		cmdData.EventCommandName = eventData.EventName.ToString();
	else
		cmdData.EventCommandName = eventData.EventNameTID;

	for (const auto& processData : eventData.EventProcessList)
	{
		FRtSctFunctionCommand Cmd;
		if (URtResourceModelSubsystem::Get()->ScriptParsing(processData.ProcessScript, Cmd) == FRtScriptParsingResult::ESuccess)
		{
			cmdData.ProcessCommandList.Add(Cmd);
		}
	}
}

void ARtScenarioModel::SetDropboxElemantCommands(FRtDropboxBaseCommand& cmdData, const FRtDropboxBase& dropboxData)
{
	for (const FRtUIMenuElement& elemantData : dropboxData.DropboxElementList)
	{
		FRtDropboxEventCommand dropboxElement;
		SetEventCommand(dropboxElement, elemantData);
		dropboxElement.EventIndex = elemantData.EventIndex;

		// 메뉴에 대한 보이스 명령어 리스트 취합.
		for (const auto& processData : elemantData.EventProcessList) {
			FRtSctFunctionCommand funCommand;
			if (FRtScriptParsingResult::ESuccess ==
				URtResourceModelSubsystem::Get()->ScriptParsing(processData.ProcessScript, funCommand)) {
				if (funCommand.FunctionCommand == ERtSctFunCmd::VoiceCommand) {
					if (funCommand.Params.Num() == 1) {
						funCommand.Params[0].ParamString.ParseIntoArray(dropboxElement.VoiceCommandList, TEXT(";"), true);
						break;
					}
				}
			}
		}

		cmdData.DropboxElemantList.Add(dropboxElement);
	}
}

void ARtScenarioModel::AddServerEventCommandMap(const FRtDropboxBaseCommand& dropboxCommand)
{
	for (const FRtEventCommand& eventCommand : dropboxCommand.DropboxElemantList)
	{
		ServerEventCommandMap.Add(eventCommand.EventID, { &eventCommand, dropboxCommand.DropboxID });
		ReplayEventCommandMap.Add(eventCommand.EventID, eventCommand);
	}
}

const FRtEventCommand* ARtScenarioModel::FindEventCommand(const FString& eventID)
{
	FRtEventCommand* command = NULL;

	command = EventCommandMap.Find(eventID);
	if (command)
		return command;

	TPair<const FRtEventCommand*, FString >* serverEvent = ServerEventCommandMap.Find(eventID);
	if (serverEvent)
		return serverEvent->Key;

	command = SuddenEventCommandMap.Find(eventID);
	if (command)
		return command;

	TArray<FRtDropboxCommand> DropBoxList;
	DropboxCommandMap.GenerateValueArray(DropBoxList);
	for (const FRtDropboxCommand& dropbox : DropBoxList)
	{
		for (const FRtDropboxEventCommand& dropboxevent : dropbox.DropboxElemantList)
		{
			if (dropboxevent.EventID == eventID)
				return &dropboxevent;
		}
	}

	return command;
}

// 값이 있을 경우에만 설정한다.
void ARtScenarioModel::SetVariableValue(FString key, int32 value)
{
	int32* curValue = VarialbleMap.Find(key);
	if (curValue) {
		OldVarialbleMap[key] = VarialbleMap[key];
		VarialbleMap[key] = value;
	}
}

void ARtScenarioModel::Show_DropboxCommands()
{
	TArray<FRtDropboxCommand> commands;
	DropboxCommandMap.GenerateValueArray(commands);

	for (FRtDropboxCommand command : commands)
	{
		FString str = FString::Printf(TEXT("Owners : %s\n"), *(FString::Join(command.DropboxOwnerList, L",")));
		str += FString::Printf(TEXT("Dropbox ID : %s\n"), *command.DropboxID);
		str += FString::Printf(TEXT("Elements : {\n"));

		for (FRtDropboxEventCommand element : command.DropboxElemantList)
		{
			str += FString::Printf(TEXT("\tEventID : %s\n"), *element.EventID);

			str += FString::Printf(TEXT("\tProcess : {\n"));
			for (FRtSctFunctionCommand process : element.ProcessCommandList)
			{
				str += FString::Printf(TEXT("\t\t%s\n"), *process.ToString());
			}
			str += FString::Printf(TEXT("\t}\n"));
		}
		str += FString::Printf(TEXT("}\n"));

		UE_LOG(RtEvent, Log, TEXT("%s"), *str);
	}
}

void ARtScenarioModel::Show_LoopEventList()
{
	TArray<FRtScenarioEventCommand> scenarioevents;

	for (FString id : EventIDLoopList)
	{
		FRtScenarioEventCommand* command = EventCommandMap.Find(id);
		if (!command)
		{
			UE_LOG(RtEvent, Error, TEXT("LoopEvent %s Not Valide Command Map"), *id);
			continue;
		}

		FString str = FString::Printf(TEXT("Event : %s\n"), *id);

		// condition list
		str += FString::Printf(TEXT("Condition List : {\n"));
		for (FRtSctFunctionCommand condition : command->ConditionCommandList)
			str += FString::Printf(TEXT("\t%s\n"), *condition.ToString());
		str += FString::Printf(TEXT("}\n"));

		// process list
		str += FString::Printf(TEXT("Process List : {\n"));
		for (FRtSctFunctionCommand process : command->ProcessCommandList)
			str += FString::Printf(TEXT("\t%s\n"), *process.ToString());
		str += FString::Printf(TEXT("}\n"));

		UE_LOG(RtEvent, Log, TEXT("%s"), *str);
	}

}

void ARtScenarioModel::Show_ServerEventCommands()
{}

void ARtScenarioModel::Show_SuddenEventCommands()
{}
