#pragma once

#include "Scene.h"
#include <memory>

#include "../GameObjects/RenderGameObject.h"
#include "../GameObjects/AnimateGameObject.h"
#include "../GameObjects/Player.h"
#include "../Text/TextRenderer.h"
#include "../Text/Text.h"
#include "../Audio/AudioManager.h"
#include "../UI/UIButton.h"
#include "../UI/UIDraggable.h"
#include "../UI/UINormal.h"
#include "../UI/UIElement.h"
#include "../GameObjects/Book.h"
#include "../UI/UIButtonEmpty.h"
#include "../GameObjects/Door.h"
#include "../GameObjects/DoorManager.h"
#include "../GameObjects/BellManager.h"
#include "../GameObjects/OrderData.h"
#include "../GameObjects/Timer.h"
#include "../GameObjects/BellSoundClueManager.h"
#include "../Effects/BackgroundParallax.h"
#include "../GameObjects/Bell.h"
#include "../GameStateManager.h"
#include "KitchenData.h"

#include "../Effects/TransitionEffects.h"
#include "../PopupWidget/PauseMenu.h"

// set each cbin time before order (in milliseconds)
#define ORDER_DURATION1 3500
#define ORDER_DURATION2 8500 // for room 2
#define ORDER_DURATION3 5000 // for room 3
#define ORDER_DURATION4 6000

#define PLAYER_SPEED 5.75f





class Hallway : public Scene
{

private:
    GameStateManager &gameStateManager = GameStateManager::GetInstance();
    std::unique_ptr<TextRenderer> textRenderer;
    std::unique_ptr<BackgroundParallax> BackgroundparallaxManager;
    AudioManager &audioManager;

    Player *player;
    Book *Journal;
    UIElement *journalUpdateIcon;

    Text *orderNoText;
    Text *teaOrderText;
    Text *sandwichOrderText;
    Text *pastryOrderText;
    UIElement *orderPaper;
    UIObject *journalArrow;

    Text *timerText;
    UIElement *timerUI;
    Text *instructionText;
    Text *tutorialText;

    UIButton *ReadyButton;

    Door *room1Door;
    Door *room2Door;
    Door *room3Door;
    Door *room4Door;
    Door *kitchenDoor;

    Bell *bellCabin1;
    Bell *bellCabin2;
    Bell *bellCabin3;
    Bell *bellCabin4;

    BellSoundClueManager *bellSoundClueManager;

    UIElement *transitionObject;
    std::unique_ptr<TransitionEffects> transitionEffects;
    bool firstEntry = true;
    bool inDoorCollision = false;
    bool entering = false;

    bool pressA = false;
    bool pressD = false;

    bool moveTextOn = true;

    PauseMenu pauseMenu;

    void SetAllDoorPermissions(bool permitted)
    {
        room1Door->setPermission(permitted);
        room2Door->setPermission(permitted);
        room3Door->setPermission(permitted);
        room4Door->setPermission(permitted);
        kitchenDoor->setPermission(permitted);
    }

