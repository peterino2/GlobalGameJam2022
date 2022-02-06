// CREATED FOR A GAMEJAM

#pragma once

#include "CoreMinimal.h"
#include "VillainfamousCard.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "VillainfamousSoloBoard.generated.h"

UCLASS()
class CARDGAMECARDS_API AVillainfamousSoloBoard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVillainfamousSoloBoard();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UArrowComponent*> EnemyPositions;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UArrowComponent* CenterPosition;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UArrowComponent*> PlayerPositions;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float width = 105.0f;
	
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Board")
	void ConstructBoardInEditor();

	UFUNCTION(BlueprintCallable)
	void ResolveCard(AVillainfamousCard* Card);

	UFUNCTION(BlueprintCallable)
	void FinishResolvingCard(AVillainfamousCard* Card);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AVillainfamousCard* CurrentPlayerCard;
	
	UFUNCTION(BlueprintCallable)
	void AddPlayerCard(AVillainfamousCard* Card);
	
	// an encounter is a set of enemy cards that you will have to defeat
	UFUNCTION(BlueprintCallable)
	void LoadEncounter(TArray<AVillainfamousCard*> Cards);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
