// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlappableInstancedStaticMesh.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

int32 UOverlappableInstancedStaticMesh::AddInstance(const FTransform& InstanceTransform, bool bWorldSpace)
{
	AActor* Parent = GetAttachParentActor();
	if (Parent != nullptr)
	{
		FTransform BoxCollisionTransform = FindBoxCollisionTransformOffset(InstanceTransform);
		UE_LOG(LogTemp, Warning, TEXT("%s is parent"), *Parent->GetName());
		UBoxComponent* NewBoxCollision = Cast<UBoxComponent>(Parent->AddComponentByClass(
			UBoxComponent::StaticClass(),
			false,
			BoxCollisionTransform,
			false
		));

		NewBoxCollision->SetBoxExtent(CollisionBoxExtents);


		if (bShowGeneratedCollisionBoxes)
		{
			NewBoxCollision->SetHiddenInGame(false);
		}

		NewBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &UOverlappableInstancedStaticMesh::AddNewInstanceForPlayerMapGeneration);

		BoxCollisionComponents.Add(NewBoxCollision);
		AddNewBoxCollisionTileLocationPair(NewBoxCollision, InstanceTransform.GetLocation());
	}
	return Super::AddInstance(InstanceTransform, bWorldSpace);
}

void UOverlappableInstancedStaticMesh::AddNewInstanceForPlayerMapGeneration(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	FTransform NewInstanceTransform{};
	
	UBoxComponent* Box = Cast<UBoxComponent>(OverlappedComponent);

	if (Box != nullptr)
	{
		FVector* OverlappedInstanceLocation = BoxCollisionTileLocationPairs.Find(Box);
		if (OverlappedInstanceLocation != nullptr)
		{
			NewInstanceTransform.SetLocation(*OverlappedInstanceLocation + PlayerMapLocationOffset);

			//TODO I think these locations will be in world space, but if there are errors, this might be the bug.
			Super::AddInstance(NewInstanceTransform, false);
		}
	}
}

FTransform UOverlappableInstancedStaticMesh::FindBoxCollisionTransformOffset(const FTransform& TileTransform)
{
	FTransform BoxCollisionTransformOffset{};

	FVector LocationOffset = TileTransform.GetLocation() + CollisionBoxLocationOffset;

	BoxCollisionTransformOffset.SetLocation(LocationOffset);
	return BoxCollisionTransformOffset;
}

TMap<UBoxComponent*, FVector> UOverlappableInstancedStaticMesh::AddNewBoxCollisionTileLocationPair(UBoxComponent* BoxComp, FVector TileLocation)
{
	BoxCollisionTileLocationPairs.Add(BoxComp, TileLocation);

	return BoxCollisionTileLocationPairs;
}