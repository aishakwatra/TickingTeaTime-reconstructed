// Room4.h
#pragma once

#include <map>
#include <string>
#include <cassert>
#include "RoomScene.h"

/// @brief Room scene for Cabin 4 — passenger Octavia Sinclair.
///
/// Invariants:
///   - poster and dagger must be valid before ManageInspectionState()
///   - inspectingObject is either empty or one of {"Poster", "Dagger"}
///   - isPosterInspected and isDaggerInspected are false
///     until each is completed
class Room4 : public RoomScene
{

public:
    /// @brief Constructs Room4, sets up all Octavia-specific assets.
    /// @post  All GameObjects pushed to m_gameObjects.
    ///        dialogueManager is initialised and ready.
    Room4() : RoomScene("Room4Door")
    {

        objectsParallaxManager = make_unique<ObjectsParallax>();

        SetupRoomObjects();

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

        assert(dialogueManager != nullptr && "Room4: dialogueManager failed to initialise");
        assert(poster != nullptr && "Room4: poster is null");
        assert(dagger != nullptr && "Room4: dagger is null");
        assert(posterInspect != nullptr && "Room4: posterInspect is null");
        assert(daggerInspect != nullptr && "Room4: daggerInspect is null");
    }

protected:
    // -----------------------------------------------------------------
    // Identity
    // -----------------------------------------------------------------

    Cabin GetCabin() const override
    {
        return Cabin::CABIN4;
    }

    GameState GetRoomGameState() const override
    {
        return GameState::ROOM4_STATE;
    }

    // -----------------------------------------------------------------
    // Room-specific setup
    // -----------------------------------------------------------------

