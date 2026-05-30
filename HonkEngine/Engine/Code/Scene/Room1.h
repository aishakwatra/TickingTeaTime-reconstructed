// Room1.h
#pragma once

#include <map>
#include <string>
#include <cassert>
#include "RoomScene.h"

/// @brief Room scene for Cabin 1 — passenger Martha Longbow.
///
/// Invariants:
///   - cane and letter must be valid before ManageInspectionState() is called
///   - inspectingObject is either empty or one of {"Cane", "Letter"}
///   - isCaneInspected and isLetterInspected are false until each is completed
class Room1 : public RoomScene
{

public:
    /// @brief Constructs Room1, sets up all Martha-specific assets.
    /// @post  All GameObjects pushed to m_gameObjects.
    ///        dialogueManager is initialised and ready.
    Room1() : RoomScene("Room1Door")
    {

        audioManager.LoadSound("cabinMusic", "Assets/Sounds/Music/BGmusic_Cabin.mp3", Music, 5.0f);
        audioManager.LoadSound("knockDoor", "Assets/Sounds/SFX_KnockDoor.mp3", SFX, 2.0f);

        objectsParallaxManager = make_unique<ObjectsParallax>();

        SetupRoomObjects();

        // Dialogue box and choices
        UIButton *dialogueBox = new UIButton("DialogueBox", "Assets/Images/UI/DialogueBox.png",
            glm::vec3(0.0f, 3.5f, 0.0f), glm::vec3(10.96f, 2.05f, 0.0f), true, true, "Assets/Fonts/OverpassMono.ttf");

        UIButton *choice1 = nullptr;
        UIButton *choice2 = nullptr;
        UIButton *choice3 = nullptr;
        SetupDialogueChoices(choice1, choice2, choice3);

        SetupDialogueManager(dialogueBox, choice1, choice2, choice3);

        m_gameObjects.push_back(dialogueBox);
        m_gameObjects.push_back(choice1);
        m_gameObjects.push_back(choice2);
        m_gameObjects.push_back(choice3);
        for (auto &pair : speakerIcons)
        {
            m_gameObjects.push_back(pair.second);
        }
        m_gameObjects.push_back(instructionText);
        m_gameObjects.push_back(&pauseMenu);
        m_gameObjects.push_back(transitionObject);

        assert(dialogueManager != nullptr && "Room1: dialogueManager failed to initialise");
        assert(cane != nullptr && "Room1: cane object is null");
        assert(letter != nullptr && "Room1: letter object is null");
    }

protected:
    // -----------------------------------------------------------------
    // Identity
    // -----------------------------------------------------------------

    Cabin GetCabin() const override
    {
        return Cabin::CABIN1;
    }

    GameState GetRoomGameState() const override
    {
        return GameState::ROOM1_STATE;
    }

    // -----------------------------------------------------------------
    // Room-specific setup
    // -----------------------------------------------------------------

