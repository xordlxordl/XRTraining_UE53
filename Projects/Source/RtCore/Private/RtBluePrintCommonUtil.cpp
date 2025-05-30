// Fill out your copyright notice in the Description page of Project Settings.


#include "RtBluePrintCommonUtil.h"

#include "AssetRegistry/AssetRegistryModule.h"

#include "HAL/FileManagerGeneric.h"

#include "RtDefines.h"

#include <windows.h>
#include <shellapi.h>


// 콤보 인스턴스와 콤보 옵션을 넣으면 옵션 색인이 나온다.
// ** 언리얼에서 콤보 값 변경 결과가 오직 스트링으로만 나와서 만들었다.
int URtBluePrintCommonUtil::GetComboIndexFromString(UComboBoxString* pComboBox, FString comboValue)
{
	if (NULL != pComboBox)
	{
		int count = pComboBox->GetOptionCount();
		int counter = 0;

		for (int i = 0; i < count; i++)
		{
			if (pComboBox->GetOptionAtIndex(i) == comboValue)
			{
				return i;
			}
		}
	}
	return -1;
}

//ERtComponentComboType URtBluePrintCommonUtil::ConvertIndexToEnum_ComponentComboType(ERtComponentComboType comboStart, int comboIndex)
//{
//	return (ERtComponentComboType)((int)comboStart + comboIndex);
//}
//
//ERtTctComponentEleType URtBluePrintCommonUtil::ConvertIndexToEnum_ComponentEleType(ERtTctComponentEleType comboStart, int comboIndex)
//{
//	return (ERtTctComponentEleType)((int)comboStart + comboIndex);
//}

template<typename EnumType>
EnumType URtBluePrintCommonUtil::GetComboTypeFromString(UComboBoxString* pComboBox, EnumType comboStart, FString comboValue)
{
	if (NULL != pComboBox)
	{
		int count = pComboBox->GetOptionCount();
		int counter = 0;

		for (int i = 0; i < count; i++)
		{
			if (pComboBox->GetOptionAtIndex(i) == comboValue)
			{
				return (EnumType)((int)comboStart + i);
			}
		}
	}

	return EnumType::ENone;
}

ERtComponentComboType URtBluePrintCommonUtil::GetComponentComboTypeFromString(UComboBoxString* pComboBox, ERtComponentComboType comboStart, FString comboValue)
{
	return GetComboTypeFromString<ERtComponentComboType>(pComboBox, comboStart, comboValue);
}

ERtTctComponentEleType URtBluePrintCommonUtil::GetComponentEleTypeFromString(UComboBoxString* pComboBox, ERtTctComponentEleType comboStart, FString comboValue)
{
	return GetComboTypeFromString<ERtTctComponentEleType>(pComboBox, comboStart, comboValue);
}

//
// 애니메이션 애셋 이름 : AnimSequence
// 스테틱 매쉬 애셋 이름 : StaticMesh
// 스켈레톤 매쉬 애셋 이름 : SkeletalMesh
//
TArray<FString> URtBluePrintCommonUtil::SearchContentFolderFiles(TArray<FString> basePathList, TArray<FString> assetFilter, TArray<FString> exceptNameList)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FString> AssetPathList;

	for(const FString& item : basePathList)
	{
		TArray<FAssetData> AssetDataList;
		AssetRegistry.GetAssetsByPath(*item, AssetDataList, true, false); // TEXT("/Game/ContentFolder")
		for (const FAssetData& AssetData : AssetDataList)
		{
			FString addString; 

			for (const FString& filterData : assetFilter)
			{
				if (AssetData.AssetClassPath.GetAssetName().ToString() == filterData)
				{
					if( filterData == TEXT("AnimSequence") )
					{
						// 애니메이션은 따로 이름만 받아 블루프린트에서 처리하고 있어서 이렇게 함.
						addString = AssetData.AssetName.ToString(); 
					}
					else
					{
						bool bExcept = false;
						if( exceptNameList.Num() > 0 )
						{
							for (auto& exceptItem : exceptNameList)
							{
								if ( exceptItem == AssetData.AssetName.ToString() )
								{
									bExcept = true;
									break;
								}
							}
						}
						
						if( !bExcept )
						{
							addString = FString::Printf(TEXT("%s.%s"), *AssetData.PackageName.ToString(), *AssetData.AssetName.ToString());
						}
					}

					break;
				}
			}

			if(!addString.IsEmpty())
			{
				AssetPathList.Add(addString);
			}
		}
	}

	AssetPathList.Sort(
		[](const FString& A, const FString& B) {
			return A < B;
		}
	);


	return AssetPathList;
}

