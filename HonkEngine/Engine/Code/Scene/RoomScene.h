#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include "Scene.h"
#include "../Engine.h"
#include "../Input/Input.h"
#include "../GameStateManager.h"
#include "../GameObjects/Timer.h"
#include "../GameObjects/CharacterData.h"
#include "../GameObjects/RenderGameObject.h"
#include "../GameObjects/DoorManager.h"
#include "../Dialogue/Dialoguemanager.h"
#include "../UI/UINormal.h"
#include "../UI/UIButton.h"
#include "../UI/UIElement.h"
#include "../PopupWidget/PauseMenu.h"
#include "../Effects/ObjectsParallax.h"
#include "../Effects/BackgroundParallax.h"
#include "../Effects/TransitionEffects.h"

using namespace std;

/// @brief Abstract base class for all passenger room scenes.
///
/// Implements the Template Method pattern. The shared gameplay flow
/// (Order -> Serve -> MealReact -> Score -> Inspection -> End) lives here.
/// Each derived room only implements what is unique to that passenger.
///
/// Invariants:
///   - dialogueManager must be initialized before OnEnter() is called
///   - roomDoor must be valid (non-null) before OnEnter() is called
///   - instructionText must be valid before Update() is called
class RoomScene : public Scene
{

public:
    /// @brief Construct a RoomScene.
    /// @param roomDoorName  Name of the door leading to this room.
    /// @pre roomDoorName must not be empty.
    explicit RoomScene(const string &roomDoorName)
        : audioManager(AudioManager::GetInstance()), gameStateManager(GameStateManager::GetInstance()),
          input(Application::GetInput()), camera(Application::GetCamera())
    {
        assert(!roomDoorName.empty() && "RoomScene: roomDoorName must not be empty");

        roomDoor = DoorManager::GetInstance().GetDoorByName(roomDoorName);
        kitchenDoor = DoorManager::GetInstance().GetDoorByName("KitchenDoor");

        assert(roomDoor != nullptr && "RoomScene: roomDoor not found");
        assert(kitchenDoor != nullptr && "RoomScene: kitchenDoor not found");

        SetupBackgrounds();
        SetupTransition();
        SetupInstructionText();
    }

    // -----------------------------------------------------------------
    // Template Method hooks — final so derived classes cannot break flow
    // -----------------------------------------------------------------

    /// @brief Called when the scene is entered.
    /// @post dialogueManager is reset and correct dialogue set is active.
    void OnEnter() final
    {
        assert(dialogueManager != nullptr && "RoomScene::OnEnter — dialogueManager must be initialised");

        transitionEffects->FadeIn(1.0f, [this]() {});
        AudioManager::GetInstance().PlaySound("doorSlide");
        audioManager.PlaySound("cabinMusic", true);

        if (gameStateManager.getRoomState() == RoomState::Prepare && KitchenData::GetInstance()->checkCompletePlate())
        {
            GameStateManager::GetInstance().SetRoomState(RoomState::Serve);
        }

        SetSequencesDialogue();
    }

    /// @brief Main update loop. Drives the shared state machine.
    /// @param dt    Delta time in seconds. Must be >= 0.
    /// @param frame Current frame counter.
    /// @pre  dt >= 0
    void Update(float dt, long frame) final
    {
        assert(dt >= 0.0f && "RoomScene::Update — dt must be non-negative");

        Scene::Update(dt, frame);
        backgroundParallaxManager->Update(dt);
        objectsParallaxManager->UpdateLayers();
        dialogueManager->Update(dt, frame);
        transitionEffects->Update(dt);

        OnPreStateUpdate(dt);
        UpdateDialogueProgress();
        HandleInstructionVisibility(dt);
        HandleKeyInputs(dt);
    }

    /// @brief Render the scene and dialogue.
    void Render() final
    {
        Scene::Render();
        dialogueManager->Render();
    }

    /// @brief Called when leaving the scene.
    void OnExit() final
    {
        AudioManager::GetInstance().PlaySound("doorSlide");
        audioManager.PauseSound("cabinMusic");
        BellManager::GetInstance().StopAllRinging();
        OnExitExtras();
    }

protected:
    // -----------------------------------------------------------------
    // Pure virtual — each room MUST implement these
    // -----------------------------------------------------------------

    /// @brief Returns the Cabin enum value for this room.
    virtual Cabin GetCabin() const = 0;

