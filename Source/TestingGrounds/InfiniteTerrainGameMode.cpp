// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"


void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	//While their are actors to count
	while (VolumeIterator)
	{
		AddToPool(*VolumeIterator);
		++VolumeIterator; //To keep the pointer going to the next actor
	}
}

void AInfiniteTerrainGameMode::AddToPool(class ANavMeshBoundsVolume* BoundsVolumeToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Bounds Volume to add: %s"), *BoundsVolumeToAdd->GetName());
}

