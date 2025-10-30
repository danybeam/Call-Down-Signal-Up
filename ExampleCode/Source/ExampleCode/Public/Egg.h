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
#include "Egg.generated.h"

// Declaration of the delegate to execute when an egg is laid.
// In this specific example it's similar to the chicken registration, but it doesn't need to be
DECLARE_DELEGATE_OneParam(FLayedEggDelegate, class AEgg*);

// Declaration of the delegate to indicate that the egg hatched instead of just spoiled or smth
// Notice how this delegate does not take any parameters. This is an example of how the call itself is the signal.
DECLARE_DELEGATE(FWasIncubatedDelegate);

UCLASS()
class EXAMPLECODE_API AEgg : public AActor
{
	GENERATED_BODY()

public:

	// Static delegate for when an egg was laid.
	// Similar logic to the chicken registration delegate, it's static so that the farm can subscribe to it from the begining.
	static FLayedEggDelegate OnEggLaid;

	// Delegate to execute if the Incubation variable is true.
	// Also like the chicken cluck delegate,
	// this is not static as the farm
	// needs to subscribe itself to each egg
	// as they may or may not call it.
	FWasIncubatedDelegate OnEggExpiredWhileIncubated;
protected:
	// Called when the game starts or when spawned
	// This calls the OnEggLaid delegate which the farm should already be subscribed to
	virtual void BeginPlay() override;
	
	// Override of the destroyed function
	// This calls the OnEggExpiredWhileIncubated delegate if appropriate
	virtual void Destroyed() override;


#pragma region hic sunt dracones
	/****************************************
	* HERE BE DRAGONS
	* The code in this section will not be documented,
	* it is glue code to make the example game code run
	* and do something remotely interesting
	*/
public:
	AEgg();
	void SetIncubation(bool incubation) { Incubation = incubation; };
private:
	bool Incubation = false;

#pragma endregion
};
