// #include "SloopedUpExplorerSubsystem.h"
// #include "Subsystem/SubsystemActorManager.h"
// #include "Kismet/GameplayStatics.h"

// #include "WheeledVehicles/FGWheeledVehicleMovementComponent.h"

// DEFINE_LOG_CATEGORY(LogSloopedUpExplorer);

// ASloopedUpExplorerSubsystem::ASloopedUpExplorerSubsystem() {
// 	ReplicationPolicy = ESubsystemReplicationPolicy::SpawnOnServer_Replicate;
// }

// ASloopedUpExplorerSubsystem* ASloopedUpExplorerSubsystem::Get(UWorld* world) {
// 	if (!world) {
// 		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("Get called with null world"));
// 		return nullptr;
// 	}
// 	USubsystemActorManager* subsystemActorManager = world->GetSubsystem<USubsystemActorManager>();
// 	if (!subsystemActorManager) {
// 		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("SubsystemActorManager not found in world"));
// 		return nullptr;
// 	}
// 	ASloopedUpExplorerSubsystem* SloopedUpExplorerSubsystem = subsystemActorManager->GetSubsystemActor<ASloopedUpExplorerSubsystem>();
// 	if (!SloopedUpExplorerSubsystem) {
// 		SloopedUpExplorerSubsystem = Cast<ASloopedUpExplorerSubsystem>(UGameplayStatics::GetActorOfClass(world, ASloopedUpExplorerSubsystem::StaticClass()));
// 		if (!SloopedUpExplorerSubsystem) {
// 			UE_LOG(LogSloopedUpExplorer, Warning, TEXT("SloopedUpExplorerSubsystem not found in world"));
// 			return nullptr;
// 		}
// 	}
// 	return SloopedUpExplorerSubsystem;
// }

// void ASloopedUpExplorerSubsystem::TuneExplorer(UFGWheeledVehicleMovementComponent* vehicleMovementComponent) {
// 	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
// 		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("TuneExplorer called with null vehicleMovementComponent"));
// 		return;
// 	}
// 	// Authority check
// 	if (!HasAuthority()) {
// 		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("TuneExplorer called on client, changes may not persist"));
// 	}

// 	// Log all default values BEFORE modification
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("========== MOVEMENT COMPONENT DEFAULT VALUES =========="));

// 	// Engine defaults
// 	FVehicleEngineConfig& engineConfig = vehicleMovementComponent->EngineSetup;
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.MaxTorque: %f"), engineConfig.MaxTorque);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.MaxRPM: %f"), engineConfig.MaxRPM);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.EngineIdleRPM: %f"), engineConfig.EngineIdleRPM);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.EngineBrakeEffect: %f"), engineConfig.EngineBrakeEffect);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.EngineRevUpMOI: %f"), engineConfig.EngineRevUpMOI);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.EngineRevDownRate: %f"), engineConfig.EngineRevDownRate);

// 	// Engine torque curve
// 	FRichCurve* defaultTorqueCurve = engineConfig.TorqueCurve.GetRichCurve();
// 	if (defaultTorqueCurve && defaultTorqueCurve->GetNumKeys() > 0) {
// 		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.TorqueCurve has %d keys:"), defaultTorqueCurve->GetNumKeys());
// 		for (auto It = defaultTorqueCurve->GetKeyIterator(); It; ++It) {
// 			const FRichCurveKey& Key = *It;
// 			UE_LOG(LogSloopedUpExplorer, Log, TEXT("  [%.1f RPM] -> %.3f torque multiplier"), Key.Time, Key.Value);
// 		}
// 	} else {
// 		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Engine.TorqueCurve: EMPTY"));
// 	}

// 	// Differential defaults
// 	FVehicleDifferentialConfig& differentialConfig = vehicleMovementComponent->DifferentialSetup;
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Differential.Type: %d"), (int32)differentialConfig.DifferentialType);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Differential.FrontRearSplit: %f"), differentialConfig.FrontRearSplit);

