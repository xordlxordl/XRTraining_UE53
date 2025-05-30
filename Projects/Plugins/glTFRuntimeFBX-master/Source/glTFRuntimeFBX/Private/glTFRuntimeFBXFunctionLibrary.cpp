// Copyright 2023-2024 - Roberto De Ioris

#include "glTFRuntimeFBXFunctionLibrary.h"
#include "Runtime/Launch/Resources/Version.h"
#include "UObject/StrongObjectPtr.h"
THIRD_PARTY_INCLUDES_START
#include "ufbx.h"
THIRD_PARTY_INCLUDES_END

struct FglTFRuntimeFBXCacheData : FglTFRuntimePluginCacheData
{
	ufbx_scene* Scene = nullptr;

	TMap<uint32, ufbx_node*> NodesMap;
	TMap<FString, ufbx_node*> NodesNamesMap;

	~FglTFRuntimeFBXCacheData()
	{
		if (Scene)
		{
			ufbx_free_scene(Scene);
		}
	}

	TMap<ufbx_texture*, TStrongObjectPtr<UTexture2D>> TexturesCache;
	FCriticalSection TexturesLock;
};

namespace glTFRuntimeFBX
{
	FTransform GetTransform(UglTFRuntimeAsset* Asset, ufbx_transform FbxTransform)
	{
		FTransform Transform;

		Transform.SetLocation(FVector(FbxTransform.translation.x, FbxTransform.translation.y, FbxTransform.translation.z));
		Transform.SetRotation(FQuat(FbxTransform.rotation.x, FbxTransform.rotation.y, FbxTransform.rotation.z, FbxTransform.rotation.w));
		Transform.SetScale3D(FVector(FbxTransform.scale.x, FbxTransform.scale.y, FbxTransform.scale.z));


		return Asset->GetParser()->TransformTransform(Transform);
	}

	void FillSkeleton(UglTFRuntimeAsset* Asset, ufbx_skin_deformer* Skin, ufbx_node* Node, const int32 ParentIndex, TArray<FglTFRuntimeBone>& Skeleton, TMap<FString, int32>& BonesMap, bool& bRootProcessed)
	{

		FglTFRuntimeBone Bone;

		bool bHasBone = false;

		for (int32 ClusterIndex = 0; ClusterIndex < Skin->clusters.count; ClusterIndex++)
		{
			if (Skin->clusters.data[ClusterIndex]->bone_node == Node)
			{
				// as soon as we find a bone, we consider the root processed
				bRootProcessed = true;

				ufbx_matrix BoneMatrix = Skin->clusters.data[ClusterIndex]->bind_to_world;

				bool bFound = false;
				// now find the parent
				for (int32 ParentClusterIndex = 0; ParentClusterIndex < Skin->clusters.count; ParentClusterIndex++)
				{
					if (Skin->clusters.data[ParentClusterIndex]->bone_node == Skin->clusters.data[ClusterIndex]->bone_node->parent)
					{
						ufbx_matrix ParentBoneInverseMatrix = ufbx_matrix_invert(&Skin->clusters.data[ParentClusterIndex]->bind_to_world);
						BoneMatrix = ufbx_matrix_mul(&ParentBoneInverseMatrix, &BoneMatrix);
						bFound = true;
						break;
					}
				}

				if (!bFound)
				{
					ufbx_matrix ParentBoneInverseMatrix = ufbx_matrix_invert(&Skin->clusters.data[ClusterIndex]->bone_node->parent->node_to_world);
					BoneMatrix = ufbx_matrix_mul(&ParentBoneInverseMatrix, &BoneMatrix);
				}

				Bone.Transform = glTFRuntimeFBX::GetTransform(Asset, ufbx_matrix_to_transform(&BoneMatrix));
				bHasBone = true;
				break;
			}
		}

		if (!bHasBone)
		{
			if (bRootProcessed)
			{
				return;
			}
			Bone.Transform = glTFRuntimeFBX::GetTransform(Asset, Node->local_transform);
		}

		Bone.BoneName = UTF8_TO_TCHAR(Node->name.data);
		Bone.ParentIndex = ParentIndex;

		const int32 NewIndex = Skeleton.Add(Bone);

		BonesMap.Add(Skeleton[NewIndex].BoneName, NewIndex);

		for (ufbx_node* Child : Node->children)
		{
			FillSkeleton(Asset, Skin, Child, NewIndex, Skeleton, BonesMap, bRootProcessed);
		}
	}

