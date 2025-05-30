// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h" // need headerfile for procedural Mesh Component
#include "RtGridLib.generated.h"

/**
 * 
 */
UCLASS()
class RTCORE_API URtGridLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	


public:

	/**
	*  액터의 프로지저럴 메시를 사각형으로 그려주는 함수입니다.
	*  블루 프린트에서 사용시 : 액터에 프로시저럴 메시 컴포넌트를 추가 하고 함수 호출하여 인수로 사용 
	* 
	*  코드에서 사용시 :  
	*  1. 헤더 파일에서 UProceduralMeshComponent* GridMesh; 선언 
	*  2. cpp 파일에서 프로시저럴 메시 생성   GridMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	*  
	*  @Param GridMesh : 사각형으로 바꾸고자 하는 프로시저럴 메시 컴포넌트
	*  @Param TileSize : 사각형의 기본 가로, 세로 크기 , 즉 , 정사각형 타일 1개의 길이  
	*  @Param GridX, GridY : 1 X 1 OR 2 X 2 OR 3 X 1 ... 등등 

	*/

	UFUNCTION(BlueprintCallable , Category = "Grid Manager")
	static void CreateSquareGrid(UProceduralMeshComponent* GridMesh, float TileSize, int GridX, int GridY, float offsetX, float offsetY);
};
