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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "EmbTdCamera.generated.h"

UENUM(BlueprintType)
enum class CameraState: uint8{
    CameraFree,
    CameraSoftFollow
};


UCLASS()
class EMBTD_API AEmbTdCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEmbTdCamera(const FObjectInitializer& ObjectInitializer);
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	// FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/

	void SetRotate(float rotate);

	FRotator GetExplicitBoomRotation();

	void SetRotateExplicit(FRotator new_rotation);

	void SetZoomIn(float zoomin);
    
    AActor* followtarget; // Any actor can be a follow target

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float zoom_sensitivity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float top_angle; // rotation of the camera boom at the top of the zoom in

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float bottom_angle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float deadzone_start;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float camera_max;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float camera_min;

    float sensitivity = 1.0;
    float hsensitivity = 1.0;

	float swing;

	float rotate;
	float zoomin;
	float slowUpdateTick;
    
	FVector velocity;
	/** Top down camera */

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FVector GetFocalLocation(void);

	void handle_transformation(float dt);
	void handle_zoomin(float dt);

	/** Description of player information. */
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = PlayerMeta, meta = (AllowPrivateAccess = "true"))
	int playerId;
};
