// CREATED FOR A GAMEJAM

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetworkedCardsController.generated.h"

UCLASS()
class CARDGAMECARDS_API ANetworkedCardsController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANetworkedCardsController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void FlipCard(AActor* card, bool force=false);

	// Input binds
	UFUNCTION(BlueprintNativeEvent)
	void OnSelectPressed();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnSelectReleased();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnAltSelectPressed();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnAltSelectReleased();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnEscapePressed();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnMoveForwardAxis(float axis);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnMoveRightAxis(float axis);

	UFUNCTION(BlueprintNativeEvent)
	void OnZoomInWheel();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnZoomOutWheel();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRotateControlReleased();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRotateControlPressed();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRotateRightAxis(float axis);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
