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

#include "Farm.h"

#include "Chicken.h"
#include "Egg.h"

// Sets default values
AFarm::AFarm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFarm::BeginPlay()
{
	Super::BeginPlay();

	AChicken::OnChickenRegistration.BindUObject(this, &AFarm::OnChickenRegistration);
	AEgg::OnEggLaid.BindUObject(this, &AFarm::OnEggLaid);
}

void AFarm::OnChickenRegistration(AChicken* newChicken)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Green,
		*FString::Printf(TEXT("Farm: Registering %s"), *newChicken->GetChickenName())
	);

	Chickens.Add(newChicken);
	newChicken->OnChickenClucked.BindUObject(this, &AFarm::OnChickenClucked);
	newChicken->OnDestroyed.AddDynamic(this, &AFarm::OnChickenDestroyed);
}

void AFarm::OnChickenClucked(FString ChickenName)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Green,
		*FString::Printf(TEXT("Farm: I heard %s cluck"), *ChickenName)
	);
}

void AFarm::OnChickenDestroyed(AActor* chicken)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Green,
		*FString::Printf(TEXT("Farm: %s died"), *Cast<AChicken>(chicken)->GetChickenName())
	);

	Chickens.Remove(Cast<AChicken>(chicken));
}

void AFarm::OnEggLaid(AEgg* egg)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Green,
		*FString::Printf(TEXT("Farm: I found an egg"))
	);
	egg->SetIncubation(FMath::FRand() > 0.25f);
	egg->OnEggExpiredWhileIncubated.BindUObject(this, &AFarm::OnEggExpiredWhileIncubated);
	egg->SetLifeSpan(FMath::FRandRange(2.0f, 10.0f));
	egg->OnDestroyed.AddDynamic(this, &AFarm::OnEggDestroyed);
	Eggs.Add(egg);
}

void AFarm::OnEggDestroyed(AActor* egg)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Green,
		*FString::Printf(TEXT("Farm: I lost an egg"))
	);
	Eggs.Remove(Cast<AEgg>(egg));
}

void AFarm::OnEggExpiredWhileIncubated()
{
	FTransform SpawnTransform;
	float randomOffsetX = FMath::FRandRange(-30.f,30.f);
	float randomOffsetY = FMath::FRandRange(-30.f,30.f);
	float randomOffsetZ = FMath::FRandRange(-30.f,30.f);
	SpawnTransform.SetLocation(FVector(1100.000000+randomOffsetX, 0.000000+randomOffsetY, 300.000000+randomOffsetZ));

	auto chicken = GetWorld()->SpawnActorDeferred<AChicken>(
		ChickenTemplate,
		SpawnTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);

	GEngine->AddOnScreenDebugMessage(
		-1,
		10.0f,
		FColor::Green,
		*FString::Printf(TEXT("Farm: Egg hatched into %s"), *chicken->GetChickenName())
	);

	chicken->FinishSpawning(SpawnTransform);
}
