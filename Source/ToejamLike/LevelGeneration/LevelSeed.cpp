// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSeed.h"
#include "../OverlappableInstancedStaticMesh.h"

// Sets default values
ALevelSeed::ALevelSeed()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	InstancedMesh = CreateDefaultSubobject<UOverlappableInstancedStaticMesh>(TEXT("InstancedMesh"));

	InstancedMesh->SetupAttachment(Root);
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

	//int32 Iteration = 1;

	AddNewInstance(x, y);
	for (int32 Iteration = 1; Iteration <= NumIterations; ++Iteration)
	{
		for (int32 i = 0; i < Iteration; ++i)
		{
			AddNewInstance(++x, y);
		}

		for (int32 i = 0; i < Iteration - 1; ++i)
		{
			AddNewInstance(x, ++y);
		}

		for (int32 i = 0; i < Iteration; ++i)
		{
			AddNewInstance(--x, ++y);
		}

		for (int32 i = 0; i < Iteration; ++i)
		{
			AddNewInstance(--x, y);
		}

		for (int32 i = 0; i < Iteration; ++i)
		{
			AddNewInstance(x, --y);
		}

		for (int32 i = 0; i < Iteration; ++i)
		{
			AddNewInstance(++x, --y);
		}


	}
}

void ALevelSeed::AddNewInstance(int32 x, int32 y)
{
	FVector StartingLocation = Root->GetComponentLocation();
	
	//x -> x + y/2, y -> Sqrt(3)/2 * y

	
	FVector NewInstanceLocation = FVector(
		(x * XDiameter) + StartingLocation.X,
		(y * YDiameter) + StartingLocation.Y,
		StartingLocation.Z
	);

	NewInstanceLocation.X = NewInstanceLocation.X + (NewInstanceLocation.Y / 2.f);

	NewInstanceLocation.Y = (NewInstanceLocation.Y * (FMath::Sqrt(3.f) / 2.f));

	FTransform NewInstanceTransform{};

	NewInstanceTransform.SetLocation(NewInstanceLocation);

	InstancedMesh->AddInstance(NewInstanceTransform, true);
}



