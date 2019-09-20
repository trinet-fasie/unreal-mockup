// Fill out your copyright notice in the Description page of Project Settings.

#include "VRGameInstance.h"
#include "UnrealMathUtility.h"
#include "LuaActorsDataAsset.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Ticker.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Engine.h"

#include <cassert>

UVRGameInstance::UVRGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UVRGameInstance::Init()
{
	// Register delegate for ticker callback
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UVRGameInstance::Tick));
	RunLua();

	Super::Init();

	UE_LOG(LogTemp, Display, TEXT("[C++] Init"));
}
void UVRGameInstance::OnReloadScript()
{	
	ShutdownLua();
	RunLua();
}

void UVRGameInstance::OpenNewLuaScript()
{
	/*
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

			TArray<FString> OutFilenames;
			if (DesktopPlatform)
			{
				//Opening the file picker!
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
				DesktopPlatform->OpenFileDialog(ParentWindowHandle, FString("Select Lua File"), FString("C:/"), FString(""), TEXT("*.lua"), SelectionFlag, OutFilenames);
				// OpenFileDialog( const void* ParentWindowHandle, const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, const FString& FileTypes, uint32 Flags, TArray<FString>& OutFilenames)
			}
		}
	}
	*/
}

// Cleanup. This acts as a quasi-destructor
void UVRGameInstance::Shutdown()
{
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	ShutdownLua();
	Super::Shutdown();
}

void UVRGameInstance::RunLua()
{
	luaManager.Init();

	luaManager.SetFunction("Log", [this](ILuaStateWrap* state) -> int {
		assert(argCount < 1);
		const char *arr = state->GetStringArg(1).c_str();
		printString(arr);
		return 0;    // return amount of arguments
	});

	luaManager.SetFunction("CreateObject", [this](ILuaStateWrap* state) -> int32 {
		int32 argCount = state->GetArgumentsCount();
		assert(argCount < 1);
		const char *type = state->GetStringArg(1).c_str();
		int32 object_id = INDEX_NONE;

		static const int createActorNumberArgs = 6;
		TArray<float> args;
		for (int i = 1; args.Num() < createActorNumberArgs; ++i)
		{
			float arg = i <= argCount ? static_cast<float>(state->GetNumberArg(i + 1)) : 0.0f;
			args.Emplace(arg);
		}
		object_id = createActor(type, args);

		assert(object_id == INDEX_NONE);

		state->PushResultInteger(object_id);
		return 1;    // return amount of arguments
	});

	luaManager.SetFunction("RemoveObject", [this](ILuaStateWrap* state) -> int32 {
		assert(state->GetArgumentsCount() != 2);
		const char *type = state->GetStringArg(1).c_str();
		int32 id = state->GetIntegerArg(2);
		removeActor(type, id);
		return 0;    // return amount of arguments
	});

	luaManager.SetFunction("ExistObject", [this](ILuaStateWrap* state) -> int32 {
		assert(state->GetArgumentsCount() != 2);
		const char *type = state->GetStringArg(1).c_str();
		int32 id = state->GetIntegerArg(2);
		int32 result = existActor(type, id);
		state->PushResultInteger(result);
		return 1;    // return amount of arguments
	});

	luaManager.SetFunction("IsUseObject", [this](ILuaStateWrap* state) -> int32 {
		assert(state->GetArgumentsCount() != 2);
		const char *type = state->GetStringArg(1).c_str();
		int32 id = state->GetIntegerArg(2);
		int32 result = isUseObject(type, id);
		state->PushResultInteger(result);
		return 1;    // return amount of arguments
	});

	luaManager.SetFunction("ShouldHandleUseEvent", [this](ILuaStateWrap* state) -> int32 {
		assert(state->GetArgumentsCount() != 2);
		const char *type = state->GetStringArg(1).c_str();
		int32 id = state->GetIntegerArg(2);
		int32 result = ShouldHandleUseEvent(type, id);
		state->PushResultInteger(result);
		return 1;    // return amount of arguments
	});

	luaManager.SetFunction("UseObject", [this](ILuaStateWrap* state) -> int32 {
		assert(state->GetArgumentsCount() != 2);
		const char *type = state->GetStringArg(1).c_str();
		int32 id = state->GetIntegerArg(2);
		useObject(type, id);
		return 0;    // return amount of arguments
	});

	luaManager.SetFunction("SetPosition", [this](ILuaStateWrap* state) -> int32 {
		assert(state->GetArgumentsCount() != 5);
		const char *type = state->GetStringArg(1).c_str();
		int32 id = state->GetIntegerArg(2);
		float x = static_cast<float>(state->GetNumberArg(3));
		float y = static_cast<float>(state->GetNumberArg(4));
		float z = static_cast<float>(state->GetNumberArg(5));
		setActorPosition(type, id, x, y, z);
		return 0;    // return amount of arguments
	});

	luaManager.SetFunction("SetRotation", [this](ILuaStateWrap* state) -> int32 {
		assert(state->GetArgumentsCount() != 5);
		const char *type = state->GetStringArg(1).c_str();
		int32 id = state->GetIntegerArg(2);
		float roll = static_cast<float>(state->GetNumberArg(3));
		float pitch = static_cast<float>(state->GetNumberArg(4));
		float yaw = static_cast<float>(state->GetNumberArg(5));
		setActorRotation(type, id, roll, pitch, yaw);
		return 0;    // return amount of arguments
	});

	luaManager.SetFunction("SendMessageInObject", [this](ILuaStateWrap* state) -> int32 {
		assert(state->GetArgumentsCount() != 3);
		const char *type = state->GetStringArg(1).c_str();
		int32 id = state->GetIntegerArg(2);
		const char *message = state->GetStringArg(3).c_str();
		sendMessageInActor(type, id, message);
		return 0;    // return amount of arguments
	});

	bLuaScriptingSystemLoadedSuccessfully = luaManager.SetUpdatableScript("C:\\logic.lua", "Update", "Exit");

	if (bLuaScriptingSystemLoadedSuccessfully)
	{
		UE_LOG(LogTemp, Display, TEXT("[C++] Lua scripting system loaded successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[C++] Lua scripting system failed load"));
	}
}