    /// @brief Returns the GameState enum value for this room.
    virtual GameState GetRoomGameState() const = 0;

    /// @brief Set up room-specific visual objects (characters, props, etc.)
    /// @post  All room-specific GameObjects pushed to m_gameObjects.
    virtual void SetupRoomObjects() = 0;

    /// @brief Load and assign meal-reaction dialogue keys.
    /// @post  teaDialogueKey, sandwichDialogueKey, dessertDialogueKey are set.
    virtual void SetMealReactionDialogue() = 0;

    /// @brief Load and assign score dialogue key.
    /// @post  scoreDialogueKey is set and non-empty.
    virtual void SetScoredDialogue() = 0;

    /// @brief Load and assign inspection dialogue keys.
    /// @post  inspectStartDialogueKey and inspectEndDialogueKey are set.
    virtual void SetInteractDialogue() = 0;

    /// @brief Handle the inspection mini-loop for room-specific objects.
    virtual void ManageInspectionState() = 0;

    /// @brief Returns true when all inspectable objects have been examined.
    virtual bool AllObjectsInspected() const = 0;

    /// @brief Set up the DialogueManager with character sprites and icons.
    /// @post  dialogueManager is non-null after this call.
    virtual void SetupDialogueManager(
        UIButton *dialogueBox, UIButton *choice1, UIButton *choice2, UIButton *choice3) = 0;

    /// @brief Called each frame before UpdateDialogueProgress().
    /// Override to receive dt for room-specific per-frame effects.
    virtual void OnPreStateUpdate(float dt)
    {
    }

    /// @brief Optional extra cleanup per room (e.g. stop shaking effects).
    /// Called at the end of OnExit(). Default is no-op.
    virtual void OnExitExtras()
    {
    }

    // -----------------------------------------------------------------
    // Shared helpers — implemented once in base class
    // -----------------------------------------------------------------

    /// @brief Decide which dialogue set to activate based on RoomState.
    /// @pre   dialogueManager != nullptr
    /// @post  dialogueManager has an active dialogue set
    void SetSequencesDialogue()
    {
        assert(dialogueManager != nullptr && "SetSequencesDialogue: dialogueManager is null");

        switch (gameStateManager.getRoomState())
        {
        case RoomState::Order:
            dialogueManager->SetDialogueSet("Order");
            break;

        case RoomState::Serve:
            serveDialogueKey = (characterData->getServeTimeLevel(GetCabin()) == ONTIME) ? "Serve_OnTime" : "Serve_Late";

            dialogueManager->SetDialogueSet(serveDialogueKey);

            // Reset meal-react flags
            teaDialogueSet = false;
            sandwichDialogueSet = false;
            dessertDialogueSet = false;

            SetMealReactionDialogue();
            SetScoredDialogue();
            SetInteractDialogue();
            break;

        default:
            break;
        }
    }

    /// @brief Routes to the correct state handler each frame.
    void UpdateDialogueProgress()
    {
        if (gameStateManager.getGameState() != GetRoomGameState())
            return;

        SetInstruction(dialogueManager->IsCurrentDialogueQuestion() ? "Select a choice to continue"
                                                                    : "Press [Space] or  [Mouse] to continue");

        switch (gameStateManager.getRoomState())
        {
        case RoomState::Order:
            ManageOrderState();
            break;
        case RoomState::Serve:
            ManageServeState();
            break;
        case RoomState::MealReact:
            ManageMealReactions();
            break;
        case RoomState::Score:
            ManageScoreState();
            break;
        case RoomState::InspectionStart:
            ManageInspectionStartState();
            break;
        case RoomState::Inspection:
            ManageInspectionState();
            break;
        case RoomState::InspectionEnd:
            ManageInspectionEndState();
            break;
        default:
            break;
        }
    }

    /// @brief Handles the Order state.
    void ManageOrderState()
    {
        if (!dialogueManager->IsDialogueFinished("Order"))
            return;

        SetInstruction("Press [E] to leave");
        ShowInstruction();

        if (input.Get().GetKeyDown(GLFW_KEY_E) && !pauseMenu.IsVisible())
        {
            transitionEffects->FadeOut(1.0f, [this]() { gameStateManager.SetRoomState(Prepare); });
        }
    }

