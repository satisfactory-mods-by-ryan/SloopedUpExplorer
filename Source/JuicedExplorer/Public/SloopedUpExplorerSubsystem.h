#pragma once

#include "Subsystem/ModSubsystem.h"
#include "SloopedUpExplorerSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSloopedUpExplorer, Log, All);

UCLASS()
class SLOOPEDUPEXPLORER_API ASloopedUpExplorerSubsystem : public AModSubsystem {
	GENERATED_BODY()

public:
	ASloopedUpExplorerSubsystem();
	static ASloopedUpExplorerSubsystem* Get(UWorld* world);

	// Modifies all relevant parameters for the Explorer to give it a more powerful, stable, and responsive feel,
	// while still being drivable and not too overpowered
	UFUNCTION(BlueprintCallable, Category = "SloopedUpExplorer")
	void TuneExplorer(UFGWheeledVehicleMovementComponent* vehicleMovementComponent);

	// Bounce the front of the Explorer by applying an upward impulse at the front wheels
	UFUNCTION(BlueprintCallable, Category = "SloopedUpExplorer|Hydraulics")
	void BounceFrontHydraulics(UFGWheeledVehicleMovementComponent* vehicleMovementComponent, float impulseStrength = 200000.0f);

private:
	// Modify Explorer wheel CDO once at startup. Affects all Explorers globally
	void TuneExplorerWheelCDO(UFGWheeledVehicleMovementComponent* vehicleMovementComponent);
	bool bWheelCDOTuned = false;

};