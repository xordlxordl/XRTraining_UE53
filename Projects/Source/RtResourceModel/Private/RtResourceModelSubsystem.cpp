// Fill out your copyright notice in the Description page of Project Settings.

#include "RtResourceModelSubsystem.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "glTFRuntimeFBXFunctionLibrary.h"
#include "glTFRuntimeFBXAssetActor.h"
#include "RealTimeImportMeshFBX.h"
#include "RtObjectActor.h"
#include "RtBluePrintCommonUtil.h"
#include "RtSceneManagerBase.h"
#include "EffekseerEffect.h"
#include "EffekseerEmitterComponent.h"
#include "EffekseerSystemComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include <JsonObjectConverter.h>

URtResourceModelSubsystem* URtResourceModelSubsystem::RMSystem = nullptr;

void FRtSctFunctionCommandParam::Init()
{
	ParamType = ERtSctFunCmdParam::ENone;
	ParamString = TEXT("");
	ParamInt = 0;
	Paramfloat = 0.0f;
	ParamStringList.Empty();
}

bool FRtSctFunctionCommandParam::SetData(ERtSctFunCmdParam Type, FString OriginData)
{
	Init();

	if (!OriginData.IsEmpty())
	{
		switch (Type)
		{
			//EStringVariable, // 문자열 변수명.
			//	EStringGameState, // 게임 상태 문자열.
			//	EStringID, // 객체 인스턴스 ID.
			//	EStringDropboxID, // 메뉴 세트 ID
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
		case ERtSctFunCmdParam::EStringDropboxID:
		case ERtSctFunCmdParam::EStringMessage:
		case ERtSctFunCmdParam::EStringAniName:
		case ERtSctFunCmdParam::EStringVideoFile:
		case ERtSctFunCmdParam::EStringFormat:
		case ERtSctFunCmdParam::EStringFileName:
		case ERtSctFunCmdParam::EStringEventID:
		case ERtSctFunCmdParam::EStringKeyValue:
		{
			ParamString = OriginData;
			ParamString = ParamString.Replace(TEXT("\""), TEXT(""));
		}
		break;
		case ERtSctFunCmdParam::EIntValue:
			ParamInt = FCString::Atoi(*OriginData);
			break;
		case ERtSctFunCmdParam::EFloatValue:
			Paramfloat = FCString::Atof(*OriginData);
			break;
		case ERtSctFunCmdParam::EStringList:
		case ERtSctFunCmdParam::EStringAniNameList:
		{
			ParamString = OriginData;
			ParamString = ParamString.Replace(TEXT("\""), TEXT(""));
			ParamString = ParamString.Replace(TEXT("["), TEXT(""));
			ParamString = ParamString.Replace(TEXT("]"), TEXT(""));
			ParamString.ParseIntoArray(ParamStringList, TEXT(","), true);
			ParamString = TEXT("");
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

FString FRtSctFunctionCommandParam::ToString()
{
	FString DebugString = "";

	DebugString += FString::Printf(TEXT("ParamType : %s\n"), *UEnum::GetValueAsString(ParamType));
	switch (ParamType)
	{
	case ERtSctFunCmdParam::EStringVariable:
	case ERtSctFunCmdParam::EStringGameState:
	case ERtSctFunCmdParam::EStringID:
	case ERtSctFunCmdParam::EStringDropboxID:
	case ERtSctFunCmdParam::EStringMessage:
	case ERtSctFunCmdParam::EStringAniName:
	case ERtSctFunCmdParam::EStringVideoFile:
	case ERtSctFunCmdParam::EStringFormat:
	case ERtSctFunCmdParam::EStringFileName:
	case ERtSctFunCmdParam::EStringEventID:
	case ERtSctFunCmdParam::EStringKeyValue:
		DebugString += FString::Printf(TEXT("Value : %s\n"), *ParamString);
		break;
	case ERtSctFunCmdParam::EIntValue:
		DebugString += FString::Printf(TEXT("Value : %d\n"), ParamInt);
		break;
	case ERtSctFunCmdParam::EFloatValue:
		DebugString += FString::Printf(TEXT("Value : %f\n"), Paramfloat);
		break;
	case ERtSctFunCmdParam::EStringList:
	case ERtSctFunCmdParam::EStringAniNameList:
		DebugString += FString::Printf(TEXT("Value : {%s}\n"), *(FString::Join(ParamStringList, L",")));
		break;
	default:
		break;
	}

	return DebugString;
}

void URtResourceModelSubsystem::InitReference(UGameInstance* pGameInstance)
{
	if (pGameInstance)
	{
		RMSystem = UGameInstance::GetSubsystem<URtResourceModelSubsystem>(pGameInstance);
		if (RMSystem)
		{
			RMSystem->Init();
		}
	}
}

URtResourceModelSubsystem::URtResourceModelSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> StartPosNotReadyMaterialRef(TEXT("/Game/CommonResources/Materials/StartPoint/TraningStartPostion_NotReady.TraningStartPostion_NotReady"));
	StartPosNotReadyMaterial = Cast<UMaterialInterface>(StartPosNotReadyMaterialRef.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> StartPosReadyMaterialRef(TEXT("/Game/CommonResources/Materials/StartPoint/TraningStartPostion_Ready.TraningStartPostion_Ready"));
	StartPosReadyMaterial = Cast<UMaterialInterface>(StartPosReadyMaterialRef.Object);
}

void URtResourceModelSubsystem::Init()
{
	FunctionCommandParamInfo = {
		{ERtSctFunCmd::IsGameState,{ERtSctFunCmdParam::EStringGameState} }, // 현재 게임 상태 확인
		{ERtSctFunCmd::IsArriveGameReadyPlace,{ERtSctFunCmdParam::EStringID} }, // 게임 준비 장소에 도착 여부

		{ERtSctFunCmd::JustDoIt,{} }, // 조건 없이 그냥 실행.
		{ERtSctFunCmd::IsValue,{ERtSctFunCmdParam::EStringVariable, ERtSctFunCmdParam::EIntValue} }, // 변수값이 인자와 같은지 확인
		{ERtSctFunCmd::IsOldValue,{ERtSctFunCmdParam::EStringVariable, ERtSctFunCmdParam::EIntValue} }, // 이전 변수값이 인자와 같은지 확인
		{ERtSctFunCmd::IsVisible,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EIntValue} }, // 객체 보이는지 or 안보이는지 확인
		{ERtSctFunCmd::IsAnimProgress,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringAniName, ERtSctFunCmdParam::EFloatValue} }, // 특정 캐릭터의 애니메이션의 일치 여부와 애니메이션의 진행도를 체크한다.
		{ERtSctFunCmd::IsTargetInteract,{ERtSctFunCmdParam::EStringID} }, // 타겟에 에임을 대고 B키를 눌렀을 때 신호 체크
		{ERtSctFunCmd::IsTargetAimed,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EIntValue} }, //  누구(혹은 객체)를 조준했는지 or 조준하지 않았는지 체크
		{ERtSctFunCmd::IsInRadius,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EFloatValue} }, // 객체 간 구 충돌 확인
		{ERtSctFunCmd::IsHitObject,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EIntValue} }, // 누구를 총으로 맞추었는지 or 안맞추었는지 확인한다.
		{ERtSctFunCmd::IsIntimidatingFire,{ERtSctFunCmdParam::EIntValue, ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EFloatValue} }, // 위협사격 여부를 판단한다.
		{ERtSctFunCmd::IsTrainingElapsedAt,{ERtSctFunCmdParam::EIntValue} }, // 훈련 시작 후 몇 초나 지났는지 판단한다.
		{ERtSctFunCmd::CheckCollision,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringID} }, // 특정 객체의 중심 값이 트리거박스 영역에 들어 있는지 체크

		{ERtSctFunCmd::ReqGameEnd,{} }, // 훈련종료 조건이 되면 서버로 보내는 신호

		{ERtSctFunCmd::NotDoing,{} }, // 아무 일도 일어나지 않는 이벤트
		{ERtSctFunCmd::SetGameState,{ERtSctFunCmdParam::EStringGameState} }, // 게임 상태 설정
		{ERtSctFunCmd::SetEnableGameReadyPlace,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EIntValue} }, // 게임 준비 장소 마크 보여주기
		{ERtSctFunCmd::SetValue,{ERtSctFunCmdParam::EStringVariable, ERtSctFunCmdParam::EIntValue} }, // 변수 설정 요청
		{ERtSctFunCmd::DelayTime,{ERtSctFunCmdParam::EIntValue} }, // 시간 지연
		{ERtSctFunCmd::SetVisible,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EIntValue} }, // 특정 객체를 보여주기
		{ERtSctFunCmd::SetVisibleGroup,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EIntValue} },
		{ERtSctFunCmd::PlayAnim,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringAniName, ERtSctFunCmdParam::EIntValue} }, // 애니메이션 플레이
		{ERtSctFunCmd::MoveToWayPoint,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EFloatValue} }, // 웨이포인트로 이동하기
		{ERtSctFunCmd::NpcMoveToWayPoint,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringAniName} }, // 웨이포인트로 이동하기
		{ERtSctFunCmd::StopAct,{ERtSctFunCmdParam::EStringID} }, // 이동 멈춤
		{ERtSctFunCmd::AttachWeapon,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringID} }, // NPC 무기 장착
		{ERtSctFunCmd::AttackTarget,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringID} }, // NPC 대상 공격
		{ERtSctFunCmd::NpcInit,{ERtSctFunCmdParam::EStringID} }, // NPC 상태 초기화

		{ERtSctFunCmd::ShowScreenText,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringMessage, ERtSctFunCmdParam::EIntValue, ERtSctFunCmdParam::EIntValue, ERtSctFunCmdParam::EIntValue, ERtSctFunCmdParam::EFloatValue} }, // HMD에 고정된 텍스트 메시지를 띄운다
		{ERtSctFunCmd::ShowTargetText,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringMessage, ERtSctFunCmdParam::EFloatValue, ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EFloatValue} }, // 타겟에 고정된 텍스트 메시지를 띄운다
		{ERtSctFunCmd::ShowScreenMenu,{ERtSctFunCmdParam::EStringDropboxID} }, // HMD에 고정된 선택 메뉴를 띄운다
		{ERtSctFunCmd::ShowTargetMenu,{ERtSctFunCmdParam::EStringDropboxID, ERtSctFunCmdParam::EStringID} }, // 타겟에 고정된 선택 메뉴를 띄운다

		{ERtSctFunCmd::RemoveScreenText,{} }, // HMD에 고정된 텍스트 메시지를 지운다
		{ERtSctFunCmd::RemoveTargetText,{ERtSctFunCmdParam::EStringMessage, ERtSctFunCmdParam::EStringID} }, // 타겟에 고정된 텍스트 메시지를 지운다
		{ERtSctFunCmd::RemoveMenu,{ERtSctFunCmdParam::EStringDropboxID} }, // 선택 메뉴를 지운다

		{ERtSctFunCmd::RecordResultLog,{ERtSctFunCmdParam::EIntValue} }, // 훈련 결과에 쓸 로그를 웹 서버로 보내 카운트
		{ERtSctFunCmd::PlayMovie,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EStringVideoFile} }, // HMD 상에 비디오 플레이

		{ERtSctFunCmd::SetTargetMenuEnabled,{ERtSctFunCmdParam::EStringID, ERtSctFunCmdParam::EIntValue} }, // 타겟 메뉴 활성화/비활성화

		{ERtSctFunCmd::VoiceCommand,{ERtSctFunCmdParam::EStringID} }, // 음성 명령어 처리
		{ERtSctFunCmd::SendLog,{ERtSctFunCmdParam::EIntValue} }, // 로그 전달 신호

		{ERtSctFunCmd::IsNotValue,{ERtSctFunCmdParam::EStringVariable, ERtSctFunCmdParam::EIntValue} }, // 변수값이 인자와 같지 않은지 확인
		{ERtSctFunCmd::IsGreaterEqualValue,{ERtSctFunCmdParam::EStringVariable, ERtSctFunCmdParam::EIntValue} }, // 변수값이 인자보다 크거나 같은지 확인
		{ERtSctFunCmd::IsLessEqualValue,{ERtSctFunCmdParam::EStringVariable, ERtSctFunCmdParam::EIntValue} }, // 변수값이 인자보다 작거나 같은지 확인
		{ERtSctFunCmd::RequestRandomNum,{ERtSctFunCmdParam::EStringVariable, ERtSctFunCmdParam::EIntValue} }, // 변수에 1 ~ N 중 1개의 랜덤 값을 리슨 서버로 요청하여 동기화를 시키는 함수.

		{ERtSctFunCmd::SetVisibleRadioMenuItem,{ERtSctFunCmdParam::EStringVariable, ERtSctFunCmdParam::EIntValue} }, // 무전기 메뉴 중 항목 한 개를 보임, 안보임 설정.
		{ERtSctFunCmd::EndTraining,{} }, // 훈련 종료 신호 보내기.
	};

	TArray<FString> functionList = {
		TEXT("LoopEvent"),

		TEXT("IsGameState"),
		TEXT("IsArriveGameReadyPlace"),

		TEXT("JustDoIt"),
		TEXT("IsValue"),
		TEXT("IsOldValue"),
		TEXT("IsVisible"),
		TEXT("IsAnimProgress"),
		TEXT("IsTargetInteract"),
		TEXT("IsInRadius"),
		TEXT("IsTargetAimed"),
		TEXT("IsHitObject"),
		TEXT("IsIntimidatingFire"),
		TEXT("IsTrainingElapsedAt"),
		TEXT("CheckCollision"),

		TEXT("ReqGameEnd"),

		TEXT("NotDoing"),
		TEXT("SetGameState"),
		TEXT("SetEnableGameReadyPlace"),
		TEXT("SetValue"),
		TEXT("DelayTime"),
		TEXT("SetVisible"),
		TEXT("SetVisibleGroup"),
		TEXT("PlayAnim"),
		TEXT("MoveToWayPoint"),
		TEXT("NpcMoveToWayPoint"),
		TEXT("StopAct"),
		TEXT("AttachWeapon"),
		TEXT("AttackTarget"),
		TEXT("NpcInit"),

		TEXT("ShowScreenText"),
		TEXT("ShowTargetText"),
		TEXT("ShowScreenMenu"),
		TEXT("ShowTargetMenu"),

		TEXT("RemoveScreenText"),
		TEXT("RemoveTargetText"),
		TEXT("RemoveMenu"),

		TEXT("RecordResultLog"),
		TEXT("PlayMovie"),

		TEXT("SetTargetMenuEnabled"),

		TEXT("VoiceCommand"),
		TEXT("SendLog"),

		TEXT("IsNotValue"),
		TEXT("IsGreaterEqualValue"),
		TEXT("IsLessEqualValue"),
		TEXT("RequestRandomNum"),
		TEXT("SetVisibleRadioMenuItem"),
		TEXT("EndTraining")
	};

	int32 counter = (int32)ERtSctFunCmd::LoopEvent;
	for (auto& key : functionList)
	{
		FunctionConverterMap.Add(key, (ERtSctFunCmd)counter++);
	}

	ImporterFBX = NewObject<URealTimeImportMeshFBX>(URealTimeImportMeshFBX::StaticClass());
	ImporterFBX->AddToRoot();
}