// 	// Transmission defaults
// 	FVehicleTransmissionConfig& transmissionConfig = vehicleMovementComponent->TransmissionSetup;
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.ChangeUpRPM: %f"), transmissionConfig.ChangeUpRPM);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.ChangeDownRPM: %f"), transmissionConfig.ChangeDownRPM);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.GearChangeTime: %f"), transmissionConfig.GearChangeTime);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.TransmissionEfficiency: %f"), transmissionConfig.TransmissionEfficiency);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.FinalRatio: %f"), transmissionConfig.FinalRatio);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Transmission.GearCount: %d"), transmissionConfig.ForwardGearRatios.Num());
// 	for (int32 i = 0; i < transmissionConfig.ForwardGearRatios.Num(); i++) {
// 		UE_LOG(LogSloopedUpExplorer, Log, TEXT("  Gear %d Ratio: %f"), i + 1, transmissionConfig.ForwardGearRatios[i]);
// 	}

// 	// Steering defaults
// 	FVehicleSteeringConfig& steeringConfig = vehicleMovementComponent->SteeringSetup;
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Steering.SteeringType: %d"), (int32)steeringConfig.SteeringType);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Steering.AngleRatio: %f"), steeringConfig.AngleRatio);

// 	// Steering curve
// 	FRichCurve* defaultSteeringCurve = steeringConfig.SteeringCurve.GetRichCurve();
// 	if (defaultSteeringCurve && defaultSteeringCurve->GetNumKeys() > 0) {
// 		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Steering.SteeringCurve has %d keys:"), defaultSteeringCurve->GetNumKeys());
// 		for (auto It = defaultSteeringCurve->GetKeyIterator(); It; ++It) {
// 			const FRichCurveKey& Key = *It;
// 			UE_LOG(LogSloopedUpExplorer, Log, TEXT("  [%.1f speed] -> %.3f angle multiplier"), Key.Time, Key.Value);
// 		}
// 	} else {
// 		UE_LOG(LogSloopedUpExplorer, Log, TEXT("Steering.SteeringCurve: EMPTY"));
// 	}

// 	// Vehicle physics defaults
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.Mass: %f"), vehicleMovementComponent->Mass);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.DragCoefficient: %f"), vehicleMovementComponent->DragCoefficient);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.DownforceCoefficient: %f"), vehicleMovementComponent->DownforceCoefficient);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.ChassisWidth: %f"), vehicleMovementComponent->ChassisWidth);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.ChassisHeight: %f"), vehicleMovementComponent->ChassisHeight);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.InertiaTensorScale: %s"), *vehicleMovementComponent->InertiaTensorScale.ToString());
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.SleepThreshold: %f"), vehicleMovementComponent->SleepThreshold);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.SleepSlopeLimit: %f"), vehicleMovementComponent->SleepSlopeLimit);

// 	// FG Custom vehicle properties
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.mHandbrakeSideSlipMultiplier: %f"), vehicleMovementComponent->mHandbrakeSideSlipMultiplier);
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("Vehicle.mSurfaceWheelForceMultiplierOverrides Count: %d"), vehicleMovementComponent->mSurfaceWheelForceMultiplierOverrides.Num());
// 	for (int32 i = 0; i < vehicleMovementComponent->mSurfaceWheelForceMultiplierOverrides.Num(); i++) {
// 		const FSurfaceForceMultiplierOverride& override = vehicleMovementComponent->mSurfaceWheelForceMultiplierOverrides[i];
// 		UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SurfaceOverride[%d] ForceMultiplier: %f, Surfaces: %d"), i, override.OverrideForceMultiplier, override.Surfaces.Num());
// 	}

