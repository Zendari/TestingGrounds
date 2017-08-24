// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors()
{
	FVector Min(0, 2000, 0);
	FVector Max(4000, 2000, 0);
	FBox BoxBounds(Min, Max);
	
	for(size_t i = 0 ; i <= 20 ; i++) //TODO remove magic number 20
	{
		FVector SpawnPoint = FMath::RandPointInBox(BoxBounds);
		UE_LOG(LogTemp, Warning, TEXT("Spawn point : %s"),*SpawnPoint.ToString());
	}

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

