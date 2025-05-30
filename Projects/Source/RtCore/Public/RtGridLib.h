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
	*  ������ ���������� �޽ø� �簢������ �׷��ִ� �Լ��Դϴ�.
	*  ��� ����Ʈ���� ���� : ���Ϳ� ���ν����� �޽� ������Ʈ�� �߰� �ϰ� �Լ� ȣ���Ͽ� �μ��� ��� 
	* 
	*  �ڵ忡�� ���� :  
	*  1. ��� ���Ͽ��� UProceduralMeshComponent* GridMesh; ���� 
	*  2. cpp ���Ͽ��� ���ν����� �޽� ����   GridMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	*  
	*  @Param GridMesh : �簢������ �ٲٰ��� �ϴ� ���ν����� �޽� ������Ʈ
	*  @Param TileSize : �簢���� �⺻ ����, ���� ũ�� , �� , ���簢�� Ÿ�� 1���� ����  
	*  @Param GridX, GridY : 1 X 1 OR 2 X 2 OR 3 X 1 ... ��� 

	*/

	UFUNCTION(BlueprintCallable , Category = "Grid Manager")
	static void CreateSquareGrid(UProceduralMeshComponent* GridMesh, float TileSize, int GridX, int GridY, float offsetX, float offsetY);
};