bool UVRGameInstance::Tick(float DeltaSeconds)
{
	if (bLuaScriptingSystemLoadedSuccessfully)
	{
		luaManager.Update(DeltaSeconds);

	
	}

	return true;
}

void UVRGameInstance::ShutdownLua()
{
	luaManager.Exit();
	for (FLuaActorsArray& actorArray : luaObjectsArray)
	{
		for (ALuaActor* actor : actorArray.actors)
		{
			if (actor) actor->ConditionalBeginDestroy();
		}
	}
	luaObjectsArray.Empty(luaObjectsArray.Num());
}

int32 UVRGameInstance::createActor(const char* type, TArray<float>& args)
{
	// Get BP class name
	FString typeName = FString(UTF8_TO_TCHAR(type));
	typeName.ToLower();
	FName bpClassName = FName(*typeName);

	// Get BP class with specified name
	FSpawnActorData* actorData = spawnClasses->actorClasses.FindByPredicate(
		[bpClassName](const FSpawnActorData& data)
	{
		return data.actorName == bpClassName;
	});
	assert(actorData);

	// Get world
	UWorld* world = GetWorld();
	assert(world);

	// Create new actor with specified BP class
	FVector actorLocation = FVector(args[0], args[1], args[2]);
	FRotator actorRotation = FRotator(args[4], args[5], args[3]);//(pitch, yaw, roll), in rotation (roll, pitch, yaw)
	ALuaActor* newActor = world->SpawnActor<ALuaActor>(actorData->actorClass, actorLocation, actorRotation);

	// Search for specified BP class actors array
	FLuaActorsArray* luaActorsArray = luaObjectsArray.FindByPredicate(
		[bpClassName](const FLuaActorsArray& data)
	{
		return data.className == bpClassName;
	});

	// If there is no such array - then create it
	if (!luaActorsArray)
	{
		luaActorsArray = &luaObjectsArray[luaObjectsArray.Emplace(FLuaActorsArray())];
		luaActorsArray->className = bpClassName;
	}
	assert(luaActorsArray);

	// Add pounter to new actor into actors array
	int32 new_id = luaActorsArray->actors.IndexOfByPredicate(
		[](const ALuaActor* actor)
	{
		return actor == nullptr;
	});;
	if (new_id == INDEX_NONE)
	{
		new_id = luaActorsArray->actors.Emplace(newActor);
	}
	else
	{
		luaActorsArray->actors[new_id] = newActor;
	}
	assert(new_id == INDEX_NONE);

	UE_LOG(LogTemp, Display, TEXT("[C++] Create object of class %s id: %i,  x: %f, y: %f, z: %f, roll: %f, pitch: %f, yaw: %f"),
		*typeName, new_id, args[0], args[1], args[2], args[4], args[5], args[3]);

	return new_id;
}