    /// @brief Creates all Martha-specific visual objects and parallax layers.
    /// @post  cane, caneInspect, letter, letterInspect are non-null.
    ///        All objects pushed to m_gameObjects.
    void SetupRoomObjects() override
    {
        const float sm = 1.23f;

        UIElement *room = new UINormal("cabin1", "Assets/Images/Martha/Martha_Cabin.png", glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(19.60f * sm, 10.55f * sm, 0.0f), true);

        UIElement *lamp = new UINormal("Lamp", "Assets/Images/Martha/Lamp.png", glm::vec3(-1.3f, -0.3f, 0.0f),
            glm::vec3(1.62f * sm, 2.25f * sm, 0.0f), true);

        UIElement *hat = new UINormal("Hat", "Assets/Images/Martha/Hat.png", glm::vec3(-1.1f, -1.22f, 0.0f),
            glm::vec3(3.12f * sm, 0.92f * sm, 0.0f), true);

        UIElement *bag = new UINormal("Bag", "Assets/Images/Martha/Bag.png", glm::vec3(-0.25f, -5.25f, 0.0f),
            glm::vec3(2.59f * sm * 1.2f, 1.64f * sm * 1.2f, 0.0f), true);

        // Inspectable objects
        cane = new UINormal("Cane", "Assets/Images/Martha/Martha_Inspection_Cane.png", glm::vec3(-4.0f, -3.72f, 0.0f),
            glm::vec3(1.07f * sm * 1.2f, 3.7f * sm * 1.2f, 0.0f), true);

        letter = new UINormal("Letter", "Assets/Images/Martha/Martha_Inspection_Letter.png",
            glm::vec3(-2.15f, -5.9f, 0.0f), glm::vec3(1.13f * sm * 1.2f, 0.73f * sm * 1.2f, 0.0f), true);

        // Inspect buttons
        caneInspect = new UIButton("Cane", "Assets/Images/Martha/Martha_Inspection_Cane.png",
            glm::vec3(-4.0f, -3.72f, 0.0f), glm::vec3(1.07f * sm * 1.2f, 3.7f * sm * 1.2f, 0.0f), true, false, "");
        caneInspect->SetHoverTexture("Assets/Images/Martha/Martha_Inspection_Cane_Highlight.png");
        caneInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "caneInspect clicked while inspectingObject already set");
                inspectingObject = "Cane";
            });

        letterInspect = new UIButton("Letter", "Assets/Images/Martha/Martha_Inspection_Letter.png",
            glm::vec3(-2.15f, -5.9f, 0.0f), glm::vec3(1.13f * sm * 1.2f, 0.73f * sm * 1.2f, 0.0f), true, false, "");
        letterInspect->SetHoverTexture("Assets/Images/Martha/Martha_Inspection_Letter_Highlight.png");
        letterInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "letterInspect clicked while inspectingObject already set");
                inspectingObject = "Letter";
            });

        caneInspect->setActiveStatus(false);
        letterInspect->setActiveStatus(false);

        // Speaker sprites
        marthaNormal = new UINormal("Martha_Normal", "Assets/Images/Martha/Martha_Normal.png",
            glm::vec3(4.95f, -2.45f, 0.0f), glm::vec3(3.81f * sm, 6.53f * sm, 0.0f), true);

        UIElement *marthaHappy = new UINormal("Martha_Happy", "Assets/Images/Martha/Martha_Happy.png",
            glm::vec3(4.95f, -2.45f, 0.0f), glm::vec3(3.81f * sm, 6.53f * sm, 0.0f), true);

        UIElement *marthaDisappoint = new UINormal("Martha_Disappoint", "Assets/Images/Martha/Martha_Disappoint.png",
            glm::vec3(4.95f, -2.45f, 0.0f), glm::vec3(3.81f * sm, 6.53f * sm, 0.0f), true);

        UIElement *marthaUpset = new UINormal("Martha_Upset", "Assets/Images/Martha/Martha_Upset.png",
            glm::vec3(4.95f, -2.45f, 0.0f), glm::vec3(3.81f * sm, 6.53f * sm, 0.0f), true);

        UIElement *marthaSad = new UINormal("Martha_Sad", "Assets/Images/Martha/Martha_Sad.png",
            glm::vec3(4.95f, -2.45f, 0.0f), glm::vec3(3.81f * sm, 6.53f * sm, 0.0f), true);

        UIElement *marthaShy = new UINormal("Martha_Shy", "Assets/Images/Martha/Martha_Shy.png",
            glm::vec3(4.95f, -2.45f, 0.0f), glm::vec3(3.81f * sm, 6.53f * sm, 0.0f), true);

        // Speaker icons
        UIElement *marthaIcon = new UINormal("MarthaIcon", "Assets/Images/UI/Speaker_icon_Martha.png",
            glm::vec3(4.18f, 3.5f, 0.0f), glm::vec3(2.19f, 1.57f, 0.0f), true);

        UIElement *waiterIcon = new UINormal("WaiterIcon", "Assets/Images/UI/Speaker_icon_Waiter.png",
            glm::vec3(4.18f, 3.43f, 0.0f), glm::vec3(1.23f, 1.4f, 0.0f), true);

        // Store sprites for later use in SetupDialogueManager
        speakerSprites = {
            { "Martha_Normal", marthaNormal },
            { "Martha_Happy", marthaHappy },
            { "Martha_Disappoint", marthaDisappoint },
            { "Martha_Upset", marthaUpset },
            { "Martha_Sad", marthaSad },
            { "Martha_Shy", marthaShy },
        };

        speakerIcons = {
            { "M", marthaIcon },
            { "W", waiterIcon },
        };

        // Parallax layers
        const float defaultLayer = 0.8f;
        const float objectLayer = 0.85f;

        objectsParallaxManager->AddObjectToLayer(room, defaultLayer);
        objectsParallaxManager->AddObjectToLayer(lamp, objectLayer);
        objectsParallaxManager->AddObjectToLayer(hat, objectLayer);
        objectsParallaxManager->AddObjectToLayer(bag, objectLayer);
        objectsParallaxManager->AddObjectToLayer(caneInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(letterInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(cane, objectLayer);
        objectsParallaxManager->AddObjectToLayer(letter, objectLayer);
        objectsParallaxManager->AddObjectToLayer(marthaNormal, objectLayer);
        objectsParallaxManager->AddObjectToLayer(marthaHappy, objectLayer);
        objectsParallaxManager->AddObjectToLayer(marthaDisappoint, objectLayer);
        objectsParallaxManager->AddObjectToLayer(marthaUpset, objectLayer);
        objectsParallaxManager->AddObjectToLayer(marthaSad, objectLayer);
        objectsParallaxManager->AddObjectToLayer(marthaShy, objectLayer);

        // Push to scene
        m_gameObjects.push_back(room);
        m_gameObjects.push_back(marthaNormal);
        m_gameObjects.push_back(marthaHappy);
        m_gameObjects.push_back(marthaDisappoint);
        m_gameObjects.push_back(marthaUpset);
        m_gameObjects.push_back(marthaSad);
        m_gameObjects.push_back(marthaShy);
        m_gameObjects.push_back(lamp);
        m_gameObjects.push_back(hat);
        m_gameObjects.push_back(bag);
        m_gameObjects.push_back(cane);
        m_gameObjects.push_back(letter);
        m_gameObjects.push_back(caneInspect);
        m_gameObjects.push_back(letterInspect);
    }

    /// @brief Initialises the DialogueManager with Martha's sprites and base dialogues.
    /// @pre   dialogueBox, choice1, choice2, choice3 must be non-null.
    /// @post  dialogueManager is non-null with Order/Serve dialogues loaded.
    void SetupDialogueManager(UIButton *dialogueBox, UIButton *choice1, UIButton *choice2, UIButton *choice3) override
    {
        assert(dialogueBox != nullptr && "SetupDialogueManager: dialogueBox is null");
        assert(choice1 != nullptr && choice2 != nullptr && choice3 != nullptr &&
               "SetupDialogueManager: choice buttons are null");

        dialogueManager = make_unique<DialogueManager>("MarthaDialogue", dialogueBox, "Martha_Normal");

        dialogueManager->LoadDialogues("Order", "Assets/Dialogue/Martha/Martha_Order.xml");
        dialogueManager->LoadDialogues("Serve_OnTime", "Assets/Dialogue/Martha/Martha_Serve_OnTime.xml");
        dialogueManager->LoadDialogues("Serve_Late", "Assets/Dialogue/Martha/Martha_Serve_Late.xml");

        for (auto &pair : speakerSprites)
        {
            dialogueManager->AddSpeakerSprite(pair.first, pair.second);
        }
        for (auto &pair : speakerIcons)
        {
            dialogueManager->AddSpeakerIcon(pair.first, pair.second);
        }

        dialogueManager->AddChoiceButton(choice1);
        dialogueManager->AddChoiceButton(choice2);
        dialogueManager->AddChoiceButton(choice3);

        dialogueManager->SetDialoguePosition(-0.5f, 3.6f);
        dialogueManager->SetDialogueScale(0.55f);
        dialogueManager->SetChoiceScale(0.56f);
    }

    // -----------------------------------------------------------------
    // Dialogue loading — Martha specific
    // -----------------------------------------------------------------

    /// @brief Loads Martha's meal reaction dialogues based on kitchen choices.
    /// @post  teaDialogueKey, sandwichDialogueKey, dessertDialogueKey are set
    ///        and non-empty.
    void SetMealReactionDialogue() override
    {
        KitchenData *kitchen = KitchenData::GetInstance();
        assert(kitchen != nullptr && "SetMealReactionDialogue: KitchenData is null");

        // Tea
        switch (kitchen->getTea())
        {
        case EARLGREYTEA:
            dialogueManager->LoadDialogues("MealReact_Tea_EarlGrey", "Assets/Dialogue/Martha/MealReact/"
                                                                     "Martha_MealReact_Tea_Assam_EarlGrey.xml");
            teaDialogueKey = "MealReact_Tea_EarlGrey";
            break;
        case ASSAMTEA:
            dialogueManager->LoadDialogues("MealReact_Tea_Assam", "Assets/Dialogue/Martha/MealReact/"
                                                                  "Martha_MealReact_Tea_Assam_EarlGrey.xml");
            teaDialogueKey = "MealReact_Tea_Assam";
            break;
        case CHAMOMILETEA:
            dialogueManager->LoadDialogues("MealReact_Tea_Charmomile", "Assets/Dialogue/Martha/MealReact/"
                                                                       "Martha_MealReact_Tea_Chamomile.xml");
            teaDialogueKey = "MealReact_Tea_Charmomile";
            break;
        case GREENTEA:
            dialogueManager->LoadDialogues("MealReact_Tea_GreenTea", "Assets/Dialogue/Martha/MealReact/"
                                                                     "Martha_MealReact_Tea_GreenTea.xml");
            teaDialogueKey = "MealReact_Tea_GreenTea";
            break;
        default:
            assert(false && "SetMealReactionDialogue: unknown tea type");
            break;
        }

        // Sandwich
        switch (kitchen->getSandwich())
        {
        case SALMON:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Salmon", "Assets/Dialogue/Martha/MealReact/"
                                                                        "Martha_MealReact_Sandwich_Salmon_Beef.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Salmon";
            break;
        case BEEF:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Beef", "Assets/Dialogue/Martha/MealReact/"
                                                                      "Martha_MealReact_Sandwich_Salmon_Beef.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Beef";
            break;
        case EGG:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Egg", "Assets/Dialogue/Martha/MealReact/"
                                                                     "Martha_MealReact_Sandwich_Egg.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Egg";
            break;
        case CUCUMBER:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Cucumber", "Assets/Dialogue/Martha/MealReact/"
                                                                          "Martha_MealReact_Sandwich_Cucumber.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Cucumber";
            break;
        default:
            assert(false && "SetMealReactionDialogue: unknown sandwich type");
            break;
        }

        // Dessert
        switch (kitchen->getDessert())
        {
        case ECLAIR:
            dialogueManager->LoadDialogues("MealReact_Dessert_Eclair",
                "Assets/Dialogue/Martha/MealReact/"
                "Martha_MealReact_Pastry_Eclair_Macaron_Tart.xml");
            dessertDialogueKey = "MealReact_Dessert_Eclair";
            break;
        case MACARON:
            dialogueManager->LoadDialogues("MealReact_Dessert_Macaron",
                "Assets/Dialogue/Martha/MealReact/"
                "Martha_MealReact_Pastry_Eclair_Macaron_Tart.xml");
            dessertDialogueKey = "MealReact_Dessert_Macaron";
            break;
        case TART:
            dialogueManager->LoadDialogues("MealReact_Dessert_Tart", "Assets/Dialogue/Martha/MealReact/"
                                                                     "Martha_MealReact_Pastry_Eclair_Macaron_Tart.xml");
            dessertDialogueKey = "MealReact_Dessert_Tart";
            break;
        case SCONE:
            dialogueManager->LoadDialogues("MealReact_Dessert_Scone", "Assets/Dialogue/Martha/MealReact/"
                                                                      "Martha_MealReact_Pastry_Scone.xml");
            dessertDialogueKey = "MealReact_Dessert_Scone";
            break;
        default:
            assert(false && "SetMealReactionDialogue: unknown dessert type");
            break;
        }

        assert(!teaDialogueKey.empty() && "SetMealReactionDialogue: teaDialogueKey not set");
        assert(!sandwichDialogueKey.empty() && "SetMealReactionDialogue: sandwichDialogueKey not set");
        assert(!dessertDialogueKey.empty() && "SetMealReactionDialogue: dessertDialogueKey not set");
    }

    /// @brief Loads Martha's score dialogue based on serve score level.
    /// @post  scoreDialogueKey is set and non-empty.
    void SetScoredDialogue() override
    {
        InteractionLevel level = characterData->getServeScoreLevel(GetCabin());

        switch (level)
        {
        case LOW:
            dialogueManager->LoadDialogues("Score_Low", "Assets/Dialogue/Martha/MealResult/Martha_MealResult_Low.xml");
            scoreDialogueKey = "Score_Low";
            break;
        case AVERAGE:
            dialogueManager->LoadDialogues("Score_Average", "Assets/Dialogue/Martha/MealResult/"
                                                            "Martha_MealResult_Average.xml");
            scoreDialogueKey = "Score_Average";
            break;
        case INFORMATIVE:
            dialogueManager->LoadDialogues("Score_Informative", "Assets/Dialogue/Martha/MealResult/"
                                                                "Martha_MealResult_Informative.xml");
            scoreDialogueKey = "Score_Informative";
            break;
        default:
            assert(false && "SetScoredDialogue: unknown interaction level");
            break;
        }

        assert(!scoreDialogueKey.empty() && "SetScoredDialogue: scoreDialogueKey not set");
    }

    /// @brief Loads Martha's inspection dialogues based on interaction level.
    /// @post  inspectStartDialogueKey, inspectCaneDialogueKey,
    ///        inspectLetterDialogueKey, inspectEndDialogueKey are set.
    void SetInteractDialogue() override
    {
        InteractionLevel level = characterData->getInteractionLevel(GetCabin());

        string prefix;
        string startPath;
        string letterPath;
        string canePath;
        string endPath;

        switch (level)
        {
        case LOW:
        case AVERAGE:
            prefix = "Average";
            startPath = "Assets/Dialogue/Martha/Average/Martha_Average_Start.xml";
            letterPath = "Assets/Dialogue/Martha/Average/Martha_Average_Letter.xml";
            canePath = "Assets/Dialogue/Martha/Average/Martha_Average_Cane.xml";
            endPath = "Assets/Dialogue/Martha/Average/Martha_Average_End.xml";
            break;

        case INFORMATIVE:
            prefix = "Informative";
            startPath = "Assets/Dialogue/Martha/Informative/"
                        "Martha_Informative_Start.xml";
            letterPath = "Assets/Dialogue/Martha/Informative/"
                         "Martha_Informative_Letter.xml";
            canePath = "Assets/Dialogue/Martha/Informative/"
                       "Martha_Informative_Cane.xml";
            endPath = "Assets/Dialogue/Martha/Informative/"
                      "Martha_Informative_End.xml";
            break;

        default:
            assert(false && "SetInteractDialogue: unknown interaction level");
            return;
        }

        const string startKey = "Inspect_" + prefix + "_Start";
        const string letterKey = "Inspect_" + prefix + "_Letter";
        const string caneKey = "Inspect_" + prefix + "_Cane";
        const string endKey = "Inspect_" + prefix + "_End";

        dialogueManager->LoadDialogues(startKey, startPath);
        dialogueManager->LoadDialogues(letterKey, letterPath);
        dialogueManager->LoadDialogues(caneKey, canePath);
        dialogueManager->LoadDialogues(endKey, endPath);

        inspectStartDialogueKey = startKey;
        inspectLetterDialogueKey = letterKey;
        inspectCaneDialogueKey = caneKey;
        inspectEndDialogueKey = endKey;

        assert(!inspectStartDialogueKey.empty() && "SetInteractDialogue: inspectStartDialogueKey not set");
        assert(!inspectEndDialogueKey.empty() && "SetInteractDialogue: inspectEndDialogueKey not set");
    }

    // -----------------------------------------------------------------
    // Inspection
    // -----------------------------------------------------------------

    /// @brief Returns true when both cane and letter have been inspected.
    bool AllObjectsInspected() const override
    {
        return isCaneInspected && isLetterInspected;
    }

    /// @brief Drives the cane/letter inspection loop each frame.
    /// @pre   cane, letter, caneInspect, letterInspect are non-null.
    void ManageInspectionState() override
    {
        assert(cane != nullptr && "ManageInspectionState: cane is null");
        assert(letter != nullptr && "ManageInspectionState: letter is null");
        assert(caneInspect != nullptr && "ManageInspectionState: caneInspect is null");
        assert(letterInspect != nullptr && "ManageInspectionState: letterInspect is null");

        if (AllObjectsInspected())
            return;

        HandleCaneDialogue();
        HandleLetterDialogue();
        SyncNormalObjects();
        SetInspectionObjectState();
    }

    // -----------------------------------------------------------------
    // Private inspection helpers
    // -----------------------------------------------------------------

private:
    /// @brief Mirrors visibility: show normal object when inspect button hidden.
    void SyncNormalObjects()
    {
        cane->setActiveStatus(!caneInspect->getActiveStatus());
        letter->setActiveStatus(!letterInspect->getActiveStatus());
    }

    /// @brief Shows or hides inspect buttons based on whether an object
    ///        is currently being inspected.
    void SetInspectionObjectState()
    {
        if (!inspectingObject.empty())
        {
            SetInstruction("Press [Space] or  [Mouse] to continue");
            caneInspect->setActiveStatus(false);
            letterInspect->setActiveStatus(false);
            objectsParallaxManager->DisableParallaxEffect();
            dialogueManager->SetDialogueVisibility(true);
        }
        else
        {
            SetInstruction("Inspect an object  to start conversation");
            ShowInstruction();
            caneInspect->setActiveStatus(!isCaneInspected);
            letterInspect->setActiveStatus(!isLetterInspected);
            objectsParallaxManager->EnableParallaxEffect();
            dialogueManager->SetDialogueVisibility(false);
        }
    }

    /// @brief Starts and monitors the cane inspection dialogue.
    void HandleCaneDialogue()
    {
        if (!inspectCaneDialogueSet && inspectingObject == "Cane")
        {
            dialogueManager->SetDialogueSet(inspectCaneDialogueKey);
            inspectCaneDialogueSet = true;
        }

        if (inspectCaneDialogueSet && dialogueManager->IsDialogueFinished(inspectCaneDialogueKey))
        {
            inspectCaneDialogueSet = false;
            inspectingObject.clear();
            isCaneInspected = true;
            CheckForEndDialogue();
        }
    }

    /// @brief Starts and monitors the letter inspection dialogue.
    void HandleLetterDialogue()
    {
        if (!inspectLetterDialogueSet && inspectingObject == "Letter")
        {
            dialogueManager->SetDialogueSet(inspectLetterDialogueKey);
            inspectLetterDialogueSet = true;
        }

        if (inspectLetterDialogueSet && dialogueManager->IsDialogueFinished(inspectLetterDialogueKey))
        {
            inspectLetterDialogueSet = false;
            inspectingObject.clear();
            isLetterInspected = true;
            CheckForEndDialogue();
        }
    }

    /// @brief Transitions to InspectionEnd when all objects are inspected.
    void CheckForEndDialogue()
    {
        if (AllObjectsInspected() && !inspectEndDialogueSet)
        {
            gameStateManager.SetRoomState(RoomState::InspectionEnd);
        }
    }

    // -----------------------------------------------------------------
    // Martha-specific members
    // -----------------------------------------------------------------

    // Inspectable objects
    UIElement *cane = nullptr;
    UIButton *caneInspect = nullptr;
    UIElement *letter = nullptr;
    UIButton *letterInspect = nullptr;

    // Speaker sprite reference (needed for parallax)
    UIElement *marthaNormal = nullptr;

    // Sprite/icon maps for DialogueManager setup
    map<string, UIElement *> speakerSprites;
    map<string, UIElement *> speakerIcons;

    // Inspection state
    string inspectingObject;
    string inspectCaneDialogueKey;
    string inspectLetterDialogueKey;
    bool inspectCaneDialogueSet = false;
    bool inspectLetterDialogueSet = false;
    bool isCaneInspected = false;
    bool isLetterInspected = false;
};