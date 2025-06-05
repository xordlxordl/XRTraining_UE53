// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtConfig.h"
#include "XmlParser.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

#include "RtDefines.h"

DEFINE_LOG_CATEGORY(LogRtConfig);

// ini 파일 위치
FString URtConfig::RtSettingsFile = TEXT("Rt/RtSettings.ini");

int32 URtConfig::NumPublicConnections = 300;
// init boolean
bool URtConfig::bMoveUsingHmdController = false;

FString URtConfig::Solo_MapName;
FString URtConfig::Menu_MapName;
FString URtConfig::Lobby_MapName;
FString URtConfig::Game_MapName;
FString URtConfig::Replay_MapName;
FString URtConfig::Device_Name;
int32 URtConfig::Device_Index;

// Map File Name (.json)
FString URtConfig::Custom_MapName;

// Url
FString URtConfig::ListenUrl;
FString URtConfig::WebServerUrl;
FString URtConfig::WebScenarioUrl;
FString URtConfig::WebReplayUploadUrl;
FString URtConfig::WebReplayDownloadUrl;
FString URtConfig::WebLogUrl;

// Socket
FString URtConfig::TCPSocketIP;
FString URtConfig::UDPSocketIP;
int32 URtConfig::TCPPort;
int32 URtConfig::UDPPort;
int32 URtConfig::UDPRecvPort;
bool URtConfig::UsingVoice;
bool URtConfig::VoiceTalk;			

// for 10->20
int32 URtConfig::PlayerNum;

// stringtable
FString URtConfig::MessageBoxPath;
FString URtConfig::ControlMainPath;
FString URtConfig::ErrorMsgPath;
FString URtConfig::ResultDetailPath;

// nation Cde
int32 URtConfig::NationCode;

// Play Setting
FRtPlaySettings URtConfig::Play_Settings;
URtConfig::FRtTrainingOptions URtConfig::TrainingOptions;


URtConfig::URtConfig() : UGameInstanceSubsystem()
{
}

void URtConfig::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Setting_From_InitFile();
	Load_IpSettings();
	Load_TrainingOptions();
	UpdateTraineeStringTable();

}

void URtConfig::Deinitialize()
{
	Super::Deinitialize();
}

// Data Load 부분인데 XML 을 사용했었음 
const FName& URtConfig::Get_ServerLevel()
{
	return Play_Settings.ServerIp_Name;
}


FString URtConfig::Get_IpSettingsFileUrl()
{
	FString file_path = TEXT("TrainingData/IpConfig/RtIpConfig.xml");
	FString directory;
	if (GIsEditor) {
		directory = FPaths::ProjectDir();
	}
	else {
		directory = FPaths::LaunchDir();
	}
	FString file_url = directory + file_path;
	FPaths::MakeStandardFilename(file_url);
	return file_url;
}


// load file path
FString URtConfig::Get_TrainingOptionsFileUrl()
{
	FString file_path = TEXT("TrainingData/TrainingOptions.ini");
	FString directory;
	if (GIsEditor) {
		directory = FPaths::ProjectDir();
	}
	else {
		directory = FPaths::LaunchDir();
	}
	FString file_url = directory + file_path;
	FPaths::MakeStandardFilename(file_url);
	return file_url;
}


bool URtConfig::Load_IpSettings()
{
	FString file_url = Get_IpSettingsFileUrl();

	bool bResult;
	FXmlFile XmlFile;
	bResult = XmlFile.LoadFile(file_url, EConstructMethod::ConstructFromFile);

	if (!XmlFile.IsValid())
	{
		UE_LOG(LogRtConfig, Error, TEXT("Can't Read File :%s"), *file_url);
		return false;
	}

	FRtPlaySettings info;
	info.bDefaultData = false;

	const TArray<FXmlNode*>& Nodes = XmlFile.GetRootNode()->GetChildrenNodes();
	for (int i = 0; i < Nodes.Num(); i++)
	{
		const FString& TempTag = Nodes[i]->GetTag();
		const FString& TempString = Nodes[i]->GetContent();
		if (TempTag == TEXT("ServerIp")) {
			info.Set_ServerId(TempString);
		}
		Setting_From_xmlFile(TempTag, TempString);
	}
	Play_Settings = info;
	return true;
}