	void FillNode(UglTFRuntimeAsset* Asset, ufbx_node* Node, FglTFRuntimeFBXNode& FBXNode)
	{
		FBXNode.Id = Node->element_id;
		FBXNode.Name = UTF8_TO_TCHAR(Node->name.data);
		FBXNode.Transform = GetTransform(Asset, Node->local_transform);
		FBXNode.bHasMesh = Node->mesh != nullptr;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> GetCacheData(UglTFRuntimeAsset* Asset)
	{
		if (Asset->GetParser()->PluginsCacheData.Contains("FBX"))
		{
			if (Asset->GetParser()->PluginsCacheData["FBX"]->bValid)
			{
				return StaticCastSharedPtr<FglTFRuntimeFBXCacheData>(Asset->GetParser()->PluginsCacheData["FBX"]);
			}
			else
			{
				return nullptr;
			}
		}

		TSharedRef<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = MakeShared<FglTFRuntimeFBXCacheData>();

		Asset->GetParser()->PluginsCacheData.Add("FBX", RuntimeFBXCacheData);

		ufbx_load_opts Options = {};
		Options.target_axes.right = UFBX_COORDINATE_AXIS_POSITIVE_X;
		Options.target_axes.up = UFBX_COORDINATE_AXIS_POSITIVE_Y;
		Options.target_axes.front = UFBX_COORDINATE_AXIS_POSITIVE_Z;
		Options.target_unit_meters = 1;
		Options.space_conversion = UFBX_SPACE_CONVERSION_MODIFY_GEOMETRY;

		ufbx_error Error;

		if (Asset->IsArchive())
		{
			TArray64<uint8> ArchiveBlob;
			for (const FString& Name : Asset->GetArchiveItems())
			{
				if (Name.EndsWith(".fbx"))
				{
					if (!Asset->GetParser()->GetBlobByName(Name, ArchiveBlob))
					{
						return nullptr;
					}

					break;
				}
			}

			if (ArchiveBlob.Num() > 0)
			{
				RuntimeFBXCacheData->Scene = ufbx_load_memory(ArchiveBlob.GetData(), ArchiveBlob.Num(), &Options, &Error);
			}
		}
		else
		{
			TArray64<uint8>& Blob = Asset->GetParser()->GetBlob();
			RuntimeFBXCacheData->Scene = ufbx_load_memory(Blob.GetData(), Blob.Num(), &Options, &Error);
		}

		if (!RuntimeFBXCacheData->Scene)
		{
			return nullptr;
		}

		for (int32 NodeIndex = 0; NodeIndex < RuntimeFBXCacheData->Scene->nodes.count; NodeIndex++)
		{
			ufbx_node* Node = RuntimeFBXCacheData->Scene->nodes.data[NodeIndex];

			RuntimeFBXCacheData->NodesMap.Add(Node->element_id, Node);
			if (Node->name.length > 0)
			{
				RuntimeFBXCacheData->NodesNamesMap.Add(UTF8_TO_TCHAR(Node->name.data), Node);
			}
		}

		RuntimeFBXCacheData->bValid = true;

		return RuntimeFBXCacheData;
	}

	bool LoadTexture(UglTFRuntimeAsset* Asset, TSharedRef<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData, ufbx_texture* Texture, UTexture2D*& TextureCache, const bool bSRGB, const FglTFRuntimeMaterialsConfig& MaterialsConfig)
	{
		if (!Texture)
		{
			return false;
		}

		FScopeLock TextureLock(&RuntimeFBXCacheData->TexturesLock);

		if (RuntimeFBXCacheData->TexturesCache.Contains(Texture))
		{
			TextureCache = RuntimeFBXCacheData->TexturesCache[Texture].Get();
			return true;
		}

		TArray64<uint8> ImageData;

		auto LoadTextureFromArchive = [Asset, &ImageData](ufbx_string* FilenameString) -> bool
			{
				const FString TextureFilename = FPaths::GetCleanFilename(UTF8_TO_TCHAR(FilenameString->data));
				for (const FString& Name : Asset->GetArchiveItems())
				{
					const FString CleanedName = FPaths::GetCleanFilename(Name);
					if (CleanedName == TextureFilename || CleanedName == TextureFilename.Replace(TEXT(" "), TEXT("_")))
					{
						return Asset->GetParser()->GetBlobByName(Name, ImageData);
					}
				}

				return true;
			};

		if (Texture->content.size > 0)
		{
			ImageData.Append(reinterpret_cast<const uint8*>(Texture->content.data), Texture->content.size);
		}
		else
		{
			if (Texture->filename.length > 0)
			{
				if (!LoadTextureFromArchive(&Texture->filename))
				{
					return false;
				}
			}

			// fallback to absolute
			if (ImageData.Num() == 0)
			{
				if (Texture->absolute_filename.length > 0)
				{
					if (!LoadTextureFromArchive(&Texture->absolute_filename))
					{
						return false;
					}
				}
			}
		}

		if (ImageData.Num() == 0)
		{
			return false;
		}

		TArray<FglTFRuntimeMipMap> Mips;
		if (!Asset->GetParser()->LoadBlobToMips(ImageData, Mips, bSRGB, MaterialsConfig))
		{
			return false;
		}

		FglTFRuntimeImagesConfig ImagesConfig = MaterialsConfig.ImagesConfig;
		ImagesConfig.bSRGB = bSRGB;
		TextureCache = Asset->GetParser()->BuildTexture(GetTransientPackage(), Mips, ImagesConfig, FglTFRuntimeTextureSampler());

		if (TextureCache)
		{
			RuntimeFBXCacheData->TexturesCache.Add(Texture, TStrongObjectPtr<UTexture2D>(TextureCache));
		}

		return TextureCache != nullptr;
	}

	UMaterialInterface* LoadMaterial(UglTFRuntimeAsset* Asset, TSharedRef<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData, ufbx_material* MeshMaterial, const FglTFRuntimeMaterialsConfig& MaterialsConfig)
	{
		FglTFRuntimeMaterial Material;

		if (MeshMaterial->pbr.base_color.has_value)
		{
			Material.BaseColorFactor = FLinearColor(MeshMaterial->pbr.base_color.value_vec4.x,
				MeshMaterial->pbr.base_color.value_vec4.y,
				MeshMaterial->pbr.base_color.value_vec4.z,
				MeshMaterial->pbr.base_color.value_vec4.w);
			Material.bHasBaseColorFactor = true;

			if (MeshMaterial->pbr.base_color.value_vec4.w < 1.0)
			{
				Material.MaterialType = EglTFRuntimeMaterialType::Translucent;
			}
		}

		LoadTexture(Asset, RuntimeFBXCacheData, MeshMaterial->pbr.base_color.texture, Material.BaseColorTextureCache, true, MaterialsConfig);

		LoadTexture(Asset, RuntimeFBXCacheData, MeshMaterial->pbr.normal_map.texture, Material.NormalTextureCache, false, MaterialsConfig);

		if (MeshMaterial->pbr.metalness.has_value)
		{
			Material.MetallicFactor = MeshMaterial->pbr.metalness.value_real;
			Material.bHasMetallicFactor = true;
		}

		LoadTexture(Asset, RuntimeFBXCacheData, MeshMaterial->pbr.metalness.texture, Material.MetallicRoughnessTextureCache, false, MaterialsConfig);

		LoadTexture(Asset, RuntimeFBXCacheData, MeshMaterial->pbr.emission_color.texture, Material.EmissiveTextureCache, true, MaterialsConfig);

		// xrdev mod plugin start mat2 /////////////////////////////////////////////////////////////////////////////////////////
		if (MeshMaterial->textures.count > 0)
		{
			FString generatorName = Asset->GetParser()->GetGenerator();
			for (int i = 0; i < MeshMaterial->textures.count; i++)
			{
				FString prop = FString(MeshMaterial->textures.data[i].material_prop.data);
				if (prop == TEXT("TransparencyFactor") || prop == TEXT("TransparentColor"))
				{
					LoadTexture(Asset, RuntimeFBXCacheData, MeshMaterial->textures.data[i].texture, Material.AlphaBlendTextureCache, true, MaterialsConfig);
					Material.bAlphaBlendOption = true;
					Material.MaterialType = EglTFRuntimeMaterialType::AlphaBlend;
				}
			}
		}
		// xrdev mod plugin end /////////////////////////////////////////////////////////////////////////////////////////

		return Asset->GetParser()->BuildMaterial(-1, UTF8_TO_TCHAR(MeshMaterial->name.data), Material, MaterialsConfig, true);
	}
}

TArray<FglTFRuntimeFBXNode> UglTFRuntimeFBXFunctionLibrary::GetFBXNodes(UglTFRuntimeAsset* Asset)
{
	TArray<FglTFRuntimeFBXNode> Nodes;

	if (!Asset)
	{
		return Nodes;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return Nodes;
		}
	}

	for (int32 NodeIndex = 0; NodeIndex < RuntimeFBXCacheData->Scene->nodes.count; NodeIndex++)
	{
		FglTFRuntimeFBXNode FBXNode;
		ufbx_node* Node = RuntimeFBXCacheData->Scene->nodes.data[NodeIndex];

		glTFRuntimeFBX::FillNode(Asset, Node, FBXNode);

		Nodes.Add(MoveTemp(FBXNode));
	}

