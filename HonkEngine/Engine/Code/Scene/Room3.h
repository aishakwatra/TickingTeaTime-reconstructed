// Room3.h
#pragma once

#include <map>
#include <string>
#include <cassert>
#include "RoomScene.h"
#include "../Effects/ShakingEffect.h"

/// @brief Room scene for Cabin 3 — passenger Archibald Pemberton.
///
/// Invariants:
///   - movingLuggage, newspaper, messyClothesInspect must be valid
///     before ManageInspectionState() is called
///   - inspectingObject is either empty or one of
///     {"MovingLuggage", "Newspaper", "MessyClothes"}
///   - shakingEffect must be non-null before OnInspectionStart() is called
///   - isMovingLuggageInspected, isNewspaperInspected,
///     isMessyClothesInspected are false until each is completed
class Room3 : public RoomScene
{

public:
    /// @brief Constructs Room3, sets up all Archibald-specific assets.
    /// @post  All GameObjects pushed to m_gameObjects.
    ///        dialogueManager and shakingEffect are initialised and ready.
    Room3() : RoomScene("Room3Door")
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

        // Push icons AFTER dialogue box so they render on top
        for (auto &pair : speakerIcons)
        {
            m_gameObjects.push_back(pair.second);
        }

        m_gameObjects.push_back(instructionText);
        m_gameObjects.push_back(&pauseMenu);
        m_gameObjects.push_back(transitionObject);

        assert(dialogueManager != nullptr && "Room3: dialogueManager failed to initialise");
        assert(movingLuggage != nullptr && "Room3: movingLuggage is null");
        assert(newspaper != nullptr && "Room3: newspaper is null");
        assert(messyClothesInspect != nullptr && "Room3: messyClothesInspect is null");
        assert(shakingEffect != nullptr && "Room3: shakingEffect is null");
    }

