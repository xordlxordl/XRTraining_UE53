// Copyright 2018-2020 David Romanski(Socke). All Rights Reserved.

#include "RealTimeImportMesh.h"


URealTimeImportMesh* URealTimeImportMesh::realTimeImportMesh;
URealTimeImportMesh::URealTimeImportMesh(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	realTimeImportMesh = this;

	
	STL = NewObject<URealTimeImportMeshSTL>(URealTimeImportMeshSTL::StaticClass());
	STL->AddToRoot();

	OBJ = NewObject<URealTimeImportMeshOBJ>(URealTimeImportMeshOBJ::StaticClass());
	OBJ->AddToRoot();

	FBX = NewObject<URealTimeImportMeshFBX>(URealTimeImportMeshFBX::StaticClass());
	FBX->AddToRoot();
}


URealTimeImportMesh* URealTimeImportMesh::getRealTimeImportMesh() {
	return realTimeImportMesh;
}

void URealTimeImportMesh::LoadMeshFile(ERTIMeshType fileType, ERTIDirectoryType directoryType, FString filePath, bool& success, FString& successMessage,
	TArray<FRTIModelStruct>& modelStructs,
	ERTICoordinateSystem coordinateSystem,
	bool calculateTangents,
	bool cacheTexture,
	bool autoDetectionNormalMap,
	bool useSRGB,
	bool createMipMaps,
	ERTIERGBFormat rgbFormat) {

	//vertices.Empty();
	//normals.Empty();
	//UV0.Empty();
	modelStructs.Empty();

	FString dir;
	if (directoryType == ERTIDirectoryType::E_ad) {
		dir = FPaths::ConvertRelativePathToFull(filePath);
	}
	else {
		FString ProjectDir = FPaths::ProjectDir();
		dir = FPaths::ConvertRelativePathToFull(ProjectDir + filePath);
	}

	switch (fileType)
	{
	case ERTIMeshType::E_obj:

		OBJ->LoadOBJFile(
			dir,
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

		break;
	case ERTIMeshType::E_fbx:
	case ERTIMeshType::E_fbxUV:

		FBX->LoadFBXFile(
			fileType,
			dir,
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

		break;
	case ERTIMeshType::E_stl:
		TArray<uint8> stlFileData;
		if (!FFileHelper::LoadFileToArray(stlFileData, *dir)) {
			UE_LOG(LogTemp, Error, TEXT("RealTimeImportPlugin: Can't load file: %s"), *dir);
			success = false;
			successMessage = "Can't load file: " + dir;
			return;
		}

		if (stlFileData.Num() < 100) {
			UE_LOG(LogTemp, Error, TEXT("RealTimeImportPlugin: Can't load file (File corrupted): %s"), *dir);
			success = false;
			successMessage = "Can't load file (File corrupted): " + dir;
			return;
		}

		FString stlFileType = byteArrayToFString(stlFileData, 0, 5);

		//is ASCII or Binary type?
		if (stlFileType.Equals("solid")) {
			STL->LoadSTL_ASCIIFile(dir,
				stlFileData,
				coordinateSystem,
				success,
				successMessage,
				modelStructs);
		}
		else {
			STL->LoadSTL_BinaryFile(dir,
				stlFileData,
				coordinateSystem,
				success,
				successMessage,
				modelStructs);
		}


		break;
	}
	
}

FRTIModelStruct URealTimeImportMesh::makeModelStruct(FString name){
	return  URealTimeImportMesh::realTimeImportMesh->makeModelStructNonStatic(name);
}

FRTIModelStruct URealTimeImportMesh::makeModelStructNonStatic(FString name){
	FRTIModelStruct* model = new FRTIModelStruct();
	model->structID = FGuid::NewGuid().ToString();

	if (name.IsEmpty() == false) {
		model->name = name;
	}
	else {
		model->name = FGuid::NewGuid().ToString();
	}

	modelStructCache.Add(model->structID, model);
	return *model;
}

FRTIMeshStruct URealTimeImportMesh::makeMeshStruct(FString meshName, TArray<FVector> vertices, TArray<int32> triangles, TArray<FVector> normals,
	TArray<FVector2D> UV0, UPrimitiveComponent* materialComponent){
	

	FRTIMeshStruct meshStruct = FRTIMeshStruct();
	
	if (triangles.Num() == 0) {
		return meshStruct;
	}

	if (materialComponent != nullptr) {
		UMaterialInterface* material = materialComponent->GetMaterial(0);
		if (material != nullptr){
			UMaterialInstanceDynamic* dynamicMaterial = materialComponent->CreateDynamicMaterialInstance(0, material);
			if (dynamicMaterial != nullptr && dynamicMaterial->TextureParameterValues.Num() > 0) {
				FRTIMaterialStruct materialStruct = FRTIMaterialStruct();
				FString materialName;// = FGuid::NewGuid().ToString();
				dynamicMaterial->GetName(materialName);
				materialStruct.materialName = materialName;
				for (int32 i = 0; i < dynamicMaterial->TextureParameterValues.Num(); i++) {
					FTextureParameterValue textureValue = dynamicMaterial->TextureParameterValues[i];
					if (textureValue.ParameterValue != nullptr) {
						if (UTexture2D* texture2d = Cast<UTexture2D>(textureValue.ParameterValue)) {
							FRTITextureStruct textureStruct = FRTITextureStruct();
							textureStruct.texture = texture2d;
							FString textureName;// = FGuid::NewGuid().ToString();
							texture2d->GetName(textureName);
							textureStruct.textureName = textureName;
							materialStruct.textures.Add(textureName, textureStruct);
						}
					}
				}
				meshStruct.hasMaterialData = true;
				meshStruct.materialData = materialStruct;
			}
		}
	}



	if (meshName.IsEmpty() == false) {
		meshStruct.geometryName = meshName;
	}
	else {
		meshStruct.geometryName = FGuid::NewGuid().ToString();
	}
	
	meshStruct.vertices = vertices;
	meshStruct.triangles = triangles;
	meshStruct.normals = normals;
	meshStruct.UV0 = UV0;
	

	return meshStruct;
}

void URealTimeImportMesh::addMeshStructToModelStruct(FRTIModelStruct modelStruct, FRTIMeshStruct meshStruct){
	modelStruct.meshStructs.Add(meshStruct);

	if (URealTimeImportMesh::realTimeImportMesh->modelStructCache.Find(modelStruct.structID) != nullptr) {
		FRTIModelStruct* m = *URealTimeImportMesh::realTimeImportMesh->modelStructCache.Find(modelStruct.structID);
		m->meshStructs.Add(meshStruct);
	}
}

void URealTimeImportMesh::saveModelsToFile(UObject* WorldContextObject,UMaterial* exportMaterial,ERTICoordinateSystem coordinateSystem,
	ERTIMeshType fileType, ERTIDirectoryType directoryType, FString filePath,
	bool& success, FString& errorMessage, TArray<FRTIModelStruct> modelStructs){

	if (fileType == ERTIMeshType::E_obj) {
		URealTimeImportMesh::realTimeImportMesh->OBJ->saveModelsToFile(WorldContextObject,exportMaterial, coordinateSystem, directoryType, filePath, success, errorMessage, modelStructs);
	}
}


//Copy from the procedural mesh plugin. See UKismetProceduralMeshLibrary::CalculateTangentsForMesh.
void URealTimeImportMesh::calculateTangents(TMap<FVector, TArray<int32>>& overlappingVertices, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector2D>& UVs, 
	TArray<FProcMeshTangent>& TangentsFull)
{
	// Number of triangles
	const int32 NumTris = Triangles.Num() / 3;
	// Number of verts
	const int32 NumVerts = Vertices.Num();

	// Map of vertex to triangles in Triangles array
	TMultiMap<int32, int32> VertToTriMap;
	// Map of vertex to triangles to consider for normal calculation
	TMultiMap<int32, int32> VertToTriSmoothMap;

	// Normal/tangents for each face
	TArray<FVector3f> FaceTangentX, FaceTangentY, FaceTangentZ;
	FaceTangentX.AddUninitialized(NumTris);
	FaceTangentY.AddUninitialized(NumTris);
	FaceTangentZ.AddUninitialized(NumTris);

	// Iterate over triangles
	for (int TriIdx = 0; TriIdx < NumTris; TriIdx++)
	{
		int32 CornerIndex[3];
		FVector3f P[3];

		for (int32 CornerIdx = 0; CornerIdx < 3; CornerIdx++)
		{
			// Find vert index (clamped within range)
			int32 VertIndex = FMath::Min(Triangles[(TriIdx * 3) + CornerIdx], NumVerts - 1);

			CornerIndex[CornerIdx] = VertIndex;
			P[CornerIdx] = (FVector3f)Vertices[VertIndex];

			// Find/add this vert to index buffer
			//RealtimePlugin This part replaced with a map. Is much faster
			TArray<int32> VertOverlaps = overlappingVertices.FindRef(Vertices[VertIndex]);
				

			// Remember which triangles map to this vert
			VertToTriMap.AddUnique(VertIndex, TriIdx);
			VertToTriSmoothMap.AddUnique(VertIndex, TriIdx);

			// Also update map of triangles that 'overlap' this vert (ie don't match UV, but do match smoothing) and should be considered when calculating normal
			for (int32 OverlapIdx = 0; OverlapIdx < VertOverlaps.Num(); OverlapIdx++)
			{
				// For each vert we overlap..
				int32 OverlapVertIdx = VertOverlaps[OverlapIdx];

				// Add this triangle to that vert
				VertToTriSmoothMap.AddUnique(OverlapVertIdx, TriIdx);

				// And add all of its triangles to us
				TArray<int32> OverlapTris;
				VertToTriMap.MultiFind(OverlapVertIdx, OverlapTris);
				for (int32 OverlapTriIdx = 0; OverlapTriIdx < OverlapTris.Num(); OverlapTriIdx++)
				{
					VertToTriSmoothMap.AddUnique(VertIndex, OverlapTris[OverlapTriIdx]);
				}
			}
		}

		// Calculate triangle edge vectors and normal
		const FVector3f Edge21 = P[1] - P[2];
		const FVector3f Edge20 = P[0] - P[2];
		const FVector3f TriNormal = (Edge21 ^ Edge20).GetSafeNormal();

		// If we have UVs, use those to calc 
		if (UVs.Num() == Vertices.Num())
		{
			const FVector2D T1 = UVs[CornerIndex[0]];
			const FVector2D T2 = UVs[CornerIndex[1]];
			const FVector2D T3 = UVs[CornerIndex[2]];

			FMatrix	ParameterToLocal(
				FPlane(P[1].X - P[0].X, P[1].Y - P[0].Y, P[1].Z - P[0].Z, 0),
				FPlane(P[2].X - P[0].X, P[2].Y - P[0].Y, P[2].Z - P[0].Z, 0),
				FPlane(P[0].X, P[0].Y, P[0].Z, 0),
				FPlane(0, 0, 0, 1)
			);

			FMatrix ParameterToTexture(
				FPlane(T2.X - T1.X, T2.Y - T1.Y, 0, 0),
				FPlane(T3.X - T1.X, T3.Y - T1.Y, 0, 0),
				FPlane(T1.X, T1.Y, 1, 0),
				FPlane(0, 0, 0, 1)
			);

			// Use InverseSlow to catch singular matrices.  Inverse can miss this sometimes.
			const FMatrix TextureToLocal = ParameterToTexture.Inverse() * ParameterToLocal;

			FaceTangentX[TriIdx] = FVector4f(TextureToLocal.TransformVector(FVector(1, 0, 0)).GetSafeNormal());
			FaceTangentY[TriIdx] = FVector4f(TextureToLocal.TransformVector(FVector(0, 1, 0)).GetSafeNormal());
		}
		else
		{
			FaceTangentX[TriIdx] = Edge20.GetSafeNormal();
			FaceTangentY[TriIdx] = (FaceTangentX[TriIdx] ^ TriNormal).GetSafeNormal();
		}

		FaceTangentZ[TriIdx] = TriNormal;
	}


	// Arrays to accumulate tangents into
	TArray<FVector3f> VertexTangentXSum, VertexTangentYSum, VertexTangentZSum;
	VertexTangentXSum.AddZeroed(NumVerts);
	VertexTangentYSum.AddZeroed(NumVerts);
	VertexTangentZSum.AddZeroed(NumVerts);

	// For each vertex..
	for (int VertxIdx = 0; VertxIdx < Vertices.Num(); VertxIdx++)
	{
		// Find relevant triangles for normal
		TArray<int32> SmoothTris;
		VertToTriSmoothMap.MultiFind(VertxIdx, SmoothTris);

		for (int i = 0; i < SmoothTris.Num(); i++)
		{
			int32 TriIdx = SmoothTris[i];
			VertexTangentZSum[VertxIdx] += FaceTangentZ[TriIdx];
		}

		// Find relevant triangles for tangents
		TArray<int32> TangentTris;
		VertToTriMap.MultiFind(VertxIdx, TangentTris);

		for (int i = 0; i < TangentTris.Num(); i++)
		{
			int32 TriIdx = TangentTris[i];
			VertexTangentXSum[VertxIdx] += FaceTangentX[TriIdx];
			VertexTangentYSum[VertxIdx] += FaceTangentY[TriIdx];
		}
	}

	// Finally, normalize tangents and build output arrays
	TArray<FProcMeshTangent> Tangents;
	Tangents.AddUninitialized(NumVerts);

	for (int VertxIdx = 0; VertxIdx < NumVerts; VertxIdx++)
	{
		FVector3f& TangentX = VertexTangentXSum[VertxIdx];
		FVector3f& TangentY = VertexTangentYSum[VertxIdx];
		FVector3f& TangentZ = VertexTangentZSum[VertxIdx];

		TangentX.Normalize();
		TangentZ.Normalize();

		// Use Gram-Schmidt orthogonalization to make sure X is orth with Z
		TangentX -= TangentZ * (TangentZ | TangentX);
		TangentX.Normalize();

		// See if we need to flip TangentY when generating from cross product
		const bool bFlipBitangent = ((TangentZ ^ TangentX) | TangentY) < 0.f;

		Tangents[VertxIdx] = FProcMeshTangent((FVector)TangentX, bFlipBitangent);
	}

	TangentsFull.Append(Tangents);
	Tangents.Empty();
}


uint32 URealTimeImportMesh::byteArrayToInt32(TArray<uint8>& data, uint32 start){
	uint32 val = 0;
	FMemory::Memcpy(&val, data.GetData() + start, sizeof(val));

	return val;
}

int64 URealTimeImportMesh::byteArrayToInt64(TArray<uint8>& data, uint32 start) {
	int64 val = 0;
	FMemory::Memcpy(&val, data.GetData() + start, sizeof(val));

	return val;
}

TArray<int32> URealTimeImportMesh::byteArrayToInt32Tarray(TArray<uint8>& data, uint32 start, uint32 size){
	TArray<int32> a;
	a.AddUninitialized(size);
	FMemory::Memcpy(a.GetData(), data.GetData() + start, size);
	return a;
}


FString URealTimeImportMesh::byteArrayToFString(TArray<uint8>& data, uint32 start, uint32 size){

	TArray<uint8> fbxCheck;
	fbxCheck.AddUninitialized(size);
	FMemory::Memcpy(fbxCheck.GetData(), data.GetData()+start, size);
	fbxCheck.Add(0x00);// null-terminator

	return FString(UTF8_TO_TCHAR(fbxCheck.GetData()));
}

TArray<uint8> URealTimeImportMesh::cutByteArray(TArray<uint8>& original, uint32 start, uint32 size){
	TArray<uint8> copy;
	copy.AddUninitialized(size);
	FMemory::Memcpy(copy.GetData(), original.GetData() + start, size);
	return copy;
}

double URealTimeImportMesh::parseBytesToDouble(TArray<uint8>& data, uint32 start){

	double val = 0.0;
	FMemory::Memcpy(&val, data.GetData()+start, sizeof(val));

	return val;
}

float URealTimeImportMesh::parseBytesToFloat(TArray<uint8>& data, uint32 start){
	float val = 0.0;
	FMemory::Memcpy(&val, data.GetData() + start, sizeof(val));

	return val;
}

//void URealTimeImportMesh::cacheModelStruct(FRTIModelStruct* modelStruct){
//	if (modelStruct == nullptr) {
//		return;
//	}
//
//	if (IsInGameThread()) {
//		modelStructsMap.Add(modelStruct->modelID, modelStruct);
//	}
//	else {
//		AsyncTask(ENamedThreads::GameThread, [modelStruct]() {
//			URealTimeImportMesh::getRealTimeImportMesh()->modelStructsMap.Add(modelStruct->modelID, modelStruct);
//		});
//	}
//	
//}



void URealTimeImportMesh::breakMeshStruct(FRTIMeshStruct meshStruct,
	FString& geometryName,
	TArray<int32>& triangles,
	TArray<FVector>& vertices,
	TArray<FVector>& normals,
	TArray<FVector2D>& UV0,
	TArray<FLinearColor>& vertexColors,
	TArray<FProcMeshTangent>& tangents,
	FRTIMaterialStruct& materialStruct) {

	geometryName = meshStruct.geometryName;
	triangles = meshStruct.triangles;
	vertices = meshStruct.vertices;
	normals = meshStruct.normals;
	UV0 = meshStruct.UV0;
	vertexColors = meshStruct.vertexColors;
	tangents = meshStruct.tangents;
	materialStruct = meshStruct.materialData;
}


void URealTimeImportMesh::breakMaterialStruct(FRTIMaterialStruct materialData,
	bool& hasTexture,
	FString& materialName,
	FString& textureName,
	TArray<FRTITextureStruct>& textures,
	FColor& ambient,
	FColor& diffuse,
	FColor& specular,
	FColor& emissive,
	FColor& transparent,
	FColor& reflection,
	float& specularExponent,
	float& dissolved,
	float& dissolvedInverted,
	float& emissiveFactor,
	float& ambientFactor,
	float& diffuseFactor,
	float& bumpFactor,
	float& transparencyFactor,
	float& displacementFactor,
	float& vectorDisplacementFactor,
	float& specularFactor,
	float& shininessExponent,
	float& reflectionFactor) {

	textures.Empty();

	hasTexture = !materialData.isEmpty;
	materialName = materialData.materialName;
	textureName = materialData.textureName;
	materialData.textures.GenerateValueArray(textures);

	ambient = materialData.ambient;
	diffuse = materialData.diffuse;
	specular = materialData.specular;
	emissive = materialData.emissive;
	transparent = materialData.transparent;
	reflection = materialData.reflection;
	specularExponent = materialData.specularExponent;
	dissolved = materialData.dissolved;
	dissolvedInverted = materialData.dissolvedInverted;
	emissiveFactor = materialData.emissiveFactor;
	ambientFactor = materialData.ambientFactor;
	diffuseFactor = materialData.diffuseFactor;
	bumpFactor = materialData.bumpFactor;
	transparencyFactor = materialData.transparencyFactor;
	displacementFactor = materialData.displacementFactor;
	vectorDisplacementFactor = materialData.vectorDisplacementFactor;
	specularFactor = materialData.specularFactor;
	shininessExponent = materialData.shininessExponent;
	reflectionFactor = materialData.reflectionFactor;

}


void URealTimeImportMesh::breakTextureStruct(FRTITextureStruct textureData, FString& textureName, UTexture2D*& texture, ERTITextureType& textureType) {
	textureName = textureData.textureName;
	texture = textureData.texture;
	textureType = textureData.textureType;
}

void URealTimeImportMesh::breakModelStruct(FRTIModelStruct modelStruct,
	int64& modelID,FString& name, FTransform& relativeTransform, TArray<FRTIMeshStruct>& meshStructs, 
	bool& hasParent, int64& parentModelID, TArray<int64>& childrenModelIDs,ERTIModelType& modelType) {

	meshStructs.Empty();
	childrenModelIDs.Empty();

	modelID = modelStruct.modelID;
	meshStructs = modelStruct.meshStructs;
	name = modelStruct.name;
	relativeTransform = modelStruct.relativeTransform;
	parentModelID = modelStruct.parent;
	childrenModelIDs = modelStruct.children;
	modelType = modelStruct.modelType;

	if (parentModelID == -1) {
		hasParent = false;
	}
	else {
		hasParent = true;
	}

}
