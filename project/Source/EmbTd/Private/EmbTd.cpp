// Copyright Epic Games, Inc. All Rights Reserved.

#include "EmbTd.h"
#include "Modules/ModuleManager.h"

#include "Log.h"

/******************************************************************************
* Function:         virtual void FEmbTd::StartupModule
* Description:      
* Where:
* Return:           
* Error:            
*****************************************************************************/
void FEmbTd::StartupModule(){
    UE_LOG(LogEmbTd, Log, TEXT("EmbTd module starting up"));
}

/******************************************************************************
* Function:         
* Description:      
* Where:
* Return:           
* Error:            
*****************************************************************************/
void FEmbTd::ShutdownModule(){
    UE_LOG(LogEmbTd, Log, TEXT("EmbTd module shutting down"));
}

 
IMPLEMENT_PRIMARY_GAME_MODULE(FEmbTd, EmbTd, "EmbTd" );
