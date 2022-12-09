#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "BaseQuest.generated.h"

class APlayerCharacter;
class UBaseItem;
class UWorld;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveUpdate);

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Main,
	Side,
	Completed
};

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	Location,
	Collect,
	Interact,
	Defeat
};

USTRUCT(BlueprintType)
struct FQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	EObjectiveType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FVector ObjectiveLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	float ObjectiveRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FName TagToLookFor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	TSubclassOf<AActor> ActorClassToLookFor;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> ConnectedActors;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentProgress = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	int32 FinalProgress = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	TSubclassOf<UBaseItem> ItemToCollect;

	// Does this objective need to be completed before showing others or not?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	bool bIsChainable = true;

	// Will be used for the tracker on the map
	UPROPERTY(BlueprintReadWrite)
	bool bIsFocused = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsActive = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCompleted = false;
};

USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExperienceReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<UBaseItem>, int32> ItemReward;
};

UCLASS(BlueprintType, Blueprintable)
class VIOLENTENDS_API UBaseQuest : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override { return this->World; }

	UFUNCTION(BlueprintCallable)
	void SetUpObjectives();

	UFUNCTION(BlueprintCallable)
	void OnQuestBegin();

	UFUNCTION(BlueprintCallable)
	void UpdateObjective(FQuestObjective& Objective, bool bIncreaseProgress = true);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerQuest(APlayerCharacter* Player);

	UFUNCTION(BlueprintCallable)
	void SelectNextQuest(APlayerCharacter* Player, UBaseQuest* FollowUpQuest);

	UFUNCTION(BlueprintCallable)
	void OnQuestCompleted(APlayerCharacter* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	EQuestType QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FQuestReward Reward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TSubclassOf<UBaseQuest> NextQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<TSubclassOf<UBaseQuest>> PrerequisiteQuests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objectives", meta = (TitleProperty = "Name"))
	TArray<FQuestObjective> Objectives;

	UPROPERTY(Transient)
	UWorld* World;

	UPROPERTY(BlueprintAssignable)
	FObjectiveUpdate RefreshObjectives;
};