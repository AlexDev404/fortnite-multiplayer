#pragma once

#include "gui.h"
#include "ufunctionhooks.h"

// #define LOGGING

std::vector<std::string> AthenaConsumables = {
    "Athena_Shields",
    "Athena_ShieldSmall",
    "Athena_PurpleStuff",
    "Athena_SuperMedkit",
    "Athena_Grenade",
    "Athena_KnockGrenade",
    "Athena_SmokeGrenade",
    "Athena_StickyGrenade",
    "Athena_Medkit",
    //"/Game/Athena/Items/Consumables/DanceGrenade/Athena_DanceGrenade.Athena_DanceGrenade",
    "Athena_SmokeGrenade"
};

std::vector<std::string> AthenaLootPool = {
    "WID_Assault_AutoHigh_Athena_SR_Ore_T03",
    "WID_Assault_AutoHigh_Athena_VR_Ore_T03",
    "WID_Assault_Auto_Athena_R_Ore_T03",
    "WID_Assault_Auto_Athena_UC_Ore_T03",
    "WID_Assault_Auto_Athena_C_Ore_T02",
    "WID_Shotgun_Standard_Athena_C_Ore_T03",
    "WID_Shotgun_Standard_Athena_C_Ore_T03",
    "WID_Shotgun_Standard_Athena_UC_Ore_T03",
    "WID_Shotgun_SemiAuto_Athena_UC_Ore_T03",
    "WID_Shotgun_SemiAuto_Athena_R_Ore_T03",
    "WID_Shotgun_SemiAuto_Athena_VR_Ore_T03",
    "WID_Shotgun_SlugFire_Athena_VR",
    "WID_Shotgun_SlugFire_Athena_SR",
    "WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03",
    "WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03",
    "WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03",
    "WID_Sniper_Standard_Scope_Athena_VR_Ore_T03",
    "WID_Sniper_Standard_Scope_Athena_SR_Ore_T03",
    "WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03",
    "WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03",
    "WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03",
    "WID_Pistol_Scavenger_Athena_UC_Ore_T03",
    "WID_Pistol_Scavenger_Athena_R_Ore_T03",
    "WID_Pistol_Scavenger_Athena_VR_Ore_T03",
    "WID_Assault_LMG_Athena_VR_Ore_T03",
    "WID_Assault_LMG_Athena_SR_Ore_T03",
    "WID_Pistol_SemiAuto_Athena_C_Ore_T02",
    "WID_Pistol_SemiAuto_Athena_UC_Ore_T03",
    "WID_Pistol_SemiAuto_Athena_R_Ore_T03",
    "WID_Pistol_SemiAuto_Athena_VR_Ore_T03",
    "WID_Pistol_SemiAuto_Athena_SR_Ore_T03",
    "WID_Pistol_SixShooter_Athena_C_Ore_T02",
    "WID_Pistol_SixShooter_Athena_UC_Ore_T03",
    "WID_Pistol_SixShooter_Athena_R_Ore_T03",
    "WID_Pistol_HandCannon_Athena_VR_Ore_T03",
    "WID_Pistol_HandCannon_Athena_SR_Ore_T03",
    "WID_Launcher_Rocket_Athena_R_Ore_T03",
    "WID_Launcher_Rocket_Athena_VR_Ore_T03",
    "WID_Launcher_Rocket_Athena_SR_Ore_T03"
};

std::vector<std::string> TrapsPool = {
    "TID_Floor_Player_Launch_Pad_Athena",
    "TID_Floor_Spikes_Athena_R_T03"
};

std::vector<std::string> ResourcePool = {
    "/Game/Items/ResourcePickups/WoodItemData.WoodItemData",
    "/Game/Items/ResourcePickups/StoneItemData.StoneItemData",
    "/Game/Items/ResourcePickups/MetalItemData.MetalItemData"
};

std::vector<std::string> AmmoPool = {
    "/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets",
    "/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells",
    "/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium",
    "/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight",
    "/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy"
};

enum class CustomMode
{
    NONE,
	JUGGERNAUT, // Gives the players 500 health and makes you slower.
    LATEGAME, // TODO: You know what late game is.
	LIFESTEAL, // TODO: You know what life steal is, but this might be a stupid idea.
    SPACE // Sets gravity like the moon // BUG: Unfortunately, the gravityscale variable doesn't update for the client, making them rubberband and making it look weird.
};