// 	// Wheel setup properties
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("WheelSetup Count: %d"), vehicleMovementComponent->WheelSetups.Num());
// 	for (int32 i = 0; i < vehicleMovementComponent->WheelSetups.Num(); i++) {
// 		const FChaosWheelSetup& wheelSetup = vehicleMovementComponent->WheelSetups[i];
// 		UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelSetup[%d] BoneName: %s"), i, *wheelSetup.BoneName.ToString());
// 	}
// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("========================================================"));

// 	// Tune wheel CDO once on first call
// 	if (!bWheelCDOTuned) {
// 		TuneExplorerWheelCDO(vehicleMovementComponent);
// 	}

// 	// Engine Config
// 	engineConfig.MaxTorque = 625.0f; // Default 300
// 	engineConfig.MaxRPM = 9000.0f; // Default 9000
// 	engineConfig.EngineRevUpMOI = 20.0f; // Default 0.6
// 	// Torque Curve: RPM vs Torque multipler. Modified to bring higher power towards higher RPMs
// 	FRichCurve* torqueCurve = engineConfig.TorqueCurve.GetRichCurve();
// 	if (torqueCurve) {
// 		torqueCurve->Reset();

// 		FKeyHandle key0 = torqueCurve->AddKey(0.0f, 0.70f);
// 		torqueCurve->SetKeyInterpMode(key0, RCIM_Cubic);
// 		torqueCurve->SetKeyTangentMode(key0, RCTM_User);
// 		torqueCurve->GetKey(key0).LeaveTangent = 0.000060f;

// 		FKeyHandle key1 = torqueCurve->AddKey(1500.0f, 0.88f);
// 		torqueCurve->SetKeyInterpMode(key1, RCIM_Cubic);
// 		torqueCurve->SetKeyTangentMode(key1, RCTM_User);
// 		torqueCurve->GetKey(key1).ArriveTangent = 0.000080f;
// 		torqueCurve->GetKey(key1).LeaveTangent = 0.000080f;

// 		FKeyHandle key2 = torqueCurve->AddKey(3500.0f, 1.0f);
// 		torqueCurve->SetKeyInterpMode(key2, RCIM_Cubic);
// 		torqueCurve->SetKeyTangentMode(key2, RCTM_User);
// 		torqueCurve->GetKey(key2).ArriveTangent = 0.000040f;
// 		torqueCurve->GetKey(key2).LeaveTangent = 0.000000f;

// 		FKeyHandle key3 = torqueCurve->AddKey(7000.0f, 0.98f);
// 		torqueCurve->SetKeyInterpMode(key3, RCIM_Cubic);
// 		torqueCurve->SetKeyTangentMode(key3, RCTM_User);
// 		torqueCurve->GetKey(key3).ArriveTangent = -0.000005f;
// 		torqueCurve->GetKey(key3).LeaveTangent = -0.000005f;

// 		FKeyHandle key4 = torqueCurve->AddKey(9000.0f, 0.96f);
// 		torqueCurve->SetKeyInterpMode(key4, RCIM_Cubic);
// 		torqueCurve->SetKeyTangentMode(key4, RCTM_User);
// 		torqueCurve->GetKey(key4).ArriveTangent = -0.000010f;
// 	}

// 	// Differential Config
// 	// differentialConfig.DifferentialType = EVehicleDifferential::RearWheelDrive; // Default AllWheelDrive
// 	// differentialConfig.FrontRearSplit = 1.0f; // Default 0.6

// 	// Transmission Config
// 	// FVehicleTransmissionConfig& transmissionConfig = vehicleMovementComponent->TransmissionSetup;
// 	// transmissionConfig.ChangeUpRPM = 8500.0f; // Default 8500
// 	// transmissionConfig.ChangeDownRPM = 3000.0f; // Default 4000
// 	// Forward Gear Ratios: lower 1st for better launch, lower 6th for higher top speed
// 	// transmissionConfig.ForwardGearRatios.Empty();
// 	// transmissionConfig.ForwardGearRatios.Add(3.20f); // 1st, default 3.76
// 	// transmissionConfig.ForwardGearRatios.Add(2.10f); // 2nd, default 2.27
// 	// transmissionConfig.ForwardGearRatios.Add(1.55f); // 3rd, default 1.65
// 	// transmissionConfig.ForwardGearRatios.Add(1.15f); // 4th, default 1.19
// 	// transmissionConfig.ForwardGearRatios.Add(0.95f); // 5th, default 1.0
// 	// transmissionConfig.ForwardGearRatios.Add(0.70f); // 6th, default 0.78
// 	// transmissionConfig.FinalRatio = 11.5f; // Default 11.5

