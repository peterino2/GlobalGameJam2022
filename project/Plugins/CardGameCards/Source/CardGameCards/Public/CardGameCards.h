// CREATED FOR A GAMEJAM

#pragma once

#include "Modules/ModuleManager.h"

class FCardGameCardsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