int32 URtResourceModelSubsystem::ScriptParsingComma(FString scriptData, TArray<FString>& scriptUnitList)
{
	if (scriptData.IsEmpty())
	{
		return -1;
	}

	scriptData.ParseIntoArray(scriptUnitList, TEXT(","), true);

	return scriptUnitList.Num();
}

// 명령 파라메터 타입이 ERtSctFunCmdParam::EStringAniName 일 경우 값이 ".fbx"를 포함하는 문자열이 아닐경우
// 해석을 해서 리턴한다. 
// 해석되는 내용은 기본 애니메이션 값이다. 예) 대기(1), 걷기(2) ..
// 애니메이션 이름 항목을 갖는 명령 문장의 경우 [누가,무엇] 에 해당하는 항목이 있고 
// 그 항목을 검사 해 애니메이션 이름을 대체해 준다.
FString URtResourceModelSubsystem::ConvertAnimationItem(TArray<FString>* parsedStrings, TArray<ERtSctFunCmdParam>* paramList, FString inputAnimItem)
{
	if (inputAnimItem.Contains(TEXT(".fbx")) || !parsedStrings || !paramList)
	{
		return "";
	}

	FRegexMatcher RegexMatcher(FRegexPattern(TEXT("\\((.*?)\\)")), inputAnimItem);
	if (RegexMatcher.FindNext())
	{
		FString changeToken = RegexMatcher.GetCaptureGroup(1);
		return changeToken;

	}

	return "";
}

