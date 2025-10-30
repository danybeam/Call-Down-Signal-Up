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

#include "Chicken.h"

#include "Egg.h"

// This needs to be here because static variables cannot be initialized in the .h
FChickenRegistrationDelegate AChicken::OnChickenRegistration;

// Sets default values
AChicken::AChicken()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChicken::BeginPlay()
{
	Super::BeginPlay();

	// Setup name
	if (PotentialNames.Num())
	{
		ChickenName = PotentialNames[FMath::RandRange(0, PotentialNames.Num() - 1)];
	}

	// Call the delegate for registration.
	// ExecuteIfBound makes it so that it doesn't explode if no one is registered to listen to it.
	// This is half of the "Signal Up" part of "Call Down Signal Up" (CDSU)
	// The Farm object should be bound to this before it gets called.
	OnChickenRegistration.ExecuteIfBound(this);

	// Setup clucking at a random time and laying eggs at a random time (not important for CDSU)
	GetWorld()->GetTimerManager().SetTimer(
		CluckTimer, // handle to cancel timer at a later time
		this, // the owning object
		&AChicken::Cluck, // function to call on elapsed
		FMath::FRandRange(3.0f, baseTimeToCluck), // float delay until elapsed
		false // looping?
	);

	GetWorld()->GetTimerManager().SetTimer(
		EggTimer, // handle to cancel timer at a later time
		this, // the owning object
		&AChicken::LayEggs, // function to call on elapsed
		FMath::FRandRange(2.0f, baseTimeToLayEgg), // float delay until elapsed
		false // looping?
	);
}

void AChicken::Cluck()
{
	// The actual clucking
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Blue,
		*FString::Printf(TEXT("Chicken(%s): I clucked"), *ChickenName)
	);

	// 50/50 chance to call the delegate
	if (FMath::RandBool())
	{
		// This delegate Signals that the chicken clucked.
		// In this code the bounded method is quite simple,
		// but it represents how to "restart a chain" if data needs to be processed by a higher component of the chain.
		// The programmers should know whether this is necessary but the class should only have visibility on the delegate.
		OnChickenClucked.ExecuteIfBound(ChickenName);
	}
	else
	{
		// In this branch we do NOT call the delegate.
		// This branch represents e.g. if the class already has enough information to process everything
		// Or another example closer to this code, the chicken could make a quiet cluck,
		// and it doesn't call the delegate to save on CPU cycles or something.
		GEngine->AddOnScreenDebugMessage(
			-1,
			10.0f,
			FColor::Blue,
			*FString::Printf(TEXT("Chicken(%s): I did not call the farm delegate"), *ChickenName)
		);
	}

	// The rest of this function can be ignored

	CluckTimer.Invalidate();


	GetWorld()->GetTimerManager().SetTimer(
		CluckTimer, // handle to cancel timer at a later time
		this, // the owning object
		&AChicken::Cluck, // function to call on elapsed
		FMath::FRandRange(3.0f, baseTimeToCluck), // float delay until elapsed
		false); // looping?
}

void AChicken::LayEggs()
{
	// Simple for loop to spawn the eggs
	for (int i = 0; i < EggsToLay; i++)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FTransform SpawnTransform;
		float randomOffsetX = FMath::FRandRange(-30.f, 30.f);
		float randomOffsetY = FMath::FRandRange(-30.f, 30.f);
		float randomOffsetZ = FMath::FRandRange(-30.f, 30.f);
		SpawnTransform.SetLocation(FVector(1000.000000 + randomOffsetX, 0.000000 + randomOffsetY,
		                                   300.000000 + randomOffsetZ));

		GetWorld()->SpawnActor<AEgg>(eggTemplate, SpawnTransform, spawnParams);
	}

	// Print on screen that eggs were laid
	// Notice how no other delegates are executed here.
	// It's not the chicken responsibility to register its eggs but the eggs' responsibility to register themselves 
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Blue,
		*FString::Printf(TEXT("Chicken(%s): I laid %d eggs"), *ChickenName, EggsToLay)
	);
}
