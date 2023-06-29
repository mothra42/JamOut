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


		if (bShowGeneratedBoxes)
		{
			NewBoxCollision->SetHiddenInGame(false);
		}

		BoxCollisionComponents.Add(NewBoxCollision);
		AddNewBoxCollisionTileLocationPair(NewBoxCollision, InstanceTransform.GetLocation());
	}
	return Super::AddInstance(InstanceTransform, bWorldSpace);
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