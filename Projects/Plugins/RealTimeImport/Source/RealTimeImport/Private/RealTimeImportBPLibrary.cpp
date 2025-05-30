// Copyright 2018-2020 David Romanski(Socke). All Rights Reserved.

#include "RealTimeImportBPLibrary.h"


URealTimeImportBPLibrary* URealTimeImportBPLibrary::RealTimeImportBPLibrary;
URealTimeImportBPLibrary::URealTimeImportBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer){
	RealTimeImportBPLibrary = this;
	if (realTimeImportThread == nullptr) {
		realTimeImportThread = new FRealTimeImportThread();
	}
}

/*Delegate functions*/
void URealTimeImportBPLibrary::LoadFileEventDelegate(const int32 errorCode, const FString eventID, const TArray<uint8>& bytes , const FString fileName, const FString errorMessage) {}
void URealTimeImportBPLibrary::SaveFileEventDelegate(const int32 errorCode, const FString eventID, const FString fileName, const FString fullPath, const FString errorMessage) {}
void URealTimeImportBPLibrary::LoadAllFilesFromDirectoryEventDelegate(const FString eventID, const int32 &count, const  TArray<FString> &files, const TArray<FString> &filePaths) {}
void URealTimeImportBPLibrary::CreateSoundWaveFromFileEventDelegate(const int32 errorCode, const FString eventID, const USoundWaveProcedural* Sound, const FString fileName, const FString errorMessage) {}
void URealTimeImportBPLibrary::CreateSoundWaveFromBytesEventDelegate(const int32 errorCode, const FString eventID, const USoundWaveProcedural* Sound, const FString errorMessage) {}
void URealTimeImportBPLibrary::PlaySoundEventDelegate(const int32 errorCode, const FString fileID, const FString errorMessage, const float durationInSeconds){}
void URealTimeImportBPLibrary::ActiveSoundPositionEventDelegate(const FString fileID, const float positionSecondsStart, const float positionSecondsEnd, const float positionPercent){}
void URealTimeImportBPLibrary::ActiveSoundEnvelopeValueEventDelegate(const float averageEnvelopeValue, const float maxEnvelope, const int32 numWaveInstances){}
void URealTimeImportBPLibrary::StopSoundEventDelegate(const FString fileID){}
void URealTimeImportBPLibrary::LoadImageFileEventDelegate(const int32 errorCode, const FString eventID, const UTexture2D* texture, const FString fileName, const FString errorMessage) {}
void URealTimeImportBPLibrary::LoadImageBytesEventDelegate(const int32 errorCode, const FString eventID, const UTexture2D* texture, const FString errorMessage) {}

URealTimeImportBPLibrary * URealTimeImportBPLibrary::getRealTimeImportTarget() {
	return RealTimeImportBPLibrary;
}

void URealTimeImportBPLibrary::CreateSoundWaveFromFile(ERTIDirectoryType directoryType, FString filePath, float maxFileSize, FString& fileID, ERTISoundFileType soundFileType, USoundWaveProcedural*& soundWave, FString& fileName, int32& errorCode, FString& errorMessage) {
	URealTimeImportSound::getRealTimeImportSound()->CreateSoundWaveFromFile(directoryType, filePath, maxFileSize, fileID, soundFileType, soundWave, fileName, errorCode, errorMessage);
}

URealTimeImportBPLibrary* URealTimeImportBPLibrary::CreateSoundWaveFromFileAsync(ERTIDirectoryType directoryType, FString filePath, float maxFileSize, ERTISoundFileType soundFileType, FString eventID) {
	URealTimeImportSound::getRealTimeImportSound()->CreateSoundWaveFromFileAsync(directoryType, filePath, maxFileSize, soundFileType, eventID);
	return RealTimeImportBPLibrary;
}

void URealTimeImportBPLibrary::CreateSoundWaveFromBytes(FString name, TArray<uint8> bytes, float maxFileSize, ERTISoundFileType soundFileType, USoundWaveProcedural*& Sound, int32& errorCode, FString& errorMessage) {
	URealTimeImportSound::getRealTimeImportSound()->CreateSoundWaveFromBytes(name, bytes, maxFileSize, soundFileType, Sound, errorCode, errorMessage);
}

URealTimeImportBPLibrary* URealTimeImportBPLibrary::CreateSoundWaveFromBytesAsync(FString name, TArray<uint8> bytes, float maxFileSize, ERTISoundFileType soundFileType, FString eventID) {
	URealTimeImportSound::getRealTimeImportSound()->CreateSoundWaveFromBytesAsync(name, bytes, maxFileSize, soundFileType, eventID);
	return RealTimeImportBPLibrary;
}

