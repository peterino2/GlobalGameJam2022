// CREATED FOR A GAMEJAM

#pragma once

#include "CoreMinimal.h"
#include "VillainfamousCard.h"
#include "GameFramework/Actor.h"
#include "VillainfamousEnemyCard.generated.h"

UCLASS()
class CARDGAMECARDS_API AVillainfamousEnemyCard : public AVillainfamousCard
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AVillainfamousEnemyCard();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Health = 5.0f;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float PublicSupport = 5.0f;
	
	UFUNCTION(BlueprintCallable)
	void TakeDamage(AActor* source, int amount, FGameplayTag DamageType);

	UFUNCTION(BlueprintNativeEvent)
	void OnDamageTakenBP(AActor* source, int amount, FGameplayTag DamageType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
