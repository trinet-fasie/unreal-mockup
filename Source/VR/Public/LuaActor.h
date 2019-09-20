#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "LuaActor.generated.h"

UCLASS(BlueprintType)
class VR_API ALuaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALuaActor();

	bool HaveUnhandledUseEvent() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Use();

	UFUNCTION(BlueprintPure)
	virtual bool IsUsed() const { return IsUse; };

	UFUNCTION(BlueprintImplementableEvent)
	void Used();

	UFUNCTION(BlueprintImplementableEvent)
	void SetMessage(const FString & msg);

	void SendMessage(FString msg);

	UFUNCTION(BlueprintCallable)
	void Pikup(USceneComponent* motionController);

	UFUNCTION(BlueprintCallable)
	void Drop();

	UFUNCTION(BlueprintCallable)
	virtual void Trigger();

	//// for buttons
	//bool IsActivated();
	//bool IsDeactivated();

protected:
	// Called when the game starts or when spawned
	UFUNCTION(BlueprintImplementableEvent)
	void Pikuped(USceneComponent* motionController);

	UFUNCTION(BlueprintImplementableEvent)
	void Droped();

	UFUNCTION(BlueprintImplementableEvent)
	void Triggered();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsUse;

private:
	mutable bool bHaveUnhandledUseEvent {false};

};
