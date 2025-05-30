// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/RtWebCommon.h"

ERtWebProtocolType FResponse_Result::Get_Protocol(const FString& InStr)
{
	if (!FCString::IsNumeric(*InStr)) {
		return ERtWebProtocolType::WebError;
	}
	int32 temp = FCString::Atoi(*InStr);
	return (ERtWebProtocolType)temp;
}

ERtWebProtocolType FResponse_Result::Get_Error(const FString& InStr)
{
	int32 temp = FCString::Atoi(*InStr);
	return (ERtWebProtocolType)temp;
}

void FResponse_Result::Set_ProtocolType(const FString& InStr)
{
	ProtocolType = Get_Protocol(InStr);
}

void FResponse_Result::Set_ErrorType(const FString& InStr)
{

	ErrorType = Get_Error(InStr);
}

void FResponse_Result::Set_Type(const FString& InProtocol, const FString& InError)
{
	ProtocolType = Get_Protocol(InProtocol);
	ErrorType = Get_Error(InError);
}
