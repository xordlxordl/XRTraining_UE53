// Fill out your copyright notice in the Description page of Project Settings.


#include "RtGridLib.h"

void URtGridLib::CreateSquareGrid(UProceduralMeshComponent* GridMesh, float TileSize, int GridX, int GridY, float offsetX, float offsetY)
{
	TArray<FVector2D> uvs;
	TArray<FLinearColor> vertexColors;
	TArray<FProcMeshTangent> tangents;
	TArray<FVector> normals;

	TArray<FVector> SquareVertices;
	TArray<int32> SquareTriangles;

	FVector ThicknessDirection;

	float LineThickness = 10;
	float HalfThickness = LineThickness * 0.5f;
	ThicknessDirection.Set(1.0f, 0.0f, 0.0f);

	for (int i = 0; i < GridX + 1; i++)
	{
		FVector StartVector;
		FVector EndVector;
		TArray<int32> IntigerArray;

		StartVector.Set(offsetX + i * TileSize, offsetY + GridY * TileSize, 0);
		EndVector.Set(offsetX + i * TileSize, offsetY, 0);

		IntigerArray.Add(SquareVertices.Num() + 0);
		IntigerArray.Add(SquareVertices.Num() + 1);
		IntigerArray.Add(SquareVertices.Num() + 3);
		IntigerArray.Add(SquareVertices.Num() + 0);
		IntigerArray.Add(SquareVertices.Num() + 3);
		IntigerArray.Add(SquareVertices.Num() + 2);

		SquareTriangles.Append(IntigerArray); // Triangle Clear

		FVector AddVector1 = StartVector - (ThicknessDirection * HalfThickness);
		SquareVertices.Add(AddVector1);

		FVector AddVector2 = StartVector + (ThicknessDirection * HalfThickness);
		SquareVertices.Add(AddVector2);

		FVector AddVector3 = EndVector - (ThicknessDirection * HalfThickness);
		SquareVertices.Add(AddVector3);

		FVector AddVector4 = EndVector + (ThicknessDirection * HalfThickness);
		SquareVertices.Add(AddVector4);

	}


	ThicknessDirection.Set(0.0f, 1.0f, 0.0f);

	for (int k = 0; k < GridY + 1; k++)
	{
		FVector StartVector;
		FVector EndVector;
		TArray<int32> IntigerArray;

		StartVector.Set(offsetX, offsetY + k * TileSize, 0);
		EndVector.Set(offsetX + GridX * TileSize, offsetY + k * TileSize, 0);

		//IntigerArray.Add(LineVertices.Num() + 0);
		//IntigerArray.Add(LineVertices.Num() + 2);
		//IntigerArray.Add(LineVertices.Num() + 3);
		//IntigerArray.Add(LineVertices.Num() + 0);
		//IntigerArray.Add(LineVertices.Num() + 3);
		//IntigerArray.Add(LineVertices.Num() + 1);
		IntigerArray.Add(SquareVertices.Num() + 0);
		IntigerArray.Add(SquareVertices.Num() + 1);
		IntigerArray.Add(SquareVertices.Num() + 3);
		IntigerArray.Add(SquareVertices.Num() + 0);
		IntigerArray.Add(SquareVertices.Num() + 3);
		IntigerArray.Add(SquareVertices.Num() + 2);


		SquareTriangles.Append(IntigerArray); // Triangle Clear

		FVector AddVector1 = StartVector + (ThicknessDirection * HalfThickness);
		SquareVertices.Add(AddVector1);
		FVector AddVector2 = EndVector + (ThicknessDirection * HalfThickness);
		SquareVertices.Add(AddVector2);
		FVector AddVector3 = StartVector - (ThicknessDirection * HalfThickness);
		SquareVertices.Add(AddVector3);
		FVector AddVector4 = EndVector - (ThicknessDirection * HalfThickness);
		SquareVertices.Add(AddVector4);

	}

	uvs.Init(FVector2D(0.0f, 0.0f), SquareVertices.Num());
	normals.Init(FVector(0.0f, 0.0f, 1.0f), SquareVertices.Num());
	vertexColors.Init(FLinearColor::Green, SquareVertices.Num());
	//tangents.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), LineVertices.Num());

	GridMesh->CreateMeshSection_LinearColor(0, SquareVertices, SquareTriangles, normals, uvs, vertexColors, tangents, true);

}
