#include "SloopedUpExplorer.h"
#include "SloopedUpExplorerSubsystem.h"
#include "Patching/NativeHookManager.h"

#include "WheeledVehicles/FGWheeledVehicleMovementComponent.h"

#define LOCTEXT_NAMESPACE "FSloopedUpExplorerModule"

void FSloopedUpExplorerModule::StartupModule() {
#if !WITH_EDITOR

	UFGWheeledVehicleMovementComponent* wvmc = GetMutableDefault<UFGWheeledVehicleMovementComponent>();

	SUBSCRIBE_METHOD_VIRTUAL(UFGWheeledVehicleMovementComponent::SetupVehicle, wvmc, [this](auto& scope, UFGWheeledVehicleMovementComponent* self, TUniquePtr<Chaos::FSimpleWheeledVehicle>& PVehicle) {
		ASloopedUpExplorerSubsystem* sloopedUpExplorerSubsystem = ASloopedUpExplorerSubsystem::Get(self->GetWorld());
		if (sloopedUpExplorerSubsystem) {
			sloopedUpExplorerSubsystem->TuneExplorer(self);
			sloopedUpExplorerSubsystem->SwapExplorerWheels(self);
		}
		scope(self, PVehicle);
	});

#endif
}

void FSloopedUpExplorerModule::ShutdownModule() {}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSloopedUpExplorerModule, SloopedUpExplorer)
