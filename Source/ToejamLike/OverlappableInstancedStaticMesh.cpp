// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlappableInstancedStaticMesh.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

int32 UOverlappableInstancedStaticMesh::AddInstance(const FTransform& InstanceTransform, bool bWorldSpace)
{
	//UE_LOG(LogTemp, Warning, TEXT("Calling custom function"));

	//Tomorrow Adam, this code is no longer executing properly since I've changed the code
	//to have a single instanced mesh in the world
	//int32 NewInstanceIndex = 0;
	if (GetOwner() != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found my parent"));
		FTransform BoxCollisionTransform = FindBoxCollisionTransformOffset(InstanceTransform);
		UBoxComponent* NewBoxCollision = Cast<UBoxComponent>(GetOwner()->AddComponentByClass(
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

		//TEST for moving instances instead of generating new ones on the fly
		//NewInstanceIndex = Super::AddInstance(InstanceTransform, bWorldSpace);
		//BoxCollisionInstancePairs.Add(NewBoxCollision, NewInstanceIndex);
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
	UE_LOG(LogTemp, Warning, TEXT("Overlapping"));
	UBoxComponent* Box = Cast<UBoxComponent>(OverlappedComponent);

	if (Box != nullptr)
	{
		FVector* OverlappedInstanceLocation = BoxCollisionTileLocationPairs.Find(Box);
		//int32* InstanceToMove = BoxCollisionInstancePairs.Find(Box);
		if (OverlappedInstanceLocation != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Updating Instance"));
			NewInstanceTransform.SetLocation(*OverlappedInstanceLocation + PlayerMapLocationOffset);
			//Super::UpdateInstanceTransform(*InstanceToMove + 1, NewInstanceTransform, true, true, true);


			//TODO I think these locations will be in world space, but if there are errors, this might be the bug.
			//Super::AddInstance(NewInstanceTransform, true);
			UE_LOG(LogTemp, Warning, TEXT("Adding New Instance at %s"), *NewInstanceTransform.GetLocation().ToString());
		}
	}
}

FTransform UOverlappableInstancedStaticMesh::FindBoxCollisionTransformOffset(const FTransform& TileTransform)
{
	FTransform BoxCollisionTransformOffset{};

	// This vector must be in the local space of the component.
	FVector LocationOffset = TileTransform.GetLocation() + CollisionBoxLocationOffset - this->GetComponentLocation();

	BoxCollisionTransformOffset.SetLocation(LocationOffset);
	BoxCollisionTransformOffset.SetRotation(FQuat(FRotator(0.f, 90.f, 0.f)));

	return BoxCollisionTransformOffset;
}

TMap<UBoxComponent*, FVector> UOverlappableInstancedStaticMesh::AddNewBoxCollisionTileLocationPair(UBoxComponent* BoxComp, FVector TileLocation)
{
	BoxCollisionTileLocationPairs.Add(BoxComp, TileLocation);

	return BoxCollisionTileLocationPairs;
}