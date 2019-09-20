// Fill out your copyright notice in the Description page of Project Settings.

#include "LuaActor.h"
#include "../Public/LuaActor.h"

// Uncomment when methods will be ready
//Lunar<ALuaActor>::RegType ALuaActor::methods[] = {
//  LUNAR_DECLARE_METHOD(ALuaActor, SetActive),
//  LUNAR_DECLARE_METHOD(ALuaActor, SetAnimation),
//  {0,0}
//};

// Sets default values
ALuaActor::ALuaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void ALuaActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALuaActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALuaActor::SendMessage(FString message)
{
	SetMessage(message);
}

void ALuaActor::Pikup(USceneComponent* motionController)
{
	Pikuped(motionController);
}

void ALuaActor::Drop()
{
	Droped();
}

void ALuaActor::Trigger()
{
	Triggered();
}

void ALuaActor::Use()
{	
	IsUse = !IsUse;
	bHaveUnhandledUseEvent = true;
	Used();
	UE_LOG(LogTemp, Display, TEXT("Used"));
}

bool ALuaActor::HaveUnhandledUseEvent() const
{
	if (bHaveUnhandledUseEvent)
	{
		bHaveUnhandledUseEvent = false;
		return true;
	}
	
	return false;
}

