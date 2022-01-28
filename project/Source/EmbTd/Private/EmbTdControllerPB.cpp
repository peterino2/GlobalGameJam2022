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
#include "EmbTdControllerPB.h"
#include "mathutils.h"
#include "Log.h"

AEmbTdControllerPB::AEmbTdControllerPB()
{
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bShowMouseCursor = true;

	velocityInputAxis = FVector(0.f, 0.f, 0.f);

	move_ratelimit = 1.0/5; // ratelimit 1/N times per second.
	move_lastupdate = 0.0;
	move_lastmoveto = false;
    rotate_button_down = false;

    CurrentControllerState = ControllerState::ControllerDefault;
    active_character = nullptr;
	sensitivity = 1200.0;
    edge_sensitivity = sensitivity * 2;
    bounds_percentage = 0.05;

    edge_movement = false;
}

void AEmbTdControllerPB::BeginPlay() 
{
    Super::BeginPlay();
    CameraPawn = Cast<AEmbTdCamera>(GetPawn());
}

void AEmbTdControllerPB::HandleCameraMovement()
{
    float s = edge_movement ? edge_sensitivity : sensitivity;
    CameraPawn->AddMovementInput(velocityInputAxis.GetSafeNormal(), s, false);
    edge_movement = false;
}

void AEmbTdControllerPB::HandleCameraRotation()
{
    if (rotate_button_down) {
        FRotator target_rotation = camera_rotation_cache;
        float yaw_delta = ((mouse_x - camera_mousex_cache) / viewport_x) * 360;
        target_rotation.Yaw += yaw_delta;
        CameraPawn -> SetRotateExplicit(target_rotation);
    }
}

void AEmbTdControllerPB::HandleCameraEdges() 
{
#ifndef WITH_EDITOR // disable edge movement if the editor is not here
    // This is so the the size is the same regardless
    int boundary_left = bounds_percentage * viewport_y;
    int boundary_right = viewport_x - boundary_left;

    int boundary_top = bounds_percentage * viewport_y; 
    int boundary_bot = viewport_y - boundary_top;
    
    if (mouse_x < boundary_left)       {edge_movement = true; velocityInputAxis.Y += -1;} 
    if (mouse_x > boundary_right) {edge_movement = true; velocityInputAxis.Y += 1;} 

    if (mouse_y < boundary_top)       {edge_movement = true; velocityInputAxis.X += 1;} 
    if (mouse_y > boundary_bot) {edge_movement = true; velocityInputAxis.X += -1;} 

    velocityInputAxis.X = CLAMP(velocityInputAxis.X, 1.0, -1.0);
    velocityInputAxis.Y = CLAMP(velocityInputAxis.Y, 1.0, -1.0);
#endif
}

void AEmbTdControllerPB::HandleCamera()
{
    HandleCameraEdges();
    HandleCameraMovement();
    HandleCameraRotation();
}

void AEmbTdControllerPB::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    if (!CameraPawn)
    {
        UE_LOG(LogEmbTd, Warning, TEXT("Pawn not registered?? retrying."));
        CameraPawn = Cast<AEmbTdCamera>(GetPawn());
    }

    GetMousePosition(mouse_x, mouse_y);
    GetViewportSize(viewport_x, viewport_y);

    if(CameraPawn)
    {
        HandleCamera();
    }
}

void AEmbTdControllerPB::SelectCharacter(AEmbTdCharacter* character)
{
    // Do c++ portion
    // Call blueprint overrides.   
    OnCharacterSelect(character);
}

void AEmbTdControllerPB::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("AltSelect", IE_Pressed, this, &AEmbTdControllerPB::OnAltSelectPressed);
	InputComponent->BindAction("AltSelect", IE_Released, this, &AEmbTdControllerPB::OnAltSelectReleased);

	InputComponent->BindAction("Select", IE_Released, this, &AEmbTdControllerPB::OnSelectReleased);
	InputComponent->BindAction("Select", IE_Pressed, this, &AEmbTdControllerPB::OnSelectPressed);

	InputComponent->BindAction("Escape", IE_Released, this, &AEmbTdControllerPB::OnEscapePressed);

	InputComponent->BindAxis("MoveForward", this, &AEmbTdControllerPB::OnMoveForwardAxis);
	InputComponent->BindAxis("MoveRight", this, &AEmbTdControllerPB::OnMoveRightAxis);
	
	InputComponent->BindAction("ZoomInWheel", IE_Pressed, this, &AEmbTdControllerPB::OnZoomInWheel); 
	InputComponent->BindAction("ZoomOutWheel", IE_Pressed, this, &AEmbTdControllerPB::OnZoomOutWheel); 

	InputComponent->BindAction("RotateControl", IE_Released, this, &AEmbTdControllerPB::OnRotateControlReleased); 
	InputComponent->BindAction("RotateControl", IE_Pressed, this, &AEmbTdControllerPB::OnRotateControlPressed); 

	InputComponent->BindAxis("RotateRightAxis", this, &AEmbTdControllerPB::OnRotateRightAxis); 
}

void AEmbTdControllerPB::OnSelectPressed()
{
    // do a raycast
    FHitResult hit;   
    GetHitResultUnderCursor(ECC_Visibility, true, hit);
    
    // after you do the raycast
    // any character that is selected will have the focus applied to them
        // focused character is not yet implemented
    
    // if the character you select is friendly and in the party
    // then primary control is switched over to that character.
    
    // If the character is not already selected 
    // then that character is set to focus.
    
    // If a character is already selected and we click on a walkable area, 
    // then either a move or interact command is given to the character.
    

    // (not in rev 2) FEmbTdPlayerCommand will have to be a class which is created.
    // Unit testing could be done significantly with that
}

void AEmbTdControllerPB::OnSelectReleased()
{
}

void AEmbTdControllerPB::OnAltSelectPressed() 
{
    // Todo
}

void AEmbTdControllerPB::OnAltSelectReleased() 
{
    // todo
}

void AEmbTdControllerPB::OnMoveForwardAxis(float axis_value)
{
	velocityInputAxis.X = CLAMP(axis_value, 1.0, -1.0);
}

void AEmbTdControllerPB::OnMoveRightAxis(float axis_value)
{
	velocityInputAxis.Y = CLAMP(axis_value, 1.0, -1.0);
}

void AEmbTdControllerPB::OnRotateRightAxis(float axis_value)
{
    if(!CameraPawn) return;
    CameraPawn->SetRotate(axis_value);
}

void AEmbTdControllerPB::OnEscapePressed()
{
    // todo
}

void AEmbTdControllerPB::OnZoomInWheel()
{
    if(!CameraPawn) return;
    CameraPawn->SetZoomIn(-1.0);
}

void AEmbTdControllerPB::OnZoomOutWheel()
{
    if(!CameraPawn) return;
    CameraPawn->SetZoomIn(1.0);
}

void AEmbTdControllerPB::OnRotateControlPressed() 
{
    UE_LOG(LogEmbTd, Warning, TEXT("RotateControl pressed"));
    camera_rotation_cache = CameraPawn->GetExplicitBoomRotation();
    rotate_button_down = true;
    GetMousePosition(camera_mousex_cache, camera_mousey_cache);
}

void AEmbTdControllerPB::OnRotateControlReleased()
{
    camera_rotation_cache = FRotator();
    camera_mousex_cache = 0;
    camera_mousey_cache = 0;
    rotate_button_down = false;
}