	return Nodes;
}

TArray<FglTFRuntimeFBXNode> UglTFRuntimeFBXFunctionLibrary::GetFBXNodesMeshes(UglTFRuntimeAsset* Asset)
{
	TArray<FglTFRuntimeFBXNode> Nodes;

	if (!Asset)
	{
		return Nodes;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return Nodes;
		}
	}

	for (int32 NodeIndex = 0; NodeIndex < RuntimeFBXCacheData->Scene->nodes.count; NodeIndex++)
	{
		FglTFRuntimeFBXNode FBXNode;
		ufbx_node* Node = RuntimeFBXCacheData->Scene->nodes.data[NodeIndex];
		if (!Node->mesh)
		{
			continue;
		}

		glTFRuntimeFBX::FillNode(Asset, Node, FBXNode);

		Nodes.Add(MoveTemp(FBXNode));
	}

	return Nodes;
}

FglTFRuntimeFBXNode UglTFRuntimeFBXFunctionLibrary::GetFBXRootNode(UglTFRuntimeAsset* Asset)
{
	FglTFRuntimeFBXNode FBXNode;

	if (!Asset)
	{
		return FBXNode;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return FBXNode;
		}
	}

	glTFRuntimeFBX::FillNode(Asset, RuntimeFBXCacheData->Scene->root_node, FBXNode);

	return FBXNode;
}

TArray<FglTFRuntimeFBXNode> UglTFRuntimeFBXFunctionLibrary::GetFBXNodeChildren(UglTFRuntimeAsset* Asset, const FglTFRuntimeFBXNode& FBXNode)
{
	TArray<FglTFRuntimeFBXNode> Nodes;

	if (!Asset)
	{
		return Nodes;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return Nodes;
		}
	}

	if (RuntimeFBXCacheData->NodesMap.Contains(FBXNode.Id))
	{
		for (uint32 ChildNodeIndex = 0; ChildNodeIndex < RuntimeFBXCacheData->NodesMap[FBXNode.Id]->children.count; ChildNodeIndex++)
		{
			FglTFRuntimeFBXNode ChildFBXNode;
			ufbx_node* Node = RuntimeFBXCacheData->NodesMap[FBXNode.Id]->children.data[ChildNodeIndex];

			glTFRuntimeFBX::FillNode(Asset, Node, ChildFBXNode);

			Nodes.Add(MoveTemp(ChildFBXNode));
		}
	}

	return Nodes;
}

bool UglTFRuntimeFBXFunctionLibrary::IsFBXNodeBone(UglTFRuntimeAsset* Asset, const FglTFRuntimeFBXNode& FBXNode)
{
	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return false;
		}
	}

	if (RuntimeFBXCacheData->NodesMap.Contains(FBXNode.Id))
	{
		ufbx_node* Node = RuntimeFBXCacheData->NodesMap[FBXNode.Id];
		for (uint32 ClusterIndex = 0; ClusterIndex < RuntimeFBXCacheData->Scene->skin_clusters.count; ClusterIndex++)
		{
			ufbx_skin_cluster* Cluster = RuntimeFBXCacheData->Scene->skin_clusters.data[ClusterIndex];
			if (Cluster->bone_node == Node)
			{
				return true;
			}
		}
	}

	return false;
}

bool UglTFRuntimeFBXFunctionLibrary::GetFBXDefaultAnimation(UglTFRuntimeAsset* Asset, FglTFRuntimeFBXAnim& FBXAnim)
{
	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return false;
		}
	}

	ufbx_anim* Anim = RuntimeFBXCacheData->Scene->anim;
	if (Anim)
	{

		for (int32 AnimStackIndex = 0; AnimStackIndex < RuntimeFBXCacheData->Scene->anim_stacks.count; AnimStackIndex++)
		{
			ufbx_anim_stack* AnimStack = RuntimeFBXCacheData->Scene->anim_stacks.data[AnimStackIndex];

			if (AnimStack->anim == Anim)
			{
				FBXAnim.Id = AnimStack->element_id;
				FBXAnim.Name = UTF8_TO_TCHAR(AnimStack->name.data);
				FBXAnim.Duration = AnimStack->time_end - AnimStack->time_begin;
				return true;
			}
		}
	}

	return false;
}

TArray<FglTFRuntimeFBXAnim> UglTFRuntimeFBXFunctionLibrary::GetFBXAnimations(UglTFRuntimeAsset* Asset)
{
	TArray<FglTFRuntimeFBXAnim> Anims;

	if (!Asset)
	{
		return Anims;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return Anims;
		}
	}

	for (int32 AnimStackIndex = 0; AnimStackIndex < RuntimeFBXCacheData->Scene->anim_stacks.count; AnimStackIndex++)
	{
		ufbx_anim_stack* AnimStack = RuntimeFBXCacheData->Scene->anim_stacks.data[AnimStackIndex];

		FglTFRuntimeFBXAnim FBXAnim;
		FBXAnim.Id = AnimStack->element_id;
		FBXAnim.Name = UTF8_TO_TCHAR(AnimStack->name.data);
		FBXAnim.Duration = AnimStack->time_end - AnimStack->time_begin;

		Anims.Add(MoveTemp(FBXAnim));
	}

	return Anims;
}

