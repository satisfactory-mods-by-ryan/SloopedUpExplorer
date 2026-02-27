#include "SloopedUpExplorerSubsystem.h"
#include "Subsystem/SubsystemActorManager.h"
#include "Kismet/GameplayStatics.h"

#include "WheeledVehicles/FGWheeledVehicleMovementComponent.h"

DEFINE_LOG_CATEGORY(LogSloopedUpExplorer);

ASloopedUpExplorerSubsystem::ASloopedUpExplorerSubsystem() {
	ReplicationPolicy = ESubsystemReplicationPolicy::SpawnOnServer_Replicate;
}

ASloopedUpExplorerSubsystem* ASloopedUpExplorerSubsystem::Get(UWorld* world) {
	if (!world) {
		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("Get called with null world"));
		return nullptr;
	}
	USubsystemActorManager* subsystemActorManager = world->GetSubsystem<USubsystemActorManager>();
	if (!subsystemActorManager) {
		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("SubsystemActorManager not found in world"));
		return nullptr;
	}
	ASloopedUpExplorerSubsystem* SloopedUpExplorerSubsystem = subsystemActorManager->GetSubsystemActor<ASloopedUpExplorerSubsystem>();
	if (!SloopedUpExplorerSubsystem) {
		SloopedUpExplorerSubsystem = Cast<ASloopedUpExplorerSubsystem>(UGameplayStatics::GetActorOfClass(world, ASloopedUpExplorerSubsystem::StaticClass()));
		if (!SloopedUpExplorerSubsystem) {
			UE_LOG(LogSloopedUpExplorer, Warning, TEXT("SloopedUpExplorerSubsystem not found in world"));
			return nullptr;
		}
	}
	return SloopedUpExplorerSubsystem;
}

