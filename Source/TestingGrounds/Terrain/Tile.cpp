// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	int ActorsToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	
	//Spawning Actors loop
	for(size_t i = 0 ; i <= ActorsToSpawn ; i++) 
	{
		FVector SpawnPoint;
		//Checking we can spawn at this Spawnpoint
		bool bFound = FindEmptyLocation(SpawnPoint, Radius);
		if (bFound)
		{
			PlaceActor(ToSpawn, SpawnPoint);
		}	
	}
}

bool ATile::FindEmptyLocation(FVector &OutLocation, float Radius)
{
	FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);
	FBox BoxBounds(Min, Max);
	const int MAX_TRIES = 100;
	for (int i = 0; i <= MAX_TRIES; i++)
	{
		//Generating random Testpoint in the tile
		FVector TestPoint = FMath::RandPointInBox(BoxBounds);
		
		// if the Testpoint didn't hit anything
		if (CanSpawnAt(TestPoint,Radius)) 
		{
			//The Testpoint is valid
			OutLocation = TestPoint;
			return true;
		}
	}
	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint)
{
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::CanSpawnAt(FVector Location, float Radius)
{
	FHitResult HitResult;

	//Used to make points across all the tiles, not only the first one
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	//If this bool is true, we have hit something
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity, //~=0 rotation
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	//HasHit is true, return Red. HasHit is false return Green
	// A ? B : C : Is A true, if yes return B, if not return C
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);

	return !HasHit;
}