// Sct 함수 스크립트를 파싱하여 사용 가능한 형태의 구조체로 넣는다.
FRtScriptParsingResult URtResourceModelSubsystem::ScriptParsing(FString ScriptData, FRtSctFunctionCommand& Command)
{
	if (ScriptData.IsEmpty())
	{
		return FRtScriptParsingResult::EErrorNoInputData;
	}

	TArray<FString> ParsedStrings;
	ScriptData.ParseIntoArray(ParsedStrings, TEXT(","), true);

	int32 TokenCount = ParsedStrings.Num();
	if (TokenCount > 0)
	{
#if WITH_EDITOR
		//FuctionSpecCollectorMap.Add(ParsedStrings[0], ScriptData);
		//AllFuctionCommandList.Add(ScriptData);
#endif
		auto pFound = FunctionConverterMap.Find(ParsedStrings[0]);
		if (pFound)
		{
			Command.FunctionCommand = *pFound;

			auto* pFoundParam = FunctionCommandParamInfo.Find(Command.FunctionCommand);
			if (pFoundParam)
			{
				int32 ParamCount = (*pFoundParam).Num();

				if (ParamCount != (TokenCount - 1)) // 파라메터가 안 맞는 경우..
				{
					return FRtScriptParsingResult::EErrorParamCount;
				}

				int TokenIndexer = 1;

				for (ERtSctFunCmdParam& type : (*pFoundParam)) // 파라메터 추가.
				{
					FRtSctFunctionCommandParam CmdParam;
					FString finalToken = ParsedStrings[TokenIndexer++];
					if (type == ERtSctFunCmdParam::EStringAniName)
					{
						FString animTypeIndex = ConvertAnimationItem(&ParsedStrings, pFoundParam, finalToken);
						if (animTypeIndex.IsEmpty())
							CmdParam.SetData(ERtSctFunCmdParam::EStringAniName, finalToken);
						else
							CmdParam.ParamType = ERtSctFunCmdParam::EStringAniName;
						Command.Params.Add(CmdParam);

						CmdParam.SetData(ERtSctFunCmdParam::EIntValue, animTypeIndex);
						CmdParam.ParamType = ERtSctFunCmdParam::EIntValue;
						Command.Params.Add(CmdParam);
					}
					else
					{
						CmdParam.SetData(type, finalToken);
						Command.Params.Add(CmdParam);
					}

				}
			}
		}
	}

	return FRtScriptParsingResult::ESuccess;
}

UMaterialInstanceDynamic* URtResourceModelSubsystem::CreateMaterial(UTexture2D* pDiffTex, UTexture2D* pNormalTex)
{
	FString BaseMaterialAddress = "Material'/Game/CommonResources/Materials/RealTimeFbxImport/importTestMat.importTestMat'";
	//	FString BaseMaterialAddress = "Material'/Game/CommonResources/Materials/RealTimeFbxImport/20_-_Default.20_-_Default'";

	UMaterial* pMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *BaseMaterialAddress, nullptr, LOAD_None, nullptr));

	UMaterialInstanceDynamic* pProcMaterial = UMaterialInstanceDynamic::Create(pMaterial, NULL);

	if (pDiffTex)
	{
		pProcMaterial->SetTextureParameterValue(TEXT("myParamDiffuse"), pDiffTex);
	}

	if (pNormalTex)
	{
		pProcMaterial->SetTextureParameterValue(TEXT("myParamNormal"), pNormalTex);
	}

	return pProcMaterial;
}

