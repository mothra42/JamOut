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
	class ABaseTile* GrassTile;
	UPROPERTY(EditAnywhere, Category = "Instanced Mesh Settings")
	ABaseTile* WaterTile;
	UPROPERTY(EditAnywhere, Category = "Instanced Mesh Settings")
	ABaseTile* SandTile;

	UPROPERTY(EditDefaultsOnly, Category = "Instanced Mesh Settings")
	float ChanceOfInstanceCreation = 0.95;

	class USceneComponent* Root;

	bool bHasTeleporterBeenSpawned = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Settings")
	FText LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Settings")
	AActor* TeleportTile;

public:	

private:
	UFUNCTION(BlueprintCallable)
	void GenerateLevel();

	//DEBUG ONLY
	UFUNCTION(BlueprintCallable)
	void ResetLevel();

	void ChooseTeleporterOrTile(int32 x, int32 y);

	void AddNewInstance(int32 x, int32 y);

	void AddTeleportTile(int32 x, int32 y);

	void ApplyCoordinateTransform(float& x, float& y);

};