UAnimSequence* UglTFRuntimeFBXFunctionLibrary::LoadFBXAnimAsSkeletalMeshAnimation(UglTFRuntimeAsset* Asset, const FglTFRuntimeFBXAnim& FBXAnim, const FglTFRuntimeFBXNode& FBXNode, USkeletalMesh* SkeletalMesh, const FglTFRuntimeSkeletalAnimationConfig& SkeletalAnimationConfig)
{
	if (!Asset || !SkeletalMesh)
	{
		return nullptr;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return nullptr;
		}
	}

	if (!RuntimeFBXCacheData->NodesMap.Contains(FBXNode.Id))
	{
		return nullptr;
	}

	ufbx_node* FoundNode = RuntimeFBXCacheData->NodesMap[FBXNode.Id];

	if (!FoundNode || !FoundNode->mesh || (FoundNode->mesh->skin_deformers.count < 1 && FoundNode->mesh->blend_deformers.count < 1))
	{
		return nullptr;
	}

	ufbx_anim_stack* FoundAnim = nullptr;

	for (int32 AnimStackIndex = 0; AnimStackIndex < RuntimeFBXCacheData->Scene->anim_stacks.count; AnimStackIndex++)
	{
		ufbx_anim_stack* AnimStack = RuntimeFBXCacheData->Scene->anim_stacks.data[AnimStackIndex];

		if (AnimStack->element_id == FBXAnim.Id)
		{
			FoundAnim = AnimStack;
			break;
		}
	}

	if (!FoundAnim)
	{
		return nullptr;
	}

	const float Duration = FoundAnim->time_end - FoundAnim->time_begin;
	const int32 NumFrames = SkeletalAnimationConfig.FramesPerSecond * Duration;
	const float Delta = Duration / NumFrames;

	ufbx_skin_deformer* Skin = FoundNode->mesh->skin_deformers.data[0];

	FglTFRuntimePoseTracksMap PosesMap;
	TMap<FName, TArray<TPair<float, float>>> MorphTargetCurves;

	for (int32 ClusterIndex = 0; ClusterIndex < Skin->clusters.count; ClusterIndex++)
	{
		ufbx_skin_cluster* Cluster = Skin->clusters.data[ClusterIndex];

		const FString BoneName = UTF8_TO_TCHAR(Cluster->bone_node->name.data);

		float Time = FoundAnim->time_begin;
		if (SkeletalMesh->GetRefSkeleton().FindBoneIndex(*BoneName) > INDEX_NONE)
		{
			FRawAnimSequenceTrack Track;
			for (int32 FrameIndex = 0; FrameIndex < NumFrames; FrameIndex++)
			{
				FTransform Transform = glTFRuntimeFBX::GetTransform(Asset, ufbx_evaluate_transform(FoundAnim->anim, Cluster->bone_node, Time));
#if ENGINE_MAJOR_VERSION >= 5
				Track.PosKeys.Add(FVector3f(Transform.GetLocation()));
				Track.RotKeys.Add(FQuat4f(Transform.GetRotation()));
				Track.ScaleKeys.Add(FVector3f(Transform.GetScale3D()));
#else
				Track.PosKeys.Add(Transform.GetLocation());
				Track.RotKeys.Add(Transform.GetRotation());
				Track.ScaleKeys.Add(Transform.GetScale3D());
#endif
				Time += Delta;
			}

			PosesMap.Add(BoneName, MoveTemp(Track));
		}
	}

	for (uint32 BlendDeformerIndex = 0; BlendDeformerIndex < FoundNode->mesh->blend_deformers.count; BlendDeformerIndex++)
	{
		for (uint32 BlendDeformerChannelIndex = 0; BlendDeformerChannelIndex < FoundNode->mesh->blend_deformers.data[BlendDeformerIndex]->channels.count; BlendDeformerChannelIndex++)
		{
			const FString MorphTargetName = UTF8_TO_TCHAR(FoundNode->mesh->blend_deformers.data[BlendDeformerIndex]->channels.data[BlendDeformerChannelIndex]->name.data);
			float Time = FoundAnim->time_begin;
			TArray<TPair<float, float>> MorphTargetValues;
			for (int32 FrameIndex = 0; FrameIndex < NumFrames; FrameIndex++)
			{
				const float Weight = ufbx_evaluate_blend_weight(FoundAnim->anim, FoundNode->mesh->blend_deformers.data[BlendDeformerIndex]->channels.data[BlendDeformerChannelIndex], Time);
				MorphTargetValues.Add(TPair<float, float>(Time, Weight));
				Time += Delta;
			}

			MorphTargetCurves.Add(*MorphTargetName, MoveTemp(MorphTargetValues));
		}
	}

	return Asset->GetParser()->LoadSkeletalAnimationFromTracksAndMorphTargets(SkeletalMesh, PosesMap, MorphTargetCurves, Duration, SkeletalAnimationConfig);
}

UAnimSequence* UglTFRuntimeFBXFunctionLibrary::LoadFBXExternalAnimAsSkeletalMeshAnimation(UglTFRuntimeAsset* Asset, const FglTFRuntimeFBXAnim& FBXAnim, USkeletalMesh* SkeletalMesh, const FglTFRuntimeSkeletalAnimationConfig& SkeletalAnimationConfig)
{
	if (!Asset || !SkeletalMesh)
	{
		return nullptr;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return nullptr;
		}
	}

	ufbx_anim_stack* FoundAnim = nullptr;

	for (int32 AnimStackIndex = 0; AnimStackIndex < RuntimeFBXCacheData->Scene->anim_stacks.count; AnimStackIndex++)
	{
		ufbx_anim_stack* AnimStack = RuntimeFBXCacheData->Scene->anim_stacks.data[AnimStackIndex];

		if (AnimStack->element_id == FBXAnim.Id)
		{
			FoundAnim = AnimStack;
			break;
		}
	}

	if (!FoundAnim)
	{
		return nullptr;
	}

	const float Duration = FoundAnim->time_end - FoundAnim->time_begin;
	const int32 NumFrames = SkeletalAnimationConfig.FramesPerSecond * Duration;
	const float Delta = Duration / NumFrames;

	FglTFRuntimePoseTracksMap PosesMap;
	TMap<FName, TArray<TPair<float, float>>> MorphTargetCurves;

	const int32 BonesNum = SkeletalMesh->GetRefSkeleton().GetNum();

	for (int32 BoneIndex = 0; BoneIndex < BonesNum; BoneIndex++)
	{
		const FString BoneName = SkeletalMesh->GetRefSkeleton().GetBoneName(BoneIndex).ToString();

		if (!RuntimeFBXCacheData->NodesNamesMap.Contains(BoneName))
		{
			continue;
		}

		ufbx_node* BoneNode = RuntimeFBXCacheData->NodesNamesMap[BoneName];

		float Time = FoundAnim->time_begin;

		FRawAnimSequenceTrack Track;
		for (int32 FrameIndex = 0; FrameIndex < NumFrames; FrameIndex++)
		{
			FTransform Transform = glTFRuntimeFBX::GetTransform(Asset, ufbx_evaluate_transform(FoundAnim->anim, BoneNode, Time));
#if ENGINE_MAJOR_VERSION >= 5
			Track.PosKeys.Add(FVector3f(Transform.GetLocation()));
			Track.RotKeys.Add(FQuat4f(Transform.GetRotation()));
			Track.ScaleKeys.Add(FVector3f(Transform.GetScale3D()));
#else
			Track.PosKeys.Add(Transform.GetLocation());
			Track.RotKeys.Add(Transform.GetRotation());
			Track.ScaleKeys.Add(Transform.GetScale3D());
#endif
			Time += Delta;
		}

		PosesMap.Add(BoneName, MoveTemp(Track));
	}

	return Asset->GetParser()->LoadSkeletalAnimationFromTracksAndMorphTargets(SkeletalMesh, PosesMap, MorphTargetCurves, Duration, SkeletalAnimationConfig);
}

