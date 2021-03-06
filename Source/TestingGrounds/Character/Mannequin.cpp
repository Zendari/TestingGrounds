// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "../Weapons/Gun.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"


// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Setting the camera
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//Setting the arms of the player
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Arms"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	AmmoNumber = 25;
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
		//Setting the gun
	if (GunBlueprint == nullptr) 
	{
		return;
	}
	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);

	if (IsPlayerControlled())
	{
		//Attach gun component here, because it doesn't exist yet in the constructor
		Gun->AttachToComponent(
			Mesh1P, 
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), 
			TEXT("GripPoint"));
	}
	else
	{
		Gun->AttachToComponent(
			GetMesh(), 
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), 
			TEXT("GripPoint_0"));	
	}

	Gun->FP_AnimInstance = Mesh1P->GetAnimInstance();
	Gun->TP_AnimInstance = GetMesh()->GetAnimInstance();
	
	//Binding the fire input
	if (InputComponent != nullptr)
	{
		InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMannequin::PullTrigger);
	}

}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMannequin::Death()
{
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_0"));
}

void AMannequin::PullTrigger()
{
	if(AmmoNumber > 0)
	{
		Gun->OnFire();
		AmmoNumber--;
	}
	else
	{
		return;
	}
}

void AMannequin::Reload()
{
	AmmoNumber = 25;
}

float AMannequin::GetAmmo()
{
	return AmmoNumber;
}