    void ApplyProgressionStateToHallway()
    {
        SetAllDoorPermissions(false);

        GameState currentGameState = gameStateManager.getGameState();
        RoomState currentRoomState = gameStateManager.getRoomState();

        if (currentRoomState == RoomState::Prepare)
        {
            if (KitchenData::GetInstance()->checkCompletePlate())
            {
                Door *targetDoor = DoorManager::GetInstance().GetDoorByName(gameStateNameToDoorName(currentGameState));
                if (targetDoor)
                {
                    targetDoor->setPermission(true);
                }
            }
            else
            {
                kitchenDoor->setPermission(true);
            }
        }
        else if (currentRoomState == RoomState::Serve || currentRoomState == RoomState::Score ||
                 currentRoomState == RoomState::MealReact || currentRoomState == RoomState::InspectionStart ||
                 currentRoomState == RoomState::Inspection || currentRoomState == RoomState::InspectionEnd)
        {
            Door *targetDoor = DoorManager::GetInstance().GetDoorByName(gameStateNameToDoorName(currentGameState));
            if (targetDoor)
            {
                targetDoor->setPermission(true);
            }
        }
    }

public:
    Hallway() : audioManager(AudioManager::GetInstance())
    {

        /*--------------------------------------------------------------🔊LOAD
         * AUDDIO🔊-------------------------------------------------------------------------------------------------------
         */
        audioManager.LoadSound("hallwayMusic", "Assets/Sounds/Music/BGmusic_Corridor_NoTimer.mp3", Music, 0.1f);
        audioManager.LoadSound("trainAmbience", "Assets/Sounds/Ambience/Ambience_Train.mp3", SFX, 0.12f);
        audioManager.LoadSound("bellRing", "Assets/Sounds/SFX_CallingBell.mp3", SFX, 0.25f);
        audioManager.LoadSound("buttonClick", "Assets/Sounds/SFX_DialogueChoice.mp3", SFX, 0.45f);
        audioManager.LoadSound("dialoguePlay", "Assets/Sounds/SFX_DialoguePlay.mp3", SFX, 0.45f);
        audioManager.LoadSound("doorSlide", "Assets/Sounds/SFX_SlideDoor.mp3", SFX, 9.0f);

        /*--------------------------------------------------------------📦CREATE
         * GAMEOBJECT📦-------------------------------------------------------------------------------------------------------
         */
        /*-------------------------------------------------------------🌲CREATE
         * ENVIRONMENT🌲-------------------------------------------------------------------------------------------------------
         */
        GameObject *background1a = new RenderGameObject("BG1A", "Assets/Images/BG/Cabin_Background_01.png");
        GameObject *background2a = new RenderGameObject("BG2A", "Assets/Images/BG/Cabin_Background_02.png");
        GameObject *background3a = new RenderGameObject("BG3A", "Assets/Images/BG/Cabin_Background_03.png");
        GameObject *background1b = new RenderGameObject("BG1B", "Assets/Images/BG/Cabin_Background_01.png");
        GameObject *background2b = new RenderGameObject("BG2B", "Assets/Images/BG/Cabin_Background_02.png");
        GameObject *background3b = new RenderGameObject("BG3B", "Assets/Images/BG/Cabin_Background_03.png");



        background1a->SetScale(glm::vec3(76.6f, 10.8f, 0.0f));
        background1a->SetPosition(glm::vec3(0.0f, 3.0f, 0.0f));
        background2a->SetScale(glm::vec3(76.6f, 10.8f, 0.0f));
        background2a->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
        background3a->SetScale(glm::vec3(76.6f, 10.8f, 0.0f));
        background3a->SetPosition(glm::vec3(0.0f, 3.0f, 0.0f));
        background1b->SetScale(glm::vec3(76.6f, 10.8f, 0.0f));
        background1b->SetPosition(glm::vec3(76.6f, 3.0f, 0.0f));
        background2b->SetScale(glm::vec3(76.6f, 10.8f, 0.0f));
        background2b->SetPosition(glm::vec3(76.6f, 5.0f, 0.0f));
        background3b->SetScale(glm::vec3(76.6f, 10.8f, 0.0f));
        background3b->SetPosition(glm::vec3(76.6f, 3.0f, 0.0f));

        BackgroundparallaxManager = std::make_unique<BackgroundParallax>();

        BackgroundparallaxManager->AddBackgroundPair(0, background1a, background1b, 0.5f);
        BackgroundparallaxManager->AddBackgroundPair(1, background2a, background2b, 1.0f);
        BackgroundparallaxManager->AddBackgroundPair(2, background3a, background3b, 1.5f);


        GameObject *hallway = new RenderGameObject("Cabin", "Assets/Images/Corridor/Corridor_Background.png");
        GameObject *hallwaylights = new RenderGameObject("CabinLights", "Assets/Images/Corridor/Corridor_Light.png");

        transitionObject = new UINormal(
            "Transition", "Assets/Images/black.png", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(25.0f, 20.0f, 0.0f), true);
        transitionEffects = std::make_unique<TransitionEffects>(transitionObject);

        /*-------------------------------------------------------------🚪CREATE
         * DOORS🚪-------------------------------------------------------------------------------------------------------
         */
        GameObject *roomdoor1Highlight =
            new RenderGameObject("RoomdoorHighlight1", "Assets/Images/Corridor/PassengerDoor_Highlight.png");
        GameObject *roomdoor2Highlight =
            new RenderGameObject("RoomdoorHighlight1", "Assets/Images/Corridor/PassengerDoor_Highlight.png");
        GameObject *roomdoor3Highlight =
            new RenderGameObject("RoomdoorHighlight3", "Assets/Images/Corridor/PassengerDoor3_Highlight.png");
        GameObject *roomdoor4Highlight =
            new RenderGameObject("RoomdoorHighlight4", "Assets/Images/Corridor/PassengerDoor4_Highlight.png");
        GameObject *kitchendoorHighlight =
            new RenderGameObject("KitchendoorHighlight", "Assets/Images/Corridor/KitchenDoor_Highlight.png");

        DoorManager &doorManager = DoorManager::GetInstance();
        room1Door = new Door("Room1Door", roomdoor1Highlight, glm::vec3(-17.58f, -0.632f, 0.0f),
            glm::vec3(2.29f * 1.2f, 4.65f * 1.2f, 0.0f), "Room1");
        doorManager.AddDoor(room1Door);

        room2Door = new Door("Room2Door", roomdoor2Highlight, glm::vec3(-8.588f, -0.632f, 0.0f),
            glm::vec3(2.29f * 1.2f, 4.65f * 1.2f, 0.0f), "Room2");
        doorManager.AddDoor(room2Door);

        room3Door = new Door("Room3Door", roomdoor3Highlight, glm::vec3(0.367f, -0.622f, 0.0f),
            glm::vec3(2.29f * 1.2f, 4.65f * 1.2f, 0.0f), "Room3");
        doorManager.AddDoor(room3Door);

        room4Door = new Door("Room4Door", roomdoor4Highlight, glm::vec3(9.345f, -0.622f, 0.0f),
            glm::vec3(2.29f * 1.2f, 4.65f * 1.2f, 0.0f), "Room4");
        doorManager.AddDoor(room4Door);

        kitchenDoor = new Door("KitchenDoor", kitchendoorHighlight, glm::vec3(18.31f, -0.629f, 0.0f),
            glm::vec3(2.29f * 1.2f, 4.65f * 1.2f, 0.0f), "Kitchen");
        doorManager.AddDoor(kitchenDoor);


        /*-------------------------------------------------------------🔔CREATE
         * BELLS🔔-------------------------------------------------------------------------------------------------------
         */
        bellCabin1 = new Bell("bellCabin1", "Assets/Images/Corridor/Bell_Ring_Sprite.png", 1, 4);
        bellCabin2 = new Bell("bellCabin2", "Assets/Images/Corridor/Bell_Ring_Sprite.png", 1, 4);
        bellCabin3 = new Bell("bellCabin3", "Assets/Images/Corridor/Bell_Ring_Sprite.png", 1, 4);
        bellCabin4 = new Bell("bellCabin4", "Assets/Images/Corridor/Bell_Ring_Sprite.png", 1, 4);

        BellManager &bellManager = BellManager::GetInstance();

        bellManager.AddBell(bellCabin1, room1Door);
        bellManager.AddBell(bellCabin2, room2Door);
        bellManager.AddBell(bellCabin3, room3Door);
        bellManager.AddBell(bellCabin4, room4Door);


        /*-------------------------------------------------------------🎮CREATE
         * PLAYER🎮-------------------------------------------------------------------------------------------------------
         */

        player = new Player("waiter", "Assets/Images/Waiter_Sprite_Walk.png", 4, 8, Journal, PLAYER_SPEED);

        // Load postion from XML
        player->LoadPosition();

        /*-------------------------------------------------------------💬CREATE
         * TEXT💬-------------------------------------------------------------------------------------------------------
         */


        orderNoText = new Text("orderNo", "", "Assets/Fonts/mvboli.ttf", true);
        teaOrderText = new Text("TeaOrder", "", "Assets/Fonts/mvboli.ttf", true);
        sandwichOrderText = new Text("sandwichOrder", "", "Assets/Fonts/mvboli.ttf", true);
        pastryOrderText = new Text("PastryOrder", "", "Assets/Fonts/mvboli.ttf", true);

        // OrderData Setup
        OrderData &orderData = OrderData::GetInstance();
        orderData.AddObserver(
            [this, &orderData]()
            {
                this->orderNoText->SetContent(orderData.GetRoomNumber());
                this->teaOrderText->SetContent(orderData.GetTeaOrder());
                this->sandwichOrderText->SetContent(orderData.GetSandwichOrder());
                this->pastryOrderText->SetContent(orderData.GetPastryOrder());
            });

        // Timer Setup
        Timer &timer = Timer::GetInstance();
        timerText = new Text("timerText", "", "Assets/Fonts/Jibril.ttf", true);
        timer.Initialize(timerText);

        timer.AddObserver(
            [&]()
            {
                timerText->SetContent(timer.GetTime());
                timerText->setActiveStatus(timer.isTimerRunning());
            });


        instructionText = new Text("instruction", "Press [E] to enter", "Assets/Fonts/mvboli.ttf", true);
        instructionText->SetScale(0.6f);
        instructionText->SetPosition(glm::vec3(7.30f, -4.5f, 0.0f));
        instructionText->SetColor(glm::vec3(1, 1, 1));


        tutorialText =
            new Text("tutorialText", "Press [A] to move left and [D] to move right", "Assets/Fonts/mvboli.ttf", true);
        tutorialText->SetScale(0.6f);
        tutorialText->SetPosition(glm::vec3(0.0f, -4.5f, 0.0f));
        tutorialText->SetColor(glm::vec3(1, 1, 1));


        /*-------------------------------------------------------------💬CREATE
         * UI💬-------------------------------------------------------------------------------------------------------
         */

        Journal = new Book();

        orderPaper = new UINormal("OrderPaper", "Assets/Images/UI/OrderPaper.png", glm::vec3(-7.72f, 4.1f, 0.0f),
            glm::vec3(3.55f, 2.54f, 0.0f), true); // Start inactive
        orderData.SetOrderPaper(orderPaper);

        timerUI = new UINormal(
            "TimerUI", "Assets/Images/UI/Timer.png", glm::vec3(7.3f, 5.22f, 0.0f), glm::vec3(4.37f, 3.13f, 0.0f), true);
        timer.SetTimerUI(this->timerUI);

        // UIElement* screenUI = new UINormal("ScreenUI", "Assets/Images/ScreenUI.png", glm::vec3(0.0f, 0.0f, 0.0f),
        // glm::vec3(16.0f * 1.19f, 9.0f * 1.19f, 0.0f), true);
        UIButton *journalButton = new UIButton("JournalButton", "Assets/Images/UI/JournalButton.png",
            glm::vec3(-8.32f, -4.8f, 0.0f), glm::vec3(3.0f, 3.0f, 0.0f), true, false, "");
        journalButton->SetHoverTexture("Assets/Images/UI/JournalButton_Highlight.png");
        journalButton->SetOnClickAction([this]() { Journal->drawBook(); });

        journalUpdateIcon = new UINormal("JournalUpdateIcon", "Assets/Images/Journal/UpdateIcon.png",
            glm::vec3(-7.38f, -3.65f, 0.0f), glm::vec3(0.62f / 2, 1.71f / 2, 0.0f), true);

        journalArrow = new UIObject("JournalArrow", "Assets/Images/Kitchen/Kitchen_Arrow_Tea.png", true);
        journalArrow->SetScale(glm::vec3(1.58f * 0.9f, 1.35f * 0.9f, 0.0f));
        journalArrow->SetPosition(glm::vec3(-5.9f, -3.2f, 0.0f));


        ReadyButton =
            new UIButton("PlayButton", "Assets/Images/Kitchen/Button_ResetMeal.png", glm::vec3(7.5f, 4.6f, 0.0f),
                glm::vec3(3.19f * 1.1, 0.92f * 1.1, 0.0f), true, true, "Assets/Fonts/OverpassMono-SemiBold.ttf");
        ReadyButton->SetHoverTexture("Assets/Images/Kitchen/Button_ResetMeal_Highlight.png");
        ReadyButton->SetButtonText("Open for next order");
        ReadyButton->SetTextSize(0.48f);
        ReadyButton->SetTextPosition(glm::vec3(7.55f, 4.55f, 0.0f));
        ReadyButton->SetOnClickAction([this]() { StartOrderTime(); });
        ReadyButton->setActiveStatus(false);


        // BellSoundClue
        BellSoundClue *leftBellClue = new BellSoundClue(
            "LeftBellClue", "Assets/Images/Corridor/BellSound.png", glm::vec3(-8.8f, 1.5f, 0.0f), 1, 4);
        BellSoundClue *rightBellClue = new BellSoundClue(
            "RightBellClue", "Assets/Images/Corridor/BellSound.png", glm::vec3(8.8f, 1.5f, 0.0f), 1, 4);

        leftBellClue->SetScale(glm::vec3(1.5f, 1.5f, 0.0f));
        rightBellClue->SetScale(glm::vec3(-1.5f, 1.5f, 0.0f));

        // Create BellSoundClueManager
        bellSoundClueManager = new BellSoundClueManager(leftBellClue, rightBellClue);

        // TO TEST DRAW EMPTY UI
        /*GameObject* box = new RenderGameObject("textbox", "Assets/Images/Square_Border.png");
        box->SetScale(glm::vec3(2.8f, 5.7f, 0.0f));
        box->SetPosition(glm::vec3(18.35f, -0.55f, 0.0f));
        m_gameObjects.push_back(box);*/

        /*-------------------------------------------------------------➡️SET
         * TRANSFORMATION➡️-------------------------------------------------------------------------------------------------------
         */

        hallway->SetScale(glm::vec3(50.00f, 10.8f, 0.0f));
        hallway->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        hallwaylights->SetScale(glm::vec3(50.0f, 10.8f, 0.0f));
        hallwaylights->SetPosition(glm::vec3(0.0f, -0.2f, 0.0f));

        bellCabin1->SetScale(glm::vec3(1.6f * 1.2f, 1.6f * 1.2f, 0.0f));
        bellCabin1->SetPosition(glm::vec3(-16.54f, 1.49f, 0.0f));
        bellCabin2->SetScale(glm::vec3(1.6f * 1.2f, 1.6f * 1.2f, 0.0f));
        bellCabin2->SetPosition(glm::vec3(-7.54f, 1.49f, 0.0f));
        bellCabin3->SetScale(glm::vec3(1.6f * 1.2f, 1.6f * 1.2f, 0.0f));
        bellCabin3->SetPosition(glm::vec3(1.41f, 1.49f, 0.0f));
        bellCabin4->SetScale(glm::vec3(1.6f * 1.2f, 1.6f * 1.2f, 0.0f));
        bellCabin4->SetPosition(glm::vec3(10.44f, 1.49f, 0.0f));


        orderNoText->SetPosition(glm::vec3(-8.8f, 4.5f, 0.0f));
        orderNoText->SetColor(glm::vec3(0.5, 0, 0));
        teaOrderText->SetPosition(glm::vec3(-7.2f, 4.6f, 0.0f));
        teaOrderText->SetScale(0.65f);
        sandwichOrderText->SetPosition(glm::vec3(-7.63f, 3.95f, 0.0f));
        sandwichOrderText->SetScale(0.55f);
        pastryOrderText->SetPosition(glm::vec3(-7.63f, 3.32f, 0.0f));
        pastryOrderText->SetScale(0.55f);

        timerText->SetPosition(glm::vec3(6.8f, 4.22f, 0.0f));
        timerText->SetColor(glm::vec3(0.78039, 0.72549, 0.44314));
        timerText->SetScale(1.45f);

        /*--------------------------------------------------------------✅PUSH
         * BACK✅-------------------------------------------------------------------------------------------------------
         */
        // Environment
        m_gameObjects.push_back(background1a);
        m_gameObjects.push_back(background1b);
        m_gameObjects.push_back(background2a);
        m_gameObjects.push_back(background2b);
        m_gameObjects.push_back(background3a);
        m_gameObjects.push_back(background3b);

        m_gameObjects.push_back(hallway);

        // Doors
        m_gameObjects.push_back(kitchenDoor);
        m_gameObjects.push_back(room1Door);
        m_gameObjects.push_back(room2Door);
        m_gameObjects.push_back(room3Door);
        m_gameObjects.push_back(room4Door);
        m_gameObjects.push_back(roomdoor1Highlight);
        m_gameObjects.push_back(roomdoor2Highlight);
        m_gameObjects.push_back(roomdoor3Highlight);
        m_gameObjects.push_back(roomdoor4Highlight);
        m_gameObjects.push_back(kitchendoorHighlight);

        // Bells
        m_gameObjects.push_back(bellCabin1);
        m_gameObjects.push_back(bellCabin2);
        m_gameObjects.push_back(bellCabin3);
        m_gameObjects.push_back(bellCabin4);

        m_gameObjects.push_back(player);
        m_gameObjects.push_back(hallwaylights);

        // UIs
        m_gameObjects.push_back(journalButton);
        m_gameObjects.push_back(journalUpdateIcon);
        m_gameObjects.push_back(journalArrow);
        m_gameObjects.push_back(timerUI);
        m_gameObjects.push_back(orderPaper);
        m_gameObjects.push_back(ReadyButton);
        m_gameObjects.push_back(leftBellClue);
        m_gameObjects.push_back(rightBellClue);




        // UITexts
        m_gameObjects.push_back(orderNoText);
        m_gameObjects.push_back(teaOrderText);
        m_gameObjects.push_back(sandwichOrderText);
        m_gameObjects.push_back(pastryOrderText);
        m_gameObjects.push_back(timerText);
        m_gameObjects.push_back(instructionText);
        m_gameObjects.push_back(tutorialText);

        // Journal
        m_gameObjects.push_back(Journal);

        m_gameObjects.push_back(&pauseMenu);

        // Transition
        m_gameObjects.push_back(transitionObject);
    }