void ASloopedUpExplorerSubsystem::TuneExplorer(UFGWheeledVehicleMovementComponent* vehicleMovementComponent) {
	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("TuneExplorer called with null vehicleMovementComponent"));
		return;
	}
	// Authority check
	if (!HasAuthority()) {
		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("TuneExplorer called on client, changes may not persist"));
	}

	// Log all default values BEFORE modification
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("========== MOVEMENT COMPONENT DEFAULT VALUES =========="));

	// Engine defaults
	FVehicleEngineConfig& engineConfig = vehicleMovementComponent->EngineSetup;
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.MaxTorque: %f"), engineConfig.MaxTorque);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.MaxRPM: %f"), engineConfig.MaxRPM);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.EngineIdleRPM: %f"), engineConfig.EngineIdleRPM);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.EngineBrakeEffect: %f"), engineConfig.EngineBrakeEffect);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.EngineRevUpMOI: %f"), engineConfig.EngineRevUpMOI);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.EngineRevDownRate: %f"), engineConfig.EngineRevDownRate);

	// Engine torque curve
	FRichCurve* defaultTorqueCurve = engineConfig.TorqueCurve.GetRichCurve();
	if (defaultTorqueCurve && defaultTorqueCurve->GetNumKeys() > 0) {
		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.TorqueCurve has %d keys:"), defaultTorqueCurve->GetNumKeys());
		for (auto It = defaultTorqueCurve->GetKeyIterator(); It; ++It) {
			const FRichCurveKey& Key = *It;
			UE_LOG(LogSloopedUpExplorer, Log, TEXT("  [%.1f RPM] -> %.3f torque multiplier"), Key.Time, Key.Value);
		}
	} else {
		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.TorqueCurve: EMPTY"));
	}

	// Differential defaults
	FVehicleDifferentialConfig& differentialConfig = vehicleMovementComponent->DifferentialSetup;
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Differential.Type: %d"), (int32)differentialConfig.DifferentialType);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Differential.FrontRearSplit: %f"), differentialConfig.FrontRearSplit);

	// Transmission defaults
	FVehicleTransmissionConfig& transmissionConfig = vehicleMovementComponent->TransmissionSetup;
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.ChangeUpRPM: %f"), transmissionConfig.ChangeUpRPM);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.ChangeDownRPM: %f"), transmissionConfig.ChangeDownRPM);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.GearChangeTime: %f"), transmissionConfig.GearChangeTime);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.TransmissionEfficiency: %f"), transmissionConfig.TransmissionEfficiency);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.FinalRatio: %f"), transmissionConfig.FinalRatio);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.GearCount: %d"), transmissionConfig.ForwardGearRatios.Num());
	for (int32 i = 0; i < transmissionConfig.ForwardGearRatios.Num(); i++) {
		UE_LOG(LogSloopedUpExplorer, Log, TEXT("  Gear %d Ratio: %f"), i + 1, transmissionConfig.ForwardGearRatios[i]);
	}

	// Steering defaults
	FVehicleSteeringConfig& steeringConfig = vehicleMovementComponent->SteeringSetup;
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Steering.SteeringType: %d"), (int32)steeringConfig.SteeringType);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Steering.AngleRatio: %f"), steeringConfig.AngleRatio);

	// Steering curve
	FRichCurve* defaultSteeringCurve = steeringConfig.SteeringCurve.GetRichCurve();
	if (defaultSteeringCurve && defaultSteeringCurve->GetNumKeys() > 0) {
		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Steering.SteeringCurve has %d keys:"), defaultSteeringCurve->GetNumKeys());
		for (auto It = defaultSteeringCurve->GetKeyIterator(); It; ++It) {
			const FRichCurveKey& Key = *It;
			UE_LOG(LogSloopedUpExplorer, Log, TEXT("  [%.1f speed] -> %.3f angle multiplier"), Key.Time, Key.Value);
		}
	} else {
		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Steering.SteeringCurve: EMPTY"));
	}

	// Vehicle physics defaults
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.Mass: %f"), vehicleMovementComponent->Mass);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.DragCoefficient: %f"), vehicleMovementComponent->DragCoefficient);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.DownforceCoefficient: %f"), vehicleMovementComponent->DownforceCoefficient);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.ChassisWidth: %f"), vehicleMovementComponent->ChassisWidth);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.ChassisHeight: %f"), vehicleMovementComponent->ChassisHeight);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.InertiaTensorScale: %s"), *vehicleMovementComponent->InertiaTensorScale.ToString());
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.SleepThreshold: %f"), vehicleMovementComponent->SleepThreshold);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.SleepSlopeLimit: %f"), vehicleMovementComponent->SleepSlopeLimit);

	// FG Custom vehicle properties
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.mHandbrakeSideSlipMultiplier: %f"), vehicleMovementComponent->mHandbrakeSideSlipMultiplier);
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.mSurfaceWheelForceMultiplierOverrides Count: %d"), vehicleMovementComponent->mSurfaceWheelForceMultiplierOverrides.Num());
	for (int32 i = 0; i < vehicleMovementComponent->mSurfaceWheelForceMultiplierOverrides.Num(); i++) {
		const FSurfaceForceMultiplierOverride& override = vehicleMovementComponent->mSurfaceWheelForceMultiplierOverrides[i];
		UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SurfaceOverride[%d] ForceMultiplier: %f, Surfaces: %d"), i, override.OverrideForceMultiplier, override.Surfaces.Num());
	}

	// Wheel setup properties
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("WheelSetup Count: %d"), vehicleMovementComponent->WheelSetups.Num());
	for (int32 i = 0; i < vehicleMovementComponent->WheelSetups.Num(); i++) {
		const FChaosWheelSetup& wheelSetup = vehicleMovementComponent->WheelSetups[i];
		UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelSetup[%d] BoneName: %s"), i, *wheelSetup.BoneName.ToString());
	}
	UE_LOG(LogSloopedUpExplorer, Log, TEXT("========================================================"));

	// Tune wheel CDO once on first call
	if (!bWheelCDOTuned) {
		TuneExplorerWheelCDO(vehicleMovementComponent);
	}

	// ============================================================
	// DRIFT TUNING - Phase 1: Engine power for drift maintenance
	// ============================================================

	// Increase torque to maintain rear wheel spin during drifts
	engineConfig.MaxTorque = 500.0f; // Default: 300 - More power to keep wheels spinning

	// Lower MOI so engine revs up faster and doesn't bog down during slides
	engineConfig.EngineRevUpMOI = 0.3f; // Default: 0.6 - Lower = engine stays revved during drift load

	// ============================================================
	// DRIFT TUNING - Phase 2: Steering for counter-steer authority
	// ============================================================

	// Improve steering at drift speeds (60-100 km/h) for better counter-steer
	// Default curve drops to 0.25 at 75 km/h and 0.20 at 120 km/h - not enough for drifting
	FRichCurve* steeringCurve = steeringConfig.SteeringCurve.GetRichCurve();
	if (steeringCurve) {
		steeringCurve->Reset();

		// Keep full steering at low speeds
		FKeyHandle key0 = steeringCurve->AddKey(0.0f, 0.85f);    // Default: 0.85
		steeringCurve->SetKeyInterpMode(key0, RCIM_Cubic);

		FKeyHandle key1 = steeringCurve->AddKey(30.0f, 0.70f);   // Default: 0.57 - MORE steering
		steeringCurve->SetKeyInterpMode(key1, RCIM_Cubic);

		FKeyHandle key2 = steeringCurve->AddKey(60.0f, 0.55f);   // Default: ~0.35 - MORE steering for drift initiation
		steeringCurve->SetKeyInterpMode(key2, RCIM_Cubic);

		FKeyHandle key3 = steeringCurve->AddKey(90.0f, 0.45f);   // Default: ~0.22 - MORE steering for drift maintenance
		steeringCurve->SetKeyInterpMode(key3, RCIM_Cubic);

		FKeyHandle key4 = steeringCurve->AddKey(120.0f, 0.35f);  // Default: 0.20 - MORE steering for high-speed drifts
		steeringCurve->SetKeyInterpMode(key4, RCIM_Cubic);
	}

	// Handbrake side slip multiplier - keeping at default for tap-and-release technique
	// At 0.55, rear produces 55% lateral force when handbrake held (154 force at 35° slip angle)
	// This makes tap-and-release work best: tap to initiate, release to maintain with full grip
	vehicleMovementComponent->mHandbrakeSideSlipMultiplier = 0.55f;

	// Force the component to recreate its physics state to apply the changes
	if (vehicleMovementComponent->UpdatedComponent) {
		vehicleMovementComponent->UpdatedComponent->RecreatePhysicsState();
	}
}

