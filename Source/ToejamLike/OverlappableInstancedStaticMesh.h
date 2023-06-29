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
		virtual int32 AddInstance(const FTransform& InstanceTransform, bool bWorldSpace);

	public:
		UFUNCTION(BlueprintCallable, Category = "Box Collision")
		const TArray<class UBoxComponent*> GetBoxCollisionComps() { return BoxCollisionComponents; }

		UFUNCTION(BlueprintCallable, Category = "Box Collision")
		const TMap<UBoxComponent*, FVector> GetBoxCollisionTileLocationPairs() { return BoxCollisionTileLocationPairs; }

	protected:
		UPROPERTY(EditDefaultsOnly, Category = "Box Collision Settings")
		FVector CollisionBoxExtents = FVector(118.179214, 164.663522, 128.368065);
		UPROPERTY(EditDefaultsOnly, Category = "Box Collision Settings")
		FVector CollisionBoxLocationOffset = FVector(0.f, 0.f, 230.f);
		UPROPERTY(EditDefaultsOnly, Category = "Box Collision Settings")
		bool bShowGeneratedBoxes = false;
	
	private:
		FTransform FindBoxCollisionTransformOffset(const FTransform& TileTransform);

		TMap<UBoxComponent*, FVector> AddNewBoxCollisionTileLocationPair(UBoxComponent* BoxComp, FVector TileLocation);

		TArray<UBoxComponent*> BoxCollisionComponents;
		TMap<UBoxComponent*, FVector> BoxCollisionTileLocationPairs;

};