constexpr CustomMode Mode = CustomMode::NONE;

namespace Hooks
{
    bool LocalPlayerSpawnPlayActor(ULocalPlayer* Player, const FString& URL, FString& OutError, UWorld* World) // prevent server's pc from spawning
    {
        if (bTraveled)
            return true;
        else
            return Native::LocalPlayer::SpawnPlayActor(Player, URL, OutError, World);
    }

    uint64 GetNetMode(UWorld* World) // PlayerController::SendClientAdjustment checks if the netmode is not client
    {
        return 2; // ENetMode::NM_ListenServer;
    }

    void TickFlush(UNetDriver* NetDriver, float DeltaSeconds)
    {
        if (!NetDriver)
            return;

        if (NetDriver->IsA(UIpNetDriver::StaticClass()) && NetDriver->ClientConnections.Num() > 0 && NetDriver->ClientConnections[0]->InternalAck == false)
        {
            Replication::ServerReplicateActors(NetDriver);
        }

        Native::NetDriver::TickFlush(NetDriver, DeltaSeconds);
    }

    void WelcomePlayer(UWorld* World, UNetConnection* IncomingConnection)
    {
        Native::World::WelcomePlayer(GetWorld(), IncomingConnection);
    }

    char KickPlayer(__int64 a1, __int64 a2, __int64 a3)
    {
        return 0;
    }

    void World_NotifyControlMessage(UWorld* World, UNetConnection* Connection, uint8 MessageType, void* Bunch)
    {
        Native::World::NotifyControlMessage(GetWorld(), Connection, MessageType, Bunch);
    }

