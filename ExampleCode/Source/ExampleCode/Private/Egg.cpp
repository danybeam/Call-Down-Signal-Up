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

#include "Egg.h"

// This needs to be here because static variables cannot be initialized in the .h
FLayedEggDelegate AEgg::OnEggLaid;

AEgg::AEgg()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEgg::BeginPlay()
{
	Super::BeginPlay();

	// Call the delegate to signal up that the egg was laid
	OnEggLaid.ExecuteIfBound(this);

	// Print on screen locally
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Green,
		*FString::Printf(TEXT("Egg: I was laid"))
	);
}

void AEgg::Destroyed()
{
	// Local printing on screen
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Green,
		*FString::Printf(TEXT("Egg: I cracked"))
	);

	// Conditionally call the OnEggExpiredWhileIncubated delegate
	// An egg should not need to know how to spawn a Chicken.
	// In this specific example a Farm knows how to spawn chickens (this is an example of "skipping steps going up")
	// Because this is a delegate it could be higher or lower down the chain,
	// the important part is that the egg produces the signal without depending directly on its ancestors.
	if (Incubation)
	{
		OnEggExpiredWhileIncubated.ExecuteIfBound();
	}

	Super::Destroyed();
}

