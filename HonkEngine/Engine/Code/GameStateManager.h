#pragma once
#include <iostream>
#include <functional>
#include <map>
#include "Application.h"
#include "GameObjects/BellManager.h"
#include "GameObjects/OrderData.h"
#include "Dialogue/DialogueManager.h"
#include "Audio/AudioManager.h"
#include "GameObjects/DoorManager.h"
#include "GameObjects/Timer.h"
#include "Scene/KitchenData.h"

#define PREPARE_DURATION 210                // in seconds
#define INITIAL_ROOM GameState::ROOM1_STATE // start room

enum GameState
{
    ROOM1_STATE,
    ROOM2_STATE,
    ROOM3_STATE,
    ROOM4_STATE,
    END_STATE
};

enum RoomState
{
    Order,
    Prepare,
    Serve,
    Score,
    MealReact,
    InspectionStart,
    Inspection,
    InspectionEnd,
    End,
};

inline std::string gameStateNameToDoorName(GameState state)
{
    switch (state)
    {
    case GameState::ROOM1_STATE:
        return "Room1Door";
    case GameState::ROOM2_STATE:
        return "Room2Door";
    case GameState::ROOM3_STATE:
        return "Room3Door";
    case GameState::ROOM4_STATE:
        return "Room4Door";
    default:
        return ""; // Default empty if no match found
    }
}

class GameStateManager
{
private:
    GameState currentGameState = INITIAL_ROOM;
    RoomState currentRoomState = RoomState::Order;
    bool transitioning = false;
    bool restoredFromSave = false;

    std::map<std::pair<GameState, RoomState>, std::function<void()>> stateActions;
    unique_ptr<DialogueManager> dialogueManager;

    GameStateManager()
    {
        // ROOM1
        stateActions[std::make_pair(GameState::ROOM1_STATE, RoomState::Prepare)] = []()
        {
            DoorManager::GetInstance().GetDoorByName("Room1Door")->setPermission(false);
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(true);
            Timer::GetInstance().start(PREPARE_DURATION);
            Application::Get().SetScene("Hallway");
            AudioManager::GetInstance().PlaySound("slideDoor");
            std::cout << "Transition to Room1 Prepare state." << std::endl;
        };

        stateActions[std::make_pair(GameState::ROOM1_STATE, RoomState::Serve)] = []()
        {
            Timer::GetInstance().stop();
            OrderData::GetInstance().ClearOrder();
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(false);
            std::cout << "Transition to Room1 Serve state." << std::endl;
        };

        stateActions[std::make_pair(GameState::ROOM1_STATE, RoomState::End)] = []()
        {
            DoorManager::GetInstance().GetDoorByName("Room1Door")->setPermission(false);
            AudioManager::GetInstance().PlaySound("slideDoor");
            KitchenData::GetInstance()->clearPlate();
            Application::Get().SetTimer(
                3000,
                []()
                {
                    GameStateManager::GetInstance().SetGameState(GameState::ROOM3_STATE);
                    GameStateManager::GetInstance().SetRoomState(RoomState::Order);
                    Application::Get().SetScene("Hallway");
                },
                false);
            std::cout << "Transition to Room1 End state." << std::endl;
        };

        // ROOM2
        stateActions[std::make_pair(GameState::ROOM2_STATE, RoomState::Prepare)] = []()
        {
            DoorManager::GetInstance().GetDoorByName("Room2Door")->setPermission(false);
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(true);
            Timer::GetInstance().start(PREPARE_DURATION);
            Application::Get().SetScene("Hallway");
            AudioManager::GetInstance().PlaySound("slideDoor");
            std::cout << "Transition to Room2 Prepare state." << std::endl;
        };

        stateActions[std::make_pair(GameState::ROOM2_STATE, RoomState::Serve)] = []()
        {
            Timer::GetInstance().stop();
            OrderData::GetInstance().ClearOrder();
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(false);
            std::cout << "Transition to Room2 Serve state." << std::endl;
        };

        stateActions[std::make_pair(GameState::ROOM2_STATE, RoomState::End)] = []()
        {
            DoorManager::GetInstance().GetDoorByName("Room2Door")->setPermission(false);
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(false);
            AudioManager::GetInstance().PlaySound("slideDoor");
            KitchenData::GetInstance()->clearPlate();
            Application::Get().SetTimer(
                3000,
                []()
                {
                    GameStateManager::GetInstance().SetGameState(GameState::ROOM4_STATE);
                    GameStateManager::GetInstance().SetRoomState(RoomState::Order);
                    Application::Get().SetScene("Hallway");
                },
                false);
        };

        // ROOM3
        stateActions[std::make_pair(GameState::ROOM3_STATE, RoomState::Prepare)] = []()
        {
            DoorManager::GetInstance().GetDoorByName("Room3Door")->setPermission(false);
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(true);
            Timer::GetInstance().start(PREPARE_DURATION);
            Application::Get().SetScene("Hallway");
            AudioManager::GetInstance().PlaySound("slideDoor");
            std::cout << "Transition to Room3 Prepare state." << std::endl;
        };

        stateActions[std::make_pair(GameState::ROOM3_STATE, RoomState::Serve)] = []()
        {
            Timer::GetInstance().stop();
            OrderData::GetInstance().ClearOrder();
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(false);
            std::cout << "Transition to Room3 Serve state." << std::endl;
        };

        stateActions[std::make_pair(GameState::ROOM3_STATE, RoomState::End)] = []()
        {
            DoorManager::GetInstance().GetDoorByName("Room3Door")->setPermission(false);
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(false);
            AudioManager::GetInstance().PlaySound("slideDoor");
            KitchenData::GetInstance()->clearPlate();
            Application::Get().SetTimer(
                3000,
                []()
                {
                    GameStateManager::GetInstance().SetGameState(GameState::ROOM2_STATE);
                    GameStateManager::GetInstance().SetRoomState(RoomState::Order);
                    Application::Get().SetScene("Hallway");
                },
                false);
        };

        // ROOM4
        stateActions[std::make_pair(GameState::ROOM4_STATE, RoomState::Prepare)] = []()
        {
            DoorManager::GetInstance().GetDoorByName("Room4Door")->setPermission(false);
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(true);
            Timer::GetInstance().start(PREPARE_DURATION);
            Application::Get().SetScene("Hallway");
            AudioManager::GetInstance().PlaySound("slideDoor");
            std::cout << "Transition to Room4 Prepare state." << std::endl;
        };

        stateActions[std::make_pair(GameState::ROOM4_STATE, RoomState::Serve)] = []()
        {
            Timer::GetInstance().stop();
            OrderData::GetInstance().ClearOrder();
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(false);
            std::cout << "Transition to Room4 Serve state." << std::endl;
        };

        stateActions[std::make_pair(GameState::ROOM4_STATE, RoomState::End)] = []()
        {
            DoorManager::GetInstance().GetDoorByName("Room4Door")->setPermission(false);
            DoorManager::GetInstance().GetDoorByName("KitchenDoor")->setPermission(false);
            AudioManager::GetInstance().PlaySound("slideDoor");
            KitchenData::GetInstance()->clearPlate();
            Application::Get().SetTimer(
                3000,
                []()
                {
                    GameStateManager::GetInstance().SetGameState(GameState::END_STATE);
                    GameStateManager::GetInstance().SetRoomState(RoomState::End);
                    Application::Get().SetScene("Hallway");
                },
                false);
        };

        // END
        stateActions[std::make_pair(GameState::END_STATE, RoomState::End)] = []()
        {
            // End cutscene
        };
    }

public:
    static GameStateManager &GetInstance()
    {
        static GameStateManager instance;
        return instance;
    }

