/**
 * Copyright (C) 2021-2021
 *
 * Peter Li - All Rights Reserved
 * Unauthorized copying of this file, 
 * or any of the code content within the Embersong source directory
 * via any medium is strictly prohibited
 * Proprietary and confidential
 * 
 * Peter Li <Peterlimail47@gmail.com>
 * */
#include "EmbTdCamera.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"

#include "EmbTdControllerPB.h"
#include "mathutils.h"

// Sets default values
AEmbTdCamera::AEmbTdCamera(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	this->bReplicates = false;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

    // create 
    auto x = CreateDefaultSubobject<USceneComponent>(TEXT("RpgCameraRoot"));
	RootComponent = x;
	x->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// Create a camera boom
	
	zoom_sensitivity = 50.0;
	top_angle = 60.f; // rotation of the camera boom at the top of the zoom in

	// what we want is to create a linear map from 800.0f == -60  to 300.0f to -30 
	bottom_angle = 42.0f;
	deadzone_start = 800.0f;
	camera_max = 1200.0f;
	camera_min = 300.0f;
    slowUpdateTick = 0.0f;
	swing = deadzone_start - camera_min;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("RpgCameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 1200.f;
    CameraBoom->SetWorldRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera  
	// I would prefer to use an rts style camera so we canconstexpr ignore the camera boom i think
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("RpgCameraComponent"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
    TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

FRotator AEmbTdCamera::GetExplicitBoomRotation()
{
    return CameraBoom->GetComponentRotation();
}

// Called when the game starts or when spawned
void AEmbTdCamera::BeginPlay()
{
	Super::BeginPlay();
}

FVector AEmbTdCamera::GetFocalLocation()
{
	// send raycast from current x,y + 10,000z 
	// grab z-value (min(0,z)
	// return (x,y, z
	auto startVector1 = GetActorLocation() + FVector(0.0f, 0.0f, 10000.0f);
	auto startVector2 = RootComponent->GetComponentLocation();
	auto startVector = startVector1.Z > startVector2.Z ? startVector1 : startVector2;
	auto endVector = GetActorLocation();
	auto destLocation = GetActorLocation();
	endVector.Z = -100.0f;
	FHitResult hitresult;
	if (GetWorld()->LineTraceSingleByChannel(hitresult, startVector, endVector, ECC_GameTraceChannel1))
	{
		if (hitresult.bBlockingHit)
		{
			if (auto x = hitresult.GetActor()) 
			{
				if (!(x->GetClass()->IsChildOf(ACharacter::StaticClass()))) 
				{
					destLocation = hitresult.Location;
				}
			}
			else 
			{
				destLocation = hitresult.Location;
			}
		}
	}
	destLocation.Z = MAX(destLocation.Z + 0.0f, 0.0f);
	return destLocation;
}

// Called every frame
void AEmbTdCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// check if mouse hits edge of screen
	// if so, move the the camera in that direction

	handle_transformation(DeltaTime);
	handle_zoomin(DeltaTime);
}


// Called to bind functionality to input
void AEmbTdCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEmbTdCamera::SetRotate(float rotate_inp)
{
	rotate = -1 * rotate_inp;
}

void AEmbTdCamera::SetZoomIn(float zoomin_inp)
{
	zoomin = zoomin_inp;
}

void AEmbTdCamera::handle_zoomin(float dt)
{

	CameraBoom->TargetArmLength = CLAMP( CameraBoom->TargetArmLength + zoom_sensitivity * zoomin, camera_max, camera_min);
	// camera rotation algorithm
	auto len = CameraBoom->TargetArmLength;
	float target_angle = (((top_angle-bottom_angle)/(swing))*(len - camera_min) + bottom_angle);
	target_angle = CLAMP(target_angle, top_angle, bottom_angle);
	
	FRotator orientation = CameraBoom->GetRelativeRotation();
	orientation.Pitch = -target_angle;
    CameraBoom->SetRelativeRotation(orientation);

	zoomin = 0.0;
}

void AEmbTdCamera::SetRotateExplicit(FRotator new_rotation) 
{
	CameraBoom->SetRelativeRotation(new_rotation);
}

void AEmbTdCamera::handle_transformation(float dt)
{
	auto next_rotation = CameraBoom->GetComponentRotation() + FRotator(0.0f, rotate * dt * 140, 0.0f);
	// SetActorRotation(next_rotation);
	CameraBoom->SetRelativeRotation(next_rotation);
	rotate = 0.0f;

	auto target_location = this->GetFocalLocation();
	auto rotate_z = CameraBoom->GetComponentRotation().Yaw;
	auto move_vector = ConsumeMovementInputVector().RotateAngleAxis(rotate_z, { 0.0f,0.0f,1.0f });
	auto destination = GetActorLocation()  + move_vector * sensitivity * dt;
	auto thisloc = FMath::VInterpTo(destination, target_location, dt, hsensitivity);
	SetActorLocation(thisloc);
}