    APlayerController* SpawnPlayActor(UWorld* World, UPlayer* NewPlayer, ENetRole RemoteRole, FURL& URL, void* UniqueId, SDK::FString& Error, uint8 NetPlayerIndex)
    {
        auto PlayerController = (AFortPlayerControllerAthena*)Native::World::SpawnPlayActor(GetWorld(), NewPlayer, RemoteRole, URL, UniqueId, Error, NetPlayerIndex);
        NewPlayer->PlayerController = PlayerController;

        auto PlayerState = (AFortPlayerStateAthena*)PlayerController->PlayerState;

        InitInventory(PlayerController);

        auto Pawn = (APlayerPawn_Athena_C*)SpawnActorTrans(APlayerPawn_Athena_C::StaticClass(), GetPlayerStart(PlayerController), PlayerController);

        PlayerController->Pawn = Pawn;
        PlayerController->AcknowledgedPawn = Pawn;
        Pawn->Owner = PlayerController;
        Pawn->OnRep_Owner();
        PlayerController->OnRep_Pawn();
        PlayerController->Possess(Pawn);

        constexpr static auto Health = (Mode == CustomMode::JUGGERNAUT) ? 500 : 100;
        const static auto Shield = 100;

        Pawn->SetMaxHealth(Health);
        Pawn->SetMaxShield(Shield);
		
		Pawn->NetUpdateFrequency *= 2; // Original is 100.0f;
        auto CM = Pawn->CharacterMovement;

        if (CM)
        {
            switch (Mode)
            {
            case CustomMode::SPACE:
                CM->GravityScale -= 0.5f; // Default is 1.0f
                break;
            case CustomMode::JUGGERNAUT:
                // CM->Mass += 200; // Default is 100.0f // this does nothing for some reason
				// CM->MaxAcceleration -= 500.0f; // Default is 1000f // this does nothing for some reason
                break;
            }
        }
        else
            std::cout << "Could not find CharacterMovementComponent!\n";

        PlayerController->bHasClientFinishedLoading = true; // should we do this on ServerSetClientHasFinishedLoading 
        PlayerController->bHasServerFinishedLoading = true;
        PlayerController->bHasInitiallySpawned = true;
        PlayerController->OnRep_bHasServerFinishedLoading();

        PlayerState->bHasFinishedLoading = true;
        PlayerState->bHasStartedPlaying = true;
        PlayerState->OnRep_bHasStartedPlaying();

        static auto FortRegisteredPlayerInfo = ((UFortGameInstance*)GetWorld()->OwningGameInstance)->RegisteredPlayers[0]; // UObject::FindObject<UFortRegisteredPlayerInfo>("FortRegisteredPlayerInfo Transient.FortEngine_0_1.FortGameInstance_0_1.FortRegisteredPlayerInfo_0_1");

        if (FortRegisteredPlayerInfo)
        {
            auto Hero = FortRegisteredPlayerInfo->AthenaMenuHeroDef;

            if (Hero)
            {
                PlayerState->HeroType = Hero->GetHeroTypeBP();
                PlayerState->OnRep_HeroType();

                for (auto i = 0; i < Hero->CharacterParts.Num(); i++)
                {
                    auto Part = Hero->CharacterParts[i];

                    if (!Part)
                        continue;

                    PlayerState->CharacterParts[i] = Part;
                }

                PlayerState->CharacterBodyType = Hero->CharacterParts[1]->BodyTypesPermitted;
                Pawn->CharacterBodyType = Hero->CharacterParts[1]->BodyTypesPermitted;
                Pawn->CharacterGender = Hero->CharacterParts[1]->GenderPermitted;
                PlayerState->OnRep_CharacterBodyType();
                PlayerState->OnRep_CharacterParts();
            }
        }

        static std::vector<UFortWeaponRangedItemDefinition*> doublePumpLoadout = {
            FindWID("WID_Harvest_Pickaxe_PajamaParty"), // Candy Axe
            FindWID("WID_Shotgun_SemiAuto_Athena_VR_Ore_T03"), // Blue Pump
            FindWID("WID_Pistol_AutoHeavy_Athena_R_Ore_T03"), // Blue Pump
            FindWID("WID_Assault_SemiAuto_Athena_UC_Ore_T03"), // Gold AR
            FindWID("WID_Pistol_HandCannon_Athena_SR_Ore_T03"), // Blue Bolt Action
            // "Athena_KnockGrenade" // Impulse Grenades
            FindWID("Athena_Shields") // Big Shield Potion
        };

        EquipLoadout(PlayerController, doublePumpLoadout);

        auto CheatManager = CreateCheatManager(PlayerController);
        CheatManager->ToggleInfiniteAmmo();
        CheatManager->ToggleInfiniteDurability();

        if (PlayerController->Pawn)
        {
            if (PlayerController->Pawn->PlayerState)
            {
                // WILL BE ADDED LATER <3
                //PlayerState->TeamIndex = EFortTeam(2); // GetMath()->STATIC_RandomIntegerInRange(2, 102));
                PlayerState->OnRep_PlayerTeam();
                //PlayerState->SquadId = PlayerState->PlayerTeam->TeamMembers.Num() + 1;
                PlayerState->OnRep_SquadId();
            }
        }

        PlayerController->OverriddenBackpackSize = 100; // i hate stw

		// TODO: Remove healing GameplayEffects

        // Pawn->K2_TeleportTo({ 37713, -52942, 461 }, { 0, 0, 0 }); // Tilted

        return PlayerController;
    }

    void Beacon_NotifyControlMessage(AOnlineBeaconHost* Beacon, UNetConnection* Connection, uint8 MessageType, int64* Bunch)
    {
        printf("Recieved control message %i\n", MessageType);

        switch (MessageType)
        {
        case 4: // NMT_Netspeed
            Connection->CurrentNetSpeed = 30000;
            return;
        case 5: // NMT_Login
        {
            Bunch[7] += (16 * 1024 * 1024);

            auto OnlinePlatformName = FString(L"");

            Native::NetConnection::ReceiveFString(Bunch, Connection->ClientResponse);
            Native::NetConnection::ReceiveFString(Bunch, Connection->RequestURL);
            Native::NetConnection::ReceiveUniqueIdRepl(Bunch, Connection->PlayerID);
            Native::NetConnection::ReceiveFString(Bunch, OnlinePlatformName);

            Bunch[7] -= (16 * 1024 * 1024);

            Native::World::WelcomePlayer(GetWorld(), Connection);
            return;
        }
        case 15: // NMT_PCSwap
            // return;
            break;
        }

        Native::World::NotifyControlMessage(GetWorld(), Connection, MessageType, Bunch);
    }

    uint8 Beacon_NotifyAcceptingConnection(AOnlineBeacon* Beacon)
    {
        return Native::World::NotifyAcceptingConnection(GetWorld());
    }

    void* SeamlessTravelHandlerForWorld(UEngine* Engine, UWorld* World)
    {
        return Native::Engine::SeamlessTravelHandlerForWorld(Engine, GetWorld());
    }