void ASloopedUpExplorerSubsystem::BounceFrontHydraulics(UFGWheeledVehicleMovementComponent* vehicleMovementComponent, float impulseStrength) {
	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("BounceFrontHydraulics called with null vehicleMovementComponent"));
		return;
	}
	UPrimitiveComponent* primComp = Cast<UPrimitiveComponent>(vehicleMovementComponent->UpdatedComponent);
	if (!primComp) {
		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("BounceFrontHydraulics: No UpdatedComponent found"));
		return;
	}

	// Get vehicle vectors and apply upward impulse at front
	FVector forwardVector = primComp->GetForwardVector();
	FVector upVector = FVector::UpVector;
	FVector frontOffset = forwardVector * 150.0f; // 150cm forward from center
	primComp->AddImpulseAtLocation(upVector * impulseStrength, primComp->GetComponentLocation() + frontOffset, NAME_None);
}

void ASloopedUpExplorerSubsystem::TuneExplorerWheelCDO(UFGWheeledVehicleMovementComponent* vehicleMovementComponent) {
	if (bWheelCDOTuned || !vehicleMovementComponent) {
		return;
	}

	UE_LOG(LogSloopedUpExplorer, Log, TEXT("========== WHEEL DEFAULT VALUES =========="));

	// Track which wheel classes we've already processed to avoid logging/modifying the same CDO multiple times
	TSet<UClass*> ProcessedWheelClasses;

	// Modify the wheel CDOs from this Explorer's wheel classes
	for (int32 i = 0; i < vehicleMovementComponent->WheelSetups.Num(); i++) {
		FChaosWheelSetup& wheelSetup = vehicleMovementComponent->WheelSetups[i];
		if (wheelSetup.WheelClass) {
			// Skip if we've already processed this wheel class
			if (ProcessedWheelClasses.Contains(wheelSetup.WheelClass)) {
				continue;
			}
			ProcessedWheelClasses.Add(wheelSetup.WheelClass);

			UChaosVehicleWheel* wheelCDO = Cast<UChaosVehicleWheel>(wheelSetup.WheelClass->GetDefaultObject());
			if (wheelCDO) {
				// Log all default values before modification
				FString axleName = (wheelCDO->AxleType == EAxleType::Front) ? TEXT("FRONT") : TEXT("REAR");
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("--- %s Wheel (Class: %s) ---"), *axleName, *wheelSetup.WheelClass->GetName());
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  Offset: %f, %f, %f"), wheelCDO->Offset.X, wheelCDO->Offset.Y, wheelCDO->Offset.Z);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelRadius: %f"), wheelCDO->WheelRadius);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelWidth: %f"), wheelCDO->WheelWidth);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelMass: %f"), wheelCDO->WheelMass);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  CorneringStiffness: %f"), wheelCDO->CorneringStiffness);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  FrictionForceMultiplier: %f"), wheelCDO->FrictionForceMultiplier);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SideSlipModifier: %f"), wheelCDO->SideSlipModifier);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SlipThreshold: %f"), wheelCDO->SlipThreshold);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SkidThreshold: %f"), wheelCDO->SkidThreshold);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  MaxSteerAngle: %f"), wheelCDO->MaxSteerAngle);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bAffectedBySteering: %d"), wheelCDO->bAffectedBySteering);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bAffectedByBrake: %d"), wheelCDO->bAffectedByBrake);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bAffectedByHandbrake: %d"), wheelCDO->bAffectedByHandbrake);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bAffectedByEngine: %d"), wheelCDO->bAffectedByEngine);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bABSEnabled: %d"), wheelCDO->bABSEnabled);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bTractionControlEnabled: %d"), wheelCDO->bTractionControlEnabled);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  MaxWheelspinRotation: %f"), wheelCDO->MaxWheelspinRotation);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  ExternalTorqueCombineMethod: %d"), static_cast<int32>(wheelCDO->ExternalTorqueCombineMethod));
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SuspensionMaxRaise: %f"), wheelCDO->SuspensionMaxRaise);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SuspensionMaxDrop: %f"), wheelCDO->SuspensionMaxDrop);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SuspensionDampingRatio: %f"), wheelCDO->SuspensionDampingRatio);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelLoadRatio: %f"), wheelCDO->WheelLoadRatio);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SpringRate: %f"), wheelCDO->SpringRate);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SpringPreload: %f"), wheelCDO->SpringPreload);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SuspensionSmoothing: %f"), wheelCDO->SuspensionSmoothing);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  RollbarScaling: %f"), wheelCDO->RollbarScaling);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SweepShape: %d"), (int32)wheelCDO->SweepShape);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SweepType: %d"), (int32)wheelCDO->SweepType);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  MaxBrakeTorque: %f"), wheelCDO->MaxBrakeTorque);
				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  MaxHandBrakeTorque: %f"), wheelCDO->MaxHandBrakeTorque);

				// Log lateral slip graph
				FRichCurve* lateralSlipCurve = wheelCDO->LateralSlipGraph.GetRichCurve();
				if (lateralSlipCurve && lateralSlipCurve->GetNumKeys() > 0) {
					UE_LOG(LogSloopedUpExplorer, Log, TEXT("  LateralSlipGraph has %d keys:"), lateralSlipCurve->GetNumKeys());
					for (auto It = lateralSlipCurve->GetKeyIterator(); It; ++It) {
						const FRichCurveKey& Key = *It;
						UE_LOG(LogSloopedUpExplorer, Log, TEXT("    [%.1f degree] -> %.1f force"), Key.Time, Key.Value);
					}
				} else {
					UE_LOG(LogSloopedUpExplorer, Log, TEXT("  LateralSlipGraph: EMPTY (no curve defined)"));
				}

				// ============================================================
				// DRIFT TUNING - Phase 3: Lateral Slip Graphs ONLY
				// All other wheel properties stay at defaults
				// ============================================================

				if (wheelCDO->AxleType == EAxleType::Front) {
					// Front wheel lateral slip graph:
					// - Strong grip for counter-steer authority
					// - Wide plateau so steering remains effective during drift
					// - Gradual drop-off (not a cliff) for predictable behavior
					FRichCurve* frontLateralSlipCurve = wheelCDO->LateralSlipGraph.GetRichCurve();
					if (frontLateralSlipCurve) {
						frontLateralSlipCurve->Reset();

						// Zero at zero slip
						FKeyHandle fKey0 = frontLateralSlipCurve->AddKey(0.0f, 0.0f);
						frontLateralSlipCurve->SetKeyInterpMode(fKey0, RCIM_Cubic);

						// Quick rise to peak - responsive steering
						FKeyHandle fKey1 = frontLateralSlipCurve->AddKey(6.0f, 950.0f);
						frontLateralSlipCurve->SetKeyInterpMode(fKey1, RCIM_Cubic);

						// Wide plateau through normal cornering - maintains steering authority
						FKeyHandle fKey2 = frontLateralSlipCurve->AddKey(15.0f, 800.0f);
						frontLateralSlipCurve->SetKeyInterpMode(fKey2, RCIM_Cubic);

						// Gradual drop in drift zone - still provides strong counter-steer force
						FKeyHandle fKey3 = frontLateralSlipCurve->AddKey(30.0f, 500.0f);
						frontLateralSlipCurve->SetKeyInterpMode(fKey3, RCIM_Cubic);

						// Moderate force at high angles - allows steering to "catch" the drift
						FKeyHandle fKey4 = frontLateralSlipCurve->AddKey(45.0f, 300.0f);
						frontLateralSlipCurve->SetKeyInterpMode(fKey4, RCIM_Cubic);

						// Low force at extreme angles
						FKeyHandle fKey5 = frontLateralSlipCurve->AddKey(75.0f, 150.0f);
						frontLateralSlipCurve->SetKeyInterpMode(fKey5, RCIM_Cubic);
					}

					wheelCDO->MaxHandBrakeTorque = 0.0f;

				} else if (wheelCDO->AxleType == EAxleType::Rear) {
					// Rear wheel lateral slip graph:
					// - Strong low-angle grip for normal driving and traction
					// - Steeper drop after 12° for easier drift entry
					// - Lower force in drift zone for more slide (280 at 35°)
					FRichCurve* rearLateralSlipCurve = wheelCDO->LateralSlipGraph.GetRichCurve();
					if (rearLateralSlipCurve) {
						rearLateralSlipCurve->Reset();

						// Zero at zero slip
						FKeyHandle rKey0 = rearLateralSlipCurve->AddKey(0.0f, 0.0f);
						rearLateralSlipCurve->SetKeyInterpMode(rKey0, RCIM_Cubic);

						// Quick rise for responsive grip
						FKeyHandle rKey1 = rearLateralSlipCurve->AddKey(5.0f, 800.0f);
						rearLateralSlipCurve->SetKeyInterpMode(rKey1, RCIM_Cubic);

						// Peak grip for normal driving - slightly lower than front (promotes understeer)
						FKeyHandle rKey2 = rearLateralSlipCurve->AddKey(12.0f, 1000.0f);
						rearLateralSlipCurve->SetKeyInterpMode(rKey2, RCIM_Cubic);

						// Steeper drop - easier drift entry
						FKeyHandle rKey3 = rearLateralSlipCurve->AddKey(20.0f, 450.0f);
						rearLateralSlipCurve->SetKeyInterpMode(rKey3, RCIM_Cubic);

						// Drift maintenance zone - lower force for more slide
						FKeyHandle rKey4 = rearLateralSlipCurve->AddKey(35.0f, 280.0f);
						rearLateralSlipCurve->SetKeyInterpMode(rKey4, RCIM_Cubic);

						// Sustained slide zone - low for extended drifts
						FKeyHandle rKey5 = rearLateralSlipCurve->AddKey(55.0f, 180.0f);
						rearLateralSlipCurve->SetKeyInterpMode(rKey5, RCIM_Cubic);

						// Minimal at extreme angles - allows recovery from big slides
						FKeyHandle rKey6 = rearLateralSlipCurve->AddKey(90.0f, 120.0f);
						rearLateralSlipCurve->SetKeyInterpMode(rKey6, RCIM_Cubic);
					}
				}
			}
		}
	}

	UE_LOG(LogSloopedUpExplorer, Log, TEXT("========== END WHEEL DEFAULT VALUES =========="));

	bWheelCDOTuned = true;
}