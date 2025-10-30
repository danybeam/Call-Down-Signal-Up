/*
MIT License

Copyright (c) 2025 Daniel Orozco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chicken.generated.h"

// Declaration of the delegate for chickens to register themselves
DECLARE_DELEGATE_OneParam(FChickenRegistrationDelegate, class AChicken*);

// Declaration of the delegate for chickens to announce they clucked
DECLARE_DELEGATE_OneParam(FChickenCluckedDelegate, FString);

UCLASS()
class EXAMPLECODE_API AChicken : public AActor
{
	GENERATED_BODY()

public:

	// Static concrete delegate so that the farm can subscribe without knowing which specific chicken
	// The chicken gets passed as a parameter of the function.
	static FChickenRegistrationDelegate OnChickenRegistration;

	// This is an instanced delegate for clucks,
	// which means that it needs to be subscribed manually whenever a chicken is spawned.
	FChickenCluckedDelegate OnChickenClucked;

protected:
	// Called when the game starts or when spawned. More details in the .cpp file implementation of the function
	virtual void BeginPlay() override;

	// Function to "cluck"
	// it prints a message on screen and randomly calls the clucking delegate.
	// Details in implementation.
	void Cluck();

	// Function to spawn 2 eggs
	// Notice how this function does NOT tell the egg to register itself
	// or call any other delegate to propagate that information up.
	// That is by design as the Farm is already aware that eggs can register themselves.
	// This is one of many ways of "skipping steps" in the chain without breaking it.
	// Steps should not be skipped going down but going up there's more leaway 
	void LayEggs();

#pragma region hic sunt dracones

	/****************************************
	* HERE BE DRAGONS
	* The code in this section will not be documented,
	* it is glue code to make the example game code run
	* and do something remotely interesting
	*/
public:
	// Sets default values for this actor's properties
	AChicken();
	FString GetChickenName() {return ChickenName;}
protected:

	FString ChickenName;

	UPROPERTY(EditDefaultsOnly)
	TArray<FString> PotentialNames;

	UPROPERTY(EditDefaultsOnly)
	float baseTimeToCluck = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	float baseTimeToLayEgg = 10.0f;
	UPROPERTY(EditDefaultsOnly)
	int EggsToLay = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEgg> eggTemplate;

	FTimerHandle CluckTimer;
	FTimerHandle EggTimer;
#pragma endregion
};