    static GameState GetNextGameState(GameState state)
    {
        switch (state)
        {
        case GameState::ROOM1_STATE:
            return GameState::ROOM3_STATE;
        case GameState::ROOM3_STATE:
            return GameState::ROOM2_STATE;
        case GameState::ROOM2_STATE:
            return GameState::ROOM4_STATE;
        case GameState::ROOM4_STATE:
        default:
            return GameState::END_STATE;
        }
    }

    void Update()
    {
        HandleTimerExpiration();
    }

    void HandleTimerExpiration()
    {
        if (currentRoomState == RoomState::Prepare && Timer::GetInstance().isTimesUp() &&
            !BellManager::GetInstance().isAnyBellRinging())
        {
            switch (currentGameState)
            {
            case GameState::ROOM1_STATE:
                BellManager::GetInstance().getBell("bellCabin1")->startRinging();
                break;
            case GameState::ROOM2_STATE:
                BellManager::GetInstance().getBell("bellCabin2")->startRinging();
                break;
            case GameState::ROOM3_STATE:
                BellManager::GetInstance().getBell("bellCabin3")->startRinging();
                break;
            case GameState::ROOM4_STATE:
                BellManager::GetInstance().getBell("bellCabin4")->startRinging();
                break;
            default:
                break;
            }
        }
    }

    ~GameStateManager() = default;

    void SetGameState(GameState state)
    {
        currentGameState = state;
        // ExecuteStateAction();
    }

    void RestoreFromSave(GameState gameState, RoomState roomState)
    {
        if (roomState == RoomState::End && gameState != GameState::END_STATE)
        {
            currentGameState = GetNextGameState(gameState);
            currentRoomState = currentGameState == GameState::END_STATE ? RoomState::End : RoomState::Order;
        }
        else if (gameState != GameState::END_STATE)
        {
            currentGameState = gameState;
            currentRoomState = RoomState::Order;
        }
        else
        {
            currentGameState = gameState;
            currentRoomState = roomState;
        }

        Application::Get().ClearAllTimers();
        Timer::GetInstance().stop();
        BellManager::GetInstance().StopAllRinging();
        OrderData::GetInstance().ClearOrder();
        KitchenData::GetInstance()->clearPlate();
        restoredFromSave = true;
    }

    void Reset()
    {
        dialogueManager.reset();
        currentGameState = INITIAL_ROOM;
        currentRoomState = RoomState::Order;
        restoredFromSave = false;
        std::cout << "Resetting game state to initial state." << std::endl;
    }

    void SetRoomState(RoomState state)
    {
        currentRoomState = state;
        ExecuteStateAction();
    }

    GameState getGameState() const
    {
        return currentGameState;
    }

    RoomState getRoomState() const
    {
        return currentRoomState;
    }

    bool WasRestoredFromSave() const
    {
        return restoredFromSave;
    }

    void ExecuteStateAction()
    {
        auto key = std::make_pair(currentGameState, currentRoomState);
        if (stateActions.find(key) != stateActions.end())
        {
            stateActions[key]();
        }
    }
};
