// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(RtCore, Log, All);
//DECLARE_LOG_CATEGORY_EXTERN(RtFirebox, Log, All);
//DECLARE_LOG_CATEGORY_EXTERN(TemperatureLog, Log, All);
#define CALLINFO ( FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")") )
#define APP_LOG(fmt,...) UE_LOG(RtCore, Warning, TEXT("%s %s\n"), *CALLINFO, *FString::Printf(TEXT(fmt), ##__VA_ARGS__))
//#define SCREEN_LOG_(dptime, fmt,...) if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, dptime, FColor::Yellow, FString::Printf(TEXT(fmt), ##__VA_ARGS__)); }
#define SCREEN_LOG(fmt,...) if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(fmt), ##__VA_ARGS__)); }
#define CUSTOM_FILE_APP_LOG(fmt,...) URtBluePrintCommonUtil::WriteToFileLog(TEXT("CustomFileLog.log"), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))
#define CUSTOM_FILE_APP_LOG_EX(filename,fmt,...) URtBluePrintCommonUtil::WriteToFileLog(TEXT(filename), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))

#define STR(X) *FString(X)

// [ 간단 사용법 ]
// 
// APP_LOG("%s %d %f", STR("test"), 10, 3.141592f);
//