    /// @brief Creates all Octavia-specific visual objects and parallax layers.
    /// @post  poster, dagger and their inspect buttons are non-null.
    ///        All objects pushed to m_gameObjects.
    void SetupRoomObjects() override
    {
        const float sm = 1.23f;

        UIElement *room = new UINormal("Cabin4", "Assets/Images/Octavia/Octavia_Cabin.png",
            glm::vec3(0.0f, 0.15f, 0.0f), glm::vec3(19.0f * sm, 10.55f * sm, 0.0f), true);

        UIElement *bag = new UINormal("Bag", "Assets/Images/Octavia/Octavia_Bag.png", glm::vec3(-1.7f, -4.75f, 0.0f),
            glm::vec3(3.75f * sm, 2.7f * sm, 0.0f), true);

        UIElement *lamp = new UINormal("Lamp", "Assets/Images/Octavia/Octavia_Lamp.png",
            glm::vec3(-1.65f, -0.19f, 0.0f), glm::vec3(1.25f * sm, 2.22f * sm, 0.0f), true);

        // Inspectable objects
        poster = new UINormal("OperaPoster", "Assets/Images/Octavia/Octavia_Inspection_Poster.png",
            glm::vec3(0.42f, 2.1f, 0.0f), glm::vec3(5.88f / 4 * sm, 7.4f / 4 * sm, 0.0f), true);

        dagger = new UINormal("Dagger", "Assets/Images/Octavia/Octavia_Inspection_Dagger.png",
            glm::vec3(-5.925f, 3.09f, 0.0f), glm::vec3(2.72f / 4 * sm, 4.44f / 4 * sm, 0.0f), true);

        // Inspect buttons
        posterInspect = new UIButton("OperaPosterInspect", "Assets/Images/Octavia/Octavia_Inspection_Poster.png",
            glm::vec3(0.42f, 2.1f, 0.0f), glm::vec3(5.88f / 4 * sm, 7.4f / 4 * sm, 0.0f), true, false, "");
        posterInspect->SetHoverTexture("Assets/Images/Octavia/Octavia_Inspection_Poster_Highlight.png");
        posterInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "posterInspect clicked while inspectingObject already set");
                inspectingObject = "Poster";
            });

        daggerInspect = new UIButton("DaggerInspect", "Assets/Images/Octavia/Octavia_Inspection_Dagger.png",
            glm::vec3(-5.925f, 3.09f, 0.0f), glm::vec3(2.72f / 4 * sm, 4.44f / 4 * sm, 0.0f), true, false, "");
        daggerInspect->SetHoverTexture("Assets/Images/Octavia/Octavia_Inspection_Dagger_Highlight.png");
        daggerInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "daggerInspect clicked while inspectingObject already set");
                inspectingObject = "Dagger";
            });

        posterInspect->setActiveStatus(false);
        daggerInspect->setActiveStatus(false);

        // Speaker sprites — Octavia has an extra Happy2 sprite
        UIElement *octaviaNormal = new UINormal("Octavia_Normal", "Assets/Images/Octavia/Octavia_Normal.png",
            glm::vec3(4.4f, -1.55f, 0.0f), glm::vec3(6.05f * sm, 7.76f * sm, 0.0f), true);

        UIElement *octaviaHappy = new UINormal("Octavia_Happy", "Assets/Images/Octavia/Octavia_Happy.png",
            glm::vec3(4.4f, -1.55f, 0.0f), glm::vec3(6.05f * sm, 7.76f * sm, 0.0f), true);

        UIElement *octaviaHappy2 = new UINormal("Octavia_Happy2", "Assets/Images/Octavia/Octavia_Happy2.png",
            glm::vec3(4.4f, -1.55f, 0.0f), glm::vec3(6.05f * sm, 7.76f * sm, 0.0f), true);

        UIElement *octaviaDisappoint =
            new UINormal("Octavia_Disappoint", "Assets/Images/Octavia/Octavia_Disappoint.png",
                glm::vec3(4.4f, -1.55f, 0.0f), glm::vec3(6.05f * sm, 7.76f * sm, 0.0f), true);

        UIElement *octaviaUpset = new UINormal("Octavia_Upset", "Assets/Images/Octavia/Octavia_Upset.png",
            glm::vec3(4.4f, -1.55f, 0.0f), glm::vec3(6.05f * sm, 7.76f * sm, 0.0f), true);

        UIElement *octaviaSad = new UINormal("Octavia_Sad", "Assets/Images/Octavia/Octavia_Sad.png",
            glm::vec3(4.4f, -1.55f, 0.0f), glm::vec3(6.05f * sm, 7.76f * sm, 0.0f), true);

        UIElement *octaviaSurprise = new UINormal("Octavia_Surprise", "Assets/Images/Octavia/Octavia_Surprise.png",
            glm::vec3(4.4f, -1.55f, 0.0f), glm::vec3(6.05f * sm, 7.76f * sm, 0.0f), true);

        // Speaker icons
        UIElement *octaviaIcon = new UINormal("OctaviaIcon", "Assets/Images/UI/Speaker_icon_Octavia.png",
            glm::vec3(4.18f, 3.5f, 0.0f), glm::vec3(1.4f, 1.57f, 0.0f), true);

        UIElement *waiterIcon = new UINormal("WaiterIcon", "Assets/Images/UI/Speaker_icon_Waiter.png",
            glm::vec3(4.18f, 3.43f, 0.0f), glm::vec3(1.23f, 1.4f, 0.0f), true);

        speakerSprites = {
            { "Octavia_Normal", octaviaNormal },
            { "Octavia_Happy", octaviaHappy },
            { "Octavia_Happy2", octaviaHappy2 },
            { "Octavia_Disappoint", octaviaDisappoint },
            { "Octavia_Upset", octaviaUpset },
            { "Octavia_Sad", octaviaSad },
            { "Octavia_Surprise", octaviaSurprise },
        };

        speakerIcons = {
            { "O", octaviaIcon },
            { "W", waiterIcon },
        };

        // Parallax layers
        const float defaultLayer = 0.8f;
        const float objectLayer = 0.85f;

        objectsParallaxManager->AddObjectToLayer(room, defaultLayer);
        objectsParallaxManager->AddObjectToLayer(dagger, objectLayer);
        objectsParallaxManager->AddObjectToLayer(daggerInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(bag, objectLayer);
        objectsParallaxManager->AddObjectToLayer(lamp, objectLayer);
        objectsParallaxManager->AddObjectToLayer(poster, objectLayer);
        objectsParallaxManager->AddObjectToLayer(posterInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(octaviaNormal, objectLayer);
        objectsParallaxManager->AddObjectToLayer(octaviaHappy, objectLayer);
        objectsParallaxManager->AddObjectToLayer(octaviaHappy2, objectLayer);
        objectsParallaxManager->AddObjectToLayer(octaviaDisappoint, objectLayer);
        objectsParallaxManager->AddObjectToLayer(octaviaUpset, objectLayer);
        objectsParallaxManager->AddObjectToLayer(octaviaSad, objectLayer);
        objectsParallaxManager->AddObjectToLayer(octaviaSurprise, objectLayer);

        // Push to scene
        m_gameObjects.push_back(room);
        m_gameObjects.push_back(bag);
        m_gameObjects.push_back(dagger);
        m_gameObjects.push_back(daggerInspect);
        m_gameObjects.push_back(poster);
        m_gameObjects.push_back(posterInspect);
        m_gameObjects.push_back(lamp);
        m_gameObjects.push_back(octaviaNormal);
        m_gameObjects.push_back(octaviaHappy);
        m_gameObjects.push_back(octaviaHappy2);
        m_gameObjects.push_back(octaviaDisappoint);
        m_gameObjects.push_back(octaviaUpset);
        m_gameObjects.push_back(octaviaSad);
        m_gameObjects.push_back(octaviaSurprise);
    }

    /// @brief Initialises the DialogueManager with Octavia's sprites.
    /// @pre   dialogueBox, choice1, choice2, choice3 must be non-null.
    /// @post  dialogueManager is non-null with Order/Serve dialogues loaded.
    void SetupDialogueManager(UIButton *dialogueBox, UIButton *choice1, UIButton *choice2, UIButton *choice3) override
    {
        assert(dialogueBox != nullptr && "SetupDialogueManager: dialogueBox is null");
        assert(choice1 != nullptr && choice2 != nullptr && choice3 != nullptr &&
               "SetupDialogueManager: choice buttons are null");

        dialogueManager = make_unique<DialogueManager>("OctaviaDialogue", dialogueBox, "Octavia_Normal");

        dialogueManager->LoadDialogues("Order", "Assets/Dialogue/Octavia/Octavia_Order.xml");
        dialogueManager->LoadDialogues("Serve_OnTime", "Assets/Dialogue/Octavia/Octavia_Serve_OnTime.xml");
        dialogueManager->LoadDialogues("Serve_Late", "Assets/Dialogue/Octavia/Octavia_Serve_Late.xml");

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
    // Dialogue loading — Octavia specific
    // -----------------------------------------------------------------

    /// @brief Loads Octavia's meal reaction dialogues based on kitchen choices.
    /// @note  Fixes a bug in the original where chamomile always loaded
    ///        the non-milk version regardless of optional choice.
    /// @post  teaDialogueKey, sandwichDialogueKey, dessertDialogueKey are
    ///        set and non-empty.
    void SetMealReactionDialogue() override
    {
        KitchenData *kitchen = KitchenData::GetInstance();
        assert(kitchen != nullptr && "SetMealReactionDialogue: KitchenData is null");

        // Tea
        switch (kitchen->getTea())
        {
        case EARLGREYTEA:
            dialogueManager->LoadDialogues("MealReact_Tea_EarlGrey", "Assets/Dialogue/Octavia/MealReact/"
                                                                     "Octavia_MealReact_Tea_EarlGrey_Assam.xml");
            teaDialogueKey = "MealReact_Tea_EarlGrey";
            break;
        case ASSAMTEA:
            dialogueManager->LoadDialogues("MealReact_Tea_Assam", "Assets/Dialogue/Octavia/MealReact/"
                                                                  "Octavia_MealReact_Tea_EarlGrey_Assam.xml");
            teaDialogueKey = "MealReact_Tea_Assam";
            break;
        case CHAMOMILETEA:
            // BUG FIX: original always fell through to non-milk version
            if (kitchen->getOptional() == MILK)
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Charmomile_WithMilk",
                    "Assets/Dialogue/Octavia/MealReact/"
                    "Octavia_MealReact_Tea_Chamomile_WithMilk.xml");
                teaDialogueKey = "MealReact_Tea_Charmomile_WithMilk";
            }
            else
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Charmomile", "Assets/Dialogue/Octavia/MealReact/"
                                                                           "Octavia_MealReact_Tea_Chamomile.xml");
                teaDialogueKey = "MealReact_Tea_Charmomile";
            }
            break;
        case GREENTEA:
            dialogueManager->LoadDialogues("MealReact_Tea_GreenTea", "Assets/Dialogue/Octavia/MealReact/"
                                                                     "Octavia_MealReact_Tea_GreenTea.xml");
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
            dialogueManager->LoadDialogues("MealReact_Sandwich_Salmon", "Assets/Dialogue/Octavia/MealReact/"
                                                                        "Octavia_MealReact_Sandwich_Salmon_Beef.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Salmon";
            break;
        case BEEF:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Beef", "Assets/Dialogue/Octavia/MealReact/"
                                                                      "Octavia_MealReact_Sandwich_Salmon_Beef.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Beef";
            break;
        case EGG:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Egg", "Assets/Dialogue/Octavia/MealReact/"
                                                                     "Octavia_MealReact_Sandwich_Egg.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Egg";
            break;
        case CUCUMBER:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Cucumber", "Assets/Dialogue/Octavia/MealReact/"
                                                                          "Octavia_MealReact_Sandwich_Cucumber.xml");
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
            dialogueManager->LoadDialogues("MealReact_Dessert_Eclair", "Assets/Dialogue/Octavia/MealReact/"
                                                                       "Octavia_MealReact_Pastry_Macaron_Eclair.xml");
            dessertDialogueKey = "MealReact_Dessert_Eclair";
            break;
        case MACARON:
            dialogueManager->LoadDialogues("MealReact_Dessert_Macaron", "Assets/Dialogue/Octavia/MealReact/"
                                                                        "Octavia_MealReact_Pastry_Macaron_Eclair.xml");
            dessertDialogueKey = "MealReact_Dessert_Macaron";
            break;
        case TART:
            dialogueManager->LoadDialogues("MealReact_Dessert_Tart", "Assets/Dialogue/Octavia/MealReact/"
                                                                     "Octavia_MealReact_Pastry_Tart.xml");
            dessertDialogueKey = "MealReact_Dessert_Tart";
            break;
        case SCONE:
            dialogueManager->LoadDialogues("MealReact_Dessert_Scone", "Assets/Dialogue/Octavia/MealReact/"
                                                                      "Octavia_MealReact_Pastry_Scone.xml");
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

    /// @brief Loads Octavia's score dialogue based on serve score level.
    /// @post  scoreDialogueKey is set and non-empty.
    void SetScoredDialogue() override
    {
        InteractionLevel level = characterData->getServeScoreLevel(GetCabin());

        switch (level)
        {
        case LOW:
            dialogueManager->LoadDialogues("Score_Low", "Assets/Dialogue/Octavia/MealResult/"
                                                        "Octavia_MealResult_Low.xml");
            scoreDialogueKey = "Score_Low";
            break;
        case AVERAGE:
            dialogueManager->LoadDialogues("Score_Average", "Assets/Dialogue/Octavia/MealResult/"
                                                            "Octavia_MealResult_Average.xml");
            scoreDialogueKey = "Score_Average";
            break;
        case INFORMATIVE:
            dialogueManager->LoadDialogues("Score_Informative", "Assets/Dialogue/Octavia/MealResult/"
                                                                "Octavia_MealResult_Informative.xml");
            scoreDialogueKey = "Score_Informative";
            break;
        default:
            assert(false && "SetScoredDialogue: unknown interaction level");
            break;
        }

        assert(!scoreDialogueKey.empty() && "SetScoredDialogue: scoreDialogueKey not set");
    }

    /// @brief Loads Octavia's inspection dialogues based on interaction level.
    /// @post  All inspect dialogue keys are set and non-empty.
    void SetInteractDialogue() override
    {
        InteractionLevel level = characterData->getInteractionLevel(GetCabin());

        string prefix;
        string startPath;
        string posterPath;
        string daggerPath;
        string endPath;

        switch (level)
        {
        case LOW:
            prefix = "Low";
            startPath = "Assets/Dialogue/Octavia/Low/Octavia_Low_Start.xml";
            posterPath = "Assets/Dialogue/Octavia/Low/Octavia_Low_Poster.xml";
            daggerPath = "Assets/Dialogue/Octavia/Low/Octavia_Low_Dagger.xml";
            endPath = "Assets/Dialogue/Octavia/Low/Octavia_Low_End.xml";
            break;
        case AVERAGE:
            prefix = "Average";
            startPath = "Assets/Dialogue/Octavia/Average/"
                        "Octavia_Average_Start.xml";
            posterPath = "Assets/Dialogue/Octavia/Average/"
                         "Octavia_Average_Poster.xml";
            daggerPath = "Assets/Dialogue/Octavia/Average/"
                         "Octavia_Average_Dagger.xml";
            endPath = "Assets/Dialogue/Octavia/Average/"
                      "Octavia_Average_End.xml";
            break;
        case INFORMATIVE:
            prefix = "Informative";
            startPath = "Assets/Dialogue/Octavia/Informative/"
                        "Octavia_Informative_Start.xml";
            posterPath = "Assets/Dialogue/Octavia/Informative/"
                         "Octavia_Informative_Poster.xml";
            daggerPath = "Assets/Dialogue/Octavia/Informative/"
                         "Octavia_Informative_Dagger.xml";
            endPath = "Assets/Dialogue/Octavia/Informative/"
                      "Octavia_Informative_End.xml";
            break;
        default:
            assert(false && "SetInteractDialogue: unknown interaction level");
            return;
        }

        const string startKey = "Inspect_" + prefix + "_Start";
        const string posterKey = "Inspect_" + prefix + "_Poster";
        const string daggerKey = "Inspect_" + prefix + "_Dagger";
        const string endKey = "Inspect_" + prefix + "_End";

        dialogueManager->LoadDialogues(startKey, startPath);
        dialogueManager->LoadDialogues(posterKey, posterPath);
        dialogueManager->LoadDialogues(daggerKey, daggerPath);
        dialogueManager->LoadDialogues(endKey, endPath);

        inspectStartDialogueKey = startKey;
        inspectPosterDialogueKey = posterKey;
        inspectDaggerDialogueKey = daggerKey;
        inspectEndDialogueKey = endKey;

        assert(!inspectStartDialogueKey.empty() && "SetInteractDialogue: inspectStartDialogueKey not set");
        assert(!inspectEndDialogueKey.empty() && "SetInteractDialogue: inspectEndDialogueKey not set");
    }

    // -----------------------------------------------------------------
    // Inspection
    // -----------------------------------------------------------------

    /// @brief Returns true when both poster and dagger have been inspected.
    bool AllObjectsInspected() const override
    {
        return isPosterInspected && isDaggerInspected;
    }

    /// @brief Drives the poster/dagger inspection loop each frame.
    /// @pre   poster, dagger, posterInspect, daggerInspect are non-null.
    void ManageInspectionState() override
    {
        assert(poster != nullptr && "ManageInspectionState: poster is null");
        assert(dagger != nullptr && "ManageInspectionState: dagger is null");
        assert(posterInspect != nullptr && "ManageInspectionState: posterInspect is null");
        assert(daggerInspect != nullptr && "ManageInspectionState: daggerInspect is null");

        if (AllObjectsInspected())
            return;

        HandlePosterDialogue();
        HandleDaggerDialogue();
        SyncNormalObjects();
        SetInspectionObjectState();
    }

