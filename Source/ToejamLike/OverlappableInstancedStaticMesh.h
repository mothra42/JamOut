// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "OverlappableInstancedStaticMesh.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Rendering, meta = (BlueprintSpawnableComponent), Blueprintable)
class TOEJAMLIKE_API UOverlappableInstancedStaticMesh : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
	protected:
		

		// returns false if adding a new map instance was unsuccessful
		UFUNCTION(BlueprintCallable, Category = "Map Instance Creation")
		void AddNewInstanceForPlayerMapGeneration(
			UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult
		);

	public:
		virtual int32 AddInstance(const FTransform& InstanceTransform, bool bWorldSpace);

		UFUNCTION(BlueprintCallable, Category = "Box Collision")
		const TArray<UBoxComponent*> GetBoxCollisionComps() { return BoxCollisionComponents; }

		UFUNCTION(BlueprintCallable, Category = "Box Collision")
		const TMap<UBoxComponent*, FVector> GetBoxCollisionTileLocationPairs() { return BoxCollisionTileLocationPairs; }

	protected:
		UPROPERTY(EditDefaultsOnly, Category = "Box Collision Settings")
		FVector CollisionBoxExtents = FVector(118.179214, 164.663522, 128.368065);
		UPROPERTY(EditDefaultsOnly, Category = "Box Collision Settings")
		FVector CollisionBoxLocationOffset = FVector(0.f, 0.f, 230.f);
		UPROPERTY(EditDefaultsOnly, Category = "Box Collision Settings")
		bool bShowGeneratedCollisionBoxes = false;
		UPROPERTY(EditDefaultsOnly, Category = "Map Instance Settings")
		FVector PlayerMapLocationOffset = FVector(100000.0, 0, 0);
	
	private:
		FTransform FindBoxCollisionTransformOffset(const FTransform& TileTransform);

		TMap<UBoxComponent*, FVector> AddNewBoxCollisionTileLocationPair(UBoxComponent* BoxComp, FVector TileLocation);

		TArray<UBoxComponent*> BoxCollisionComponents;
		TMap<UBoxComponent*, FVector> BoxCollisionTileLocationPairs;

};
