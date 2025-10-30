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
#include "Farm.generated.h"

UCLASS()
class EXAMPLECODE_API AFarm : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFarm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Chicken methods
	virtual void OnChickenRegistration(class AChicken* newChicken);
	virtual void OnChickenClucked(FString ChickenName);
	UFUNCTION()
	virtual void OnChickenDestroyed(AActor* chicken);

	// Egg methods
	virtual void OnEggLaid(class AEgg* egg);
	virtual void OnEggExpiredWhileIncubated();
	UFUNCTION()
	virtual void OnEggDestroyed(AActor* egg);

	TArray<class AChicken*> Chickens;
	TArray<class AEgg*> Eggs;

#pragma region hic sunt dracones

	/****************************************
	* HERE BE DRAGONS
	* The code in this section will not be documented,
	* it is glue code to make the example game code run
	* and do something remotely interesting
	*/

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AChicken> ChickenTemplate;
#pragma endregion
};