    /// @brief Handles the Serve state.
    void ManageServeState()
    {
        if (!serveDialogueSet)
        {
            PromptForNextDialogue(serveDialogueKey, serveDialogueSet);
        }
        else if (serveDialogueSet && dialogueManager->IsDialogueFinished(serveDialogueKey))
        {
            gameStateManager.SetRoomState(RoomState::MealReact);
        }
    }

    /// @brief Handles sequential meal-reaction dialogues (tea -> sandwich -> dessert).
    void ManageMealReactions()
    {
        if (!teaDialogueSet && dialogueManager->IsDialogueFinished(serveDialogueKey) && !dessertDialogueSet)
        {
            PromptForNextDialogue(teaDialogueKey, teaDialogueSet);
        }
        else if (teaDialogueSet && !sandwichDialogueSet && dialogueManager->IsDialogueFinished(teaDialogueKey) &&
                 !dessertDialogueSet)
        {
            PromptForNextDialogue(sandwichDialogueKey, sandwichDialogueSet);
        }
        else if (sandwichDialogueSet && !dessertDialogueSet && dialogueManager->IsDialogueFinished(sandwichDialogueKey))
        {
            PromptForNextDialogue(dessertDialogueKey, dessertDialogueSet);
        }

        if (teaDialogueSet && sandwichDialogueSet && dessertDialogueSet &&
            dialogueManager->IsDialogueFinished(dessertDialogueKey))
        {
            gameStateManager.SetRoomState(RoomState::Score);
        }
    }

    /// @brief Handles the Score state.
    void ManageScoreState()
    {
        if (!scoreDialogueSet)
        {
            PromptForNextDialogue(scoreDialogueKey, scoreDialogueSet);
        }
        else if (scoreDialogueSet && dialogueManager->IsDialogueFinished(scoreDialogueKey))
        {
            gameStateManager.SetRoomState(RoomState::InspectionStart);
        }
    }

    /// @brief Handles the InspectionStart state.
    void ManageInspectionStartState()
    {
        if (!inspectStartDialogueSet)
        {
            PromptForNextDialogue(inspectStartDialogueKey, inspectStartDialogueSet);
        }
        else if (inspectStartDialogueSet && dialogueManager->IsDialogueFinished(inspectStartDialogueKey))
        {
            gameStateManager.SetRoomState(RoomState::Inspection);
            OnInspectionStart();
            ShowInstruction();
        }
    }

    /// @brief Handles the InspectionEnd state.
    void ManageInspectionEndState()
    {
        if (!inspectEndDialogueSet)
        {
            objectsParallaxManager->DisableParallaxEffect();
            PromptForNextDialogue(inspectEndDialogueKey, inspectEndDialogueSet);
        }
        else if (inspectEndDialogueSet && dialogueManager->IsDialogueFinished(inspectEndDialogueKey))
        {
            gameStateManager.SetRoomState(RoomState::End);
            Application::Get().SetTimer(2000, [this]() { transitionEffects->FadeOut(1.0f, [this]() {}); }, false);
        }
    }

    /// @brief Called once when inspection begins. Override for room-specific setup
    ///        (e.g. starting a shaking effect). Default is no-op.
    virtual void OnInspectionStart()
    {
    }

    /// @brief Waits for player input then sets a dialogue key and marks flag true.
    /// @param nextKey  Key of the dialogue set to activate.
    /// @param flag     Boolean flag to set to true once activated.
    /// @pre  nextKey must not be empty.
    void PromptForNextDialogue(const string &nextKey, bool &flag)
    {
        assert(!nextKey.empty() && "PromptForNextDialogue: nextKey must not be empty");

        SetInstruction("Press [Space] or  [Mouse] to continue");
        if (!flag && (input.Get().GetKeyDown(GLFW_KEY_SPACE) || input.Get().GetMouseButtonDown(0)) &&
            !pauseMenu.IsVisible())
        {
            dialogueManager->SetDialogueSet(nextKey);
            flag = true;
        }
    }

    /// @brief Sets the instruction text content.
    /// @param message  Text to display. Must not be empty.
    void SetInstruction(const string &message)
    {
        assert(!message.empty() && "SetInstruction: message must not be empty");
        assert(instructionText != nullptr && "SetInstruction: instructionText is null");
        instructionText->SetContent(message);
    }