protected:
    // -----------------------------------------------------------------
    // Identity
    // -----------------------------------------------------------------

    Cabin GetCabin() const override
    {
        return Cabin::CABIN3;
    }

    GameState GetRoomGameState() const override
    {
        return GameState::ROOM3_STATE;
    }

    void OnPreStateUpdate(float dt) override
    {
        currentDeltaTime = dt;
    }

    // -----------------------------------------------------------------
    // Room-specific setup
    // -----------------------------------------------------------------

    /// @brief Creates all Archibald-specific visual objects, parallax layers,
    ///        and the shaking effect on the moving luggage.
    /// @post  movingLuggage, newspaper, messyClothesInspect and their inspect
    ///        buttons are non-null. shakingEffect is initialised.
    ///        All objects pushed to m_gameObjects.
    void SetupRoomObjects() override
    {
        const float sm = 1.23f;

        UIElement *room = new UINormal("cabin3", "Assets/Images/Archibald/Archibald_Cabin.png",
            glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(19.0f * sm, 10.55f * sm, 0.0f), true);

        UIElement *lamp = new UINormal("Lamp", "Assets/Images/Archibald/Archibald_Lamp.png",
            glm::vec3(1.3f, -0.3f, 0.0f), glm::vec3(0.84f * sm, 1.94f * sm, 0.0f), true);

        UIElement *luggage = new UINormal("Luggage", "Assets/Images/Archibald/Archibald_Bag.png",
            glm::vec3(6.2f, -1.67f, 0.0f), glm::vec3(1.59f * sm * 1.2f, 2.32f * sm * 1.2f, 0.0f), true);

        // Inspectable objects
        movingLuggage = new UINormal("MovingLuggage", "Assets/Images/Archibald/Archibald_Inspection_MovingLuggage.png",
            glm::vec3(1.7f, -4.45f, 0.0f), glm::vec3(3.78f * sm * 1.2f, 2.38f * sm * 1.2f, 0.0f), true);

        newspaper = new UINormal("Newspaper", "Assets/Images/Archibald/Archibald_Inspection_Newspaper.png",
            glm::vec3(-3.139f, 0.415f, 0.0f), glm::vec3(1.09f * sm, 2.22f * sm, 0.0f), true);

        // Inspect buttons
        movingLuggageInspect =
            new UIButton("MovingLuggageInspect", "Assets/Images/Archibald/Archibald_Inspection_MovingLuggage.png",
                glm::vec3(1.7f, -4.45f, 0.0f), glm::vec3(3.78f * sm * 1.2f, 2.38f * sm * 1.2f, 0.0f), true, false, "");
        movingLuggageInspect->SetHoverTexture("Assets/Images/Archibald/"
                                              "Archibald_Inspection_MovingLuggage_Highlight.png");
        movingLuggageInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "movingLuggageInspect clicked while "
                                                   "inspectingObject already set");
                inspectingObject = "MovingLuggage";
            });

        newspaperInspect =
            new UIButton("NewspaperInspect", "Assets/Images/Archibald/Archibald_Inspection_Newspaper.png",
                glm::vec3(-3.139f, 0.415f, 0.0f), glm::vec3(1.09f * sm, 2.22f * sm, 0.0f), true, false, "");
        newspaperInspect->SetHoverTexture("Assets/Images/Archibald/"
                                          "Archibald_Inspection_Newspaper_Highlight.png");
        newspaperInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "newspaperInspect clicked while "
                                                   "inspectingObject already set");
                inspectingObject = "Newspaper";
            });

        // Messy clothes — inspect button only (no normal object counterpart)
        messyClothesInspect =
            new UIButton("MessyClothesInspect", "Assets/Images/Archibald/Archibald_Inspection_MessyClothes.png",
                glm::vec3(-3.58f, -1.8f, 0.0f), glm::vec3(6.6f * sm, 7.6f * sm, 0.0f), true, false, "");
        messyClothesInspect->SetHoverTexture("Assets/Images/Archibald/"
                                             "Archibald_Inspection_MessyClothes_Highlight.png");
        messyClothesInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "messyClothesInspect clicked while "
                                                   "inspectingObject already set");
                inspectingObject = "MessyClothes";
            });

        movingLuggageInspect->setActiveStatus(false);
        newspaperInspect->setActiveStatus(false);
        messyClothesInspect->setActiveStatus(false);

        // Speaker sprites
        UIElement *archibaldNormal = new UINormal("Archibald_Normal", "Assets/Images/Archibald/Archibald_Normal.png",
            glm::vec3(-3.58f, -1.8f, 0.0f), glm::vec3(6.46f * sm, 7.32f * sm, 0.0f), true);

        UIElement *archibaldHappy = new UINormal("Archibald_Happy", "Assets/Images/Archibald/Archibald_Happy.png",
            glm::vec3(-3.58f, -1.8f, 0.0f), glm::vec3(6.46f * sm, 7.32f * sm, 0.0f), true);

        UIElement *archibaldDisappoint =
            new UINormal("Archibald_Disappoint", "Assets/Images/Archibald/Archibald_Disappoint.png",
                glm::vec3(-3.58f, -1.8f, 0.0f), glm::vec3(6.46f * sm, 7.32f * sm, 0.0f), true);

        UIElement *archibaldUpset = new UINormal("Archibald_Upset", "Assets/Images/Archibald/Archibald_Upset.png",
            glm::vec3(-3.58f, -1.8f, 0.0f), glm::vec3(6.46f * sm, 7.32f * sm, 0.0f), true);

        UIElement *archibaldSad = new UINormal("Archibald_Sad", "Assets/Images/Archibald/Archibald_Sad.png",
            glm::vec3(-3.58f, -1.8f, 0.0f), glm::vec3(6.46f * sm, 7.32f * sm, 0.0f), true);

        UIElement *archibaldShy = new UINormal("Archibald_Shy", "Assets/Images/Archibald/Archibald_Shy.png",
            glm::vec3(-3.58f, -1.8f, 0.0f), glm::vec3(6.46f * sm, 7.32f * sm, 0.0f), true);

        // Speaker icons
        UIElement *archibaldIcon = new UINormal("ArchibaldIcon", "Assets/Images/UI/Speaker_icon_Archibald.png",
            glm::vec3(4.18f, 3.5f, 0.0f), glm::vec3(1.4f, 1.57f, 0.0f), true);

        UIElement *waiterIcon = new UINormal("WaiterIcon", "Assets/Images/UI/Speaker_icon_Waiter.png",
            glm::vec3(4.18f, 3.43f, 0.0f), glm::vec3(1.23f, 1.4f, 0.0f), true);

        speakerSprites = {
            { "Archibald_Normal", archibaldNormal },
            { "Archibald_Happy", archibaldHappy },
            { "Archibald_Disappoint", archibaldDisappoint },
            { "Archibald_Upset", archibaldUpset },
            { "Archibald_Sad", archibaldSad },
            { "Archibald_Shy", archibaldShy },
        };

        speakerIcons = {
            { "A", archibaldIcon },
            { "W", waiterIcon },
        };

        // Shaking effect on moving luggage only
        shakingEffect = make_unique<ShakingEffect>(0.06f, 100, 220, 3000);
        shakingEffect->AddObject(movingLuggage);
        shakingEffect->AddObject(movingLuggageInspect);

        // Parallax layers
        const float defaultLayer = 0.8f;
        const float objectLayer = 0.85f;

        objectsParallaxManager->AddObjectToLayer(room, objectLayer);
        objectsParallaxManager->AddObjectToLayer(lamp, objectLayer);
        objectsParallaxManager->AddObjectToLayer(luggage, objectLayer);
        objectsParallaxManager->AddObjectToLayer(movingLuggage, objectLayer);
        objectsParallaxManager->AddObjectToLayer(movingLuggageInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(newspaper, objectLayer);
        objectsParallaxManager->AddObjectToLayer(newspaperInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(messyClothesInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(archibaldNormal, objectLayer);
        objectsParallaxManager->AddObjectToLayer(archibaldHappy, objectLayer);
        objectsParallaxManager->AddObjectToLayer(archibaldDisappoint, objectLayer);
        objectsParallaxManager->AddObjectToLayer(archibaldUpset, objectLayer);
        objectsParallaxManager->AddObjectToLayer(archibaldSad, objectLayer);
        objectsParallaxManager->AddObjectToLayer(archibaldShy, objectLayer);

        // Push to scene
        m_gameObjects.push_back(room);
        m_gameObjects.push_back(movingLuggage);
        m_gameObjects.push_back(movingLuggageInspect);
        m_gameObjects.push_back(messyClothesInspect);
        m_gameObjects.push_back(archibaldNormal);
        m_gameObjects.push_back(archibaldHappy);
        m_gameObjects.push_back(archibaldDisappoint);
        m_gameObjects.push_back(archibaldUpset);
        m_gameObjects.push_back(archibaldSad);
        m_gameObjects.push_back(archibaldShy);
        m_gameObjects.push_back(newspaper);
        m_gameObjects.push_back(newspaperInspect);
        m_gameObjects.push_back(lamp);
        m_gameObjects.push_back(luggage);
    }

    /// @brief Initialises the DialogueManager with Archibald's sprites.
    /// @pre   dialogueBox, choice1, choice2, choice3 must be non-null.
    /// @post  dialogueManager is non-null with Order/Serve dialogues loaded.
    void SetupDialogueManager(UIButton *dialogueBox, UIButton *choice1, UIButton *choice2, UIButton *choice3) override
    {
        assert(dialogueBox != nullptr && "SetupDialogueManager: dialogueBox is null");
        assert(choice1 != nullptr && choice2 != nullptr && choice3 != nullptr &&
               "SetupDialogueManager: choice buttons are null");

        dialogueManager = make_unique<DialogueManager>("ArchibaldDialogue", dialogueBox, "Archibald_Normal");

        dialogueManager->LoadDialogues("Order", "Assets/Dialogue/Archibald/Archibald_Order.xml");
        dialogueManager->LoadDialogues("Serve_OnTime", "Assets/Dialogue/Archibald/Archibald_Serve_OnTime.xml");
        dialogueManager->LoadDialogues("Serve_Late", "Assets/Dialogue/Archibald/Archibald_Serve_Late.xml");

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
    // Dialogue loading — Archibald specific
    // -----------------------------------------------------------------

    /// @brief Loads Archibald's meal reaction dialogues based on kitchen choices.
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
            if (kitchen->getOptional() == MILK)
            {
                dialogueManager->LoadDialogues("MealReact_Tea_EarlGrey_WithMilk",
                    "Assets/Dialogue/Archibald/MealReact/"
                    "Archibald_MealReact_Tea_Assam_EarlGrey_WithMilk.xml");
                teaDialogueKey = "MealReact_Tea_EarlGrey_WithMilk";
            }
            else
            {
                dialogueManager->LoadDialogues("MealReact_Tea_EarlGrey", "Assets/Dialogue/Archibald/MealReact/"
                                                                         "Archibald_MealReact_Tea_Assam_EarlGrey.xml");
                teaDialogueKey = "MealReact_Tea_EarlGrey";
            }
            break;
        case ASSAMTEA:
            if (kitchen->getOptional() == MILK)
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Assam_WithMilk",
                    "Assets/Dialogue/Archibald/MealReact/"
                    "Archibald_MealReact_Tea_Assam_EarlGrey_WithMilk.xml");
                teaDialogueKey = "MealReact_Tea_Assam_WithMilk";
            }
            else
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Assam", "Assets/Dialogue/Archibald/MealReact/"
                                                                      "Archibald_MealReact_Tea_Assam_EarlGrey.xml");
                teaDialogueKey = "MealReact_Tea_Assam";
            }
            break;
        case CHAMOMILETEA:
            if (kitchen->getOptional() == MILK)
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Charmomile_WithMilk",
                    "Assets/Dialogue/Archibald/MealReact/"
                    "Archibald_MealReact_Tea_Chamomile_WithMilk.xml");
                teaDialogueKey = "MealReact_Tea_Charmomile_WithMilk";
            }
            else
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Charmomile", "Assets/Dialogue/Archibald/MealReact/"
                                                                           "Archibald_MealReact_Tea_Chamomile.xml");
                teaDialogueKey = "MealReact_Tea_Charmomile";
            }
            break;
        case GREENTEA:
            if (kitchen->getOptional() == MILK)
            {
                dialogueManager->LoadDialogues("MealReact_Tea_GreenTea_WithMilk",
                    "Assets/Dialogue/Archibald/MealReact/"
                    "Archibald_MealReact_Tea_GreenTea_WithMilk.xml");
                teaDialogueKey = "MealReact_Tea_GreenTea_WithMilk";
            }
            else
            {
                dialogueManager->LoadDialogues("MealReact_Tea_GreenTea", "Assets/Dialogue/Archibald/MealReact/"
                                                                         "Archibald_MealReact_Tea_GreenTea.xml");
                teaDialogueKey = "MealReact_Tea_GreenTea";
            }
            break;
        default:
            assert(false && "SetMealReactionDialogue: unknown tea type");
            break;
        }

        // Sandwich
        switch (kitchen->getSandwich())
        {
        case SALMON:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Salmon", "Assets/Dialogue/Archibald/MealReact/"
                                                                        "Archibald_MealReact_Sandwich_Salmon.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Salmon";
            break;
        case BEEF:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Beef", "Assets/Dialogue/Archibald/MealReact/"
                                                                      "Archibald_MealReact_Sandwich_Beef.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Beef";
            break;
        case EGG:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Egg", "Assets/Dialogue/Archibald/MealReact/"
                                                                     "Archibald_MealReact_Sandwich_Egg.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Egg";
            break;
        case CUCUMBER:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Cucumber", "Assets/Dialogue/Archibald/MealReact/"
                                                                          "Archibald_MealReact_Sandwich_Cucumber.xml");
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
            dialogueManager->LoadDialogues("MealReact_Dessert_Eclair", "Assets/Dialogue/Archibald/MealReact/"
                                                                       "Archibald_MealReact_Pastry_Eclair.xml");
            dessertDialogueKey = "MealReact_Dessert_Eclair";
            break;
        case MACARON:
            dialogueManager->LoadDialogues("MealReact_Dessert_Macaron", "Assets/Dialogue/Archibald/MealReact/"
                                                                        "Archibald_MealReact_Pastry_Other.xml");
            dessertDialogueKey = "MealReact_Dessert_Macaron";
            break;
        case TART:
            dialogueManager->LoadDialogues("MealReact_Dessert_Tart", "Assets/Dialogue/Archibald/MealReact/"
                                                                     "Archibald_MealReact_Pastry_LemonTart.xml");
            dessertDialogueKey = "MealReact_Dessert_Tart";
            break;
        case SCONE:
            dialogueManager->LoadDialogues("MealReact_Dessert_Scone", "Assets/Dialogue/Archibald/MealReact/"
                                                                      "Archibald_MealReact_Pastry_Other.xml");
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

    /// @brief Loads Archibald's score dialogue based on serve score level.
    /// @post  scoreDialogueKey is set and non-empty.
    void SetScoredDialogue() override
    {
        InteractionLevel level = characterData->getServeScoreLevel(GetCabin());

        switch (level)
        {
        case LOW:
            dialogueManager->LoadDialogues("Score_Low", "Assets/Dialogue/Archibald/MealResult/"
                                                        "Archibald_MealResult_Low.xml");
            scoreDialogueKey = "Score_Low";
            break;
        case AVERAGE:
            dialogueManager->LoadDialogues("Score_Average", "Assets/Dialogue/Archibald/MealResult/"
                                                            "Archibald_MealResult_Average.xml");
            scoreDialogueKey = "Score_Average";
            break;
        case INFORMATIVE:
            dialogueManager->LoadDialogues("Score_Informative", "Assets/Dialogue/Archibald/MealResult/"
                                                                "Archibald_MealResult_Informative.xml");
            scoreDialogueKey = "Score_Informative";
            break;
        default:
            assert(false && "SetScoredDialogue: unknown interaction level");
            break;
        }

        assert(!scoreDialogueKey.empty() && "SetScoredDialogue: scoreDialogueKey not set");
    }

    /// @brief Loads Archibald's inspection dialogues based on interaction level.
    /// @post  All inspect dialogue keys are set and non-empty.
    void SetInteractDialogue() override
    {
        InteractionLevel level = characterData->getInteractionLevel(GetCabin());

        string prefix;
        string startPath;
        string movingLuggagePath;
        string newspaperPath;
        string messyClothesPath;
        string endPath;

        switch (level)
        {
        case LOW:
            prefix = "Low";
            startPath = "Assets/Dialogue/Archibald/Low/"
                        "Archibald_Low_Start.xml";
            movingLuggagePath = "Assets/Dialogue/Archibald/Low/"
                                "Archibald_Low_MovingLuggage.xml";
            newspaperPath = "Assets/Dialogue/Archibald/Low/"
                            "Archibald_Low_Newspaper.xml";
            messyClothesPath = "Assets/Dialogue/Archibald/Low/"
                               "Archibald_Low_MessyClothes.xml";
            endPath = "Assets/Dialogue/Archibald/Low/"
                      "Archibald_Low_End.xml";
            break;
        case AVERAGE:
            prefix = "Average";
            startPath = "Assets/Dialogue/Archibald/Average/"
                        "Archibald_Average_Start.xml";
            movingLuggagePath = "Assets/Dialogue/Archibald/Average/"
                                "Archibald_Average_MovingLuggage.xml";
            newspaperPath = "Assets/Dialogue/Archibald/Average/"
                            "Archibald_Average_Newspaper.xml";
            messyClothesPath = "Assets/Dialogue/Archibald/Average/"
                               "Archibald_Average_MessyClothes.xml";
            endPath = "Assets/Dialogue/Archibald/Average/"
                      "Archibald_Average_End.xml";
            break;
        case INFORMATIVE:
            prefix = "Informative";
            startPath = "Assets/Dialogue/Archibald/Informative/"
                        "Archibald_Informative_Start.xml";
            movingLuggagePath = "Assets/Dialogue/Archibald/Informative/"
                                "Archibald_Informative_MovingLuggage.xml";
            newspaperPath = "Assets/Dialogue/Archibald/Informative/"
                            "Archibald_Informative_Newspaper.xml";
            messyClothesPath = "Assets/Dialogue/Archibald/Informative/"
                               "Archibald_Informative_MessyClothes.xml";
            endPath = "Assets/Dialogue/Archibald/Informative/"
                      "Archibald_Informative_End.xml";
            break;
        default:
            assert(false && "SetInteractDialogue: unknown interaction level");
            return;
        }

        const string startKey = "Inspect_" + prefix + "_Start";
        const string movingLuggageKey = "Inspect_" + prefix + "_MovingLuggage";
        const string newspaperKey = "Inspect_" + prefix + "_Newspaper";
        const string messyClothesKey = "Inspect_" + prefix + "_MessyClothes";
        const string endKey = "Inspect_" + prefix + "_End";

        dialogueManager->LoadDialogues(startKey, startPath);
        dialogueManager->LoadDialogues(movingLuggageKey, movingLuggagePath);
        dialogueManager->LoadDialogues(newspaperKey, newspaperPath);
        dialogueManager->LoadDialogues(messyClothesKey, messyClothesPath);
        dialogueManager->LoadDialogues(endKey, endPath);

        inspectStartDialogueKey = startKey;
        inspectMovingLuggageDialogueKey = movingLuggageKey;
        inspectNewspaperDialogueKey = newspaperKey;
        inspectMessyClothesDialogueKey = messyClothesKey;
        inspectEndDialogueKey = endKey;

        assert(!inspectStartDialogueKey.empty() && "SetInteractDialogue: inspectStartDialogueKey not set");
        assert(!inspectEndDialogueKey.empty() && "SetInteractDialogue: inspectEndDialogueKey not set");
    }

    // -----------------------------------------------------------------
    // Inspection
    // -----------------------------------------------------------------

    /// @brief Starts the shaking effect when inspection phase begins.
    /// @pre   shakingEffect must be non-null.
    void OnInspectionStart() override
    {
        assert(shakingEffect != nullptr && "OnInspectionStart: shakingEffect is null");
        shakingEffect->StartShaking();
    }

    /// @brief Returns true when all three objects have been inspected.
    bool AllObjectsInspected() const override
    {
        return isMovingLuggageInspected && isNewspaperInspected && isMessyClothesInspected;
    }

    /// @brief Drives the moving luggage/newspaper/messy clothes inspection loop.
    /// @pre   movingLuggage, newspaper, messyClothesInspect are non-null.
    void ManageInspectionState() override
    {
        assert(movingLuggage != nullptr && "ManageInspectionState: movingLuggage is null");
        assert(newspaper != nullptr && "ManageInspectionState: newspaper is null");
        assert(messyClothesInspect != nullptr && "ManageInspectionState: messyClothesInspect is null");

        shakingEffect->Update(currentDeltaTime);

        if (AllObjectsInspected())
            return;

        HandleMovingLuggageDialogue();
        HandleNewspaperDialogue();
        HandleMessyClothesDialogue();
        SyncNormalObjects();
        SetInspectionObjectState();
    }