// 	// Steering Config
// 	// Steering Curve: Speed vs Steering Angle multiplier. Modified to allow sharper steering at low speeds while tapering off more aggressively at high speeds for stability
// 	FRichCurve* steeringCurve = steeringConfig.SteeringCurve.GetRichCurve();
// 	if (steeringCurve) {
// 		steeringCurve->Reset();

// 		FKeyHandle key0 = steeringCurve->AddKey(0.0f, 1.0f);
// 		steeringCurve->SetKeyInterpMode(key0, RCIM_Cubic);
// 		steeringCurve->SetKeyTangentMode(key0, RCTM_User);
// 		steeringCurve->GetKey(key0).LeaveTangent = -0.008f;

// 		FKeyHandle key1 = steeringCurve->AddKey(30.0f, 1.0f);
// 		steeringCurve->SetKeyInterpMode(key1, RCIM_Cubic);
// 		steeringCurve->SetKeyTangentMode(key1, RCTM_User);
// 		steeringCurve->GetKey(key1).ArriveTangent = -0.008f;
// 		steeringCurve->GetKey(key1).LeaveTangent = -0.010f;

// 		FKeyHandle key2 = steeringCurve->AddKey(60.0f, 1.0f);
// 		steeringCurve->SetKeyInterpMode(key2, RCIM_Cubic);
// 		steeringCurve->SetKeyTangentMode(key2, RCTM_User);
// 		steeringCurve->GetKey(key2).ArriveTangent = -0.010f;
// 		steeringCurve->GetKey(key2).LeaveTangent = -0.008f;

// 		FKeyHandle key3 = steeringCurve->AddKey(100.0f, 0.75f);
// 		steeringCurve->SetKeyInterpMode(key3, RCIM_Cubic);
// 		steeringCurve->SetKeyTangentMode(key3, RCTM_User);
// 		steeringCurve->GetKey(key3).ArriveTangent = -0.005f;
// 		steeringCurve->GetKey(key3).LeaveTangent = -0.002f;

// 		FKeyHandle key4 = steeringCurve->AddKey(150.0f, 0.55f); // More steering at drift speeds
// 		steeringCurve->SetKeyInterpMode(key4, RCIM_Cubic);
// 		steeringCurve->SetKeyTangentMode(key4, RCTM_User);
// 		steeringCurve->GetKey(key4).ArriveTangent = -0.002f;
// 		steeringCurve->GetKey(key4).LeaveTangent = -0.001f;

// 		FKeyHandle key5 = steeringCurve->AddKey(200.0f, 0.45f); // Better control at high speed
// 		steeringCurve->SetKeyInterpMode(key5, RCIM_Cubic);
// 		steeringCurve->SetKeyTangentMode(key5, RCTM_User);
// 		steeringCurve->GetKey(key5).ArriveTangent = -0.001f;

// 		FKeyHandle key6 = steeringCurve->AddKey(250.0f, 0.35f);
// 		steeringCurve->SetKeyInterpMode(key6, RCIM_Cubic);
// 		steeringCurve->SetKeyTangentMode(key6, RCTM_User);
// 		steeringCurve->GetKey(key6).ArriveTangent = -0.0005f;
// 	}

