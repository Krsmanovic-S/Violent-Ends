#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/Set.h"


AMainPlayerController::AMainPlayerController()
{
    DebugLineTraces = true;
    IsOcclusionEnabled = true;
}

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if(IsValid(GetPawn()))
    {
        this->ActiveSpringArm = Cast<USpringArmComponent>(GetPawn()->GetComponentByClass(USpringArmComponent::StaticClass()));
        this->ActiveCamera = Cast<UCameraComponent>(GetPawn()->GetComponentByClass(UCameraComponent::StaticClass()));
    }
}

void AMainPlayerController::SyncOccludedActors()
{
    if(!ShouldCheckCameraOcclusion()) return;

    // Camera is currently colliding, show all current 
    // occluded actors and do not perform further occlusion.
    if(this->ActiveSpringArm->bDoCollisionTest)
    {
        ForceShowOccludedActors();
        return;
    }

    FVector Start = ActiveCamera->GetComponentLocation();
    FVector End = GetPawn()->GetActorLocation();

    TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

    // Only WorldStatic objects will be subjected to occulsion.
    CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

    TArray<AActor*> ActorsToIgnore; // TODO: Add configuration to ignore actor types

    TArray<FHitResult> OutHits;

    auto ShouldDebug = this->DebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

    bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        GetWorld(), Start, End, this->CapsuleRadius, this->CapsuleHeight, 
        CollisionObjectTypes, true, ActorsToIgnore, ShouldDebug, OutHits, true);

    if(bGotHits)
    {
        // The list of actors hit by the line trace, that means that they are occluded from view
        TSet<const AActor*> ActorsJustOccluded;

        // Hide actors that are occluded by the camera.
        for(FHitResult Hit : OutHits)
        {
            const AActor* HitActor = Cast<AActor>(Hit.GetActor());

            HideOccludedActor(HitActor);
            ActorsJustOccluded.Add(HitActor);
        }

        // Show actors that are currently hidden but that are not occluded by the camera anymore.
        for(auto& Elem : OccludedActors)
        {
            if(!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.IsOccluded)
            {
                ShowOccludedActor(Elem.Value);
            }
        }
    }
    else
    {
        ForceShowOccludedActors();
    }
}

bool AMainPlayerController::HideOccludedActor(const AActor* Actor)
{
    FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);

    // Early return if we encounter an already occluded actor.
    if(ExistingOccludedActor && ExistingOccludedActor->IsOccluded)
    {
        return false;
    }

    if(ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
    {
        ExistingOccludedActor->IsOccluded = true;

        OnHideOccludedActor(*ExistingOccludedActor);
    }
    else
    {
        UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

        FCameraOccludedActor OccludedActor;
        OccludedActor.Actor = Actor;
        OccludedActor.StaticMesh = StaticMesh;
        OccludedActor.Materials = StaticMesh->GetMaterials();
        OccludedActor.IsOccluded = true;
        OccludedActors.Add(Actor, OccludedActor);
        OnHideOccludedActor(OccludedActor);
    }

    return true;
}

void AMainPlayerController::ForceShowOccludedActors()
{
    for(auto& MapValue : OccludedActors)
    {
        // Show only actors that have been occluded.
        if(MapValue.Value.IsOccluded)
        {
            this->ShowOccludedActor(MapValue.Value);
        }
    }
}

void AMainPlayerController::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
    if(!IsValid(OccludedActor.Actor))
    {
        OccludedActors.Remove(OccludedActor.Actor);
    }

    OccludedActor.IsOccluded = false;
    OnShowOccludedActor(OccludedActor);
}

bool AMainPlayerController::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
    // Return the original materials of the occluded actor back to normal.
    for(int i = 0; i < OccludedActor.Materials.Num(); i++)
    {
        OccludedActor.StaticMesh->SetMaterial(i, OccludedActor.Materials[i]);
    }

    return true;
}

bool AMainPlayerController::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
    // Apply the transparent material to the occluded actors.
    for(int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); i++)
    {
        OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
    }

    return true;
}