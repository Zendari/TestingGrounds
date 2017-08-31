// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


AActor* UActorPool::Checkout()
{
	//If nothing in pool
	if (Pool.Num() == 0)
	{
		return nullptr;
	}
	//Return element type of the array
	return Pool.Pop(true);
}

void UActorPool::Add(AActor* ActorToAdd)
{
	//Add Actor to the pool
	if (ActorToAdd == nullptr)
	{
		return;
	}
	Pool.Push(ActorToAdd);
}

void UActorPool::Return(AActor* ActorToReturn)
{
	Add(ActorToReturn);
}