    void* NetDebug(UObject* _this)
    {
        return nullptr;
    }

    void PostRender(UGameViewportClient* _this, UCanvas* Canvas)
    {
        ZeroGUI::SetupCanvas(Canvas);
        GUI::Tick();

        return Native::GameViewportClient::PostRender(_this, Canvas);
    }

    __int64 CollectGarbage(__int64 a1)
    {
        return 0;
    };

    void InitNetworkHooks()
    {
        DETOUR_START
        DetourAttachE(Native::World::WelcomePlayer, WelcomePlayer);
        DetourAttachE(Native::Actor::GetNetMode, GetNetMode);
        DetourAttachE(Native::World::NotifyControlMessage, World_NotifyControlMessage);
        DetourAttachE(Native::World::SpawnPlayActor, SpawnPlayActor);
        DetourAttachE(Native::OnlineBeaconHost::NotifyControlMessage, Beacon_NotifyControlMessage);
        DetourAttachE(Native::OnlineSession::KickPlayer, KickPlayer);
        DetourAttachE(Native::GameViewportClient::PostRender, PostRender);
        DetourAttachE(Native::GC::CollectGarbage, CollectGarbage);
        DETOUR_END
    }

    static UFortItemDefinition* GetRandomItemDefinition()
    {
        while (true)
        {
            auto Idx = ((UKismetMathLibrary*)UKismetMathLibrary::StaticClass())->STATIC_RandomInteger(AthenaLootPool.size());
            auto Item = AthenaLootPool[Idx];
            auto Def = UObject::FindObject<UFortItemDefinition>(Item);
            if (!Def)
                continue;

            auto rng = std::default_random_engine {};
            std::shuffle(AthenaLootPool.begin(), AthenaLootPool.end(), rng);
            return Def;
        }
    }

    static UFortItemDefinition* GetRandomResourceItemDefinition()
    {
        while (true)
        {
            auto Idx = ((UKismetMathLibrary*)UKismetMathLibrary::StaticClass())->STATIC_RandomInteger(ResourcePool.size());
            auto Item = ResourcePool[Idx];
            auto Def = UObject::FindObject<UFortItemDefinition>(Item);
            if (!Def)
                continue;

            auto rng = std::default_random_engine {};
            std::shuffle(ResourcePool.begin(), ResourcePool.end(), rng);
            return Def;
        }
    }

    static UFortAmmoItemDefinition* GetRandomAmmoItemDefinition()
    {
        while (true)
        {
            auto Idx = ((UKismetMathLibrary*)UKismetMathLibrary::StaticClass())->STATIC_RandomInteger(AmmoPool.size());
            auto Item = AmmoPool[Idx];
            auto Def = UObject::FindObject<UFortAmmoItemDefinition>(Item);
            if (!Def)
                continue;

            auto rng = std::default_random_engine {};
            std::shuffle(AmmoPool.begin(), AmmoPool.end(), rng);
            return Def;
        }
    }

    static UFortItemDefinition* GetRandomConsumableItemDefinition()
    {
        while (true)
        {
            auto Idx = ((UKismetMathLibrary*)UKismetMathLibrary::StaticClass())->STATIC_RandomInteger(AthenaConsumables.size());
            auto Item = AthenaConsumables[Idx];
            auto Def = FindWID(Item);
            if (!Def)
                continue;

            auto rng = std::default_random_engine {};
            std::shuffle(AthenaConsumables.begin(), AthenaConsumables.end(), rng);
            return Def;
        }
    }

    static AFortPickupAthena* SpawnPickup(FVector Location, UFortItemDefinition* ItemDef = GetRandomItemDefinition(), int Count = 1)
    {
        if (Count == 1 && ItemDef->IsA(UFortAmmoItemDefinition::StaticClass()))
            Count = ((UFortAmmoItemDefinition*)(ItemDef))->DropCount;

        FTransform Transform;
        Transform.Translation = Location;
        Transform.Rotation = {};
        Transform.Scale3D = { 1, 1, 1 };

        auto Pickup = (AFortPickupAthena*)SpawnActorTrans(AFortPickupAthena::StaticClass(), Transform, {});

        Pickup->PrimaryPickupItemEntry.ItemDefinition = ItemDef;
        Pickup->PrimaryPickupItemEntry.Count = Count;

        Pickup->TossPickup(Location, nullptr, 6, true);

        return Pickup;
    }

