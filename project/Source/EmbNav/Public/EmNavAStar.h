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

// Unreal engine Includes
#include "CoreMinimal.h"
#include "Engine/World.h"

// Embersong includes
#include "EmbNavDefines.h"
#include "EmNavTypes.h"

#include "EmNavAstar.generated.h"

struct FAStarCtx;

USTRUCT(Blueprintable)
struct FAStarNode{
    GENERATED_BODY()

    FGridNode* node;
    FGridNode* shortestkey;     // refernce key to the shortest path

    /* Internal Algo based */
    float fcost; // Overall Comparison heuristic
    float hcost; // movement traveled heuristic
    float gcost; // distance heuristic 

    bool visited;

    float DistToDest;       
    float MoveCostForShortest; // saves me from doing some extra hashes

    // ----- methods (c++ only) ----------

    FAStarNode(); 

    FAStarNode(FVector dest, FGridNode* node);
    FAStarNode(FVector dest, FVector location, UWorld* world);

    void CalculateNeighbourCosts(FAStarCtx& ctx);

    inline FGridNode* GetNextGridNode(uint32_t in, FGridNode* startkey) 
    {
        return node->GetNext(in, startkey); 
    };

    inline float GetCost(uint32_t in) { return node->GetCost(in); }
};

USTRUCT(Blueprintable)
struct FAStarCtx{

    GENERATED_BODY()

    TMap<FGridNode*, FAStarNode> searched;

    FGridNode* destkey;
    FGridNode* startkey;
    FGridNode* currkey;

    UWorld* world;
    float total_cost;
    bool no_path;
    bool partial_mode;
    FVector Dest;
    float acceptable_radius;

    UPROPERTY(Visibleanywhere, BlueprintReadOnly)
    bool done;
    bool initialized;

    float closest_hcost;
    float closest_dist;
    FGridNode* closest_key;

    FAStarCtx() {  initialized = false; } // default constructor... do not use
    FAStarCtx(FVector start, FVector dest, UWorld* _world);

    inline void SetRadius(float acceptance) { acceptable_radius = acceptance; };

    FAStarNode* searchAtKey(FGridNode* key) {return searched.Find(key);}

    void CalculateNeighbourCosts();
    
    void next();

    bool isDone() {
        if (no_path) {
            return true;
        }
        else
        {
            if (auto tdest = searchAtKey(destkey))
            {
                return tdest->visited;
            }
        }
        return false;
    };

    // if isDone will garuntee return the shortest path
    FGridPathVectors GetDestinationPath(bool optimized=false); 

    FGridPath GetDestinationPathInner(); 
};

UCLASS(Blueprintable)
class EMBNAV_API UEmNavLibHelper: public UObject 
{
    GENERATED_BODY()

public:
    UEmNavLibHelper(){}

    // returns an FGridPathVectors synchronously, sets partial path if the 
	UFUNCTION(BlueprintCallable, Category = "Nav3") 
    static FGridPathVectors PathToLocationSynchronous( UWorld* WorldContextObject, FVector start, FVector dest, float radius=141.0f);

	UFUNCTION(BlueprintCallable, Category = "Nav3") 
    static FGridPathVectors PathToLocationSynchronousOptimized( UWorld* WorldContextObject, FVector start, FVector dest, float radius=141.0f);

	UFUNCTION(BlueprintCallable, Category = "Nav3") 
    static FAStarCtx StartPathFindingTask(UWorld* WorldContextObject, FVector start, FVector dest, float radius=0.0f); 

	UFUNCTION(BlueprintCallable, Category = "Nav3") 
    static bool PathFindingTaskNext(FAStarCtx& ctx);
   
	UFUNCTION(BlueprintCallable, Category = "Nav3") 
    static FGridPathVectors PathFindingTask_GetBestPath(FAStarCtx& ctx); 
};