void URealTimeImportBPLibrary::removeSoundFromCache(FString fileID) {
	URealTimeImportSound::getRealTimeImportSound()->removeSoundFromCache(fileID);
}

void URealTimeImportBPLibrary::emptySoundCache() {
	URealTimeImportSound::getRealTimeImportSound()->emptySoundCache();
}

void URealTimeImportBPLibrary::loadAndPlaySoundFileAsync(const UObject* WorldContextObject, ERTIDirectoryType directoryType, FString filePath,
	float maxFileSize, float volumeMultiplier, float playSpeed, int32 startTimeInSeconds , bool persistAcrossLevelTransition, ERTISoundFileType soundFileType) {
	URealTimeImportSound::getRealTimeImportSound()->loadAndPlaySoundFileAsync(WorldContextObject, directoryType, filePath, maxFileSize, volumeMultiplier, playSpeed, startTimeInSeconds, persistAcrossLevelTransition, soundFileType);
}

void URealTimeImportBPLibrary::loadAndPlaySoundBytesAsync(const UObject* WorldContextObject, TArray<uint8> bytes, float volumeMultiplier, float playSpeed, int32 startTimeInSeconds, bool persistAcrossLevelTransition) {
	URealTimeImportSound::getRealTimeImportSound()->loadAndPlaySoundBytesAsync(WorldContextObject, bytes, volumeMultiplier, playSpeed, startTimeInSeconds, persistAcrossLevelTransition);
}

void URealTimeImportBPLibrary::streamWavFile(bool& success, FString& errorMessage, const UObject* WorldContextObject, ERTIDirectoryType directoryType, FString filePath, float volumeMultiplier, int32 startTimeInSeconds, bool persistAcrossLevelTransition) {
	URealTimeImportSound::getRealTimeImportSound()->streamWavFile(success, errorMessage, WorldContextObject, directoryType, filePath, volumeMultiplier, startTimeInSeconds, persistAcrossLevelTransition);
}

void URealTimeImportBPLibrary::seekWavFileStream(FString fileID, float positionInSeconds) {
	URealTimeImportSound::getRealTimeImportSound()->seekWavFileStream(fileID, positionInSeconds);
}

void URealTimeImportBPLibrary::pauseSound(FString fileID) {
	URealTimeImportSound::getRealTimeImportSound()->pauseSound(fileID);
}

void URealTimeImportBPLibrary::resumeSound(FString fileID) {
	URealTimeImportSound::getRealTimeImportSound()->resumeSound(fileID);
}

void URealTimeImportBPLibrary::stopSound(FString fileID, bool removeFromCache) {
	URealTimeImportSound::getRealTimeImportSound()->stopSound(fileID, removeFromCache);
}

void URealTimeImportBPLibrary::stopAllSounds(bool removeFromCache){
	URealTimeImportSound::getRealTimeImportSound()->stopAllSounds(removeFromCache);
}

void URealTimeImportBPLibrary::changeVolumeSound(FString fileID, float volumeMultiplier) {
	URealTimeImportSound::getRealTimeImportSound()->changeVolumeSound(fileID, volumeMultiplier);
};

bool URealTimeImportBPLibrary::isSoundActive(FString fileID) {
	return URealTimeImportSound::getRealTimeImportSound()->isSoundActive(fileID);
};

void URealTimeImportBPLibrary::getSoundInfo(ERTIDirectoryType directoryType, FString filePath, FString& name, FString& fileNameExtension, int64& fileSize, float& duration,
	int64& numChannels, int32& quality, int64& sampleDataSize, int64& sampleRate) {
	
	URealTimeImportSound::getRealTimeImportSound()->getSoundInfo(directoryType, filePath, name, fileNameExtension, fileSize, duration,
		numChannels, quality, sampleDataSize, sampleRate);
}
UTexture2DDynamic* URealTimeImportBPLibrary::texture2DToTexture2DDynamic(UTexture2D* texture2D, bool destroyTexture2D){
	return URealTimeImportImage::getRealTimeImportImage()->texture2DToTexture2DDynamic(texture2D, destroyTexture2D);
};





void URealTimeImportBPLibrary::LoadImageFile(ERTIDirectoryType directoryType, FString filePath,
	UTexture2D*& texture, FString& fileName, int32& errorCode, FString& errorMessage,
	TextureCompressionSettings compressionSettings, bool cacheTexture, bool SRGB, bool createMipMaps, ERTIERGBFormat rgbFormat) {

	URealTimeImportImage::getRealTimeImportImage()->LoadImageFile(directoryType, filePath, texture, fileName, errorCode, errorMessage,
		cacheTexture, compressionSettings, SRGB, createMipMaps,rgbFormat);
};

