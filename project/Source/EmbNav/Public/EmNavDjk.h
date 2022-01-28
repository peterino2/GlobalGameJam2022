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

#include "EmNavDjk.generated.h"

/** 
 * Contains Non UOBJECT based implementation of 
 * 
 * Djkstra's algorithm.
 * */

USTRUCT(Blueprintable)
struct FGridSpanMulti{
    GENERATED_BODY()
    
    TArray<FGridSpan> spans;
    int unique_nodecount;
};

USTRUCT()
struct FDjkCtx{

    GENERATED_BODY()

    TMap<FGridNode*, FSearchedNode> searched;
    UWorld* world;
    FGridNode* destkey;
    FGridNode* startkey;
    FGridNode* currkey;
    FGridNode* closest_key; // when finished and no path is set and partial mode is set, this will point at the node in the search tree with the closest distance

    FGridPath* path;

    // FVector TargetDest; // if this is defined over here it fails. 
    // this suggests maybe platform specific  memory access inconsistencies.
    //
    // Suggest maybe converting this into a UFDjkCtx and subclass from UOBJECT
    
    float closest_dist;

    float max_cost; // only used for GetWalkableSpanSynchronously.


    bool partial_mode; // if set will use the tempary gridnode FakeDest
    bool extents_mode; // if set will ignore unset destkey and search all grids within max_cost regardless
    bool no_path;
    bool done;

    FVector TargetDest; 
    
    // void SetPartialMode(bool mode){partial_mode = mode;} // if partial_mode is set, then 
    FSearchedNode* searchAtKey(FGridNode* key) {return searched.Find(key);}
    FDjkCtx();
    FDjkCtx(FVector location, FGridPath* _path, UWorld* world);

    void setDestination(FVector destination); // single vector location initialization

    void setMaxCost(float _max_cost); // single vector location initialization

    void SetExtentsMode(bool mode); // single vector location initialization

    void SetStartKey(FGridNode* _startkey);

    void SetDestKey(FGridNode* _destkey); // will disable extents mode
    
    void next();

    float CalcDistCompare(FVector start, FVector end);
    
    void calcNeighbourCosts();
    
    FGridSpan GetSpanSynchronously(float max_cost);       // deprecated

    FGridSpan GetSpanExtends(float max_cost);             // deprecated

    FGridSpan GetSpanExtendsOptimized(float max_cost);    // deprecated

    FGridSpan GetSpanExtendsInner(FGridNode* start_edge, int init_dir, TMap<uint32, int>* keycheck);

    void OptimizeSpanExtends(FGridSpan* span);

    FGridSpanMulti GetMultiSpanOptimized(float max_cost, FGridSpanSearchParams params); // use this one for new stuff

    bool spanFound(); 

    bool isDone() { 
        auto dest = searchAtKey(destkey);
        bool visited = false;

        if(dest)
        {
            visited = dest->visited;
        }
        
        return visited || no_path; 
    }

    bool getDestinationPath(); // Only valid when isDone returns true, this will return true if a path is found.

    bool GetPathInner(FGridNode* target_key);
    bool GetPartialPath();
};