bool UglTFRuntimeFBXFunctionLibrary::FillFBXSkinDeformer(UglTFRuntimeAsset* Asset, ufbx_skin_deformer* SkinDeformer, TArray<FglTFRuntimeBone>& Skeleton, TMap<uint32, TArray<TPair<int32, float>>>& JointsWeightsMap, int32& JointsWeightsGroups)
{
	TSet<FString> Bones;

	ufbx_node* RootNode = nullptr;

	TMap<FString, int32> BonesMap;

	for (int32 ClusterIndex = 0; ClusterIndex < SkinDeformer->clusters.count; ClusterIndex++)
	{
		ufbx_skin_cluster* Cluster = SkinDeformer->clusters.data[ClusterIndex];

		if (!RootNode)
		{
			RootNode = Cluster->bone_node;
			ufbx_node* Parent = RootNode->parent;
			while (Parent && !Parent->is_root)
			{

				RootNode = Parent;

				Parent = Parent->parent;
			}

			bool bRootProcessed = false;
			glTFRuntimeFBX::FillSkeleton(Asset, SkinDeformer, RootNode, INDEX_NONE, Skeleton, BonesMap, bRootProcessed);
		}

		const FString BoneName = UTF8_TO_TCHAR(Cluster->bone_node->name.data);

		if (BonesMap.Contains(BoneName))
		{
			const int32 BoneIndex = BonesMap[BoneName];

			for (uint32 VertexIndex = 0; VertexIndex < Cluster->vertices.count; VertexIndex++)
			{
				const uint32 VertexIndexValue = Cluster->vertices.data[VertexIndex];
				const float VertexWeight = Cluster->weights.data[VertexIndex];
				if (!JointsWeightsMap.Contains(VertexIndexValue))
				{
					JointsWeightsMap.Add(VertexIndexValue);
				}

				JointsWeightsMap[VertexIndexValue].Add(TPair<int32, float>(BoneIndex, VertexWeight));
			}
		}
	}

	int32 MaxBoneInfluences = 4;

	for (const TPair<uint32, TArray<TPair<int32, float>>>& Pair : JointsWeightsMap)
	{
		if (Pair.Value.Num() > MaxBoneInfluences)
		{
			MaxBoneInfluences = Pair.Value.Num();
		}
	}

	JointsWeightsGroups = MaxBoneInfluences / 4;
	if (MaxBoneInfluences % 4 != 0)
	{
		JointsWeightsGroups++;
	}

	return true;
}

// xrdev mod plugin start /////////////////////////////////////////////////////////////////////////////////////////
// FBX 파일이 블랜더에서 익스포트 되었는지 확인한다.
bool UglTFRuntimeFBXFunctionLibrary::GetFbxInfo(FString fbxFilePath, FglTFRuntimeFBX_PreLoadInfo & fbxInfo)
{
	const char* fbxFilePathCStr = TCHAR_TO_UTF8(*fbxFilePath);

	ufbx_load_opts opts = { 0 }; // Optional, pass NULL for defaults
	ufbx_error error; // Optional, pass NULL if you don't care about errors
	ufbx_scene* scene = ufbx_load_file(fbxFilePathCStr, &opts, &error);
	if (!scene) {
		UE_LOG(LogGLTFRuntime, Log, TEXT("GetFbxInfo ufbx_load_file Error : %s, path %s "), *FString(error.description.data), *fbxFilePath);
	}
	else {
		if (scene->metadata.exporter == UFBX_EXPORTER_BLENDER_BINARY ||
			scene->metadata.exporter == UFBX_EXPORTER_BLENDER_ASCII)
		{
			fbxInfo.bBlenderCreateor = true;
		}

		fbxInfo.bSkeletonMesh = (scene->bones.count > 0);

		ufbx_free_scene(scene);

		return true;
	}

	return false;
}
// xrdev mod plugin end /////////////////////////////////////////////////////////////////////////////////////////

bool UglTFRuntimeFBXFunctionLibrary::LoadFBXAsRuntimeLODByNode(UglTFRuntimeAsset* Asset, const FglTFRuntimeFBXNode& FBXNode, FglTFRuntimeMeshLOD& RuntimeLOD, bool& bIsSkeletal, const FglTFRuntimeMaterialsConfig& StaticMeshMaterialsConfig, const FglTFRuntimeMaterialsConfig& SkeletalMeshMaterialsConfig)
{
	if (!Asset)
	{
		return false;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return false;
		}
	}

	if (!RuntimeFBXCacheData->NodesMap.Contains(FBXNode.Id))
	{
		return false;
	}

	RuntimeLOD.Empty();

	ufbx_node* Node = RuntimeFBXCacheData->NodesMap[FBXNode.Id];

	ufbx_mesh* Mesh = Node->mesh;
	if (!Mesh)
	{
		return false;
	}

	TMap<uint32, TArray<TPair<int32, float>>> JointsWeightsMap;
	int32 JointsWeightsGroups = 1;


	// skeletal mesh ?
	if (Mesh->skin_deformers.count > 0)
	{
		if (!FillFBXSkinDeformer(Asset, Mesh->skin_deformers.data[0], RuntimeLOD.Skeleton, JointsWeightsMap, JointsWeightsGroups))
		{
			return false;
		}
	}

	bIsSkeletal = JointsWeightsMap.Num() > 0;

	const FglTFRuntimeMaterialsConfig* MaterialsConfig = &StaticMeshMaterialsConfig;

	if (bIsSkeletal)
	{
		MaterialsConfig = &SkeletalMeshMaterialsConfig;
	}

	TArray<FglTFRuntimePrimitive> Primitives;

	if (!FillFBXPrimitives(Asset, RuntimeFBXCacheData, Node, 0, Primitives, JointsWeightsMap, JointsWeightsGroups, *MaterialsConfig))
	{
		return false;
	}

	// ensure only non-empty primitives are added
	for (FglTFRuntimePrimitive& Primitive : Primitives)
	{
		if (Primitive.Indices.Num() > 0)
		{
			RuntimeLOD.Primitives.Add(MoveTemp(Primitive));
		}
	}

	return true;
}

