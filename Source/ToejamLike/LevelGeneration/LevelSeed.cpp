// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSeed.h"
#include "../OverlappableInstancedStaticMesh.h"
#include "Kismet/KismetMathLibrary.h"
#include "TeleportTile.h"
#include "BaseTile.h"

// Sets default values
ALevelSeed::ALevelSeed()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void ALevelSeed::BeginPlay()
{
	Super::BeginPlay();

	GenerateLevel();
}

void ALevelSeed::GenerateLevel()
{
	/*
	int x=0, y=0;
	add(x, y); // add the first cell
	int N=1
	for( int N=1; <some condition>; ++N ) {
		for(int i=0; i<N; ++i) add(++x, y);  // move right
		for(int i=0; i<N-1; ++i) add(x, ++y); // move down right. Note N-1
		for(int i=0; i<N; ++i) add(--x, ++y); // move down left
		for(int i=0; i<N; ++i) add(--x, y); // move left
		for(int i=0; i<N; ++i) add(x, --y); // move up left
		for(int i=0; i<N; ++i) add(++x, --y); // move up right
	}
	*/
	int32 x = 0, y = 0;

	AddNewInstance(x, y);
	for (int32 Iteration = 1; Iteration <= NumIterations; ++Iteration)
	{
		for (int32 i = 0; i < Iteration; ++i)
		{
			if (UKismetMathLibrary::RandomBoolWithWeight(ChanceOfInstanceCreation))
			{
				AddNewInstance(++x, y);
			}
		}

		for (int32 i = 0; i < Iteration - 1; ++i)
		{
			if (UKismetMathLibrary::RandomBoolWithWeight(ChanceOfInstanceCreation))
			{
				AddNewInstance(x, ++y);
			}
		}

		for (int32 i = 0; i < Iteration; ++i)
		{
			if (UKismetMathLibrary::RandomBoolWithWeight(ChanceOfInstanceCreation))
			{
				if (UKismetMathLibrary::RandomBoolWithWeight(0.08) && !bHasTeleporterBeenSpawned)
				{
					AddTeleportTile(--x, ++y);
					bHasTeleporterBeenSpawned = true;
				}
				else
				{
					AddNewInstance(--x, ++y);
				}
			}
		}

		for (int32 i = 0; i < Iteration; ++i)
		{
			if (UKismetMathLibrary::RandomBoolWithWeight(ChanceOfInstanceCreation))
			{
				AddNewInstance(--x, y);
			}
		}

		for (int32 i = 0; i < Iteration; ++i)
		{
			if (UKismetMathLibrary::RandomBoolWithWeight(ChanceOfInstanceCreation))
			{
				AddNewInstance(x, --y);
			}
		}

		for (int32 i = 0; i < Iteration; ++i)
		{
			if (UKismetMathLibrary::RandomBoolWithWeight(ChanceOfInstanceCreation))
			{
				AddNewInstance(++x, --y);
			}
		}
	}
}

void ALevelSeed::AddNewInstance(int32 x, int32 y)
{
	FVector StartingLocation = Root->GetComponentLocation();

	float XTransform = x;

	float YTransform = y;

	ApplyCoordinateTransform(XTransform, YTransform);
	
	FVector NewInstanceLocation = FVector(
		(XTransform * XDiameter) + StartingLocation.X,
		YTransform * YDiameter * (FMath::Sqrt(3.f)/2) + StartingLocation.Y,
		StartingLocation.Z
	);
	
	FTransform NewInstanceTransform{};

	FTransform MapInstanceTransform{};

	NewInstanceTransform.SetLocation(NewInstanceLocation);

	float TileDecider = FMath::FRandRange(0.f, 3.0);

	if (TileDecider < 1.0)
	{
		GrassTile->GetInstancedMesh()->AddInstance(NewInstanceTransform, true);
	}
	else if (TileDecider > 1.0 && TileDecider < 2.0)
	{
		WaterTile->GetInstancedMesh()->AddInstance(NewInstanceTransform, true);
	}
	else
	{
		SandTile->GetInstancedMesh()->AddInstance(NewInstanceTransform, true);
	}
}

void ALevelSeed::ApplyCoordinateTransform(float& x, float& y)
{
	//x -> x + y/2, y -> Sqrt(3)/2 * y

	//apply transform to go to skewed coordinates for hexagonal layout

	x = x + (y / 2.f);

	y = (FMath::Sqrt(3.f) / 2.f) * y;
}

void ALevelSeed::ResetLevel()
{
	GrassTile->GetInstancedMesh()->ClearInstances();
	WaterTile->GetInstancedMesh()->ClearInstances();
	SandTile->GetInstancedMesh()->ClearInstances();
}

void ALevelSeed::AddTeleportTile(int32 x, int32 y)
{
	FVector StartingLocation = Root->GetComponentLocation();

	float XTransform = x;

	float YTransform = y;

	ApplyCoordinateTransform(XTransform, YTransform);

	FVector TeleportTileLocation = FVector(
		(XTransform * XDiameter) + StartingLocation.X,
		YTransform * YDiameter * (FMath::Sqrt(3.f) / 2) + StartingLocation.Y,
		StartingLocation.Z
	);

	FTransform NewTeleportTileTransform{};

	NewTeleportTileTransform.SetLocation(TeleportTileLocation);
	
	TeleportTile->SetActorLocation(TeleportTileLocation);
}