URealTimeImportBPLibrary* URealTimeImportBPLibrary::LoadImageFileAsync(ERTIDirectoryType directoryType, FString filePath, FString eventID,
	TextureCompressionSettings compressionSettings, bool SRGB, bool createMipMaps, ERTIERGBFormat rgbFormat) {

	URealTimeImportImage::getRealTimeImportImage()->LoadImageFileAsync(directoryType, filePath, eventID, compressionSettings, SRGB, createMipMaps,rgbFormat);
	return RealTimeImportBPLibrary;
};

void URealTimeImportBPLibrary::LoadImageBytes(FString name, TArray<uint8> bytes, UTexture2D*& texture, int32& errorCode, FString& errorMessage,
	TextureCompressionSettings compressionSettings, bool cacheTexture, bool SRGB, bool createMipMaps, ERTIERGBFormat rgbFormat) {

	URealTimeImportImage::getRealTimeImportImage()->LoadImageBytes(name, bytes, texture, errorCode, errorMessage,cacheTexture, compressionSettings, SRGB, createMipMaps,rgbFormat);
};

URealTimeImportBPLibrary* URealTimeImportBPLibrary::LoadImageBytesAsync(FString name, TArray<uint8> bytes, FString eventID,
	TextureCompressionSettings compressionSettings, bool SRGB, bool createMipMaps, ERTIERGBFormat rgbFormat) {

	URealTimeImportImage::getRealTimeImportImage()->LoadImageBytesAsync(name, bytes, eventID, compressionSettings, SRGB, createMipMaps,rgbFormat);
	return RealTimeImportBPLibrary;
};

void URealTimeImportBPLibrary::removeTextureFromCacheAndDestroyTexture(UTexture* texture, bool emptyTextureBeforeDeleting) {
	URealTimeImportImage::getRealTimeImportImage()->removeTextureFromCacheAndDestroyTexture(texture, emptyTextureBeforeDeleting);
};

void URealTimeImportBPLibrary::emptyTextureCacheAndDestroyTextures(bool emptyTexturesBeforeDeleting) {
	URealTimeImportImage::getRealTimeImportImage()->emptyTextureCacheAndDestroyTextures(emptyTexturesBeforeDeleting);
}

void URealTimeImportBPLibrary::changeTexturDestroyThreadSettings(float threadSleepTimeInSeconds, bool showLogs){
	URealTimeImportImage::getRealTimeImportImage()->changeTexturDestroyThreadSettings(threadSleepTimeInSeconds, showLogs);
}

TArray<uint8> URealTimeImportBPLibrary::textureRenderTarget2D_ToByteArray(bool& success, FString& errorMessage, UTextureRenderTarget2D* textureRenderTarget2D,
	ERTIExportImageType type, int32 quality) {

	return URealTimeImportImage::textureRenderTarget2D_ToByteArray(success, errorMessage, textureRenderTarget2D, type, quality);
};


void URealTimeImportBPLibrary::LoadMeshFile(ERTIMeshType fileType, ERTIDirectoryType directoryType,	FString filePath,
	bool& success,	FString& successMessage, TArray<FRTIModelStruct>& modelStructs,
	ERTICoordinateSystem coordinateSystem, bool calculateTangents,bool cacheTexture, bool autoDetectionNormalMap, bool useSRGB, bool createMipMaps, ERTIERGBFormat rgbFormat) {

	URealTimeImportMesh::getRealTimeImportMesh()->LoadMeshFile(fileType, directoryType, filePath, success, successMessage, modelStructs,
		coordinateSystem, calculateTangents, cacheTexture, autoDetectionNormalMap, useSRGB, createMipMaps,rgbFormat);
};


void URealTimeImportBPLibrary::breakModelStruct(FRTIModelStruct modelStruct,int64& modelID, FString& name, FTransform& relativeTransform,
	TArray<FRTIMeshStruct>& meshStructs,bool& hasParent, int64& parentModelID, TArray<int64>& childrenModelIDs, ERTIModelType& modelType) {

	URealTimeImportMesh::getRealTimeImportMesh()->breakModelStruct(modelStruct,modelID, name, relativeTransform, meshStructs,
		hasParent, parentModelID, childrenModelIDs,modelType);
};