bool UglTFRuntimeFBXFunctionLibrary::LoadAndMergeFBXAsRuntimeLODsGroupBySkinDeformer(UglTFRuntimeAsset* Asset, TArray<FglTFRuntimeMeshLOD>& RuntimeLODs, const FglTFRuntimeMaterialsConfig& SkeletalMeshMaterialsConfig)
{

	if (!Asset)
	{
		return false;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return false;
		}
	}

	struct FglTFRuntimeFBXNodeToMerge
	{
		TArray<FglTFRuntimeBone> Skeleton;
		TMap<uint32, TArray<TPair<int32, float>>> JointsWeightsMap;
		int32 JointsWeightsGroups;
	};

	TArray< FglTFRuntimeFBXNodeToMerge> DiscoveredSkinDeformers;

	for (int32 SkinDeformerIndex = 0; SkinDeformerIndex < RuntimeFBXCacheData->Scene->skin_deformers.count; SkinDeformerIndex++)
	{
		FglTFRuntimeFBXNodeToMerge CurrentSkinDeformer;
		CurrentSkinDeformer.JointsWeightsGroups = 1;
		if (!FillFBXSkinDeformer(Asset, RuntimeFBXCacheData->Scene->skin_deformers.data[SkinDeformerIndex], CurrentSkinDeformer.Skeleton, CurrentSkinDeformer.JointsWeightsMap, CurrentSkinDeformer.JointsWeightsGroups))
		{
			return false;
		}

		DiscoveredSkinDeformers.Add(MoveTemp(CurrentSkinDeformer));
	}

	auto SkinDeformerIsCompatible = [](const TArray<FglTFRuntimeBone>& BaseSkeleton, const TArray<FglTFRuntimeBone>& CurrentSkeleton)
		{
			if (BaseSkeleton.Num() != CurrentSkeleton.Num())
			{
				return false;
			}

			for (int32 BoneIndex = 0; BoneIndex < BaseSkeleton.Num(); BoneIndex++)
			{
				if (BaseSkeleton[BoneIndex].ParentIndex != CurrentSkeleton[BoneIndex].ParentIndex)
				{
					return false;
				}

				if (BaseSkeleton[BoneIndex].BoneName != CurrentSkeleton[BoneIndex].BoneName)
				{
					return false;
				}
			}

			return true;
		};

	TSet<int32> ProcessedSkinDeformers;
	TArray<int32> SkinDeformerGroups;

	for (int32 SkinDeformerIndex = 0; SkinDeformerIndex < DiscoveredSkinDeformers.Num(); SkinDeformerIndex++)
	{
		if (ProcessedSkinDeformers.Contains(SkinDeformerIndex))
		{
			continue;
		}

		SkinDeformerGroups.Add(SkinDeformerIndex);
		ProcessedSkinDeformers.Add(SkinDeformerIndex);

		for (int32 CheckSkinDeformerIndex = 0; CheckSkinDeformerIndex < DiscoveredSkinDeformers.Num(); CheckSkinDeformerIndex++)
		{
			if (CheckSkinDeformerIndex == SkinDeformerIndex)
			{
				continue;
			}

			if (ProcessedSkinDeformers.Contains(CheckSkinDeformerIndex))
			{
				continue;
			}

			if (SkinDeformerIsCompatible(DiscoveredSkinDeformers[SkinDeformerIndex].Skeleton, DiscoveredSkinDeformers[CheckSkinDeformerIndex].Skeleton))
			{
				ProcessedSkinDeformers.Add(CheckSkinDeformerIndex);
			}
		}
	}

	for (const int32 SkinDeformerIndex : SkinDeformerGroups)
	{
		FglTFRuntimeMeshLOD LOD;
		if (LoadAndMergeFBXAsRuntimeLODBySkinDeformer(Asset, SkinDeformerIndex, LOD, SkeletalMeshMaterialsConfig))
		{
			RuntimeLODs.Add(MoveTemp(LOD));
		}
	}

	return true;
}

bool UglTFRuntimeFBXFunctionLibrary::LoadAndMergeFBXAsRuntimeLODByBiggestSkinDeformer(UglTFRuntimeAsset* Asset, FglTFRuntimeMeshLOD& RuntimeLOD, const FglTFRuntimeMaterialsConfig& SkeletalMeshMaterialsConfig)
{
	if (!Asset)
	{
		return false;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return false;
		}
	}

	struct FglTFRuntimeFBXNodeToMerge
	{
		FglTFRuntimeFBXNode FBXNode;
		TArray<FglTFRuntimeBone> Skeleton;
		TMap<uint32, TArray<TPair<int32, float>>> JointsWeightsMap;
		int32 JointsWeightsGroups;
	};

	int32 BiggestDeformerValue = -1;

	TArray<FglTFRuntimeBone> BiggestSkeleton;

	for (int32 SkinDeformerIndex = 0; SkinDeformerIndex < RuntimeFBXCacheData->Scene->skin_deformers.count; SkinDeformerIndex++)
	{
		FglTFRuntimeFBXNodeToMerge CurrentSkinDeformer;
		CurrentSkinDeformer.JointsWeightsGroups = 1;
		if (FillFBXSkinDeformer(Asset, RuntimeFBXCacheData->Scene->skin_deformers.data[SkinDeformerIndex], CurrentSkinDeformer.Skeleton, CurrentSkinDeformer.JointsWeightsMap, CurrentSkinDeformer.JointsWeightsGroups))
		{
			if (CurrentSkinDeformer.Skeleton.Num() > BiggestDeformerValue)
			{
				BiggestSkeleton = CurrentSkinDeformer.Skeleton;
				BiggestDeformerValue = CurrentSkinDeformer.Skeleton.Num();
			}
		}
	}

	if (BiggestDeformerValue < 0 || BiggestSkeleton.Num() < 1)
	{
		return false;
	}

	RuntimeLOD.Skeleton = MoveTemp(BiggestSkeleton);

	auto SkinDeformerIsCompatible = [](const TArray<FglTFRuntimeBone>& BaseSkeleton, const TArray<FglTFRuntimeBone>& CurrentSkeleton)
		{
			const int32 NumBones = FMath::Min(BaseSkeleton.Num(), CurrentSkeleton.Num());
			if (NumBones <= 0)
			{
				return false;
			}

			for (int32 BoneIndex = 0; BoneIndex < NumBones; BoneIndex++)
			{
				if (BaseSkeleton[BoneIndex].ParentIndex != CurrentSkeleton[BoneIndex].ParentIndex)
				{
					return false;
				}

				if (BaseSkeleton[BoneIndex].BoneName != CurrentSkeleton[BoneIndex].BoneName)
				{
					return false;
				}
			}

			return true;
		};

	const TArray<FglTFRuntimeFBXNode> FBXNodes = GetFBXNodesMeshes(Asset);

	if (FBXNodes.Num() < 1)
	{
		return false;
	}

	TArray<FglTFRuntimeFBXNodeToMerge> NodesToMerge;

	for (const FglTFRuntimeFBXNode& FBXNode : FBXNodes)
	{
		if (!RuntimeFBXCacheData->NodesMap.Contains(FBXNode.Id))
		{
			continue;
		}

		ufbx_node* Node = RuntimeFBXCacheData->NodesMap[FBXNode.Id];

		ufbx_mesh* Mesh = Node->mesh;
		if (!Mesh)
		{
			continue;
		}

		// skeletal mesh ?
		if (Mesh->skin_deformers.count < 1)
		{
			continue;
		}

		FglTFRuntimeFBXNodeToMerge FBXNodeToMerge;
		FBXNodeToMerge.FBXNode = FBXNode;
		FBXNodeToMerge.JointsWeightsGroups = 1;

		if (!FillFBXSkinDeformer(Asset, Mesh->skin_deformers.data[0], FBXNodeToMerge.Skeleton, FBXNodeToMerge.JointsWeightsMap, FBXNodeToMerge.JointsWeightsGroups))
		{
			continue;
		}

		if (!SkinDeformerIsCompatible(RuntimeLOD.Skeleton, FBXNodeToMerge.Skeleton))
		{
			continue;
		}

		NodesToMerge.Add(MoveTemp(FBXNodeToMerge));
	}

	if (NodesToMerge.Num() < 1)
	{
		return false;
	}

	TArray<FglTFRuntimePrimitive> Primitives;

	for (const FglTFRuntimeFBXNodeToMerge& FBXNodeToMerge : NodesToMerge)
	{
		ufbx_node* Node = RuntimeFBXCacheData->NodesMap[FBXNodeToMerge.FBXNode.Id];

		if (!FillFBXPrimitives(Asset, RuntimeFBXCacheData, Node, Primitives.Num(), Primitives, FBXNodeToMerge.JointsWeightsMap, FBXNodeToMerge.JointsWeightsGroups, SkeletalMeshMaterialsConfig))
		{
			return false;
		}
	}

	// ensure only non-empty primitives are added
	for (FglTFRuntimePrimitive& Primitive : Primitives)
	{
		if (Primitive.Indices.Num() > 0)
		{
			RuntimeLOD.Primitives.Add(MoveTemp(Primitive));
		}
	}

	return true;
}

