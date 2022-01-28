/** *
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
#include "EmbNavDefines.h"
#include "mathutils.h"

#include "EmNavTypes.generated.h"

#define GRID_SIZE 100.0f

#define UP 0
#define UP_LEFT 1
#define LEFT 2
#define DOWN_LEFT 3
#define DOWN 4
#define DOWNRIGHT 5
#define RIGHT 6
#define UPRIGHT 7

#define PATH_COUNT 8

struct FGridPoint; // Forward Declaration
class AGridMarker; 

// We intend to use FGridPoint as a key so it must be hashable
uint32 GetTypeHash(const FGridPoint& Thing);

USTRUCT() 
struct FGridPoint{
	GENERATED_BODY()

    int32_t x;
    int32_t y;

    uint32 hash() const;

    inline bool operator == (const FGridPoint& rhs) const { 
        return (x == rhs.x) && (y == rhs.y);
    }
};


USTRUCT(Blueprintable)
struct FGridNode{

    /** 
     * Gridnode is the programmatic construt for 
     * performing calculations with regards to 
     *
     * the custom navigation system.
     * */

    GENERATED_BODY()
    
    AGridMarker* root;
    uint32_t ism_index;
    FGridPoint pos;

    // I'm going to be dumb and assume that we 
    // will never have a node that can access more than 8 nodes;
    //
    // The first four paths are harcoded
    //
    // Up, UpLeft, Left, DownLeft,
    // Down, DownRight, Right, UpRight
    //      are indicies 4, 5, 6, 7
   
    FGridNode* path_nodes[PATH_COUNT];
    float path_costs[PATH_COUNT];
    bool isValid;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector location;

    // getPath(<UP/DOWN/LEFT/RIGHT>)
    inline FGridNode* GetNext(uint32_t in, FGridNode* start) {
        if (path_nodes[in])
        {
            if(path_nodes[in]->isValid || (path_nodes[in] == start))
            {
                return path_nodes[in]; 
            }
        }

        return nullptr;
    };

    inline FGridNode* GetAtDir(uint32_t in) {
        if (path_nodes[in])
        {
            if(path_nodes[in]->isValid)
            {
                return path_nodes[in]; 
            }
        }

        return nullptr;
    };
    inline float GetCost(uint32_t in) { return path_costs[in]; };

    static FGridNode* GetNearestNode(FVector location, UWorld* world);
    static FGridNode* GetNearestNodeAtPoint(FVector location, UWorld* world);
};

USTRUCT(Blueprintable)
struct FGridPath{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGridNode> path;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float total_cost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool is_complete;

    FGridPath();

    void ParsePathPoints(TArray<FVector>* opath); // deprecated

    void AllPathPoints(TArray<FVector>* opath); // deprecated
};

USTRUCT(Blueprintable)
struct FGridPathVectors {
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> path;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float total_cost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool is_complete; //!< if the path is not a complete path then this flag is set

};

USTRUCT(Blueprintable)
struct FSpanNode{
    GENERATED_BODY()

    FVector location;
    float cost;
};

USTRUCT(Blueprintable)
struct FGridSpan{
    GENERATED_BODY()
    TArray<FSpanNode> nodes;
};


USTRUCT(Blueprintable)
struct FSearchedNode{
    GENERATED_BODY()
    FGridNode* node;
    float fcost;
    float dist;
    FGridNode* shortestkey;
    bool visited;

    FSearchedNode();
    FSearchedNode(FVector location, UWorld* world);
    FSearchedNode(FGridNode* gridnode);

    inline FGridNode* GetNextGridNode(uint32_t in, FGridNode* start) { return node->GetNext(in, start); };
    inline float GetCost(uint32_t in) { return node->GetCost(in); };
};

USTRUCT(Blueprintable)
struct FGridSpanSearchParams{
    GENERATED_BODY()

    bool ignore_isValid;

    FGridSpanSearchParams(){ignore_isValid = true;}
};

struct FGridSpanKey{
    FGridNode* node;
    int dir;

    uint32 hash(){
        uint32 Hash = FCrc::MemCrc32(this, sizeof(FGridSpanKey));
        return Hash;
    };

    FGridSpanKey(FGridNode* _node, int _dir)
    {
        memset(this, 0, sizeof(FGridSpanKey)); // because man fuck windows size padding
        node = _node;
        dir = _dir;
    }
};