UMaterialInstanceDynamic* URtResourceModelSubsystem::CreateAlphaMaterial(UTexture2D* pAlphaTex, FLinearColor diffColor)
{
	// 모델 머티리얼의 [DiffuseColor]와 [Alpha 텍스처의 RGB] 값을 사용하는 언리얼 머티리얼 
	FString BaseMaterialAddress = "Material'/Game/CommonResources/Materials/RealTimeFbxImport/importTestMatAlpha.importTestMatAlpha'";

	UMaterial* pMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *BaseMaterialAddress, nullptr, LOAD_None, nullptr));

	UMaterialInstanceDynamic* pProcMaterial = UMaterialInstanceDynamic::Create(pMaterial, NULL);

	if (pAlphaTex)
	{
		pProcMaterial->SetTextureParameterValue(TEXT("myParamAlpha"), pAlphaTex);
		pProcMaterial->SetVectorParameterValue(TEXT("myParamDiffuseColor"), diffColor);
	}
	//if (pNormalTex)
	//{
	//	pProcMaterial->SetTextureParameterValue(TEXT("myParamNormal"), pNormalTex);
	//}

	return pProcMaterial;
}


UMaterialInstanceDynamic* URtResourceModelSubsystem::CreateColorMaterial(FLinearColor diffColor)
{
	FString BaseMaterialAddress = "Material'/Game/CommonResources/Materials/RealTimeFbxImport/importTestColorMat.importTestColorMat'";

	UMaterial* pMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *BaseMaterialAddress, nullptr, LOAD_None, nullptr));

	UMaterialInstanceDynamic* ProcMaterial = UMaterialInstanceDynamic::Create(pMaterial, NULL);

	if (ProcMaterial)
	{
		ProcMaterial->SetVectorParameterValue(TEXT("myParamDiffuse"), diffColor);
	}

	return ProcMaterial;
}

bool URtResourceModelSubsystem::MakeOneModelStructToObjectActor(ARtObjectActor* pObjectActor, FRTIModelStruct& modelStruct, int32& sectionIndex)
{
	if (pObjectActor == nullptr)
	{
		return false;
	}
	//FRTIModelStruct* modelStruct = Cast<FRTIModelStruct>(meshStructVoid);

	int64 modelID;
	FString name;
	FTransform relativeTransform;
	TArray<FRTIMeshStruct> meshStructs;
	bool hasParent;
	int64 parentModelID;
	TArray<int64> childrenModelIDs;
	ERTIModelType modelType;

	URealTimeImportBPLibrary::breakModelStruct(
		modelStruct,
		modelID,
		name,
		relativeTransform,
		meshStructs,
		hasParent,
		parentModelID,
		childrenModelIDs,
		modelType
	);

	if (modelType == ERTIModelType::E_group)
	{
		//	TestActor->CreateNewProceduralMesh(relativeTransform, modelID, hasParent, parentModelID);
	}
	else if (modelType == ERTIModelType::E_mesh)
	{
		UProceduralMeshComponent* procMeshCom = pObjectActor->GetProcedualMesh();

		if (procMeshCom)
		{
			FVector scale = relativeTransform.GetScale3D();
			procMeshCom->SetRelativeScale3D(scale);
			FString geometryName;
			TArray<FVector> vertices;
			TArray<int32> triangles;
			TArray<FVector> normals;
			TArray<FVector2D> UV0;
			TArray<FVector2D> UV1;
			TArray<FVector2D> UV2;
			TArray<FVector2D> UV3;
			TArray<FLinearColor> vertexColors;
			TArray<FProcMeshTangent> tangents;
			FRTIMaterialStruct materialStruct;
			bool bCreateCollision = false;
			bool bSRGBConversion = false;

			for (const auto& meshStruct : modelStruct.meshStructs)
			{
				URealTimeImportBPLibrary::breakMeshStruct(
					meshStruct,
					geometryName,
					vertices,
					triangles,
					normals,
					UV0,
					vertexColors,
					tangents,
					materialStruct
				);

				procMeshCom->CreateMeshSection_LinearColor(
					sectionIndex,
					vertices,
					triangles,
					normals,
					UV0,
					UV1,
					UV2,
					UV3,
					vertexColors,
					tangents,
					bCreateCollision,
					bSRGBConversion
				);

				bool hasTexture;
				FString materialName;
				FString textureName;
				TArray<FRTITextureStruct> textures;
				FColor ambient;
				FColor diffuse;
				FColor specular;
				FColor emissive;
				FColor transparent;
				FColor reflection;
				float specularExponent;
				float dissolved;
				float dissolvedInverted;
				float emissiveFactor;
				float ambientFactor;
				float diffuseFactor;
				float bumpFactor;
				float transparencyFactor;
				float displacementFactor;
				float vectorDisplacementFactor;
				float specularFactor;
				float shininessExponent;
				float reflectionFactor;

				URealTimeImportBPLibrary::breakMaterialStruct(
					materialStruct,
					hasTexture,
					materialName,
					textureName,
					textures,
					ambient,
					diffuse,
					specular,
					emissive,
					transparent,
					reflection,
					specularExponent,
					dissolved,
					dissolvedInverted,
					emissiveFactor,
					ambientFactor,
					diffuseFactor,
					bumpFactor,
					transparencyFactor,
					displacementFactor,
					vectorDisplacementFactor,
					specularFactor,
					shininessExponent,
					reflectionFactor
				);

				UMaterialInstanceDynamic* mat = nullptr;

				if (hasTexture)
				{
					UTexture2D* pDiffTex = nullptr;
					UTexture2D* pNormalTex = nullptr;
					UTexture2D* pAlphaTex = nullptr;

					for (auto& tex : textures)
					{
						switch (tex.textureType)
						{
						case ERTITextureType::E_Kd: // Diffuse
							pDiffTex = tex.texture;
							break;
						case ERTITextureType::E_bump:
							pNormalTex = tex.texture;
							break;
						case ERTITextureType::E_norm: // Normal
							pNormalTex = tex.texture;
							break;
						case ERTITextureType::E_none: // Alpha
						case ERTITextureType::E_d: // Alpha
							pAlphaTex = tex.texture;
							break;
						default:
							break;
						}
					}

					if (pAlphaTex)
					{
						mat = CreateAlphaMaterial(pAlphaTex, diffuse);
					}
					else
					{
						mat = CreateMaterial(pDiffTex, pNormalTex);
					}

					procMeshCom->SetMaterial(sectionIndex, mat);
				}
				else
				{
					mat = CreateColorMaterial(diffuse);

					procMeshCom->SetMaterial(sectionIndex, mat);
				}

				sectionIndex++;
			}

			//FbxProMeshCloneMap.Add(fbxFileName, DuplicateObject<UProceduralMeshComponent>(pObjectActor->GetProcedualMesh(), pObjectActor));
		}
	}

	return true;
}

