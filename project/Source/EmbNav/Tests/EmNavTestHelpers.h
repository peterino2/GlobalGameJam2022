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

#include "EmNavTypes.h"

struct FGridNodeGraph{
    TArray<FGridNode> nodes;
    TMap<uint32, FGridNode*> overlay;
    
    FGridNode* GetAtPos(int x, int y);
    FGridNode* GetAtPos(FGridPoint pos);

    void GenerateLinks(FGridNode* this_node);

    FGridNodeGraph();

    FGridNodeGraph(int xmax, int ymax);
};