    void OnEnter() override
    {
        // Scene::OnEnter();

        if (!audioManager.IsSoundPlaying("hallwayMusic"))
            audioManager.PlaySound("hallwayMusic", true);
        if (!audioManager.IsSoundPlaying("trainAmbience"))
            audioManager.PlaySound("trainAmbience", true);

        if (gameStateManager.WasRestoredFromSave())
        {
            firstEntry = false;
            moveTextOn = false;
            pressA = true;
            pressD = true;
        }

        if (firstEntry)
        {
            tutorialText->setActiveStatus(true);
            journalArrow->setActiveStatus(false);
        }
        else
        {
            tutorialText->setActiveStatus(false);
            journalArrow->setActiveStatus(false);
        }

        journalUpdateIcon->setActiveStatus(JournalData::GetInstance()->HasUnopenedClue());

        // Ensure kitchen door is always accessible
        // kitchenDoor->setPermission(true);

        // Set each state's order phrase behavior
        GameState currentGameState = gameStateManager.getGameState();
        RoomState currentRoomState = gameStateManager.getRoomState();

        ApplyProgressionStateToHallway();

        transitionEffects->FadeIn(2.0f, [this]() {});


        if ((currentGameState == GameState::ROOM1_STATE || currentGameState == GameState::ROOM2_STATE ||
                currentGameState == GameState::ROOM3_STATE || currentGameState == GameState::ROOM4_STATE) &&
            currentRoomState == RoomState::Order && !firstEntry)
        {
            ReadyButton->setActiveStatus(true);
        }
        else if (currentGameState == GameState::END_STATE && currentRoomState == RoomState::End)
        {
            player->StopMovement();
            transitionEffects->FadeOut(3.0f,
                [this]()
                {
                    std::cout << "Fade Out complete" << std::endl;
                    audioManager.StopSound("hallwayMusic");

                    player->ResumeMovement();
                    Application::Get().SetScene("JournalEntry");
                });
        }
        else if ((currentGameState == GameState::ROOM1_STATE || currentGameState == GameState::ROOM2_STATE ||
                     currentGameState == GameState::ROOM3_STATE || currentGameState == GameState::ROOM4_STATE) &&
                 currentRoomState == RoomState::Prepare)
        {
            tutorialText->setActiveStatus(true);
            if (KitchenData::GetInstance()->checkCompletePlate())
            {
                tutorialText->SetContent("Return to the passenger to serve.");
            }
            else
            {
                tutorialText->SetContent("Head to the kitchen.");
            }
        }

        // Reset/Hide The Pause Menu
        pauseMenu.Hide();
        player->LoadPosition();


        entering = false;


        // Test clue activation

        /*JournalData::GetInstance()->ActivateClue(CABIN21, 0);
        JournalData::GetInstance()->ActivateClue(CABIN21, 1);
        JournalData::GetInstance()->ActivateClue(CABIN21, 2);
        JournalData::GetInstance()->ActivateClue(CABIN21, 3);
        JournalData::GetInstance()->ActivateClue(CABIN21, 4);
        JournalData::GetInstance()->ActivateClue(CABIN21, 5);
        JournalData::GetInstance()->ActivateClue(CABIN21, 6);

        JournalData::GetInstance()->ActivateClue(CABIN1, 0);
        JournalData::GetInstance()->ActivateClue(CABIN1, 1);
        JournalData::GetInstance()->ActivateClue(CABIN1, 2);
        JournalData::GetInstance()->ActivateClue(CABIN1, 3);
        JournalData::GetInstance()->ActivateClue(CABIN1, 4);
        JournalData::GetInstance()->ActivateClue(CABIN1, 5);
        JournalData::GetInstance()->ActivateClue(CABIN1, 6);

        JournalData::GetInstance()->ActivateClue(CABIN4, 0);
        JournalData::GetInstance()->ActivateClue(CABIN4, 1);
        JournalData::GetInstance()->ActivateClue(CABIN4, 2);
        JournalData::GetInstance()->ActivateClue(CABIN4, 3);
        JournalData::GetInstance()->ActivateClue(CABIN4, 4);
        JournalData::GetInstance()->ActivateClue(CABIN4, 5);
        JournalData::GetInstance()->ActivateClue(CABIN4, 6);*/
        // JournalData::GetInstance()->ActivateClue(CABIN4, 7);

        // Draggable paper clues test
        // JournalData::GetInstance()->ActivateClue(CABIN3, 10);
        // JournalData::GetInstance()->ActivateClue(CABIN4, 7);
    }