bool URtResourceModelSubsystem::LoadFbxFileToObjectActor_Sub(ARtObjectActor* pObjectActor, TArray<FRTIModelStruct> modelStructs, bool isForGame)
{
	if (!pObjectActor)
	{
		return false;
	}

	int sectionIndex = 0;
	for (FRTIModelStruct& meshStruct : modelStructs)
	{
		MakeOneModelStructToObjectActor(pObjectActor, meshStruct, sectionIndex);
	}

	//if (!isForGame)
	//{
	//	pObjectActor->SetInvisiblePickingStaticMesh(); // 툴 전용.
	//}

	return true;
}

bool URtResourceModelSubsystem::LoadFbxFileToObjectActor(FString fbxFileName, ARtObjectActor* pObjectActor, bool isForGame, FglTFRuntimeFBX_PreLoadInfo preLoadInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("URtResourceModelSubsystem::LoadFbxFileToObjectActor() ~ begin %s"), *fbxFileName);

	if (FbxModelPathBase.IsEmpty())
	{
		LoadFbxModelPathBase();
	}

	if (FbxModelPathBase.IsEmpty() || fbxFileName.IsEmpty())
	{
		return false;
	}

	FString fbxFilePath = FString::Printf(TEXT("%s/%s"), *FbxModelPathBase, *fbxFileName);

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*fbxFilePath)) // 파일이 존재 유무 확인.
	{
		UE_LOG(LogTemp, Warning, TEXT("URtResourceModelSubsystem::LoadFbxFileToObjectActor() ~ file not found! "));
		return false;
	}

	if (ImporterFBX && pObjectActor)
	{
		auto found = FbxProMeshCloneMap.Find(fbxFileName);
		if (!found)
		{
			ERTIMeshType fileType = ERTIMeshType::E_fbx;

			bool success = false;
			FString successMessage = TEXT("");
			TArray<FRTIModelStruct> modelStructs;
			ERTICoordinateSystem coordinateSystem = ERTICoordinateSystem::E_RightHanded;
			bool calculateTangents = true;
			bool	cacheTexture = true;
			bool	autoDetectionNormalMap = true;
			bool	useSRGB = true;
			bool	createMipMaps = true;
			ERTIERGBFormat	rgbFormat = ERTIERGBFormat::E_BGRA;

			ImporterFBX->LoadFBXFile(
				fileType,
				fbxFilePath,
				success,
				successMessage,
				modelStructs,
				coordinateSystem,
				calculateTangents,
				cacheTexture,
				autoDetectionNormalMap,
				useSRGB,
				createMipMaps,
				rgbFormat);

			if (!success)
			{
				FbxImportErrorMessage = successMessage;
				return false;
			}

			UE_LOG(LogTemp, Warning, TEXT("URtResourceModelSubsystem::LoadFbxFileToObjectActor() ~ File Loaded"));

			FbxProMeshCloneMap.Add(fbxFileName, modelStructs);
			FbxCreatorBlenderMap.Add(fbxFileName, preLoadInfo.bBlenderCreateor);

			LoadFbxFileToObjectActor_Sub(pObjectActor, modelStructs, isForGame);
		}
		else
		{
			LoadFbxFileToObjectActor_Sub(pObjectActor, *found, isForGame);

			APP_LOG("LoadFbxFileToObjectActor ok : %s", *fbxFileName);
			//UE_LOG(LogTemp, Warning, TEXT("URtResourceModelSubsystem::LoadFbxFileToObjectActor() ~ File Loaded"));
		}

		auto foundBlender = FbxCreatorBlenderMap.Find(fbxFileName);
		if (foundBlender)
		{
			UProceduralMeshComponent* proMesh = pObjectActor->GetProcedualMesh();
			if (proMesh)
			{
				if (!(*foundBlender))
				{ // Max에서 익스포트 된 경우 다음과 같이 임의 회전을 시킨다.
// proMesh->SetRelativeRotation(FRotator(0, 90, 270)); // 이것이 Max와 같은 방향이다.
					proMesh->SetRelativeRotation(FRotator(0, 180, 270)); // 기존 데이터들과의 호환성을 위해서 이렇게 진행한다.
				}
				else
				{ // 블렌더에서 익스포트 한 것은 그대로 쓴다.
					proMesh->SetRelativeRotation(FRotator(0, 0, 0));
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("URtResourceModelSubsystem::LoadFbxFileToObjectActor() ~ end "));

		return true;
	}

	return false;
}

void URtResourceModelSubsystem::SetFbxModelPathBase(FString fbxModelPathBase)
{
	FbxModelPathBase = fbxModelPathBase;
}

// 맵 로딩 전에 반드시 초기화해 주어야 한다.
void URtResourceModelSubsystem::ClearFbxProMeshCloneMap()
{
	FbxProMeshCloneMap.Empty();
}

void URtResourceModelSubsystem::LoadFbxModelPathBase()
{
	FbxModelPathBase = GetTrainingDataPath() + TEXT("/FbxFiles");
	return;

	FString baseDir;

	//ARtSceneManagerBase* sceneManagerBase = Cast<ARtSceneManagerBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ARtSceneManagerBase::StaticClass()));
	//if (sceneManagerBase)
	//{
	//	if (sceneManagerBase->GetProductType() == ERtProductType::EGame)
	//	{
	//		FbxModelPathBase = FString::Printf(TEXT("%s/%s/%s"), *baseDir, CLIENT_PROJECT_DATA_PATH, TEXT("FbxFiles"));
	//		FbxModelPathBase = FbxModelPathBase.Replace(TEXT("//"),TEXT("/"));
	//		FbxModelPathBase = FbxModelPathBase.Replace(TEXT("\\"), TEXT("/"));
	//		return;
	//	}
	//}

	FString configPath = baseDir + "cct_config.ini"; // tct, mct(sct포함) 모두 하나의 설정 파일을 사용한다.

	FConfigFile ConfigFile;
	if (FConfigCacheIni::LoadLocalIniFile(ConfigFile, *configPath, false))
	{
		bool bUseParentDir = false;
		FString DefaultProjectPath;
		FString Value;

		if (ConfigFile.GetString(TEXT("PATH"), TEXT("project_path_parent_dir_use"), Value))
		{
			if (Value == "true")
			{
				bUseParentDir = true;
			}
		}

		if (ConfigFile.GetString(TEXT("PATH"), TEXT("project_path"), Value))
		{
			DefaultProjectPath = Value;

			if (bUseParentDir)
			{
				FString tempDir = URtBluePrintCommonUtil::RemoveLastFolder(baseDir);
				DefaultProjectPath = FPaths::ConvertRelativePathToFull(tempDir + DefaultProjectPath);
			}
			else
			{
				DefaultProjectPath = FPaths::ConvertRelativePathToFull(baseDir + DefaultProjectPath);
			}

			//CUSTOM_FILE_APP_LOG("~ tct ProjectPath %s", *DefaultProjectPath);

			//DefaultTctFilesPath = DefaultProjectPath + TEXT("/TctFiles");
			//DefaultPngFilesPath = DefaultProjectPath + TEXT("/PngFiles");
			FbxModelPathBase = DefaultProjectPath + TEXT("/FbxFiles");
			//DefaultWavFilesPath = DefaultProjectPath + TEXT("/WavFiles");
		}

	}
}

UglTFRuntimeAsset* URtResourceModelSubsystem::LoadGltfAssetSub(const FString& fbxFileName)
{
	FString FileExtension = FPaths::GetExtension(fbxFileName).ToLower();
	FglTFRuntimeConfig LoaderConfig;
	LoaderConfig.bAllowExternalFiles = true;

	bool bIsFbx = FileExtension == TEXT("fbx");
	bool bIsGltf = FileExtension == TEXT("gltf") || FileExtension == TEXT("glb");

	if (!bIsFbx && !bIsGltf)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadGltfAsset: Unsupported file extension: %s"), *FileExtension);
		return nullptr;
	}

	UglTFRuntimeAsset* ModelAsset = nullptr;
	if (bIsGltf)
	{
		ModelAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromFilename(fbxFileName, false, LoaderConfig);
	}
	else if (bIsFbx)
	{
		LoaderConfig.bAsBlob = true;
		ModelAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromFilename(fbxFileName, false, LoaderConfig);
		ModelAssetBP = ModelAsset;
		FglTFRuntimeMaterialsConfig gltfMatConfig;
		int32 MaterialCount = ModelAsset->GetMaterialsVariants().Num();
		for (int32 i = 0; i < MaterialCount; ++i)
		{
			UMaterialInterface* Material = ModelAsset->LoadMaterial(i, gltfMatConfig, false);
			if (Material)
			{
				UE_LOG(LogTemp, Log, TEXT("LoadModelFile: Material %d loaded successfully."), i);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("LoadModelFile: Failed to load material %d."), i);
			}
		}
	}

	if (!ModelAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadModelFile: Failed to load asset from file: %s"), *fbxFileName);
	}

	return ModelAsset;
}

