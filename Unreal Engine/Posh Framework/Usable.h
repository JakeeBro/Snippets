// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Usable.generated.h"

UENUM()
enum class EInteractionType : uint8
{
	Press UMETA(DisplayName = "Press"),
	Hold UMETA(DisplayName = "Hold")
};

UINTERFACE(MinimalAPI)
class UUsable : public UInterface
{
	GENERATED_BODY()
};

class POSH_API IUsable
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual void InitiateInteraction(AActor* Initiator, EInteractionType InteractionType);

	UFUNCTION(BlueprintNativeEvent, Category = "Usable")
	void Press(AActor* Initiator);

	UFUNCTION(BlueprintNativeEvent, Category = "Usable")
	void Hold(AActor* Initiator);
};