int32 UVRGameInstance::existActor(const char* type, int32 id)
{
	// Get BP class name
	FString typeString = FString(UTF8_TO_TCHAR(type));
	typeString.ToLower();
	FName typeName = FName(*typeString);

	// Search for specified BP class actors array
	FLuaActorsArray* luaActorsArray = luaObjectsArray.FindByPredicate(
		[typeName](const FLuaActorsArray& data)
	{
		return data.className == typeName;
	});

	bool actorExist = false;

	if (luaActorsArray)
	{
		if (luaActorsArray->actors.IsValidIndex(id))
		{
			actorExist = luaActorsArray->actors[id] != nullptr;
		}
	}

	return actorExist ? 1 : 0;
}

void UVRGameInstance::removeActor(const char* type, int32 id)
{
	// Get BP class name
	FString typeString = FString(UTF8_TO_TCHAR(type));
	typeString.ToLower();
	FName typeName = FName(*typeString);

	// Search for specified BP class actors array
	FLuaActorsArray* luaActorsArray = luaObjectsArray.FindByPredicate(
		[typeName](const FLuaActorsArray& data)
	{
		return data.className == typeName;
	});

	assert(luaActorsArray);
	assert(luaActorsArray->actors[id]);

	luaActorsArray->actors[id]->ConditionalBeginDestroy();
	luaActorsArray->actors[id] = nullptr;

	//UE_LOG(LogTemp, Display, TEXT("[C++] remove of actor %s id: %i "), *type, id);
}

void UVRGameInstance::setActorPosition(const char * type, int32 id, float x, float y, float z)
{
	FVector loc = FVector(x, y, z);
	ALuaActor* actor = searchForActor(type, id);
	actor->SetActorLocation(loc);

	UE_LOG(LogTemp, Display, TEXT("[C++] set position of actor %s id: %i,  x: %f, y: %f, z: %f"), *type, id, x, y, z);
}

void UVRGameInstance::setActorRotation(const char * type, int32 id, float roll, float pitch, float yaw)
{
	FRotator actorRotation = FRotator(pitch, yaw, roll);
	ALuaActor* actor = searchForActor(type, id);
	actor->SetActorRotation(actorRotation);

	UE_LOG(LogTemp, Display, TEXT("[C++] set rotation of actor %s id: %i,  roll: %f, pitch: %f, yaw: %f"), *type, id, roll, pitch, yaw);
}

void UVRGameInstance::sendMessageInActor(const char * type, int32 id, const char * msg)
{
	FString message = FString(UTF8_TO_TCHAR(msg));
	ALuaActor* actor = searchForActor(type, id);
	actor->SendMessage(message);
}

int32 UVRGameInstance::ShouldHandleUseEvent(const char * type, int32 id) const
{
	ALuaActor* actor = searchForActor(type, id);
	return actor->HaveUnhandledUseEvent() ? 1 : 0;
}

int32 UVRGameInstance::isUseObject(const char * type, int32 id) const
{
	ALuaActor* actor = searchForActor(type, id);
	return actor->IsUsed() ? 1 : 0;
}

int32 UVRGameInstance::useObject(const char * type, int32 id)
{
	ALuaActor* actor = searchForActor(type, id);
	actor->Use();
	//UE_LOG(LogTemp, Display, TEXT("[C++] set position of actor %s id: %i,  x: %f, y: %f, z: %f"), *type, id, x, y, z);
	return 0;
}

void UVRGameInstance::printString(const char* msg)
{
	FString Fs = FString(UTF8_TO_TCHAR(msg));
	UE_LOG(LogTemp, Display, TEXT("%s"), *Fs);
}

ALuaActor* UVRGameInstance::searchForActor(const char* type, int32 id) const
{
	// Get BP class name
	FString typeString = FString(UTF8_TO_TCHAR(type));
	typeString.ToLower();
	FName typeName = FName(*typeString);

	// Search for specified BP class actors array
	const FLuaActorsArray* luaActorsArray = luaObjectsArray.FindByPredicate(
		[typeName](const FLuaActorsArray& data)
	{
		return data.className == typeName;
	});

	assert(luaActorsArray);
	assert(luaActorsArray->actors[id]);

	return luaActorsArray->actors[id];
}