bool URtConfig::Load_TrainingOptions()
{
	FString file_url = Get_TrainingOptionsFileUrl();

	FConfigFile ConfigFile;
	if (FConfigCacheIni::LoadLocalIniFile(ConfigFile, *file_url, false)) {
		int32 readInt = 0;
		float readFloat = 0.0f;
		if (ConfigFile.GetInt(TEXT("Tracking"), TEXT("IsVR"), readInt))
			TrainingOptions.IsVR = readInt > 0;
		if (ConfigFile.GetInt(TEXT("Tracking"), TEXT("WeaponTrackerMode"), readInt))
			TrainingOptions.WeaponTrackerMode = readInt;
		if (ConfigFile.GetInt(TEXT("Tracking"), TEXT("IsUsingMotion"), readInt))
			TrainingOptions.IsUsingMotion = readInt > 0;
		
		if (ConfigFile.GetInt(TEXT("Safety"), TEXT("MapSizeX"), readInt))
			TrainingOptions.MapSizeX = readInt;
		if (ConfigFile.GetInt(TEXT("Safety"), TEXT("MapSizeY"), readInt))
			TrainingOptions.MapSizeY = readInt;
		if (ConfigFile.GetInt(TEXT("Safety"), TEXT("AlertDistance_Wall"), readInt))
			TrainingOptions.AlertDistance_Wall = readInt;
		if (ConfigFile.GetInt(TEXT("Safety"), TEXT("AlertDistance_HMD"), readInt))
			TrainingOptions.AlertDistance_HMD = readInt;

		if (ConfigFile.GetInt(TEXT("Shot"), TEXT("IsGuideAim"), readInt))
			TrainingOptions.IsGuideAim = readInt > 0;
		if (ConfigFile.GetFloat(TEXT("Shot"), TEXT("IntimidatingFire_Distance"), readFloat))
			TrainingOptions.IntimidatingFire_Distance = readFloat;
		if (ConfigFile.GetFloat(TEXT("Shot"), TEXT("IntimidatingFire_Angle"), readFloat))
			TrainingOptions.IntimidatingFire_Angle = readFloat;

		if (ConfigFile.GetFloat(TEXT("Sound"), TEXT("AlertVolume"), readFloat))
			TrainingOptions.AlertVolume = readFloat;
		if (ConfigFile.GetFloat(TEXT("Sound"), TEXT("SCT_Volume"), readFloat))
			TrainingOptions.SCT_Volume = readFloat;

		if (ConfigFile.GetFloat(TEXT("Sound"), TEXT("TCT_Volume"), readFloat))
			TrainingOptions.TCT_Volume = readFloat;
		if (ConfigFile.GetFloat(TEXT("Sound"), TEXT("TCT_AttenuationStart"), readFloat))
			TrainingOptions.TCT_AttenuationStart = readFloat;
		if (ConfigFile.GetFloat(TEXT("Sound"), TEXT("TCT_AttenuationEnd"), readFloat))
			TrainingOptions.TCT_AttenuationEnd = readFloat;
	}

	return true;
}




void URtConfig::Setting_From_InitFile(const FString& InIni, bool InUseDefaultVaule)
{
	if (GConfig) {
	    // Map Name Confirm
		GConfig->GetString(TEXT("RT_INIT"), TEXT("Solo_MapName"), Solo_MapName, InIni);
		GConfig->GetString(TEXT("RT_INIT"), TEXT("Menu_MapName"), Menu_MapName, InIni);
		GConfig->GetString(TEXT("RT_INIT"), TEXT("Lobby_MapName"), Lobby_MapName, InIni);
		GConfig->GetString(TEXT("RT_INIT"), TEXT("Game_MapName"), Game_MapName, InIni);
		GConfig->GetString(TEXT("RT_INIT"), TEXT("Replay_MapName"), Replay_MapName, InIni);
		GConfig->GetString(TEXT("RT_INIT"), TEXT("Custom_MapName"), Custom_MapName, InIni);

		// Device Name
		GConfig->GetString(TEXT("RT_INIT"), TEXT("Device_Name"), Device_Name, InIni);

		// Socket Port
		GConfig->GetInt(TEXT("RT_INIT"), TEXT("TCPPort"), TCPPort, InIni);
		GConfig->GetInt(TEXT("RT_INIT"), TEXT("UDPPort"), UDPPort, InIni);
		GConfig->GetBool(TEXT("RT_INIT"), TEXT("Voice"), UsingVoice, InIni);
		GConfig->GetBool(TEXT("RT_INIT"), TEXT("VoiceTalk"), VoiceTalk, InIni); // dave

		// for Fix 10->20 
		GConfig->GetInt(TEXT("RT_INIT"), TEXT("PlayerNum"), PlayerNum, InIni);
		
	}
}

void URtConfig::Setting_From_InitFile()
{
	//`` load game ini
	if (GIsEditor) {
		FString ConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() / RtSettingsFile);
		Setting_From_InitFile(ConfigFilePath, true);

	}
	else {
		FString ConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() / RtSettingsFile);
		Setting_From_InitFile(ConfigFilePath, true);
		ConfigFilePath = FPaths::ProjectConfigDir() / RtSettingsFile;
		Setting_From_InitFile(ConfigFilePath, false);
	}
}