// 파일을 로드하면서 gltf Asset 을 제작한다. 
// gltf 또는 fbx 를 로드 할 수 있다.
UglTFRuntimeAsset* URtResourceModelSubsystem::LoadGltfAsset(const FString& fbxFileName, ARtObjectActor* pObjectActor, bool isForGame)
{
	if (!pObjectActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadModelFile: pObjectActor is null"));
		return nullptr;
	}

	FString FileExtension = FPaths::GetExtension(fbxFileName).ToLower();
	FglTFRuntimeConfig LoaderConfig;
	LoaderConfig.bAllowExternalFiles = true;

	bool bIsFbx = FileExtension == TEXT("fbx");
	bool bIsGltf = FileExtension == TEXT("gltf") || FileExtension == TEXT("glb");

	if (!bIsFbx && !bIsGltf)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadGltfAsset: Unsupported file extension: %s"), *FileExtension);
		return nullptr;
	}

	UglTFRuntimeAsset* ModelAsset = nullptr;
	if (bIsGltf)
	{
		ModelAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromFilename(fbxFileName, false, LoaderConfig);
	}
	else if (bIsFbx)
	{
		LoaderConfig.bAsBlob = true;
		ModelAsset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromFilename(fbxFileName, false, LoaderConfig);
		ModelAssetBP = ModelAsset;
		FglTFRuntimeMaterialsConfig gltfMatConfig;
		int32 MaterialCount = ModelAsset->GetMaterialsVariants().Num();
		for (int32 i = 0; i < MaterialCount; ++i)
		{
			UMaterialInterface* Material = ModelAsset->LoadMaterial(i, gltfMatConfig, false);
			if (Material)
			{
				UE_LOG(LogTemp, Log, TEXT("LoadModelFile: Material %d loaded successfully."), i);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("LoadModelFile: Failed to load material %d."), i);
			}
		}
	}

	if (!ModelAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadModelFile: Failed to load asset from file: %s"), *fbxFileName);
	}

	return ModelAsset;
}

void URtResourceModelSubsystem::UpdateTrainingDataPath(bool isDefault)
{
	FString baseDir;

#if WITH_EDITOR
	baseDir = FPaths::ProjectDir();
#else
	baseDir = FPaths::LaunchDir();
#endif

	TrainingDataPath = FString::Printf(TEXT("%s/%s"), *baseDir, CLIENT_PROJECT_DATA_PATH);
	TrainingDataPath = TrainingDataPath.Replace(TEXT("//"), TEXT("/"));
	TrainingDataPath = TrainingDataPath.Replace(TEXT("\\"), TEXT("/"));

	if (!isDefault)
	{
		FString iniFilePath = TrainingDataPath + TEXT("/RtSingleSettings.ini");
		FConfigFile ConfigFile;
		if (FConfigCacheIni::LoadLocalIniFile(ConfigFile, *iniFilePath, false))
		{
			ConfigFile.GetString(TEXT("PATH"), TEXT("ProjFilePath"), TrainingDataPath);
			ConfigFile.GetString(TEXT("PATH"), TEXT("SctFileName"), Single_SctFileName);
			ConfigFile.GetString(TEXT("PATH"), TEXT("SctRole"), Single_RoleID);

			TrainingDataPath = TrainingDataPath.Replace(TEXT("//"), TEXT("/"));
			TrainingDataPath = TrainingDataPath.Replace(TEXT("\\"), TEXT("/"));
		}

	}

	return;
}

FString URtResourceModelSubsystem::GetFolderNameFromPath(const FString& FilePath)
{
	// 경로를 디렉토리로 분리
	TArray<FString> PathParts;
	FilePath.ParseIntoArray(PathParts, TEXT("/"), true); // Unix 스타일 경로 구분자

	if (PathParts.Num() < 2)
	{
		// 폴더명과 파일명이 부족하면 빈 문자열 반환
		return FString();
	}

	// 마지막 폴더명과 파일명 추출
	FString LastFolder = PathParts[PathParts.Num() - 2];
	FString FileName = PathParts[PathParts.Num() - 1];

	// 폴더명과 파일명 결합
	FString Result = LastFolder + TEXT("/") + FileName;

	return Result;
}

