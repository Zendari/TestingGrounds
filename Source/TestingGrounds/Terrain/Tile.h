// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()
	FVector Location;
	float Rotation;
	float Scale;
};

USTRUCT(BlueprintType)
struct FSpawnArguments
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "FSpawnArguments")
	int MinSpawn = 1;

	UPROPERTY(EditDefaultsOnly, Category = "FSpawnArguments")
	int MaxSpawn = 1;

	UPROPERTY(EditDefaultsOnly, Category = "FSpawnArguments")
	float Radius = 500;

	UPROPERTY(EditDefaultsOnly, Category = "FSpawnArguments")
	float MinScale = 1; 

	UPROPERTY(EditDefaultsOnly, Category = "FSpawnArguments")
	float MaxScale = 1;
};

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, const FSpawnArguments& SpawnArgs );

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, const FSpawnArguments& SpawnArgs);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Offset of the navmesh
	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavMeshBoundsOffset;

	//Size of the tile, set defaults at constructor
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	FVector MaxExtent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	bool FindEmptyLocation(FVector &OutLocation,float Radius);

	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, const FSpawnArguments& SpawnArgs);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition);

	void PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition);

	bool CanSpawnAt(FVector Location, float Radius);

	void PositionNavMeshBoundsVolume();
	
	//Component holding the navmeshes
	class UActorPool* Pool;

	AActor* NavMeshVolume;
};