void URtConfig::Setting_From_xmlFile(const FString& TempTag, const FString& TempString)
{
	if (TempTag == TEXT("ServerIp")) {
		ListenUrl = TempString;
	}
	else if (TempTag == TEXT("TCPSocketIp")) {
		TCPSocketIP = TempString;
	}
	else if (TempTag == TEXT("UDPSocketIp")) {
		UDPSocketIP = TempString;
	}
	else if (TempTag == TEXT("WebMainUrl")) {
		WebServerUrl = TempString;
	}
	else if (TempTag == TEXT("WebDownUrl")) {
		WebScenarioUrl = TempString;
	}
	else if (TempTag == TEXT("WebRepUpUrl")) {
		WebReplayUploadUrl = TempString;
	}
	else if (TempTag == TEXT("WebRepDnUrl")) {
		WebReplayDownloadUrl = TempString;
	}
	else if (TempTag == TEXT("WebLogUrl")) {
		WebLogUrl = TempString;
	}
	else if (TempTag == TEXT("NationCode")) {

		NationCode = FCString::Atoi(*TempString);
		NationCode_Parse(NationCode);
	}
}

void URtConfig::UpdateTraineeStringTable()
{
	// Load Csv
	TArray<FString> CSVLines;
	FString LaguageFilePath;
#if WITH_EDITOR
	LaguageFilePath = FPaths::ProjectDir();
#else
	LaguageFilePath = FPaths::LaunchDir();
#endif

	LaguageFilePath += TEXT("TrainingData/Languages/");
	FString LaguageTableFilePath = LaguageFilePath + TEXT("LanguageTable.csv");
	FString CurLanguageTableName = "";

	if (FFileHelper::LoadFileToStringArray(CSVLines, *LaguageTableFilePath)) {
		for (const FString& Line : CSVLines)
		{
			TArray<FString> ParsedLine;
			Line.ParseIntoArray(ParsedLine, TEXT(","), true);

			if (ParsedLine.Num() >= 2)
			{
				if (NationCode == FCString::Atoi(*ParsedLine[0])) {
					CurLanguageTableName = ParsedLine[1];
					break;
				}
			}
		}
	}

	if (CurLanguageTableName.IsEmpty())
		return;

	LaguageTableFilePath = LaguageFilePath + CurLanguageTableName + TEXT(".csv");

	if (FFileHelper::LoadFileToStringArray(CSVLines, *LaguageTableFilePath)) {
		UE_LOG(LogTemp, Log, TEXT("CSV File Loaded Successfully"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to Load CSV File"));
		return;
	}

	// Update String Table
	UStringTable* StringTable = LoadObject<UStringTable>(nullptr, TRAINEE_STRINGTABLE_PATH);
	if (StringTable == nullptr)
		return;

	FStringTableConstPtr TargetTable = FStringTableRegistry::Get().FindStringTable(TRAINEE_STRINGTABLE_PATH);
	FStringTable* MutableTargetTable = TraineeStringTable = const_cast<FStringTable*>(TargetTable.Get());

	if (MutableTargetTable == nullptr)
		return;

	MutableTargetTable->ClearSourceStrings();

	for (const FString& Line : CSVLines)
	{
		TArray<FString> ParsedLine;
		Line.ParseIntoArray(ParsedLine, TEXT(","), true);

		if (ParsedLine.Num() >= 2)
		{
			FString Key = ParsedLine[0];
			FString Value = ParsedLine[1];

			if (Value == TEXT("SourceString"))
				continue;

			Value = Value.Replace(TEXT("\\n"), TEXT("\n"));

			MutableTargetTable->SetSourceString(Key, Value);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("String Table Updated Successfully"));
}

FString URtConfig::GetStringAtTraineeST(const FString& key)
{
	URtConfig* config = GWorld->GetGameInstance()->GetSubsystem<URtConfig>();
	if(!config)
		return FString();

	if(!config->TraineeStringTable)
		return FString();

	FString find;
	config->TraineeStringTable->GetSourceString(key, find);
	
	return find;
}

void URtConfig::NationCode_Parse(const int32 nationcode)
{
	switch (nationcode)
	{
		// USA
	case 1 : 
		ControlMainPath = TEXT("/Game/RS/Common/Table/ST_ControlMode_Eng.ST_ControlMode_Eng");
		MessageBoxPath = TEXT("/Game/RS/Common/Table/ST_ControlMessage_Eng.ST_ControlMessage_Eng");
		ErrorMsgPath = TEXT("/Game/RS/Common/Table/ST_ControlErrorMsg_Eng.ST_ControlErrorMsg_Eng");
		ResultDetailPath = TEXT("/Game/RS/Common/Table/ST_ControlResult_Eng.ST_ControlResult_Eng");
		break;
	case 82 :
		ControlMainPath = TEXT("/Game/RS/Common/Table/ST_ControlMode_Kor.ST_ControlMode_Kor");
		MessageBoxPath = TEXT("/Game/RS/Common/Table/ST_ControlMessage_Kor.ST_ControlMessage_Kor");
		ErrorMsgPath = TEXT("/Game/RS/Common/Table/ST_ControlErrorMsg_Kor.ST_ControlErrorMsg_Kor");
		ResultDetailPath = TEXT("/Game/RS/Common/Table/ST_ControlResult_Kor.ST_ControlResult_Kor");
		break;
	default:
		break;
	}
}
