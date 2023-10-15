// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"

// Print string on screen macro
#define DEBUGMESSAGE(Duration, Color, Message, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FString::Printf(TEXT(Message), __VA_ARGS__));}


class INVENTORYPROJECTV3_API Utility
{
public:
	Utility();
	~Utility();
};