bool UglTFRuntimeFBXFunctionLibrary::LoadAndMergeFBXAsRuntimeLODBySkinDeformer(UglTFRuntimeAsset* Asset, const int32 SkinDeformerIndex, FglTFRuntimeMeshLOD& RuntimeLOD, const FglTFRuntimeMaterialsConfig& SkeletalMeshMaterialsConfig)
{
	if (!Asset)
	{
		return false;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return false;
		}
	}

	if (SkinDeformerIndex >= RuntimeFBXCacheData->Scene->skin_deformers.count)
	{
		return false;
	}

	TMap<uint32, TArray<TPair<int32, float>>> JointsWeightsMap;
	int32 JointsWeightsGroups = 1;

	if (!FillFBXSkinDeformer(Asset, RuntimeFBXCacheData->Scene->skin_deformers.data[SkinDeformerIndex], RuntimeLOD.Skeleton, JointsWeightsMap, JointsWeightsGroups))
	{
		return false;
	}

	struct FglTFRuntimeFBXNodeToMerge
	{
		FglTFRuntimeFBXNode FBXNode;
		TArray<FglTFRuntimeBone> Skeleton;
		TMap<uint32, TArray<TPair<int32, float>>> JointsWeightsMap;
		int32 JointsWeightsGroups;
	};

	auto SkinDeformerIsCompatible = [](const TArray<FglTFRuntimeBone>& BaseSkeleton, const TArray<FglTFRuntimeBone>& CurrentSkeleton)
		{
			if (BaseSkeleton.Num() != CurrentSkeleton.Num())
			{
				return false;
			}

			for (int32 BoneIndex = 0; BoneIndex < BaseSkeleton.Num(); BoneIndex++)
			{
				if (BaseSkeleton[BoneIndex].ParentIndex != CurrentSkeleton[BoneIndex].ParentIndex)
				{
					return false;
				}

				if (BaseSkeleton[BoneIndex].BoneName != CurrentSkeleton[BoneIndex].BoneName)
				{
					return false;
				}
			}

			return true;
		};

	const TArray<FglTFRuntimeFBXNode> FBXNodes = GetFBXNodesMeshes(Asset);

	if (FBXNodes.Num() < 1)
	{
		return false;
	}

	TArray<FglTFRuntimeFBXNodeToMerge> NodesToMerge;

	for (const FglTFRuntimeFBXNode& FBXNode : FBXNodes)
	{
		if (!RuntimeFBXCacheData->NodesMap.Contains(FBXNode.Id))
		{
			continue;
		}

		ufbx_node* Node = RuntimeFBXCacheData->NodesMap[FBXNode.Id];

		ufbx_mesh* Mesh = Node->mesh;
		if (!Mesh)
		{
			continue;
		}

		// skeletal mesh ?
		if (Mesh->skin_deformers.count < 1)
		{
			continue;
		}

		FglTFRuntimeFBXNodeToMerge FBXNodeToMerge;
		FBXNodeToMerge.FBXNode = FBXNode;
		FBXNodeToMerge.JointsWeightsGroups = 1;

		if (!FillFBXSkinDeformer(Asset, Mesh->skin_deformers.data[0], FBXNodeToMerge.Skeleton, FBXNodeToMerge.JointsWeightsMap, FBXNodeToMerge.JointsWeightsGroups))
		{
			continue;
		}

		if (!SkinDeformerIsCompatible(RuntimeLOD.Skeleton, FBXNodeToMerge.Skeleton))
		{
			continue;
		}

		NodesToMerge.Add(MoveTemp(FBXNodeToMerge));
	}

	if (NodesToMerge.Num() < 1)
	{
		return false;
	}

	TArray<FglTFRuntimePrimitive> Primitives;

	for (const FglTFRuntimeFBXNodeToMerge& FBXNodeToMerge : NodesToMerge)
	{
		ufbx_node* Node = RuntimeFBXCacheData->NodesMap[FBXNodeToMerge.FBXNode.Id];

		if (!FillFBXPrimitives(Asset, RuntimeFBXCacheData, Node, Primitives.Num(), Primitives, FBXNodeToMerge.JointsWeightsMap, FBXNodeToMerge.JointsWeightsGroups, SkeletalMeshMaterialsConfig))
		{
			return false;
		}
	}

	// ensure only non-empty primitives are added
	for (FglTFRuntimePrimitive& Primitive : Primitives)
	{
		if (Primitive.Indices.Num() > 0)
		{
			RuntimeLOD.Primitives.Add(MoveTemp(Primitive));
		}
	}

	return true;

}