    void ProcessEventHook(UObject* Object, UFunction* Function, void* Parameters)
    {
        if (!bPlayButton)
        {
            static auto PlayButtonFn = UObject::FindObject<UFunction>("BndEvt__BP_PlayButton_K2Node_ComponentBoundEvent_1_CommonButtonClicked__DelegateSignature");

            // if (FunctionName.find("BP_PlayButton") != -1)
            if (Function == PlayButtonFn)
            {
                bPlayButton = true;
                Game::Start();
                printf("[Game::Start] Done\n");

                InitNetworkHooks();
                printf("[InitNetworkHooks] Done\n");
            }
        }

        if (bTraveled)
        {
#ifdef LOGGING
            auto FunctionName = Function->GetName();
            if (Function->FunctionFlags & 0x00200000 || (Function->FunctionFlags & 0x01000000 && FunctionName.find("Ack") == -1 && FunctionName.find("AdjustPos") == -1))
            {
                if (FunctionName.find("ServerUpdateCamera") == -1 && FunctionName.find("ServerMove") == -1)
                {
                    std::cout << "RPC Called: " << FunctionName << '\n';
                }
            }
#endif

            for (int i = 0; i < UFunctionHooks::toHook.size(); i++)
            {
                if (Function == UFunctionHooks::toHook[i])
                {
                    if (UFunctionHooks::toCall[i](Object, Parameters))
                    {
                        return;
                    }
                    break;
                }
            }
        }

        //if (Function->GetName() == "ServerAttemptInteract")
        //{
        //    auto PlayerController = (AFortPlayerControllerAthena*)Object;
        //    auto CurrentParams = (AFortPlayerController_ServerAttemptInteract_Params*)Parameters;

        //    auto ReceivingActor = CurrentParams->ReceivingActor;

        //    if (ReceivingActor && ReceivingActor->Class->GetName().find("Tiered_Short_Ammo") != std::string::npos)
        //    {
        //        auto Ammo = (ABuildingContainer*)ReceivingActor;
        //        Ammo->bAlreadySearched = true;
        //        Ammo->OnRep_bAlreadySearched();

        //        auto Location = ReceivingActor->K2_GetActorLocation();

        //        for (int i = 0; i < 4; i++)
        //        {
        //            SpawnPickup(Location, GetRandomAmmoItemDefinition());
        //        }
        //    }

        //    else if (ReceivingActor && ReceivingActor->Class->GetName().find("Tiered_Chest") != std::string::npos)
        //    {
        //        auto Chest = (ABuildingContainer*)ReceivingActor;
        //        Chest->bAlreadySearched = true;
        //        Chest->OnRep_bAlreadySearched();

        //        auto Location = ReceivingActor->K2_GetActorLocation();

        //        auto WeaponDef = GetRandomItemDefinition();
        //        auto Pickup = SpawnPickup(Location, WeaponDef);

        //        SpawnPickup(Location, GetRandomConsumableItemDefinition());
        //        SpawnPickup(Location, ((UFortWeaponItemDefinition*)WeaponDef)->GetAmmoWorldItemDefinition_BP());
        //        SpawnPickup(Location, GetRandomResourceItemDefinition(), 30);

        //        return;
        //    }

        //    else if (ReceivingActor && ReceivingActor->GetName().find("AthenaSupplyDrop_C"))
        //    {
        //        auto Location = ReceivingActor->K2_GetActorLocation();
        //        bool Is2 = ((UKismetMathLibrary*)UKismetMathLibrary::StaticClass())->STATIC_RandomBool();
        //        if (Is2 == true)
        //        {
        //            SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03"));
        //        }
        //        else
        //        {
        //            SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"));
        //        }
        //        SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells"), 25);
        //        SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets"), 5);
        //        SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy"), 15);
        //        SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium"), 100);
        //        SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight"), 150);
        //        SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData"), 100);
        //        SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData"), 100);
        //        SpawnPickup(Location, UObject::FindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData"), 100);
        //    }

        //    /*else if (ReceivingActor)
        //    {
        //        LOG("Interacted with Unknown Object: " + ReceivingActor->Class->GetName());
        //    }*/
        //}

        return ProcessEvent(Object, Function, Parameters);
    }
}