UEffekseerEffect* URtResourceModelSubsystem::ImportEffekseerEffect(const FString& FilePath)
{
	UEffekseerEffect* LoadedEffect = NewObject<UEffekseerEffect>();

	UE_LOG(LogTemp, Log, TEXT("Successfully loaded Effekseer effect from path"));

	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to read file from path: %s"), *FilePath);
		return nullptr;
	}

	if (LoadedEffect)
	{
		// 이펙트 로드
		LoadedEffect->Load(FileData.GetData(), FileData.Num(), *FilePath);
		// 확인용 로그 추가
		UE_LOG(LogTemp, Log, TEXT("Effect loaded successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UEffekseerEffect object."));

		return nullptr;
	}

	return LoadedEffect;
}

void URtResourceModelSubsystem::PlayEffekseerEffectLooping(UEffekseerEmitterComponent* newEffekseer)
{
	if (newEffekseer)
	{
		newEffekseer->IsLooping = true;
		//newEffekseer->Activate();  // 이펙트를 재생합니다.

		// 확인용 로그 추가
		UE_LOG(LogTemp, Log, TEXT("Playing Effekseer effect in looping mode."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EmitterComponent or Effect is null."));
	}
}

void URtResourceModelSubsystem::PlayEffekseerEffectOnce(UEffekseerEmitterComponent* newEffekseer)
{
	if (newEffekseer->Effect)
	{
		newEffekseer->IsLooping = false;
		//newEffekseer->Activate();  // 이펙트를 재생합니다.

		// 확인용 로그 추가
		UE_LOG(LogTemp, Log, TEXT("Playing Effekseer effect once."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EmitterComponent or Effect is null."));
	}
}

bool URtResourceModelSubsystem::StopEffekseerEffect(AActor* firstActor)
{
	if (!firstActor)
	{
		return false;
	}

	// 이미터 컴포넌트가 있는 객체를 찾는다.
	UEffekseerEmitterComponent* emitterComponent = nullptr;
	if (!GetEffekseerEmitterComponent(firstActor, emitterComponent))
	{
		return false;
	}

	if (emitterComponent)
	{
		//emitterComponent->Deactivate();
		emitterComponent->Deactivate();
		return true;
	}

	return false;
}

void URtResourceModelSubsystem::SetEffekseerEffectSpeed(AActor* firstActor, float newSpeed)
{
	if (!firstActor)
	{
		return;
	}

	// 이미터 컴포넌트가 있는 객체를 찾는다.
	UEffekseerEmitterComponent* emitterComponent = nullptr;
	if (!GetEffekseerEmitterComponent(firstActor, emitterComponent))
	{
		return;
	}

	if (emitterComponent)
	{
		//emitterComponent->Deactivate();
		emitterComponent->Speed = newSpeed;
	}
}

// 모든 프로그램 시작 후 렌더링 가능 시점에서 해당 함수를 호출해 주어야 한다.
// 이펙트 루트 폴더를 절대 경로로 붙여 넣어 전체 efk 패스를 검색하여
// 임의 실행을 통한 세이더 초기화를 하도록 유도한다.
// 이는 플레이를 위해 처음 실행이 안되는 플러그인 이슈를 막기위한 일종의 초기화 작업이다.
bool URtResourceModelSubsystem::InitEffekseerEffect(FString efkFolderPath)
{
	if (efkFolderPath.IsEmpty())
	{
		return false;
	}

	if (EffectSystemComponent == nullptr || EffectEmitterComponent == nullptr)
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), foundActors);
		for (const auto& actor : foundActors)
		{
			UEffekseerSystemComponent* tempEffectSystemComponent = actor->FindComponentByClass<UEffekseerSystemComponent>();
			if (tempEffectSystemComponent)
			{
				EffectSystemComponent = tempEffectSystemComponent;
			}
			UEffekseerEmitterComponent* tempEmitterComponent = actor->FindComponentByClass<UEffekseerEmitterComponent>();
			if (tempEmitterComponent)
			{
				EffectEmitterComponent = tempEmitterComponent;
			}
		}
	}

	if (EffectSystemComponent && EffectEmitterComponent)
	{
		TArray<uint8> FileData;

		// 폴더 탐색
		IFileManager& FileManager = IFileManager::Get();
		TArray<FString> EfkPaths;
		FileManager.FindFilesRecursive(EfkPaths, *efkFolderPath, TEXT("*.efk"), true, false);

		for (auto& efkPath : EfkPaths)
		{
			UEffekseerEffect* LoadedEffect = NewObject<UEffekseerEffect>();
			if (FFileHelper::LoadFileToArray(FileData, *efkPath))
			{
				LoadedEffect->Load(FileData.GetData(), FileData.Num(), *efkPath);
				EffectFileArray.Add(LoadedEffect);
			}
		}

		if (EffectFileArray.Num() > 0)
		{
			EffectInitCounter = 0;
			AActor* owner = EffectSystemComponent->GetOwner();
			if (owner)
			{
				owner->GetWorldTimerManager().SetTimer(EffectInitTimerHandle, [&] {
					EffectEmitterComponent->Effect = EffectFileArray[EffectInitCounter++];
					//					EffectEmitterComponent->Deactivate();
					EffectEmitterComponent->Activate();
					if (EffectEmitterComponent->Effect)
					{
						APP_LOG("timer~ %d %s", EffectInitCounter, *EffectEmitterComponent->Effect->Name);
					}
					if (EffectInitCounter >= EffectFileArray.Num())
					{
						EffectSystemComponent->GetOwner()->GetWorldTimerManager().ClearTimer(EffectInitTimerHandle);
					}

					}, 0.2f, true);
			}
		}
	}

	return false;
}

// 이펙시어 이미터 컴포넌트를 가져온다.
bool URtResourceModelSubsystem::GetEffekseerEmitterComponent(AActor* firstActor, UEffekseerEmitterComponent*& emitterComponent)
{
	if (firstActor)
	{
		TArray<AActor*> AttachedActors;
		firstActor->GetAttachedActors(AttachedActors);
		for (AActor* AttachedActor : AttachedActors)
		{
			UEffekseerEmitterComponent* effekEmit = AttachedActor->FindComponentByClass<UEffekseerEmitterComponent>();
			if (effekEmit)
			{
				emitterComponent = effekEmit;
				return true;
			}
		}
	}

	emitterComponent = nullptr;
	return false;
}

// 이펙시어 이미터 컴포넌트를 재활성화 시킨다.
bool URtResourceModelSubsystem::PlayEffekseerEffect(AActor* firstActor, bool bIsLoop)
{
	if (!firstActor)
	{
		return false;
	}

	// 이미터 컴포넌트가 있는 객체를 찾는다.
	UEffekseerEmitterComponent* emitterComponent = nullptr;
	if (!GetEffekseerEmitterComponent(firstActor, emitterComponent))
	{
		return false;
	}

	if (emitterComponent)
	{
		emitterComponent->IsLooping = bIsLoop;
		emitterComponent->Activate();
		return true;
	}

	return false;
}

// 이펙시어 이미터를 액터에 붙인다.
bool URtResourceModelSubsystem::AttachEffekseerEmitterComponent(AActor* firstActor, UEffekseerEmitterComponent*& emitterComponent)
{
	// 이미터 컴포넌트가 있는 객체를 찾는다.
	if (GetEffekseerEmitterComponent(firstActor, emitterComponent))
	{
		return false;
	}

	UObject* effekseerEmitterObject = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL,
		TEXT("/Effekseer/BluePrints/BP_EffekseerEmitter.BP_EffekseerEmitter_C")));
	UClass* effekseerEmitterObjectClass = Cast<UClass>(effekseerEmitterObject);

	FActorSpawnParameters spawnParams;
	UWorld* World = GetWorld();
	AActor* EmitterActor = World->SpawnActor<AActor>(effekseerEmitterObjectClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
	if (EmitterActor)
	{
		emitterComponent = Cast<UEffekseerEmitterComponent>(EmitterActor->GetRootComponent());
		//emitterComponent->AttachToComponent(EmitterActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		//emitterComponent->bAutoActivate = false;
		EmitterActor->AttachToActor(firstActor, FAttachmentTransformRules::KeepRelativeTransform);
		EmitterActor->SetActorHiddenInGame(false);

		emitterComponent->Deactivate();

		return true;
	}

	return false;
}

