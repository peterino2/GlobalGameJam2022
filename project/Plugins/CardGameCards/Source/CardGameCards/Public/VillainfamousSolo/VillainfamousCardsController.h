// CREATED FOR A GAMEJAM

#pragma once

#include "CoreMinimal.h"
#include "NetworkedCardsController.h"
#include "GameFramework/Actor.h"
#include "VillainfamousCardsController.generated.h"

UCLASS()
class CARDGAMECARDS_API AVillainfamousCardsController : public ANetworkedCardsController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVillainfamousCardsController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void HandleMouseOverCards(float DeltaTime);
};