// 일반 폴더의 파일들을 찾아온다.
bool URtBluePrintCommonUtil::FindAllFileList(const FString& folderPath, TArray<FString>& outFileList)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FString FinalFolderPath = FPaths::ConvertRelativePathToFull(folderPath);

	if (PlatformFile.DirectoryExists(*FinalFolderPath))
	{
		FString SearchPath = FinalFolderPath + TEXT("/*.*");

		FFileManagerGeneric::Get().FindFiles(outFileList, *SearchPath, true, false);

		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("폴더를 찾을 수 없습니다."));
	}

	return false;
}

FString URtBluePrintCommonUtil::RemoveLastFolder(const FString& srcPath)
{
	FString tempDir = srcPath.Replace(TEXT("\\"), TEXT("/"));

	int32 lastIndex = 0;
	//APP_LOG("RemoveLastFolder 0 ~ %s", *tempDir);
	//CUSTOM_FILE_APP_LOG("RemoveLastFolder 0 ~ %s", *tempDir);
	if (tempDir.FindLastChar('/', lastIndex))
	{
		if (tempDir.Len() - 1 == lastIndex)
		{
			tempDir = tempDir.Left(lastIndex);
			//APP_LOG("RemoveLastFolder 1 ~ %s", *tempDir);
			//CUSTOM_FILE_APP_LOG("RemoveLastFolder 1 ~ %s", *tempDir);
			if (tempDir.FindLastChar('/', lastIndex))
			{
				tempDir = tempDir.Left(lastIndex + 1);
				//APP_LOG("RemoveLastFolder 2 ~ %s", *tempDir);
				//CUSTOM_FILE_APP_LOG("RemoveLastFolder 2 ~ %s", *tempDir);
			}
		}
		else
		{
			tempDir = tempDir.Left(lastIndex + 1);
			//APP_LOG("RemoveLastFolder 11 ~ %s", *tempDir);
			//CUSTOM_FILE_APP_LOG("RemoveLastFolder 11 ~ %s", *tempDir);
		}
	}

	return tempDir;
}

void URtBluePrintCommonUtil::WriteToFileLog(const FString& LogFileName, const FString& LogMessage)
{
	//FString temp = FString::Printf(TEXT("Logs/%s"), *LogFileName);
	FString baseDir;
#if WITH_EDITOR
	baseDir = FPaths::ProjectDir();
#else
	baseDir = FPaths::LaunchDir();
#endif

	baseDir = FPaths::ConvertRelativePathToFull(baseDir);

	// 파일 경로 및 이름 설정
	//FString LogFilePath = FPaths::Combine(baseDir, *LogFileName);
	FString LogFilePath = baseDir + LogFileName;

	// 파일 열기 또는 생성 (덮어쓰기)
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	IFileHandle* FileHandle = PlatformFile.OpenWrite(*LogFilePath, true);

	if (FileHandle)
	{
		// 로그 메시지 기록
		FString LogWithNewLine = LogMessage + TEXT("\r\n");
		FTCHARToUTF8 Convert(*LogWithNewLine);
		FileHandle->Write((const uint8*)Convert.Get(), Convert.Length() * sizeof(UTF8CHAR));
		// 파일 닫기
		delete FileHandle;
	}
}

void URtBluePrintCommonUtil::RunExternalExeFile(FString ExeFileName, const FString ExeFilePath)
{
	ShellExecute(NULL, TEXT("open"), *ExeFileName, NULL, *ExeFilePath, SW_SHOWNORMAL);
}