// 액터에 이펙시어 이펙트를 장전시킨다. ( 일단 한 개의 이미터만 허용한다 )
bool URtResourceModelSubsystem::SetEffekseerEffect(AActor* firstActor, const FString& FilePath)
{
	if (!firstActor)
	{
		return false;
	}

	UEffekseerEffect* LoadedEffect = NewObject<UEffekseerEffect>();

	UE_LOG(LogTemp, Log, TEXT("Successfully loaded Effekseer effect from path"));

	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to read file from path: %s"), *FilePath);
		return false;
	}

	if (LoadedEffect)
	{
		// 이펙트 로드
		LoadedEffect->Load(FileData.GetData(), FileData.Num(), *FilePath);
		// 확인용 로그 추가
		UE_LOG(LogTemp, Log, TEXT("Effect loaded successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UEffekseerEffect object."));
		return false;
	}

	// 이미터 컴포넌트가 있는 객체를 찾는다.
	UEffekseerEmitterComponent* emitterComponent = nullptr;
	if (!GetEffekseerEmitterComponent(firstActor, emitterComponent))
	{
		return false;
	}

	if (emitterComponent)
	{

		if (emitterComponent->Effect)
		{
			emitterComponent->Effect->ConditionalBeginDestroy();
		}

		emitterComponent->Effect = LoadedEffect;
		emitterComponent->IsLooping = false;
		//emitterComponent->Deactivate();

		return true;
	}

	return false;
}

bool URtResourceModelSubsystem::ImportEffekseerEffectCVersion(AActor* firstActor, const FString& FilePath, bool bIsLoop)
{
	FString ProjectDir = FPaths::ProjectDir();

	// 상대 경로를 절대 경로로 변환
	FString AbsolutePath = FPaths::ConvertRelativePathToFull(ProjectDir, FilePath);

	UEffekseerEmitterComponent* emitterComponent = nullptr;
	AttachEffekseerEmitterComponent(firstActor, emitterComponent);

	if (emitterComponent)
	{
		if (SetEffekseerEffect(firstActor, AbsolutePath))
		{
			return PlayEffekseerEffect(firstActor, bIsLoop);
		}
	}

	return false;
}

void URtResourceModelSubsystem::UpdateStringTableFromAsset(const FString& AssetPath, const FString& TargetTableName)
{
	FSoftObjectPath AssetRef(AssetPath);
	UStringTable* StringTable = Cast<UStringTable>(AssetRef.TryLoad());

	if (StringTable)
	{
		// FStringTableRegistry에서 TargetTable을 찾아서 가져옴
		FStringTableConstPtr TargetTable = FStringTableRegistry::Get().FindStringTable(FName(*TargetTableName));

		if (TargetTable.IsValid())
		{
			FStringTable* MutableTargetTable = const_cast<FStringTable*>(TargetTable.Get());

			MutableTargetTable->ClearSourceStrings();

			FStringTableConstRef SourceTable = StringTable->GetStringTable();
			TMap<FTextKey, FString> SourceData;

			// SourceTable에서 데이터를 열거하여 SourceData에 저장
			SourceTable->EnumerateKeysAndSourceStrings([&SourceData](const FTextKey& Key, const FString& SourceString) -> bool
				{
					SourceData.Add(Key, SourceString);
					return true;
				});

			for (const auto& Entry : SourceData)
			{
				MutableTargetTable->SetSourceString(Entry.Key, Entry.Value);
			}

			UE_LOG(LogTemp, Log, TEXT("String Table '%s' successfully updated from asset: %s"), *TargetTableName, *AssetPath);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Target String Table '%s' not found"), *TargetTableName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load UStringTable from asset: %s"), *AssetPath);
	}
}

int32 URtResourceModelSubsystem::CheckTctFileVersion(FString tctFilePath)
{
	bool isSuccess = false;
	FString outMessage;

	TSharedPtr<FJsonObject> tctJsonObject = URtJsonFunctionLib::ReadJson(tctFilePath, isSuccess, outMessage);
	if (tctJsonObject.IsValid())
	{
		FRtTctStruct tctStruct;

		if (!FJsonObjectConverter::JsonObjectToUStruct<FRtTctStruct>(tctJsonObject.ToSharedRef(), &tctStruct))
		{
			return false;
		}

		return tctStruct.Version;
	}

	return -1;
}

// Mct, Sct 공용
int32 URtResourceModelSubsystem::GetMapFileVersion(FString mapFilePath)
{
	bool isSuccess;
	FString outMessage;
	TSharedPtr<FJsonObject> tctJsonObject = URtJsonFunctionLib::ReadJson(mapFilePath, isSuccess, outMessage);
	if (tctJsonObject.IsValid())
	{
		int32 version = tctJsonObject->GetIntegerField(TCT2_VERSION_TAG);
		return version;
	}

	return -1;
}

// 특정 Enum 의 값을 입력하면 Enum String을 출력한다.
FString URtResourceModelSubsystem::GetEnumToString(FString enumClassName, int32 enumValue)
{
	if (enumClassName.IsEmpty())
	{
		return FString("enumClassName Empty!");
	}

	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *enumClassName, true);
	if (!EnumPtr)
	{
		return FString("Invalid");
	}

	return EnumPtr->GetNameStringByValue(static_cast<int64>(enumValue));
}