// 	// Vehicle Config
// 	vehicleMovementComponent->Mass = 750.0f; // Increased for better momentum conservation through drifts
// 	vehicleMovementComponent->DragCoefficient = 0.2f; // Default 0.3, modified to reduce top speed loss from drag and allow better coasting
// 	vehicleMovementComponent->DownforceCoefficient = 1.0f; // Default 1.0, modified so suspension doesn't bottom out at high speeds

// 	// Force the component to recreate its physics state to apply the changes
// 	if (vehicleMovementComponent->UpdatedComponent) {
// 		vehicleMovementComponent->UpdatedComponent->RecreatePhysicsState();
// 	}
// 	// Mark for replication if in multiplayer
// 	vehicleMovementComponent->MarkPackageDirty();
// }

// void ASloopedUpExplorerSubsystem::BounceFrontHydraulics(UFGWheeledVehicleMovementComponent* vehicleMovementComponent, float impulseStrength) {
// 	if (!vehicleMovementComponent || !IsValid(vehicleMovementComponent)) {
// 		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("BounceFrontHydraulics called with null vehicleMovementComponent"));
// 		return;
// 	}
// 	UPrimitiveComponent* primComp = Cast<UPrimitiveComponent>(vehicleMovementComponent->UpdatedComponent);
// 	if (!primComp) {
// 		UE_LOG(LogSloopedUpExplorer, Warning, TEXT("BounceFrontHydraulics: No UpdatedComponent found"));
// 		return;
// 	}

// 	// Get vehicle vectors and apply upward impulse at front
// 	FVector forwardVector = primComp->GetForwardVector();
// 	FVector upVector = FVector::UpVector;
// 	FVector frontOffset = forwardVector * 150.0f; // 150cm forward from center
// 	primComp->AddImpulseAtLocation(upVector * impulseStrength, primComp->GetComponentLocation() + frontOffset, NAME_None);
// }

// void ASloopedUpExplorerSubsystem::TuneExplorerWheelCDO(UFGWheeledVehicleMovementComponent* vehicleMovementComponent) {
// 	if (bWheelCDOTuned || !vehicleMovementComponent) {
// 		return;
// 	}

// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("========== WHEEL DEFAULT VALUES =========="));

// 	// Track which wheel classes we've already processed to avoid logging/modifying the same CDO multiple times
// 	TSet<UClass*> ProcessedWheelClasses;

// 	// Modify the wheel CDOs from this Explorer's wheel classes
// 	for (int32 i = 0; i < vehicleMovementComponent->WheelSetups.Num(); i++) {
// 		FChaosWheelSetup& wheelSetup = vehicleMovementComponent->WheelSetups[i];
// 		if (wheelSetup.WheelClass) {
// 			// Skip if we've already processed this wheel class
// 			if (ProcessedWheelClasses.Contains(wheelSetup.WheelClass)) {
// 				continue;
// 			}
// 			ProcessedWheelClasses.Add(wheelSetup.WheelClass);

