// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSeed.generated.h"

UCLASS()
class TOEJAMLIKE_API ALevelSeed : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSeed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Generation Rules")
	int32 NumIterations = 100;

	//diameter of mesh in the x direction
	UPROPERTY(EditDefaultsOnly, Category = "Generation Rules")
	float XDiameter = 400.f;
	
	//diameter of mesh in the y direction
	UPROPERTY(EditDefaultsOnly, Category = "Generation Rules")
	float YDiameter = 400.f;

	//This property must be set to a instanced mesh that lives in the world.
	//If this is not set level generation will fail!
	UPROPERTY(EditAnywhere, Category = "Instanced Mesh Settings")
	class ABaseTile* Tile;

	UPROPERTY(EditDefaultsOnly, Category = "Instanced Mesh Settings")
	float ChanceOfInstanceCreation = 0.95;

	class USceneComponent* Root;

public:	

private:
	UFUNCTION(BlueprintCallable)
	void GenerateLevel();

	UFUNCTION(BlueprintCallable)
	void ResetLevel();

	void AddNewInstance(int32 x, int32 y);

	void ApplyCoordinateTransform(float& x, float& y);

};