    /// @brief Shows the instruction text after the timer expires.
    void HandleInstructionVisibility(float dt)
    {
        assert(dt >= 0.0f && "HandleInstructionVisibility: dt must be non-negative");

        if (instructionVisible)
            return;

        instructionTimer -= dt;
        if (instructionTimer <= 0.0f)
        {
            ShowInstruction();
            instructionTimer = kInstructionResetTime;
        }
    }

    /// @brief Handles input — Space/Mouse advance dialogue,
    ///        hold Left Ctrl to fast forward (stops at choices),
    ///        Escape to show pause menu.
    /// @param dt Delta time in seconds.
    /// @pre   dt >= 0
    void HandleKeyInputs(float dt)
    {
        assert(dt >= 0.0f && "HandleKeyInputs: dt must be non-negative");

        // Normal single advance — Space, Left click, or choice clicked
        if ((input.Get().GetKeyDown(GLFW_KEY_SPACE) || input.Get().GetMouseButtonDown(0) ||
                dialogueManager->isChoicesClicked()) &&
            !pauseMenu.IsVisible())
        {
            dialogueManager->PlayNextDialogue();
            HideInstruction();
            dialogueManager->setChoicesClicked(false);
            fastForwardTimer = 0.0f;
        }

        // Fast forward — hold Left Ctrl, stops at choices
        if (ShouldFastForward(input.Get().GetKey(GLFW_KEY_LEFT_CONTROL), pauseMenu.IsVisible(),
                dialogueManager->IsCurrentDialogueQuestion()))
        {
            fastForwardTimer -= dt;
            if (fastForwardTimer <= 0.0f)
            {
                dialogueManager->PlayNextDialogue();
                HideInstruction();
                fastForwardTimer = kFastForwardInterval;
            }
        }
        else
        {
            // Reset timer when not fast forwarding
            fastForwardTimer = 0.0f;
        }

        if (input.Get().GetKeyDown(GLFW_KEY_ESCAPE))
        {
            pauseMenu.Show();
        }
    }

    void ShowInstruction()
    {
        assert(instructionText != nullptr && "ShowInstruction: instructionText is null");
        instructionText->setActiveStatus(true);
        instructionVisible = true;
    }

    void HideInstruction()
    {
        assert(instructionText != nullptr && "HideInstruction: instructionText is null");
        instructionText->setActiveStatus(false);
        instructionVisible = false;
        instructionTimer = kInstructionResetTime;
    }

    // -----------------------------------------------------------------
    // Shared setup helpers called from constructor
    // -----------------------------------------------------------------

