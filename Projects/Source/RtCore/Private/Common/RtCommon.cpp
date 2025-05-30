// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/RtCommon.h"



namespace RtMatchState
{
	const FName EnteringMap = FName(TEXT("EnteringMap"));              // Unreal Basic
	const FName WaitingToStart = FName(TEXT("WaitingToStart"));       // Unreal Basic
	const FName InProgress_StartTrng = FName(TEXT("InProgress_StartTrng"));   // Start Match
	const FName InProgress_PlayerReady = FName(TEXT("InProgress_PlayerReady"));   // Positioning
	const FName InProgress_RunningTrng = FName(TEXT("InProgress_RunningTrng"));	// running scenario
	const FName InProgress_PreResult = FName(TEXT("InProgress_PreResult"));	// pre result, small space 
	const FName InProgress_Result = FName(TEXT("InProgress_Result"));	// result
	const FName WaitingPostMatch = FName(TEXT("WaitingPostMatch"));  // Unreal Basic
	const FName LeavingMap = FName(TEXT("LeavingMap"));                // Unreal Basic
	const FName Aborted = FName(TEXT("Aborted"));                       // Unreal Basic
}

RtCommon::RtCommon()
{
}

RtCommon::~RtCommon()
{
}
