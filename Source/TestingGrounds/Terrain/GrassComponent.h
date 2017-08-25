// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

/**
 * 
 */
UCLASS(Classgroup=("Custom"), meta = (BlueprintSpawnableComponent))
class TESTINGGROUNDS_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
public :
	UGrassComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	FBox SpawningExtents;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	int SpawnCount;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;


private : 

	void SpawnGrass();

};