// 			UChaosVehicleWheel* wheelCDO = Cast<UChaosVehicleWheel>(wheelSetup.WheelClass->GetDefaultObject());
// 			if (wheelCDO) {
// 				// Log all default values before modification
// 				FString axleName = (wheelCDO->AxleType == EAxleType::Front) ? TEXT("FRONT") : TEXT("REAR");
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("--- %s Wheel (Class: %s) ---"), *axleName, *wheelSetup.WheelClass->GetName());
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  Offset: %f, %f, %f"), wheelCDO->Offset.X, wheelCDO->Offset.Y, wheelCDO->Offset.Z);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelRadius: %f"), wheelCDO->WheelRadius);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelWidth: %f"), wheelCDO->WheelWidth);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelMass: %f"), wheelCDO->WheelMass);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  CorneringStiffness: %f"), wheelCDO->CorneringStiffness);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  FrictionForceMultiplier: %f"), wheelCDO->FrictionForceMultiplier);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SideSlipModifier: %f"), wheelCDO->SideSlipModifier);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SlipThreshold: %f"), wheelCDO->SlipThreshold);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SkidThreshold: %f"), wheelCDO->SkidThreshold);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  MaxSteerAngle: %f"), wheelCDO->MaxSteerAngle);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bAffectedBySteering: %d"), wheelCDO->bAffectedBySteering);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bAffectedByBrake: %d"), wheelCDO->bAffectedByBrake);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bAffectedByHandbrake: %d"), wheelCDO->bAffectedByHandbrake);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bAffectedByEngine: %d"), wheelCDO->bAffectedByEngine);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bABSEnabled: %d"), wheelCDO->bABSEnabled);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  bTractionControlEnabled: %d"), wheelCDO->bTractionControlEnabled);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  MaxWheelspinRotation: %f"), wheelCDO->MaxWheelspinRotation);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  ExternalTorqueCombineMethod: %d"), static_cast<int32>(wheelCDO->ExternalTorqueCombineMethod));
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SuspensionMaxRaise: %f"), wheelCDO->SuspensionMaxRaise);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SuspensionMaxDrop: %f"), wheelCDO->SuspensionMaxDrop);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SuspensionDampingRatio: %f"), wheelCDO->SuspensionDampingRatio);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  WheelLoadRatio: %f"), wheelCDO->WheelLoadRatio);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SpringRate: %f"), wheelCDO->SpringRate);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SpringPreload: %f"), wheelCDO->SpringPreload);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SuspensionSmoothing: %f"), wheelCDO->SuspensionSmoothing);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  RollbarScaling: %f"), wheelCDO->RollbarScaling);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SweepShape: %d"), (int32)wheelCDO->SweepShape);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  SweepType: %d"), (int32)wheelCDO->SweepType);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  MaxBrakeTorque: %f"), wheelCDO->MaxBrakeTorque);
// 				UE_LOG(LogSloopedUpExplorer, Log, TEXT("  MaxHandBrakeTorque: %f"), wheelCDO->MaxHandBrakeTorque);

// 				// Log lateral slip graph
// 				FRichCurve* lateralSlipCurve = wheelCDO->LateralSlipGraph.GetRichCurve();
// 				if (lateralSlipCurve && lateralSlipCurve->GetNumKeys() > 0) {
// 					UE_LOG(LogSloopedUpExplorer, Log, TEXT("  LateralSlipGraph has %d keys:"), lateralSlipCurve->GetNumKeys());
// 					for (auto It = lateralSlipCurve->GetKeyIterator(); It; ++It) {
// 						const FRichCurveKey& Key = *It;
// 						UE_LOG(LogSloopedUpExplorer, Log, TEXT("    [%.1f degree] -> %.1f force"), Key.Time, Key.Value);
// 					}
// 				} else {
// 					UE_LOG(LogSloopedUpExplorer, Log, TEXT("  LateralSlipGraph: EMPTY (no curve defined)"));
// 				}

// 				if (wheelCDO->AxleType == EAxleType::Front) {
// 					wheelCDO->WheelMass = 80.0f;
// 					wheelCDO->CorneringStiffness = 1400.0f;
// 					wheelCDO->FrictionForceMultiplier = 4.2f;
// 					wheelCDO->SideSlipModifier = 0.8f;
// 					wheelCDO->SlipThreshold = 15.0f;
// 					wheelCDO->SkidThreshold = 35.0f;
// 					// wheelCDO->MaxSteerAngle = 75.0f;
// 					wheelCDO->bAffectedBySteering = true;
// 					wheelCDO->bAffectedByBrake = true;
// 					wheelCDO->bAffectedByHandbrake = false;
// 					wheelCDO->bAffectedByEngine = false;
// 					wheelCDO->bABSEnabled = false;
// 					wheelCDO->bTractionControlEnabled = false;
// 					wheelCDO->MaxWheelspinRotation = 1000000.0f;