void URealTimeImportBPLibrary::breakMeshStruct(FRTIMeshStruct meshStruct,FString& geometryName,TArray<FVector>& vertices,TArray<int32>& triangles,
	TArray<FVector>& normals,TArray<FVector2D>& UV0, TArray<FLinearColor>& vertexColors,TArray<FProcMeshTangent>& tangents, FRTIMaterialStruct& materialStruct) {

	URealTimeImportMesh::getRealTimeImportMesh()->breakMeshStruct(meshStruct, geometryName, triangles, vertices, normals, UV0,vertexColors,tangents, materialStruct);
};

void URealTimeImportBPLibrary::breakMaterialStruct(FRTIMaterialStruct materialData,	bool& hasTexture,FString& materialName,	FString& textureName,
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

	URealTimeImportMesh::getRealTimeImportMesh()->breakMaterialStruct(materialData, hasTexture, materialName, textureName, textures,
		ambient,
		diffuse,
		specular,
		emissive,
		transparent,
		reflection,
		specularExponent,
		dissolved,
		dissolvedInverted,
		emissiveFactor,
		ambientFactor,
		diffuseFactor,
		bumpFactor,
		transparencyFactor,
		displacementFactor,
		vectorDisplacementFactor,
		specularFactor,
		shininessExponent,
		reflectionFactor);
};

void URealTimeImportBPLibrary::breakTextureStruct(FRTITextureStruct textureData,FString& textureName,	UTexture2D*& texture,	ERTITextureType& textureType) {
	URealTimeImportMesh::getRealTimeImportMesh()->breakTextureStruct(textureData,textureName, texture, textureType);
}
TArray<FVector> URealTimeImportBPLibrary::Multiply_VectorArrayFloat(TArray<FVector> A, float B){
	for (int32 i = 0; i < A.Num(); i++)
	{
		A[i] = A[i] * B;
	}
	return A;
}

void URealTimeImportBPLibrary::addJobToThread(FSRealtimeImportThreadJobData* jobData){
	URealTimeImportBPLibrary::getRealTimeImportTarget()->realTimeImportThread->addJob(jobData);
}



void URealTimeImportBPLibrary::LoadFile(ERTIDirectoryType directoryType, FString filePath, FString& fileName, TArray<uint8>& bytes, int32& errorCode, FString& errorMessage){
	UFileFunctionsRealTimeImport::getFileFunctionsRealTimeImport()->LoadFile(directoryType, filePath, fileName, bytes, errorCode, errorMessage);
}

URealTimeImportBPLibrary* URealTimeImportBPLibrary::LoadFileAsync(ERTIDirectoryType directoryType, FString filePath, FString eventID){
	UFileFunctionsRealTimeImport::getFileFunctionsRealTimeImport()->LoadFileAsync(directoryType, filePath, eventID);
	return this;
}

void URealTimeImportBPLibrary::SaveFile(ERTIDirectoryType directoryType, FString filePath, bool createDirectories, bool overwriteFile, TArray<uint8> bytes, FString& fileName, FString& fullPath, int32& errorCode, FString& errorMessage){
	UFileFunctionsRealTimeImport::getFileFunctionsRealTimeImport()->SaveFile(directoryType, filePath, createDirectories, overwriteFile, bytes, fileName, fullPath, errorCode, errorMessage);
}

URealTimeImportBPLibrary* URealTimeImportBPLibrary::SaveFileAsync(ERTIDirectoryType directoryType, FString filePath, bool createDirectories, bool overwriteFile, TArray<uint8> bytes, FString eventID){
	UFileFunctionsRealTimeImport::getFileFunctionsRealTimeImport()->SaveFileAsync(directoryType, filePath, createDirectories, overwriteFile, bytes, eventID);
	return this;
}

bool URealTimeImportBPLibrary::LoadAllFilesFromDirectory(ERTIDirectoryType directoryType, ERTIFileType fileType, FString dirPath, bool recursiveSearch, int32& count, TArray<FString>& files, TArray<FString>& filePaths){
	files.Empty();
	filePaths.Empty();
	return UFileFunctionsRealTimeImport::getFileFunctionsRealTimeImport()->LoadAllFilesFromDirectory(directoryType, fileType, dirPath, recursiveSearch, count, files, filePaths);
}

URealTimeImportBPLibrary* URealTimeImportBPLibrary::LoadAllFilesFromDirectoryAsync(ERTIDirectoryType directoryType, ERTIFileType fileType, FString dirPath, bool recursiveSearch, FString eventID){
	UFileFunctionsRealTimeImport::getFileFunctionsRealTimeImport()->LoadAllFilesFromDirectoryAsync(directoryType, fileType, dirPath, recursiveSearch, eventID);
	return this;
}



