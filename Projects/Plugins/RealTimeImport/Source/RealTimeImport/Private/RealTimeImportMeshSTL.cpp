// Copyright 2018-2020 David Romanski(Socke). All Rights Reserved.

#include "RealTimeImportMeshSTL.h"


URealTimeImportMeshSTL::URealTimeImportMeshSTL(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

}


void URealTimeImportMeshSTL::LoadSTL_ASCIIFile(FString dir, TArray<uint8>& stlFileData, ERTICoordinateSystem coordinateSystem, bool& success, FString& successMessage,
	TArray<FRTIModelStruct>& modelStructs) {

	FString stringFileData = FString((char*)stlFileData.GetData());

	TArray<FString> lines;
	int32 lineCount = stringFileData.ParseIntoArray(lines, TEXT("\n"), true);
	stringFileData.Empty();
	stlFileData.Empty();

	if (lineCount < 1) {
		UE_LOG(LogTemp, Error, TEXT("RealTimeImportPlugin: corrupted file (1): %s"), *dir);
		success = false;
		successMessage = "Corrupted file (1): " + dir;
		return;
	}

	TArray<FVector> vertices;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;


	//we need a default meshstruct
	TArray<FRTIMeshStruct> meshStructs;
	FRTIMeshStruct defaultMesh;
	defaultMesh.geometryName = lines[0];
	defaultMesh.isDefaultStruct = true;
	meshStructs.Add(defaultMesh);

	for (int32 i = 1; i < lineCount; i++) {
		FString line = lines[i].TrimStartAndEnd();
		//UE_LOG(LogTemp, Display, TEXT("%s"), *line);
		if (line.StartsWith("vertex")) {
			TArray<FString> vertexLines;
			int32 vertexLineCount = line.ParseIntoArray(vertexLines, TEXT(" "), true);
			if (vertexLineCount != 4) {
				continue;
			}
			if (coordinateSystem == ERTICoordinateSystem::E_RightHanded) {
				vertices.Add(FVector(FCString::Atof(*vertexLines[1]), FCString::Atof(*vertexLines[3]), FCString::Atof(*vertexLines[2])));
			}
			else {
				vertices.Add(FVector(FCString::Atof(*vertexLines[1]), FCString::Atof(*vertexLines[2]), FCString::Atof(*vertexLines[3])));
			}

			continue;
		}
		if (line.StartsWith("facet normal")) {
			TArray<FString> normalLines;
			int32 normalLineCount = line.ParseIntoArray(normalLines, TEXT(" "), true);
			if (normalLineCount != 5) {
				continue;
			}
			FVector normal;
			if (coordinateSystem == ERTICoordinateSystem::E_RightHanded) {
				normal = FVector(FCString::Atof(*normalLines[2]), FCString::Atof(*normalLines[4]), FCString::Atof(*normalLines[3]));
			}
			else {
				normal = FVector(FCString::Atof(*normalLines[2]), FCString::Atof(*normalLines[3]), FCString::Atof(*normalLines[4]));
			}

			normals.Add(normal);
			normals.Add(normal);
			normals.Add(normal);
			continue;
		}
		if (line.StartsWith("endloop") && vertices.Num() % 3 == 0) {
			//polygon
			if (coordinateSystem == ERTICoordinateSystem::E_RightHanded) {
				meshStructs[0].triangles.Add(vertices.Num() - 1);
				meshStructs[0].triangles.Add(vertices.Num() - 3);
				meshStructs[0].triangles.Add(vertices.Num() - 2);
			}
			else {
				meshStructs[0].triangles.Add(vertices.Num() - 1);
				meshStructs[0].triangles.Add(vertices.Num() - 2);
				meshStructs[0].triangles.Add(vertices.Num() - 3);
			}

		}
	}


	for (int32 i = 0; i < meshStructs.Num(); i++) {
		meshStructs[i].vertices = vertices;
		meshStructs[i].normals = normals;
		meshStructs[i].UV0 = UV0;
	}


	if (vertices.Num() < 3) {
		UE_LOG(LogTemp, Error, TEXT("RealTimeImportPlugin: Less than 3 vertices found in file. Cancel: %s"), *dir);
		success = false;
		successMessage = "Less than 3 vertices found in file. Cancel: " + dir;
		return;
	}


	FRTIModelStruct defaultModelStruct;
	defaultModelStruct.meshStructs.Append(meshStructs);
	modelStructs.Add(defaultModelStruct);


	if (normals.Num() == 0 || normals.Num() != vertices.Num()) {
		UE_LOG(LogTemp, Warning, TEXT("RealTimeImportPlugin: No normals found in this file. Normals will be automatically created. This can lead to problems with the lighting.: %s"), *dir);
	}

	UE_LOG(LogTemp, Display, TEXT("RealTimeImportPlugin: Vertices: %i Triangles: %i Normals: %i "), vertices.Num(), meshStructs[0].triangles.Num(), normals.Num());

	success = true;
	successMessage = "Vertices: " + FString::FromInt(vertices.Num()) + " Triangles: " + FString::FromInt(meshStructs[0].triangles.Num()) + " Normals: " + FString::FromInt(normals.Num());


	lines.Empty();

	vertices.Empty();
	normals.Empty();
	UV0.Empty();
}


