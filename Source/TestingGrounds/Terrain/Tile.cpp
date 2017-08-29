// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "AI/Navigation/NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "Engine/World.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NavMeshBoundsOffset = FVector(2000, 0, 0);
	MinExtent = FVector(0,-2000,0);
	MaxExtent = FVector(4000, 2000, 0);
}

void ATile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting the pool : %s"),*InPool->GetName());
	Pool = InPool;

	PositionNavMeshBoundsVolume();

}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshVolume = Pool->Checkout();

	if (!NavMeshVolume)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] No more navmesh in pool"),*GetName());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out:{%s}"),*GetName(),*NavMeshVolume->GetName());
	//Center the navmesh onto the tile
	NavMeshVolume->SetActorLocation(GetActorLocation() + NavMeshBoundsOffset);
	//Used to build the navmesh
	GetWorld()->GetNavigationSystem()->Build(); 
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, const FSpawnArguments& SpawnArgs)
{
	RandomlyPlaceActors(ToSpawn, SpawnArgs);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, const FSpawnArguments& SpawnArgs)
{

	RandomlyPlaceActors(ToSpawn, SpawnArgs);
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	//Place an actor with some properties
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);
	if (Spawned)
	{
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn,SpawnPosition.Location,Rotation);
	if(Spawned)
	{
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
	}
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, const FSpawnArguments& SpawnArgs)
{
	int ActorsToSpawn = FMath::RandRange(SpawnArgs.MinSpawn, SpawnArgs.MaxSpawn);
	//Spawning Actors loop
	for (size_t i = 0; i <= ActorsToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(SpawnArgs.MinScale, SpawnArgs.MaxScale);
		//Checking we can spawn at this Spawnpoint
		bool bFound = FindEmptyLocation(SpawnPosition.Location, SpawnArgs.Radius * SpawnPosition.Scale);
		if (bFound)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

bool ATile::FindEmptyLocation(FVector &OutLocation, float Radius)
{
	FBox BoxBounds(MinExtent, MaxExtent);
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

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Pool != nullptr && NavMeshVolume != nullptr)
	{
		//Called when Tile destroyed
		Pool->Return(NavMeshVolume);
	}
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
	return !HasHit;
}


