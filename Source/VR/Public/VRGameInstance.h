// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LuaManager.h"
#include "LuaActor.h"
#include "VRGameInstance.generated.h"

USTRUCT()
struct FLuaActorsArray
{
	GENERATED_BODY()

	FName className;
	TArray<ALuaActor*> actors;
};

UCLASS(BlueprintType)
class VR_API UVRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UVRGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	void RunLua();

	void ShutdownLua();

	UFUNCTION(BlueprintCallable)
	void OnReloadScript();

	UFUNCTION(BlueprintCallable)
	void OpenNewLuaScript();
protected:
	// This is where we will clean up, as the game is shut down
	virtual void Shutdown() override;

	virtual bool Tick(float DeltaSeconds);

	FDelegateHandle TickDelegateHandle;

	UPROPERTY(EditAnywhere)
	class ULuaActorsDataAsset *spawnClasses;

private:
	UPROPERTY(Transient)
	TArray<FLuaActorsArray> luaObjectsArray;

	class LuaManager luaManager;

	bool bLuaScriptingSystemLoadedSuccessfully = false;

	int32 createActor(const char* type, TArray<float>& args);
	void removeActor(const char* type, int32 id);
	int32 existActor(const char* type, int32 id);

	void setActorPosition(const char* type, int32 id, float x, float y, float z);
	void setActorRotation(const char* type, int32 id, float x, float y, float z);
	void sendMessageInActor(const char* type, int32 id, const char* msg);

	int32 ShouldHandleUseEvent(const char * type, int32 id) const;
	int32 isUseObject(const char* type, int32 id) const;
	int32 useObject(const char* type, int32 id);

	ALuaActor* searchForActor(const char* type, int32 id) const;

	void printString(const char* msg);
	
	
};