// 					wheelCDO->SuspensionMaxRaise = 105.0f;
// 					wheelCDO->SuspensionMaxDrop = 50.0f;
// 					wheelCDO->SuspensionDampingRatio = 1.0f;
// 					wheelCDO->WheelLoadRatio = 0.55f;
// 					wheelCDO->SpringRate = 70.0f;
// 					wheelCDO->SpringPreload = 450.0f;
// 					wheelCDO->SuspensionSmoothing = 0.8f;
// 					wheelCDO->RollbarScaling = 0.4f;
// 					wheelCDO->MaxBrakeTorque = 7500.0f;
// 					wheelCDO->MaxHandBrakeTorque = 0.0f;

// 					// Front wheel lateral slip graph: Narrow peak grip, drops off quickly for drift flow
// 					FRichCurve* frontLateralSlipCurve = wheelCDO->LateralSlipGraph.GetRichCurve();
// 					if (frontLateralSlipCurve) {
// 						frontLateralSlipCurve->Reset();

// 						// Strong initial grip for normal driving
// 						FKeyHandle fKey0 = frontLateralSlipCurve->AddKey(0.0f, 0.0f);
// 						frontLateralSlipCurve->SetKeyInterpMode(fKey0, RCIM_Cubic);
// 						frontLateralSlipCurve->SetKeyTangentMode(fKey0, RCTM_Auto);

// 						// Narrow peak at low angles for acceleration and straight driving
// 						FKeyHandle fKey1 = frontLateralSlipCurve->AddKey(5.0f, 1000.0f);
// 						frontLateralSlipCurve->SetKeyInterpMode(fKey1, RCIM_Cubic);
// 						frontLateralSlipCurve->SetKeyTangentMode(fKey1, RCTM_Auto);

// 						// Quick drop-off after peak
// 						FKeyHandle fKey2 = frontLateralSlipCurve->AddKey(15.0f, 400.0f);
// 						frontLateralSlipCurve->SetKeyInterpMode(fKey2, RCIM_Cubic);
// 						frontLateralSlipCurve->SetKeyTangentMode(fKey2, RCTM_Auto);

// 						// Moderate drift steering zone
// 						FKeyHandle fKey3 = frontLateralSlipCurve->AddKey(30.0f, 300.0f);
// 						frontLateralSlipCurve->SetKeyInterpMode(fKey3, RCIM_Cubic);
// 						frontLateralSlipCurve->SetKeyTangentMode(fKey3, RCTM_Auto);

// 						// Low force at high angles - allows front to slide during counter-steer
// 						FKeyHandle fKey4 = frontLateralSlipCurve->AddKey(45.0f, 200.0f);
// 						frontLateralSlipCurve->SetKeyInterpMode(fKey4, RCIM_Cubic);
// 						frontLateralSlipCurve->SetKeyTangentMode(fKey4, RCTM_Auto);

// 						// Minimal at extreme angles
// 						FKeyHandle fKey5 = frontLateralSlipCurve->AddKey(75.0f, 120.0f);
// 						frontLateralSlipCurve->SetKeyInterpMode(fKey5, RCIM_Cubic);
// 						frontLateralSlipCurve->SetKeyTangentMode(fKey5, RCTM_Auto);
// 					}
// 				} else if (wheelCDO->AxleType == EAxleType::Rear) {
// 					wheelCDO->WheelMass = 80.0f;
// 					wheelCDO->CorneringStiffness = 1000.0f;
// 					wheelCDO->FrictionForceMultiplier = 5.0f;
// 					wheelCDO->SideSlipModifier = 0.8f;
// 					wheelCDO->SlipThreshold = 200.0f;
// 					wheelCDO->SkidThreshold = 1000.0f;
// 					// wheelCDO->MaxSteerAngle = 50.0f; // ARVP -5.0f
// 					wheelCDO->bAffectedBySteering = false;
// 					wheelCDO->bAffectedByBrake = true;
// 					wheelCDO->bAffectedByHandbrake = true;
// 					wheelCDO->bAffectedByEngine = true;
// 					wheelCDO->bABSEnabled = false;
// 					wheelCDO->bTractionControlEnabled = false;
// 					wheelCDO->MaxWheelspinRotation = 1000000.0f; // ARVP 20.0f

