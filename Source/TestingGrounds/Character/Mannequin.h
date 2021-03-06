// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mannequin.generated.h"

UCLASS()
class TESTINGGROUNDS_API AMannequin : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AMannequin();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//When player dies, reattach the gun to Mesh
	UFUNCTION(BlueprintCallable, Category = "Death")
	void Death();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullTrigger();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetAmmo();

	/**Used to spawn the gun*/
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AGun> GunBlueprint;

	/*Amount of Ammo*/
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	float AmmoNumber;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	AGun* Gun;
	
};
