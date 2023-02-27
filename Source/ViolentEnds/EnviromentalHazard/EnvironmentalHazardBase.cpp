#include "EnvironmentalHazardBase.h"

AEnvironmentalHazardBase::AEnvironmentalHazardBase() {}

void AEnvironmentalHazardBase::Activate() {}

bool AEnvironmentalHazardBase::CanBeReused()
{
	return false;
}

void AEnvironmentalHazardBase::OnActivated_Implementation() {}
