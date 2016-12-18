// Copyright 2014 Volumes of Fun. All Rights Reserved.

#include "CubiquityPluginPrivatePCH.h"

#include "CubiquityTerrainVolume.h"

#include "CubiquityOctreeNode.h"
#include "CubiquityMeshComponent.h"
#include "CubiquityMaterialSet.h"

ACubiquityTerrainVolume::ACubiquityTerrainVolume(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	volumeFileName = TEXT("D:/Unreal Projects/FPSTemplate/Plugins/Cubiquity/Dependencies/example-vdb/SmoothVoxeliensTerrain.vdb");
	//static ConstructorHelpers::FObjectFinder<ACubiquityTerrainVolume> CubiquityColoredCubesVolume(TEXT("CubiquityTerrainVolume'/Cubiquity/CubiquityTerrain.CubiquityTerrain'"));
}

void ACubiquityTerrainVolume::PostActorCreated()
{
	UE_LOG(CubiquityLog, Log, TEXT("ACubiquityTerrainVolume::PostActorCreated"));
	
	Super::PostActorCreated();
}

void ACubiquityTerrainVolume::PostLoad()
{
	UE_LOG(CubiquityLog, Log, TEXT("ACubiquityTerrainVolume::PostLoad"));

	Super::PostLoad();
}

void ACubiquityTerrainVolume::Destroyed()
{
	Super::Destroyed();

	m_volume.reset(nullptr);
}

void ACubiquityTerrainVolume::loadVolume()
{
	m_volume = loadVolumeImpl<Cubiquity::TerrainVolume>();
}

void ACubiquityTerrainVolume::sculptTerrain(FVector localPosition, float innerRadius, float outerRadius, float opacity)
{
	m_volume->sculpt({ localPosition.X, localPosition.Y, localPosition.Z }, innerRadius, outerRadius, opacity);
}

FVector ACubiquityTerrainVolume::pickSurface(FVector localStartPosition, FVector localDirection) const
{
	bool success;
	auto hitLocation = m_volume->pickSurface({ localStartPosition.X, localStartPosition.Y, localStartPosition.Z }, { localDirection.X, localDirection.Y, localDirection.Z }, &success);

	if (!success)
	{
		UE_LOG(CubiquityLog, Log, TEXT("Surface pick found nothing"));
		return FVector::ZeroVector;
	}

	return { hitLocation.x, hitLocation.y, hitLocation.z };
}

void ACubiquityTerrainVolume::setVoxel(FVector position, const UCubiquityMaterialSet* materialSet)
{
	m_volume->setVoxel({ (int32_t)position.X, (int32_t)position.Y, (int32_t)position.Z }, *materialSet);
}

UCubiquityMaterialSet* ACubiquityTerrainVolume::getVoxel(FVector position) const
{
	const auto& voxel = m_volume->getVoxel({ (int32_t)position.X, (int32_t)position.Y, (int32_t)position.Z });
	auto materialSet = NewObject<UCubiquityMaterialSet>();
	materialSet->m_materialSet = voxel;
	return materialSet;
}