// 					wheelCDO->SuspensionMaxRaise = 105.0f;
// 					wheelCDO->SuspensionMaxDrop = 50.0f;
// 					wheelCDO->SuspensionDampingRatio = 1.0f;
// 					wheelCDO->WheelLoadRatio = 0.45f;
// 					wheelCDO->SpringRate = 55.0f;
// 					wheelCDO->SpringPreload = 380.0f;
// 					wheelCDO->SuspensionSmoothing = 1.0f;
// 					wheelCDO->RollbarScaling = 0.15f;
// 					wheelCDO->MaxBrakeTorque = 500.0f;
// 					wheelCDO->MaxHandBrakeTorque = 6000.0f;

// 					// Rear wheel lateral slip graph: Strong low-angle grip, sharp drop for drift momentum
// 					FRichCurve* rearLateralSlipCurve = wheelCDO->LateralSlipGraph.GetRichCurve();
// 					if (rearLateralSlipCurve) {
// 						rearLateralSlipCurve->Reset();

// 						// Strong forces at very low angles for forward traction
// 						FKeyHandle rKey0 = rearLateralSlipCurve->AddKey(0.0f, 0.0f);
// 						rearLateralSlipCurve->SetKeyInterpMode(rKey0, RCIM_Linear);

// 						// Very strong initial grip for hard acceleration
// 						FKeyHandle rKey1 = rearLateralSlipCurve->AddKey(3.0f, 1500.0f);
// 						rearLateralSlipCurve->SetKeyInterpMode(rKey1, RCIM_Linear);

// 						// Peak grip for normal driving
// 						FKeyHandle rKey2 = rearLateralSlipCurve->AddKey(8.0f, 2200.0f);
// 						rearLateralSlipCurve->SetKeyInterpMode(rKey2, RCIM_Linear);

// 						// Maintain strong grip through normal cornering
// 						FKeyHandle rKey3 = rearLateralSlipCurve->AddKey(15.0f, 2400.0f);
// 						rearLateralSlipCurve->SetKeyInterpMode(rKey3, RCIM_Linear);

// 						// Sharp cliff - grip drops dramatically for drift breakaway
// 						FKeyHandle rKey4 = rearLateralSlipCurve->AddKey(20.0f, 600.0f);
// 						rearLateralSlipCurve->SetKeyInterpMode(rKey4, RCIM_Linear);

// 						// Low drift zone - allows momentum to carry through
// 						FKeyHandle rKey5 = rearLateralSlipCurve->AddKey(28.0f, 280.0f);
// 						rearLateralSlipCurve->SetKeyInterpMode(rKey5, RCIM_Linear);

// 						// Sustained drift - minimal resistance for sliding
// 						FKeyHandle rKey6 = rearLateralSlipCurve->AddKey(40.0f, 180.0f);
// 						rearLateralSlipCurve->SetKeyInterpMode(rKey6, RCIM_Linear);

// 						// Deep drift - very low for free sliding
// 						FKeyHandle rKey7 = rearLateralSlipCurve->AddKey(60.0f, 120.0f);
// 						rearLateralSlipCurve->SetKeyInterpMode(rKey7, RCIM_Linear);

// 						// Extreme angle - minimal resistance
// 						FKeyHandle rKey8 = rearLateralSlipCurve->AddKey(90.0f, 80.0f);
// 						rearLateralSlipCurve->SetKeyInterpMode(rKey8, RCIM_Linear);
// 					}
// 				}
// 			}
// 		}
// 	}

// 	UE_LOG(LogSloopedUpExplorer, Log, TEXT("========== END WHEEL DEFAULT VALUES =========="));

// 	bWheelCDOTuned = true;
// }