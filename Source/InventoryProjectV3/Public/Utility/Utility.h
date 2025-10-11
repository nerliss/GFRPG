// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"

// Print string on screen macro
#define DEBUGMESSAGE(Duration, Color, Message, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FString::Printf(TEXT(Message) __VA_OPT__(,) __VA_ARGS__));}

// Wrapper for log with automatic function name
#define LOG_WITH_FUNCTION_NAME(Category, Verbosity, Format, ...) UE_LOG(Category, Verbosity, TEXT("[%s] " Format), TEXT(__FUNCTION__), ##__VA_ARGS__)

class INVENTORYPROJECTV3_API Utility
{
public:
	Utility();
	~Utility();
};