private:
    /// @brief Mirrors visibility of normal objects vs inspect buttons.
    ///        Note: messyClothes has no normal counterpart — button only.
    void SyncNormalObjects()
    {
        movingLuggage->setActiveStatus(!movingLuggageInspect->getActiveStatus());
        newspaper->setActiveStatus(!newspaperInspect->getActiveStatus());
    }

    /// @brief Shows or hides inspect buttons based on current inspection state.
    void SetInspectionObjectState()
    {
        if (!inspectingObject.empty())
        {
            SetInstruction("Press [Space] or  [Mouse] to continue");
            movingLuggageInspect->setActiveStatus(false);
            newspaperInspect->setActiveStatus(false);
            messyClothesInspect->setActiveStatus(false);
            objectsParallaxManager->DisableParallaxEffect();
            dialogueManager->SetDialogueVisibility(true);
        }
        else
        {
            SetInstruction("Inspect an object  to start conversation");
            ShowInstruction();
            movingLuggageInspect->setActiveStatus(!isMovingLuggageInspected);
            newspaperInspect->setActiveStatus(!isNewspaperInspected);
            messyClothesInspect->setActiveStatus(!isMessyClothesInspected);
            objectsParallaxManager->EnableParallaxEffect();
            dialogueManager->SetDialogueVisibility(false);
        }
    }

    /// @brief Starts and monitors the moving luggage dialogue.
    ///        Stops shaking when completed.
    void HandleMovingLuggageDialogue()
    {
        if (!inspectMovingLuggageDialogueSet && inspectingObject == "MovingLuggage")
        {
            dialogueManager->SetDialogueSet(inspectMovingLuggageDialogueKey);
            inspectMovingLuggageDialogueSet = true;
        }
        if (inspectMovingLuggageDialogueSet && dialogueManager->IsDialogueFinished(inspectMovingLuggageDialogueKey))
        {
            inspectMovingLuggageDialogueSet = false;
            inspectingObject.clear();
            isMovingLuggageInspected = true;
            shakingEffect->StopShaking();
            CheckForEndDialogue();
        }
    }

    /// @brief Starts and monitors the newspaper dialogue.
    void HandleNewspaperDialogue()
    {
        if (!inspectNewspaperDialogueSet && inspectingObject == "Newspaper")
        {
            dialogueManager->SetDialogueSet(inspectNewspaperDialogueKey);
            inspectNewspaperDialogueSet = true;
        }
        if (inspectNewspaperDialogueSet && dialogueManager->IsDialogueFinished(inspectNewspaperDialogueKey))
        {
            inspectNewspaperDialogueSet = false;
            inspectingObject.clear();
            isNewspaperInspected = true;
            CheckForEndDialogue();
        }
    }

    /// @brief Starts and monitors the messy clothes dialogue.
    void HandleMessyClothesDialogue()
    {
        if (!inspectMessyClothesDialogueSet && inspectingObject == "MessyClothes")
        {
            dialogueManager->SetDialogueSet(inspectMessyClothesDialogueKey);
            inspectMessyClothesDialogueSet = true;
        }
        if (inspectMessyClothesDialogueSet && dialogueManager->IsDialogueFinished(inspectMessyClothesDialogueKey))
        {
            inspectMessyClothesDialogueSet = false;
            inspectingObject.clear();
            isMessyClothesInspected = true;
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
    // Archibald-specific members
    // -----------------------------------------------------------------

    // Inspectable objects
    UIElement *movingLuggage = nullptr;
    UIButton *movingLuggageInspect = nullptr;
    UIElement *newspaper = nullptr;
    UIButton *newspaperInspect = nullptr;
    UIButton *messyClothesInspect = nullptr; // no normal counterpart

    // Shaking effect
    unique_ptr<ShakingEffect> shakingEffect;
    float currentDeltaTime = 0.0f;

    // Sprite/icon maps for DialogueManager setup
    map<string, UIElement *> speakerSprites;
    map<string, UIElement *> speakerIcons;

    // Inspection state
    string inspectingObject;
    string inspectMovingLuggageDialogueKey;
    string inspectNewspaperDialogueKey;
    string inspectMessyClothesDialogueKey;
    bool inspectMovingLuggageDialogueSet = false;
    bool inspectNewspaperDialogueSet = false;
    bool inspectMessyClothesDialogueSet = false;
    bool isMovingLuggageInspected = false;
    bool isNewspaperInspected = false;
    bool isMessyClothesInspected = false;
};