    void Update(float dt, long frame)
    {
        Scene::Update(dt, frame);
        BackgroundparallaxManager->Update(dt);

        Timer &timer = Timer::GetInstance();

        timer.Update(dt);

        gameStateManager.Update();

        BellManager &bellManager = BellManager::GetInstance();
        bellManager.Update(dt, frame);

        bellSoundClueManager->Update(dt, frame);

        if (Journal->isOpen())
        {
            Journal->Update(dt, frame);
            if (journalArrow->getActiveStatus() == true)
            {
                journalArrow->setActiveStatus(false);
            }
        }

        // To test position of update icon
        // journalUpdateIcon->setActiveStatus(true);

        journalUpdateIcon->setActiveStatus(JournalData::GetInstance()->HasUnopenedClue());

        KitchenData *foodData = KitchenData::GetInstance();

        transitionEffects->Update(dt);

        if (player)
        {
            glm::vec3 playerPos = player->GetPosition();
            Camera &camera = Application::GetCamera();
            float targetX = std::max(-12.74f, std::min(playerPos.x, 12.91f));
            camera.SetPosition(targetX + 0.05f * (targetX - camera.GetPosX()), camera.GetPosY());

            DoorManager &doorManager = DoorManager::GetInstance();
            bool isNearDoor = doorManager.CheckDoorCollisions(playerPos, player->GetScale(),
                [this, &bellManager](Door *door)
                {
                    Input &input = Application::GetInput();

                    if (door)
                    {
                        if (door->getPermission())
                        {
                            instructionText->SetContent("Press [E] to enter");

                            if (input.Get().GetKeyDown(GLFW_KEY_E) && !entering)
                            {
                                if (door->GetName() == "KitchenDoor")
                                {
                                    player->StopMovement();
                                    transitionEffects->FadeOut(1.0f,
                                        [this]()
                                        {
                                            Application::Get().SetScene("Kitchen");
                                            player->ResumeMovement();
                                        });
                                    entering = true;
                                }
                                else
                                {
                                    player->StopMovement();
                                    bellManager.StopAllRinging();

                                    audioManager.PlaySound("knockDoor");

                                    Application::Get().SetTimer(
                                        1000, [this]() { transitionEffects->FadeOut(1.0f, [this]() {}); }, false);

                                    audioManager.PauseSound("hallwayMusic");

                                    Application::Get().SetTimer(
                                        3000,
                                        [this, door]()
                                        {
                                            Application::Get().SetScene(door->GetSceneName());
                                            player->ResumeMovement();
                                        },
                                        false);

                                    entering = true;
                                }
                            }
                        }
                        else
                        {
                            GameState currentGameState = gameStateManager.getGameState();
                            RoomState currentRoomState = gameStateManager.getRoomState();

                            if (currentRoomState == RoomState::Prepare &&
                                door->GetName() == gameStateNameToDoorName(currentGameState))
                            {
                                instructionText->SetContent("Meal is required before serving.");
                            }
                            else
                            {
                                instructionText->SetContent("Locked. Permission required.");
                            }
                        }
                    }
                });

            instructionText->setActiveStatus(isNearDoor);
        }

        Input &input = Application::GetInput();

        if (input.Get().GetKeyDown(GLFW_KEY_ESCAPE))
        {
            pauseMenu.Show();
        }
        if (input.Get().GetKeyDown(GLFW_KEY_TAB))
        {
            if (Journal->isOpen())
            {
                Journal->closeBook();
            }
            else if (!Journal->isOpen())
            {
                Journal->drawBook();
            }
        }

        if (moveTextOn)
        {
            if (input.Get().GetKeyDown(GLFW_KEY_A))
            {
                pressA = true;
            }

            if (input.Get().GetKeyDown(GLFW_KEY_D))
            {
                pressD = true;
            }

            if (pressA || pressD)
            {
                // Start countdown to hide text
                Application::Get().SetTimer(
                    2500,
                    [this]()
                    {
                        tutorialText->SetContent("");
                        journalArrow->setActiveStatus(true);
                    },
                    false);
                moveTextOn = false;
            }
        }

        if (firstEntry && (pressA || pressD))
        {
            if (Journal->isOpen())
            {
                journalArrow->setActiveStatus(false);
                ReadyButton->setActiveStatus(true);
                firstEntry = false;
            }
        }
    }

