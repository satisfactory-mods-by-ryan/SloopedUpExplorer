#pragma once

#include "Subsystem/ModSubsystem.h"
#include "SloopedUpExplorerSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSloopedUpExplorer, Warning, All);

UCLASS()
class SLOOPEDUPEXPLORER_API ASloopedUpExplorerSubsystem : public AModSubsystem {
	GENERATED_BODY()

public:
	ASloopedUpExplorerSubsystem();
	static ASloopedUpExplorerSubsystem* Get(UWorld* world);

	virtual void BeginPlay() override;

	// Modifies all relevant parameters for the Explorer to give it a more powerful, stable, and responsive feel,
	// while still being drivable and not too overpowered
	UFUNCTION(BlueprintCallable, Category = "SloopedUpExplorer")
	void TuneExplorer(UFGWheeledVehicleMovementComponent* vehicleMovementComponent);

	UFUNCTION(BlueprintCallable, Category = "SloopedUpExplorer")
	void SwapExplorerWheels(UFGWheeledVehicleMovementComponent* vehicleMovementComponent);

	// Bounce the front of the Explorer by applying an upward impulse at the front wheels
	UFUNCTION(BlueprintCallable, Category = "SloopedUpExplorer|Hydraulics")
	void BounceFrontHydraulics(UFGWheeledVehicleMovementComponent* vehicleMovementComponent, float impulseStrength = 200000.0f);

private:
	TSubclassOf<class UChaosVehicleWheel> FrontWheelClass;
	TSubclassOf<class UChaosVehicleWheel> RearWheelClass;

};