private:
    /// @brief Mirrors visibility of normal objects vs inspect buttons.
    void SyncNormalObjects()
    {
        poster->setActiveStatus(!posterInspect->getActiveStatus());
        dagger->setActiveStatus(!daggerInspect->getActiveStatus());
    }

    /// @brief Shows or hides inspect buttons based on current inspection state.
    void SetInspectionObjectState()
    {
        if (!inspectingObject.empty())
        {
            SetInstruction("Press [Space] or  [Mouse] to continue");
            posterInspect->setActiveStatus(false);
            daggerInspect->setActiveStatus(false);
            objectsParallaxManager->DisableParallaxEffect();
            dialogueManager->SetDialogueVisibility(true);
        }
        else
        {
            SetInstruction("Inspect an object  to start conversation");
            ShowInstruction();
            posterInspect->setActiveStatus(!isPosterInspected);
            daggerInspect->setActiveStatus(!isDaggerInspected);
            objectsParallaxManager->EnableParallaxEffect();
            dialogueManager->SetDialogueVisibility(false);
        }
    }

    /// @brief Starts and monitors the poster inspection dialogue.
    void HandlePosterDialogue()
    {
        if (!inspectPosterDialogueSet && inspectingObject == "Poster")
        {
            dialogueManager->SetDialogueSet(inspectPosterDialogueKey);
            inspectPosterDialogueSet = true;
        }
        if (inspectPosterDialogueSet && dialogueManager->IsDialogueFinished(inspectPosterDialogueKey))
        {
            inspectPosterDialogueSet = false;
            inspectingObject.clear();
            isPosterInspected = true;
            CheckForEndDialogue();
        }
    }

    /// @brief Starts and monitors the dagger inspection dialogue.
    void HandleDaggerDialogue()
    {
        if (!inspectDaggerDialogueSet && inspectingObject == "Dagger")
        {
            dialogueManager->SetDialogueSet(inspectDaggerDialogueKey);
            inspectDaggerDialogueSet = true;
        }
        if (inspectDaggerDialogueSet && dialogueManager->IsDialogueFinished(inspectDaggerDialogueKey))
        {
            inspectDaggerDialogueSet = false;
            inspectingObject.clear();
            isDaggerInspected = true;
            CheckForEndDialogue();
        }
    }

    /// @brief Transitions to InspectionEnd when both objects are inspected.
    void CheckForEndDialogue()
    {
        if (AllObjectsInspected() && !inspectEndDialogueSet)
        {
            gameStateManager.SetRoomState(RoomState::InspectionEnd);
        }
    }

    // -----------------------------------------------------------------
    // Octavia-specific members
    // -----------------------------------------------------------------

    // Inspectable objects
    UIElement *poster = nullptr;
    UIButton *posterInspect = nullptr;
    UIElement *dagger = nullptr;
    UIButton *daggerInspect = nullptr;

    // Sprite/icon maps for DialogueManager setup
    map<string, UIElement *> speakerSprites;
    map<string, UIElement *> speakerIcons;

    // Inspection state
    string inspectingObject;
    string inspectPosterDialogueKey;
    string inspectDaggerDialogueKey;
    bool inspectPosterDialogueSet = false;
    bool inspectDaggerDialogueSet = false;
    bool isPosterInspected = false;
    bool isDaggerInspected = false;
};