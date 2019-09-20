// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Public/LuaActor.h"
#include "LuaActorsDataAsset.generated.h"

USTRUCT()
struct FSpawnActorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName actorName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALuaActor> actorClass;
};

/**
 * 
 */
UCLASS()
class ULuaActorsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FSpawnActorData> actorClasses;
	
};
