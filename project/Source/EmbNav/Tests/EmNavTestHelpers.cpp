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

#include "EmNavTestHelpers.h"

FGridNodeGraph::FGridNodeGraph(int xmax, int ymax)
{
    nodes.Empty();
    overlay.Empty();

    for(int x = 0; x < xmax; x++)
    {
        for(int y = 0; y < ymax; y++)
        {
            FGridNode node;
            node.root = nullptr;
            node.ism_index = x + y * xmax;
            node.pos.x = x;
            node.pos.y = y;
            node.location = FVector(100.0f*x, 100.0f*y, 0.0f);
            node.isValid = true;

            for (int i = 0; i < PATH_COUNT; i ++)
            {
                node.path_nodes[i] = nullptr;
                node.path_costs[i] = FLT_MAX;
            }

            int id = nodes.Add(node);
            overlay.Add(node.pos.hash(), nodes.GetData()+id);
        }
    }

    for(int x = 0; x < xmax; x++)
    {
        for(int y = 0; y < ymax; y++)
        {
            auto node = GetAtPos(x, y);

            // GenerateLinks(node);
        }
    }

}
FGridNodeGraph::FGridNodeGraph()
{
    nodes.Empty();
    overlay.Empty();
}

void FGridNodeGraph::GenerateLinks(FGridNode* this_node)
{
    auto thispoint = this_node -> pos;
    WARN_ASSERT(this_node == GetAtPos(thispoint), "");
	const FGridPoint sdirs[] = { 
		{ thispoint.x    , thispoint.y - 1 }, // UP
		{ thispoint.x - 1, thispoint.y - 1 }, // UP_LEFT
		{ thispoint.x - 1, thispoint.y     }, // LEFT
		{ thispoint.x - 1, thispoint.y + 1 }, // DOWN_LEFT

		{ thispoint.x    , thispoint.y + 1 }, // DOWN
		{ thispoint.x + 1, thispoint.y + 1 }, // DOWNRIGHT
		{ thispoint.x + 1, thispoint.y     }, // RIGHT
		{ thispoint.x + 1, thispoint.y - 1 }  // UPRIGHT
	};

    for(int dir = 0; dir < sizeof(sdirs)/sizeof(sdirs[0]); dir++)
    {
        auto* search = GetAtPos(sdirs[dir]);
        if(search)
        {
            int rdir = (dir + 4) % PATH_COUNT;
            float diagonal_modifier = dir % 2 ? 1.4 : 1;
            this_node->path_nodes[dir] = search; //assign to 
            this_node->path_costs[dir] = 1 * diagonal_modifier; //assign to 
            this_node->isValid = true;
        }
    }
}

FGridNode* FGridNodeGraph::GetAtPos(FGridPoint p)
{
    auto hash = p.hash();
    if(overlay.Contains(hash))
    {
        return overlay[hash];
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Hash at %d %d not available in this graph"), p.x, p.y);
        return nullptr;
    }
}

FGridNode* FGridNodeGraph::GetAtPos(int x, int y)
{
    return GetAtPos(FGridPoint{x, y});
}