bool UglTFRuntimeFBXFunctionLibrary::FillFBXPrimitives(UglTFRuntimeAsset* Asset, TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData, struct ufbx_node* Node, const int32 PrimitiveBase, TArray<FglTFRuntimePrimitive>& Primitives, const TMap<uint32, TArray<TPair<int32, float>>>& JointsWeightsMap, const int32 JointsWeightsGroups, const FglTFRuntimeMaterialsConfig& MaterialsConfig)
{
	ufbx_mesh* Mesh = Node->mesh;
	if (!Mesh)
	{
		return false;
	}

	uint32 NumTriangleIndices = Mesh->max_face_triangles * 3;
	TArray<uint32> TriangleIndices;
	TriangleIndices.AddUninitialized(NumTriangleIndices);

	const bool bIsSkeletal = JointsWeightsMap.Num() > 0;


	uint32 NumMaterials = Node->materials.count;
	if (NumMaterials == 0)
	{
		// fallback to the default material
		NumMaterials = 1;
	}

	Primitives.AddDefaulted(NumMaterials);

	TMap<FString, ufbx_blend_shape*> MorphTargets;

	for (uint32 BlendDeformerIndex = 0; BlendDeformerIndex < Mesh->blend_deformers.count; BlendDeformerIndex++)
	{
		for (uint32 BlendDeformerChannelIndex = 0; BlendDeformerChannelIndex < Mesh->blend_deformers.data[BlendDeformerIndex]->channels.count; BlendDeformerChannelIndex++)
		{
			MorphTargets.Add(UTF8_TO_TCHAR(Mesh->blend_deformers.data[BlendDeformerIndex]->channels.data[BlendDeformerChannelIndex]->name.data)
				, Mesh->blend_deformers.data[BlendDeformerIndex]->channels.data[BlendDeformerChannelIndex]->target_shape);
		}
	}


	for (uint32 PrimitiveIndex = PrimitiveBase; PrimitiveIndex < PrimitiveBase + NumMaterials; PrimitiveIndex++)
	{
		FglTFRuntimePrimitive& Primitive = Primitives[PrimitiveIndex];

		if (Node->materials.count > 0)
		{
			ufbx_material* MeshMaterial = Mesh->materials.data[PrimitiveIndex - PrimitiveBase];

			if (!MaterialsConfig.bSkipLoad)
			{
				Primitive.Material = glTFRuntimeFBX::LoadMaterial(Asset, RuntimeFBXCacheData.ToSharedRef(), MeshMaterial, MaterialsConfig);
			}
		}

		if (bIsSkeletal)
		{
			for (int32 JWIndex = 0; JWIndex < JointsWeightsGroups; JWIndex++)
			{
				Primitive.Joints.AddDefaulted();
				Primitive.Weights.AddDefaulted();
			}
		}

		if (Mesh->vertex_uv.exists)
		{
			Primitive.UVs.AddDefaulted();
		}

		for (const TPair<FString, ufbx_blend_shape*>& Pair : MorphTargets)
		{
			FglTFRuntimeMorphTarget MorphTarget;
			MorphTarget.Name = Pair.Key;
			Primitive.MorphTargets.Add(MoveTemp(MorphTarget));
		}
	}

	for (uint32 FaceIndex = 0; FaceIndex < Mesh->num_faces; FaceIndex++)
	{
		uint32 MaterialIndex = 0;
		if (FaceIndex < Mesh->face_material.count)
		{
			MaterialIndex = Mesh->face_material.data[FaceIndex];
		}

		if (Primitives.IsValidIndex(PrimitiveBase + MaterialIndex))
		{
			FglTFRuntimePrimitive& Primitive = Primitives[PrimitiveBase + MaterialIndex];

			ufbx_face Face = Mesh->faces.data[FaceIndex];
			uint32 NumTriangles = ufbx_triangulate_face(TriangleIndices.GetData(), NumTriangleIndices, Mesh, Face);

			for (uint32 VertexIndex = 0; VertexIndex < NumTriangles * 3; VertexIndex++)
			{
				const uint32 Index = TriangleIndices[VertexIndex];

				if (bIsSkeletal)
				{
					for (int32 JWIndex = 0; JWIndex < JointsWeightsGroups; JWIndex++)
					{
						Primitive.Joints[JWIndex].AddZeroed();
						Primitive.Weights[JWIndex].AddZeroed();
					}

					if (JointsWeightsMap.Contains(Mesh->vertex_indices.data[Index]))
					{
						const TArray<TPair<int32, float>>& JointsWeights = JointsWeightsMap[Mesh->vertex_indices.data[Index]];
						int32 BoneIndex = 0;
						for (const TPair<int32, float>& Pair : JointsWeights)
						{
							Primitive.Joints[BoneIndex / 4].Last()[BoneIndex % 4] = Pair.Key;
							Primitive.Weights[BoneIndex / 4].Last()[BoneIndex % 4] = Pair.Value;
							BoneIndex++;
						}
					}
				}

				const ufbx_vec3 Position = ufbx_get_vertex_vec3(&Mesh->vertex_position, Index);

				if (bIsSkeletal)
				{
					Primitive.Positions.Add(glTFRuntimeFBX::GetTransform(Asset, Node->local_transform).TransformPosition(Asset->GetParser()->TransformPosition(FVector(Position.x, Position.y, Position.z))));
				}
				else
				{
					Primitive.Positions.Add(Asset->GetParser()->TransformPosition(FVector(Position.x, Position.y, Position.z)));
				}

				for (FglTFRuntimeMorphTarget& MorphTarget : Primitive.MorphTargets)
				{
					ufbx_blend_shape* BlendShape = MorphTargets[MorphTarget.Name];
					const ufbx_vec3 MorphTargetPosition = ufbx_get_blend_shape_vertex_offset(BlendShape, Mesh->vertex_indices.data[Index]);
					if (bIsSkeletal)
					{
						MorphTarget.Positions.Add(glTFRuntimeFBX::GetTransform(Asset, Node->local_transform).TransformPosition(Asset->GetParser()->TransformPosition(FVector(MorphTargetPosition.x, MorphTargetPosition.y, MorphTargetPosition.z))));
					}
					else
					{
						MorphTarget.Positions.Add(Asset->GetParser()->TransformPosition(FVector(MorphTargetPosition.x, MorphTargetPosition.y, MorphTargetPosition.z)));
					}
				}

				if (Mesh->vertex_normal.exists)
				{
					const ufbx_vec3 Normal = ufbx_get_vertex_vec3(&Mesh->vertex_normal, Index);
					if (bIsSkeletal)
					{
						Primitive.Normals.Add(glTFRuntimeFBX::GetTransform(Asset, Node->local_transform).TransformVector(Asset->GetParser()->TransformVector(FVector(Normal.x, Normal.y, Normal.z))));
					}
					else
					{
						Primitive.Normals.Add(Asset->GetParser()->TransformVector(FVector(Normal.x, Normal.y, Normal.z)));
					}
				}

				if (Mesh->vertex_uv.exists)
				{
					const ufbx_vec2 UV = ufbx_get_vertex_vec2(&Mesh->vertex_uv, Index);
					Primitive.UVs[0].Add(FVector2D(UV.x, 1 - UV.y));
				}

				if (Mesh->vertex_color.exists)
				{
					const ufbx_vec4 Color = ufbx_get_vertex_vec4(&Mesh->vertex_color, Index);
					Primitive.Colors.Add(FVector4(Color.x, Color.y, Color.z, Color.w));
				}

				Primitive.Indices.Add(Primitive.Positions.Num() - 1);
			}
		}
	}

	return true;
}

int32 UglTFRuntimeFBXFunctionLibrary::GetFBXSkinDeformersNum(UglTFRuntimeAsset* Asset)
{
	if (!Asset)
	{
		return false;
	}

	TSharedPtr<FglTFRuntimeFBXCacheData> RuntimeFBXCacheData = nullptr;
	{
		FScopeLock Lock(&(Asset->GetParser()->PluginsCacheDataLock));

		RuntimeFBXCacheData = glTFRuntimeFBX::GetCacheData(Asset);
		if (!RuntimeFBXCacheData)
		{
			return false;
		}
	}

	return static_cast<int32>(RuntimeFBXCacheData->Scene->skin_deformers.count);
}