    /// @brief Creates the 6-layer scrolling background (identical across rooms).
    void SetupBackgrounds()
    {
        // Layer pairs — same assets in every room
        const char *bgPaths[] = {
            "Assets/Images/BG/Cabin_Background_01.png",
            "Assets/Images/BG/Cabin_Background_02.png",
            "Assets/Images/BG/Cabin_Background_03.png",
            "Assets/Images/BG/Cabin_Background_04.png",
            "Assets/Images/BG/Cabin_Background_05.png",
            "Assets/Images/BG/Cabin_Background_06.png",
        };

        const glm::vec3 scaleVec(76.6f, 10.8f, 0.0f);
        const glm::vec3 yOffsets[] = {
            { 0.0f, 3.0f, 0.0f },
            { 0.0f, 3.0f, 0.0f },
            { 0.0f, 3.0f, 0.0f },
            { 0.0f, 2.0f, 0.0f },
            { 0.0f, 2.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
        };
        const float speeds[] = { 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f };

        backgroundParallaxManager = make_unique<BackgroundParallax>();

        for (int i = 0; i < 6; ++i)
        {
            auto *bgA = new RenderGameObject(string("BG") + to_string(i) + "A", bgPaths[i]);
            auto *bgB = new RenderGameObject(string("BG") + to_string(i) + "B", bgPaths[i]);

            bgA->SetScale(scaleVec);
            bgB->SetScale(scaleVec);
            bgA->SetPosition(yOffsets[i]);
            bgB->SetPosition(yOffsets[i] + glm::vec3(76.6f, 0.0f, 0.0f));

            backgroundParallaxManager->AddBackgroundPair(i, bgA, bgB, speeds[i]);

            m_gameObjects.push_back(bgA);
            m_gameObjects.push_back(bgB);
        }
    }

    /// @brief Creates the black fade transition object.
    void SetupTransition()
    {
        transitionObject = new UINormal(
            "Transition", "Assets/Images/black.png", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(25.0f, 20.0f, 0.0f), true);
        transitionEffects = make_unique<TransitionEffects>(transitionObject);
    }

    /// @brief Creates the instruction text object.
    void SetupInstructionText()
    {
        instructionText =
            new Text("dialogueinstruction", "Use [Left-click] or [Space] to continue", "Assets/Fonts/mvboli.ttf", true);
        instructionText->SetScale(0.6f);
        instructionText->SetPosition(glm::vec3(6.5f, -4.6f, 0.0f));
        instructionText->SetColor(glm::vec3(1, 1, 1));
    }

    /// @brief Creates the three shared dialogue choice buttons.
    /// @post  choice1, choice2, choice3 are valid non-null pointers.
    void SetupDialogueChoices(UIButton *&choice1, UIButton *&choice2, UIButton *&choice3)
    {
        const string hoverTex = "Assets/Images/UI/DialogueChoiceBox_Hover.png";
        const string boxTex = "Assets/Images/UI/DialogueChoiceBox.png";
        const string font = "Assets/Fonts/OverpassMono-SemiBold.ttf";

        choice1 = new UIButton("DialogueChoiceBox1", boxTex, glm::vec3(0.0f, -2.8f, 0.0f),
            glm::vec3(7.47f * 0.9f, 1.07f * 0.9f, 0.0f), true, true, font);

        choice2 = new UIButton("DialogueChoiceBox2", boxTex, glm::vec3(0.0f, -3.8f, 0.0f),
            glm::vec3(7.47f * 0.9f, 1.07f * 0.9f, 0.0f), true, true, font);

        choice3 = new UIButton("DialogueChoiceBox3", boxTex, glm::vec3(0.0f, -1.8f, 0.0f),
            glm::vec3(7.47f * 0.9f, 1.07f * 0.9f, 0.0f), true, true, font);

        choice1->SetHoverTexture(hoverTex);
        choice2->SetHoverTexture(hoverTex);
        choice3->SetHoverTexture(hoverTex);
    }

    // -----------------------------------------------------------------
    // Shared members
    // -----------------------------------------------------------------

    CharacterData *characterData = CharacterData::GetInstance();
    AudioManager &audioManager;
    GameStateManager &gameStateManager;
    Input &input;
    Camera &camera;

    Text *instructionText = nullptr;
    Door *roomDoor = nullptr;
    Door *kitchenDoor = nullptr;
    UIElement *transitionObject = nullptr;

    std::unique_ptr<TransitionEffects> transitionEffects;
    std::unique_ptr<DialogueManager> dialogueManager;
    std::unique_ptr<ObjectsParallax> objectsParallaxManager;
    std::unique_ptr<BackgroundParallax> backgroundParallaxManager;

    PauseMenu pauseMenu;

    // Dialogue keys
    string serveDialogueKey;
    string teaDialogueKey;
    string sandwichDialogueKey;
    string dessertDialogueKey;
    string scoreDialogueKey;
    string inspectStartDialogueKey;
    string inspectEndDialogueKey;

    // Dialogue flags
    bool serveDialogueSet = false;
    bool teaDialogueSet = false;
    bool sandwichDialogueSet = false;
    bool dessertDialogueSet = false;
    bool scoreDialogueSet = false;
    bool inspectStartDialogueSet = false;
    bool inspectEndDialogueSet = false;

    float instructionTimer = kInstructionResetTime;
    bool instructionVisible = true;

private:
    static constexpr float kInstructionResetTime = 8.0f;
    static constexpr float kFastForwardInterval = 0.05f; // seconds between advances

    float fastForwardTimer = 0.0f; // countdown between fast forward advances

    /// @brief Returns true when fast forward should fire.
    /// @param ctrlHeld         True if Left Ctrl is currently held.
    /// @param isPaused         True if pause menu is visible.
    /// @param isChoiceDialogue True if current dialogue is a choice prompt.
    /// @pre   none
    /// @post  false if any blocking condition is true
    static bool ShouldFastForward(bool ctrlHeld, bool isPaused, bool isChoiceDialogue)
    {
        if (!ctrlHeld)
            return false;
        if (isPaused)
            return false;
        if (isChoiceDialogue)
            return false;
        return true;
    }
};