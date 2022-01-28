/** *
 * Copyright (C) 2020-2021
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
#include "GameFramework/PlayerController.h"
#include "EmbTdCamera.h"
#include "EmbTdCharacter.h"
#include "EmbTdControllerPB.generated.h"

UENUM(BlueprintType)
enum class ControllerState: uint8{
    ControllerDefault,
    ControllerTargeting,
    ControllerPaused,
    ControllerDialogue
};

UCLASS()
class EMBTD_API AEmbTdControllerPB : public APlayerController
{
	GENERATED_BODY()

public:
	AEmbTdControllerPB();

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Characters")
    TArray<AEmbTdCharacter*> party_members;
    
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Characters")
    AEmbTdCharacter* active_character;

    // Blueprint character callback
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterSelect(AEmbTdCharacter* selected_character);

	UFUNCTION(BlueprintCallable)
	void SelectCharacter(AEmbTdCharacter* selected_character);

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    ControllerState CurrentControllerState;

    AEmbTdCamera* CameraPawn;

protected:

    void HandleCameraMovement();
    void HandleCameraRotation();
    void HandleCamera();

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    float move_ratelimit;
    float move_lastupdate;
    bool move_lastmoveto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true")) 
    float bounds_percentage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true")) 
    float sensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true")) 
    float edge_sensitivity;

    bool edge_movement;

    FVector velocityInputAxis;

    FRotator camera_rotation_cache;
    float camera_mousex_cache;
    float camera_mousey_cache;
    bool rotate_button_down;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

    float mouse_x, mouse_y;
    int viewport_x, viewport_y;
    void HandleCameraEdges();

	// End PlayerController interface

	/** Input handlers for AltSelect action. */
	void OnAltSelectPressed(); // The better Name for this is Right Click
	void OnAltSelectReleased();

	/** Input handlers for camera movement. */
	void OnMoveForwardAxis(float axis_value);
	void OnMoveRightAxis(float axis_value);

	void OnEscapePressed();
	void OnZoomIn_Inner(float value);

	void OnZoomInWheel();
	void OnZoomOutWheel();
    
	void OnRotateRightAxis(float value);

    // Middle mouse button will cause the camera to switch into a rotation
    // mode whereby moving the mouse left/right will cause the camera 
    // to rotate.
    void OnRotateControlPressed();
    void OnRotateControlReleased();

	void OnSelectPressed();
	void OnSelectReleased();
    void HandleOpenWorldMovement(float DeltaTime);
	void HandleUpdateControllerState();
};

