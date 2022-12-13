#include "InteractiveObject.h"

#include "Components/WidgetComponent.h"

// Add default functionality here for any IInteractiveObject functions that are not pure virtual.
void IInteractiveObject::InteractionWidgetVisibility(UWidgetComponent* Widget, bool bShowWidget)
{
	if (Widget != NULL) { Widget->SetVisibility(bShowWidget); }
	else { UE_LOG(LogTemp, Warning, TEXT("Tried setting widget visibility on a NULL widget.")); }
}