    void StartOrderTime()
    {

        audioManager.PlaySound("servingBellRing", false);
        GameState currentGameState = gameStateManager.getGameState();
        RoomState currentRoomState = gameStateManager.getRoomState();
        ReadyButton->setActiveStatus(false);
        if (currentGameState == GameState::ROOM1_STATE)
        {
            tutorialText->setActiveStatus(true);
            if (currentRoomState == RoomState::Order)
            {
                tutorialText->SetContent("Head to the alarming cabin to receive order.");
                Application::Get().SetTimer(
                    ORDER_DURATION1,
                    [this]()
                    {
                        bellCabin1->startRinging();
                        room1Door->setPermission(true);
                    },
                    false);
            }
        }
        else if (currentGameState == GameState::ROOM2_STATE)
        {
            Application::Get().SetTimer(
                ORDER_DURATION2,
                [this]()
                {
                    bellCabin2->startRinging();
                    room2Door->setPermission(true);
                },
                false);
        }
        else if (currentGameState == GameState::ROOM3_STATE)
        {
            Application::Get().SetTimer(
                ORDER_DURATION3,
                [this]()
                {
                    bellCabin3->startRinging();
                    room3Door->setPermission(true);
                },
                false);
        }
        else if (currentGameState == GameState::ROOM4_STATE)
        {
            Application::Get().SetTimer(
                ORDER_DURATION4,
                [this]()
                {
                    bellCabin4->startRinging();
                    room4Door->setPermission(true);
                },
                false);
        }
    }


    void OnExit() override
    {
        // Scene::OnExit();
        // audioManager.PauseSound("hallwayMusic");
        audioManager.StopSound("Player_footsteps");
    }
};
