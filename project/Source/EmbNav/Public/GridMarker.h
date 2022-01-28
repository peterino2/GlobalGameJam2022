/**
 * Copyright (C) 2021-2021
 *
 * Peter Li - All Rights Reserved
 * Unauthorized copying of this file, 
 * or any of the code content within the Emb source directory
 * via any medium is strictly prohibited
 * Proprietary and confidential
 * 
 * Peter Li <Peterlimail47@gmail.com>
 * */

#pragma once

// Unreal includes

#include "CoreMinimal.h"
#include "EmbNavDefines.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Volume.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "mathutils.h"

#include "EmNavTypes.h"
#include "EmNavDjk.h"

#include "GridMarker.generated.h"


struct NodeInfo_t{
	
	uint32_t index;
	AGridMarker* root;
	UInstancedStaticMeshComponent* ism;

	NodeInfo_t(): index(0),
		root(nullptr),
		ism(nullptr)
	{};

	NodeInfo_t(const FHitResult& hit);
	NodeInfo_t(FVector location, UWorld* world);

	FVector GetLocation();
	bool GetTransform(FTransform& trans);
    void initFromNearest(FVector location, UWorld* world);

    static NodeInfo_t GetNearestNode(FVector location);
};



UCLASS()
class EMBNAV_API AGridMarker : public AActor
{
	GENERATED_BODY()

    TMap<int32_t, FGridNode>        GridNodeMap; // Change this to TArray later?
    TMap<int32_t, FGridNode*>       GridOverlay; // key is 16 bit shifted y  + x
    
    float slowUpdateTick;
    int32_t x_max;
    int32_t y_max;

    UFUNCTION(BlueprintCallable, CallInEditor, Category="ControlGrid")
    void rtInit();

    void generateLinks(FGridNode* this_node);
	
public:	
	// Sets default values for this actor's properties
	AGridMarker();
    UFUNCTION(BlueprintCallable, CallInEditor, Category="ControlGrid")
    static TArray<FVector> GetSpanLocations(FGridSpan span);

    FVector rot_fwd;
    FVector rot_up;     
    FVector rot_right;

    /**
     * Add path request parameters
     *
     * expand PathToIsmByLocation into a path request class
     *
     * */

    // deprecated -- start
	UFUNCTION(BlueprintCallable, Category = "ControlGrid") 
    static FGridPath PathToIsmByLocation(FVector start, FVector location, UWorld* WorldContextObject, float max_cost/*Parameters*/);

	UFUNCTION(BlueprintCallable, Category = "ControlGrid", meta=(WorldContext="WorldContextObject")) 
    static FGridPath PathToIsmByLocationBP(UWorld* WorldContextObject, FVector start, FVector location, float max_cost/*Parameters*/);

	UFUNCTION(BlueprintCallable, Category = "ControlGrid", meta=(WorldContext="WorldContextObject"))
    static FGridSpan GetSpanSynchronously(UObject* WorldContextObject, FVector start, float cost/*Parameters*/);

	UFUNCTION(BlueprintCallable, Category = "ControlGrid", meta=(WorldContext="WorldContextObject"))
    static FGridSpan GetSpanExtends(UObject* WorldContextObject, FVector start, float cost/*Parameters*/);

	UFUNCTION(BlueprintCallable, Category = "ControlGrid", meta=(WorldContext="WorldContextObject"))
    static FGridSpan GetSpanExtendsOptimized(UObject* WorldContextObject, FVector start, float cost/*Parameters*/);

	UFUNCTION(BlueprintCallable, Category = "ControlGrid", meta=(WorldContext="WorldContextObject"))
    static FGridPathVectors GetVectorsToIsm(
            FVector start, 
            FVector location, 
            UWorld* WorldContextObject, 
            float max_cost, 
            bool optimzed 
    );

    /**
     * 
     * */
	UFUNCTION(BlueprintCallable, Category = "ControlGrid", meta=(WorldContext="WorldContextObject"))
    static FGridSpanMulti GetMultiSpanOptimized(
            UObject* WorldContextObject, 
            FVector start, 
            float cost, 
            FGridSpanSearchParams params 
    );

	UFUNCTION(BlueprintCallable, Category = "ControlGrid")
		void SetGridVisible(bool visibile);

    FGridNode* getGridNodeAtIndex(int32_t key);
    FGridNode* getGridNodeAtPoint(FGridPoint point);

    // sets the cost of this node
    //
    //
    // use halfiter=false for normal uses
    //  halfiter will cause the node to only update the top 4 positions
    //
    //  Assumes that the generateLinks has been called for every single node already;
    
    //Sends an update to all EmCharaBase actors notifying them of a navmesh update.
    static void SendGridNodesUpdate(UWorld* world); 

    void ScanCostAtNode(FGridNode* this_node, float cost, bool halfiter = false);

    // Function for testing only,
    // Only gridmarkers should rescan or better yet implement 
    // a marked actor iterator scan.
    void RescanCosts();

    /**
     * Given a Fvector location, return the NodeInfo_t for the 
     * Nearest GridMarker and ISM.
     * */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* ism;

	inline UInstancedStaticMeshComponent* get_ism() {
		if (ism)
		{
			return ism;
		}
		else 
		{
			SHOW_FVECTOR(GetActorLocation(), "No ism created for gridmarker??? This should be impossible");
			return nullptr;
		}
	}
protected:

	
	// generate a normal and a plane
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* gridarea;

	NodeInfo_t GetGridNode(const FHitResult& hitresult);


	int AddNode(const FTransform& ft, int32_t , int32_t y);
	void ClearNodes();

	void OnConstruction(const FTransform& Transform);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