void URealTimeImportMeshSTL::LoadSTL_BinaryFile(FString dir, TArray<uint8>& stlFileData, ERTICoordinateSystem coordinateSystem, bool& success, FString& successMessage,
	TArray<FRTIModelStruct>& modelStructs) {

	TArray<FVector> vertices;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;

	//we need a default meshstruct
	TArray<FRTIMeshStruct> meshStructs;
	FRTIMeshStruct defaultMesh;
	defaultMesh.geometryName = FPaths::GetCleanFilename(dir);
	defaultMesh.isDefaultStruct = true;
	meshStructs.Add(defaultMesh);

	//uint32 polygonCount = byteArrayToInt32(stlFileData, 80);
	int32 dataPosition = 84;
	while ((dataPosition + 49) < stlFileData.Num()) {
		//normals
		float n1 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;
		float n2 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;
		float n3 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;


		//vertices
		float vA1 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;
		float vA2 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;
		float vA3 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;



		float vB1 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;
		float vB2 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;
		float vB3 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;



		float vC1 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;
		float vC2 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 4;
		float vC3 = URealTimeImportMesh::parseBytesToFloat(stlFileData, dataPosition);
		dataPosition += 6;


		if (coordinateSystem == ERTICoordinateSystem::E_RightHanded) {
			FVector normal = FVector(n1, n3, n2);
			normals.Add(normal);
			normals.Add(normal);
			normals.Add(normal);

			vertices.Add(FVector(vA1, vA3, vA2));
			vertices.Add(FVector(vB1, vB3, vB2));
			vertices.Add(FVector(vC1, vC3, vC2));
		}
		else {
			FVector normal = FVector(n1, n2, n3);
			normals.Add(normal);
			normals.Add(normal);
			normals.Add(normal);

			vertices.Add(FVector(vA1, vA2, vA3));
			vertices.Add(FVector(vB1, vB2, vB3));
			vertices.Add(FVector(vC1, vC2, vC3));
		}





		//polygon
		if (coordinateSystem == ERTICoordinateSystem::E_RightHanded) {
			if (vertices.Num() % 3 == 0) {
				meshStructs[0].triangles.Add(vertices.Num() - 1);
				meshStructs[0].triangles.Add(vertices.Num() - 3);
				meshStructs[0].triangles.Add(vertices.Num() - 2);
			}
		}
		else {
			if (vertices.Num() % 3 == 0) {
				meshStructs[0].triangles.Add(vertices.Num() - 1);
				meshStructs[0].triangles.Add(vertices.Num() - 2);
				meshStructs[0].triangles.Add(vertices.Num() - 3);
			}
		}


		//UE_LOG(LogTemp, Error, TEXT("RealTimeImportPlugin: n1:%f n2:%f n3:%f "), n1,n2,n3);
	}
	//UE_LOG(LogTemp, Error, TEXT("RealTimeImportPlugin: xxxxxxxxx %i"),stlFileData.Num());


	for (int32 i = 0; i < meshStructs.Num(); i++) {
		meshStructs[i].vertices = vertices;
		meshStructs[i].normals = normals;
		meshStructs[i].UV0 = UV0;
	}

	if (vertices.Num() < 3) {
		UE_LOG(LogTemp, Error, TEXT("RealTimeImportPlugin: Less than 3 vertices found in file. Cancel: %s"), *dir);
		success = false;
		successMessage = "Less than 3 vertices found in file. Cancel: " + dir;
		return;
	}

	FRTIModelStruct defaultModelStruct;
	defaultModelStruct.meshStructs.Append(meshStructs);
	modelStructs.Add(defaultModelStruct);

	if (normals.Num() == 0 || normals.Num() != vertices.Num()) {
		UE_LOG(LogTemp, Warning, TEXT("RealTimeImportPlugin: No normals found in this file. Normals will be automatically created. This can lead to problems with the lighting.: %s"), *dir);
	}

	UE_LOG(LogTemp, Display, TEXT("RealTimeImportPlugin: Vertices: %i Triangles: %i Normals: %i "), vertices.Num(), meshStructs[0].triangles.Num(), normals.Num());

	success = true;
	successMessage = "Vertices: " + FString::FromInt(vertices.Num()) + " Triangles: " + FString::FromInt(meshStructs[0].triangles.Num()) + " Normals: " + FString::FromInt(normals.Num());

	stlFileData.Empty();

	vertices.Empty();
	normals.Empty();
	UV0.Empty();
}