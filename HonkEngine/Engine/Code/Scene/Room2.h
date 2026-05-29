// Room2.h
#pragma once

#include <map>
#include <string>
#include <cassert>
#include "RoomScene.h"

/// @brief Room scene for Cabin 2 — passengers Nathaniel and Evelyn Hartington.
///
/// Invariants:
///   - medicineFront, hat, bag must be valid before ManageInspectionState()
///   - inspectingObject is either empty or one of {"Medicine", "Hat", "Bag"}
///   - isMedicineInspected, isHatInspected, isBagInspected are false
///     until each is completed
class Room2 : public RoomScene
{

public:
    /// @brief Constructs Room2, sets up all Nathaniel/Evelyn-specific assets.
    /// @post  All GameObjects pushed to m_gameObjects.
    ///        dialogueManager is initialised and ready.
    Room2() : RoomScene("Room2Door")
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

        assert(dialogueManager != nullptr && "Room2: dialogueManager failed to initialise");
        assert(medicineFront != nullptr && "Room2: medicineFront is null");
        assert(hat != nullptr && "Room2: hat is null");
        assert(bag != nullptr && "Room2: bag is null");
    }

protected:
    // -----------------------------------------------------------------
    // Identity
    // -----------------------------------------------------------------

    Cabin GetCabin() const override
    {
        return Cabin::CABIN21;
    }

    GameState GetRoomGameState() const override
    {
        return GameState::ROOM2_STATE;
    }

    // -----------------------------------------------------------------
    // Room-specific setup
    // -----------------------------------------------------------------

    /// @brief Creates all Nathaniel/Evelyn-specific visual objects and
    ///        parallax layers.
    /// @post  medicineFront, hat, bag and their inspect buttons are non-null.
    ///        All objects pushed to m_gameObjects.
    void SetupRoomObjects() override
    {
        const float sm = 1.23f;

        UIElement *room = new UINormal("cabin2", "Assets/Images/Twins/Twin_Cabin.png", glm::vec3(0.0f, 0.17f, 0.0f),
            glm::vec3(19.0f * sm, 10.55f * sm, 0.0f), true);

        UIElement *medicineBack = new UINormal("MedicineBack", "Assets/Images/Twins/Twin_Inspection_MedicineBack.png",
            glm::vec3(-6.4f, -2.7f, 0.0f), glm::vec3(3.44f / 4 * sm, 3.96f / 4 * sm, 0.0f), true);

        // Inspectable objects
        medicineFront = new UINormal("MedicineFront", "Assets/Images/Twins/Twin_Inspection_MedicineFront.png",
            glm::vec3(-5.25f, -3.2f, 0.0f), glm::vec3(4.56f / 4 * sm, 1.76f / 4 * sm, 0.0f), true);

        hat = new UINormal("Hat", "Assets/Images/Twins/Twin_Inspection_Hat.png", glm::vec3(4.52f, -1.97f, 0.0f),
            glm::vec3(4.89f / 3 * sm, 4.05f / 3 * sm, 0.0f), true);

        bag = new UINormal("Bag", "Assets/Images/Twins/Twin_Inspection_Bag.png", glm::vec3(2.6f, -5.3f, 0.0f),
            glm::vec3(5.58f / 3 * sm, 4.92f / 3 * sm, 0.0f), true);

        // Inspect buttons
        medicineFrontInspect =
            new UIButton("MedicineFrontInspect", "Assets/Images/Twins/Twin_Inspection_MedicineFront.png",
                glm::vec3(-5.25f, -3.2f, 0.0f), glm::vec3(4.56f / 4 * sm, 1.76f / 4 * sm, 0.0f), true, false, "");
        medicineFrontInspect->SetHoverTexture("Assets/Images/Twins/Twin_Inspection_MedicineFront_Highlight.png");
        medicineFrontInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "medicineFrontInspect clicked while "
                                                   "inspectingObject already set");
                inspectingObject = "Medicine";
            });

        hatInspect = new UIButton("HatInspect", "Assets/Images/Twins/Twin_Inspection_Hat.png",
            glm::vec3(4.52f, -1.97f, 0.0f), glm::vec3(4.89f / 3 * sm, 4.05f / 3 * sm, 0.0f), true, false, "");
        hatInspect->SetHoverTexture("Assets/Images/Twins/Twin_Inspection_Hat_Highlight.png");
        hatInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "hatInspect clicked while inspectingObject already set");
                inspectingObject = "Hat";
            });

        bagInspect = new UIButton("BagInspect", "Assets/Images/Twins/Twin_Inspection_Bag.png",
            glm::vec3(2.6f, -5.3f, 0.0f), glm::vec3(5.58f / 3 * sm, 4.92f / 3 * sm, 0.0f), true, false, "");
        bagInspect->SetHoverTexture("Assets/Images/Twins/Twin_Inspection_Bag_Highlight.png");
        bagInspect->SetOnClickAction(
            [this]()
            {
                assert(inspectingObject.empty() && "bagInspect clicked while inspectingObject already set");
                inspectingObject = "Bag";
            });

        medicineFrontInspect->setActiveStatus(false);
        hatInspect->setActiveStatus(false);
        bagInspect->setActiveStatus(false);

        // Nathaniel sprites
        UIElement *nathanielNormal = new UINormal("Nathaniel_Normal", "Assets/Images/Twins/Nathaniel_Normal.png",
            glm::vec3(-3.95f, -2.39f, 0.0f), glm::vec3(4.72f * sm, 6.19f * sm, 0.0f), true);

        UIElement *nathanielHappy = new UINormal("Nathaniel_Happy", "Assets/Images/Twins/Nathaniel_Happy.png",
            glm::vec3(-3.95f, -2.39f, 0.0f), glm::vec3(4.72f * sm, 6.19f * sm, 0.0f), true);

        UIElement *nathanielDisappoint =
            new UINormal("Nathaniel_Disappoint", "Assets/Images/Twins/Nathaniel_Disappoint.png",
                glm::vec3(-3.95f, -2.39f, 0.0f), glm::vec3(4.72f * sm, 6.19f * sm, 0.0f), true);

        UIElement *nathanielUpset = new UINormal("Nathaniel_Upset", "Assets/Images/Twins/Nathaniel_Upset.png",
            glm::vec3(-3.95f, -2.39f, 0.0f), glm::vec3(4.72f * sm, 6.19f * sm, 0.0f), true);

        UIElement *nathanielSad = new UINormal("Nathaniel_Sad", "Assets/Images/Twins/Nathaniel_Sad.png",
            glm::vec3(-3.95f, -2.39f, 0.0f), glm::vec3(4.72f * sm, 6.19f * sm, 0.0f), true);

        UIElement *nathanielShy = new UINormal("Nathaniel_Shy", "Assets/Images/Twins/Nathaniel_Shy.png",
            glm::vec3(-3.95f, -2.39f, 0.0f), glm::vec3(4.72f * sm, 6.19f * sm, 0.0f), true);

        // Evelyn sprites
        UIElement *evelynNormal = new UINormal("Evelyn_Normal", "Assets/Images/Twins/Evelyn_Normal.png",
            glm::vec3(5.2f, -2.45f, 0.0f), glm::vec3(3.06f * sm, 5.87f * sm, 0.0f), true);

        UIElement *evelynHappy = new UINormal("Evelyn_Happy", "Assets/Images/Twins/Evelyn_Happy.png",
            glm::vec3(5.2f, -2.45f, 0.0f), glm::vec3(3.06f * sm, 5.87f * sm, 0.0f), true);

        UIElement *evelynDisappoint = new UINormal("Evelyn_Disappoint", "Assets/Images/Twins/Evelyn_Disappoint.png",
            glm::vec3(5.2f, -2.45f, 0.0f), glm::vec3(3.06f * sm, 5.87f * sm, 0.0f), true);

        UIElement *evelynUpset = new UINormal("Evelyn_Upset", "Assets/Images/Twins/Evelyn_Upset.png",
            glm::vec3(5.2f, -2.45f, 0.0f), glm::vec3(3.06f * sm, 5.87f * sm, 0.0f), true);

        UIElement *evelynSad = new UINormal("Evelyn_Sad", "Assets/Images/Twins/Evelyn_Sad.png",
            glm::vec3(5.2f, -2.45f, 0.0f), glm::vec3(3.06f * sm, 5.87f * sm, 0.0f), true);

        UIElement *evelynShy = new UINormal("Evelyn_Shy", "Assets/Images/Twins/Evelyn_Shy.png",
            glm::vec3(5.2f, -2.45f, 0.0f), glm::vec3(3.06f * sm, 5.87f * sm, 0.0f), true);

        // Speaker icons
        UIElement *nathanielIcon = new UINormal("NathanielIcon", "Assets/Images/UI/Speaker_icon_Nathaniel.png",
            glm::vec3(4.1f, 3.49f, 0.0f), glm::vec3(1.45f, 1.51f, 0.0f), true);

        UIElement *evelynIcon = new UINormal("EvelynIcon", "Assets/Images/UI/Speaker_icon_Evelyn.png",
            glm::vec3(4.14f, 3.49f, 0.0f), glm::vec3(1.57f, 1.5f, 0.0f), true);

        UIElement *waiterIcon = new UINormal("WaiterIcon", "Assets/Images/UI/Speaker_icon_Waiter.png",
            glm::vec3(4.18f, 3.43f, 0.0f), glm::vec3(1.23f, 1.4f, 0.0f), true);

        // Store sprites for DialogueManager setup
        speakerSprites = {
            { "Nathaniel_Normal", nathanielNormal },
            { "Nathaniel_Happy", nathanielHappy },
            { "Nathaniel_Disappoint", nathanielDisappoint },
            { "Nathaniel_Upset", nathanielUpset },
            { "Nathaniel_Sad", nathanielSad },
            { "Nathaniel_Shy", nathanielShy },
        };

        // Evelyn uses AddSpeakerSprite2 — stored separately
        secondarySpeakerSprites = {
            { "Evelyn_Normal", evelynNormal },
            { "Evelyn_Happy", evelynHappy },
            { "Evelyn_Disappoint", evelynDisappoint },
            { "Evelyn_Upset", evelynUpset },
            { "Evelyn_Sad", evelynSad },
            { "Evelyn_Shy", evelynShy },
        };

        speakerIcons = {
            { "N", nathanielIcon },
            { "E", evelynIcon },
            { "W", waiterIcon },
        };

        // Parallax layers
        const float defaultLayer = 0.8f;
        const float objectLayer = 0.85f;

        objectsParallaxManager->AddObjectToLayer(room, defaultLayer);
        objectsParallaxManager->AddObjectToLayer(hat, objectLayer);
        objectsParallaxManager->AddObjectToLayer(hatInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(bag, objectLayer);
        objectsParallaxManager->AddObjectToLayer(bagInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(medicineBack, objectLayer);
        objectsParallaxManager->AddObjectToLayer(medicineFront, objectLayer);
        objectsParallaxManager->AddObjectToLayer(medicineFrontInspect, objectLayer);
        objectsParallaxManager->AddObjectToLayer(nathanielNormal, objectLayer);
        objectsParallaxManager->AddObjectToLayer(nathanielHappy, objectLayer);
        objectsParallaxManager->AddObjectToLayer(nathanielDisappoint, objectLayer);
        objectsParallaxManager->AddObjectToLayer(nathanielUpset, objectLayer);
        objectsParallaxManager->AddObjectToLayer(nathanielSad, objectLayer);
        objectsParallaxManager->AddObjectToLayer(nathanielShy, objectLayer);
        objectsParallaxManager->AddObjectToLayer(evelynNormal, objectLayer);
        objectsParallaxManager->AddObjectToLayer(evelynHappy, objectLayer);
        objectsParallaxManager->AddObjectToLayer(evelynDisappoint, objectLayer);
        objectsParallaxManager->AddObjectToLayer(evelynUpset, objectLayer);
        objectsParallaxManager->AddObjectToLayer(evelynSad, objectLayer);
        objectsParallaxManager->AddObjectToLayer(evelynShy, objectLayer);

        // Push to scene
        m_gameObjects.push_back(room);
        m_gameObjects.push_back(medicineBack);
        m_gameObjects.push_back(bag);
        m_gameObjects.push_back(bagInspect);
        m_gameObjects.push_back(nathanielNormal);
        m_gameObjects.push_back(nathanielHappy);
        m_gameObjects.push_back(nathanielDisappoint);
        m_gameObjects.push_back(nathanielUpset);
        m_gameObjects.push_back(nathanielSad);
        m_gameObjects.push_back(nathanielShy);
        m_gameObjects.push_back(medicineFront);
        m_gameObjects.push_back(medicineFrontInspect);
        m_gameObjects.push_back(evelynNormal);
        m_gameObjects.push_back(evelynHappy);
        m_gameObjects.push_back(evelynDisappoint);
        m_gameObjects.push_back(evelynUpset);
        m_gameObjects.push_back(evelynSad);
        m_gameObjects.push_back(evelynShy);
        m_gameObjects.push_back(hat);
        m_gameObjects.push_back(hatInspect);
    }

    /// @brief Initialises the DialogueManager with both Nathaniel and Evelyn
    ///        sprites and base dialogues.
    /// @pre   dialogueBox, choice1, choice2, choice3 must be non-null.
    /// @post  dialogueManager is non-null with Order/Serve dialogues loaded.
    void SetupDialogueManager(UIButton *dialogueBox, UIButton *choice1, UIButton *choice2, UIButton *choice3) override
    {
        assert(dialogueBox != nullptr && "SetupDialogueManager: dialogueBox is null");
        assert(choice1 != nullptr && choice2 != nullptr && choice3 != nullptr &&
               "SetupDialogueManager: choice buttons are null");

        dialogueManager =
            make_unique<DialogueManager>("TwinDialogue", dialogueBox, "Nathaniel_Normal", "Evelyn_Normal");

        dialogueManager->LoadDialogues("Order", "Assets/Dialogue/Twin/Twin_Order.xml");
        dialogueManager->LoadDialogues("Serve_OnTime", "Assets/Dialogue/Twin/Twin_Serve_OnTime.xml");
        dialogueManager->LoadDialogues("Serve_Late", "Assets/Dialogue/Twin/Twin_Serve_Late.xml");

        for (auto &pair : speakerSprites)
        {
            dialogueManager->AddSpeakerSprite(pair.first, pair.second);
        }
        for (auto &pair : secondarySpeakerSprites)
        {
            dialogueManager->AddSpeakerSprite2(pair.first, pair.second);
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
    // Dialogue loading — Twin specific
    // -----------------------------------------------------------------

    /// @brief Loads Twin meal reaction dialogues based on kitchen choices.
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
                    "Assets/Dialogue/Twin/MealReact/"
                    "Twin_MealReact_Tea_EarlGrey_Chamomile_Green_WithMilk.xml");
                teaDialogueKey = "MealReact_Tea_EarlGrey_WithMilk";
            }
            else
            {
                dialogueManager->LoadDialogues("MealReact_Tea_EarlGrey", "Assets/Dialogue/Twin/MealReact/"
                                                                         "Twin_MealReact_Tea_Assam_EarlGrey.xml");
                teaDialogueKey = "MealReact_Tea_EarlGrey";
            }
            break;
        case ASSAMTEA:
            if (kitchen->getOptional() == MILK)
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Assam_WithMilk", "Assets/Dialogue/Twin/MealReact/"
                                                                               "Twin_MealReact_Tea_Assam_WithMilk.xml");
                teaDialogueKey = "MealReact_Tea_Assam_WithMilk";
            }
            else
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Assam", "Assets/Dialogue/Twin/MealReact/"
                                                                      "Twin_MealReact_Tea_Assam_EarlGrey.xml");
                teaDialogueKey = "MealReact_Tea_Assam";
            }
            break;
        case CHAMOMILETEA:
            if (kitchen->getOptional() == MILK)
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Charmomile_WithMilk",
                    "Assets/Dialogue/Twin/MealReact/"
                    "Twin_MealReact_Tea_EarlGrey_Chamomile_Green_WithMilk.xml");
                teaDialogueKey = "MealReact_Tea_Charmomile_WithMilk";
            }
            else
            {
                dialogueManager->LoadDialogues("MealReact_Tea_Charmomile", "Assets/Dialogue/Twin/MealReact/"
                                                                           "Twin_MealReact_Tea_Chamomile_Green.xml");
                teaDialogueKey = "MealReact_Tea_Charmomile";
            }
            break;
        case GREENTEA:
            if (kitchen->getOptional() == MILK)
            {
                dialogueManager->LoadDialogues("MealReact_Tea_GreenTea_WithMilk",
                    "Assets/Dialogue/Twin/MealReact/"
                    "Twin_MealReact_Tea_GreenTea_WithMilk.xml");
                teaDialogueKey = "MealReact_Tea_GreenTea_WithMilk";
            }
            else
            {
                dialogueManager->LoadDialogues("MealReact_Tea_GreenTea", "Assets/Dialogue/Twin/MealReact/"
                                                                         "Twin_MealReact_Tea_Chamomile_Green.xml");
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
            dialogueManager->LoadDialogues("MealReact_Sandwich_Salmon", "Assets/Dialogue/Twin/MealReact/"
                                                                        "Twin_MealReact_Sandwich_Salmon_Egg.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Salmon";
            break;
        case BEEF:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Beef", "Assets/Dialogue/Twin/MealReact/"
                                                                      "Twin_MealReact_Sandwich_Beef.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Beef";
            break;
        case EGG:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Egg", "Assets/Dialogue/Twin/MealReact/"
                                                                     "Twin_MealReact_Sandwich_Salmon_Egg.xml");
            sandwichDialogueKey = "MealReact_Sandwich_Egg";
            break;
        case CUCUMBER:
            dialogueManager->LoadDialogues("MealReact_Sandwich_Cucumber", "Assets/Dialogue/Twin/MealReact/"
                                                                          "Twin_MealReact_Sandwich_Cucumber.xml");
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
            dialogueManager->LoadDialogues("MealReact_Dessert_Eclair", "Assets/Dialogue/Twin/MealReact/"
                                                                       "Twin_MealReact_Pastry_Eclair.xml");
            dessertDialogueKey = "MealReact_Dessert_Eclair";
            break;
        case MACARON:
            dialogueManager->LoadDialogues("MealReact_Dessert_Macaron", "Assets/Dialogue/Twin/MealReact/"
                                                                        "Twin_MealReact_Pastry_Macaron.xml");
            dessertDialogueKey = "MealReact_Dessert_Macaron";
            break;
        case TART:
            dialogueManager->LoadDialogues("MealReact_Dessert_Tart", "Assets/Dialogue/Twin/MealReact/"
                                                                     "Twin_MealReact_Pastry_Other.xml");
            dessertDialogueKey = "MealReact_Dessert_Tart";
            break;
        case SCONE:
            dialogueManager->LoadDialogues("MealReact_Dessert_Scone", "Assets/Dialogue/Twin/MealReact/"
                                                                      "Twin_MealReact_Pastry_Other.xml");
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

    /// @brief Loads Twin score dialogue based on serve score level.
    /// @post  scoreDialogueKey is set and non-empty.
    void SetScoredDialogue() override
    {
        InteractionLevel level = characterData->getServeScoreLevel(GetCabin());

        switch (level)
        {
        case LOW:
            dialogueManager->LoadDialogues("Score_Low", "Assets/Dialogue/Twin/MealResult/Twin_MealResult_Low.xml");
            scoreDialogueKey = "Score_Low";
            break;
        case AVERAGE:
            dialogueManager->LoadDialogues(
                "Score_Average", "Assets/Dialogue/Twin/MealResult/Twin_MealResult_Average.xml");
            scoreDialogueKey = "Score_Average";
            break;
        case INFORMATIVE:
            dialogueManager->LoadDialogues("Score_Informative", "Assets/Dialogue/Twin/MealResult/"
                                                                "Twin_MealResult_Informative.xml");
            scoreDialogueKey = "Score_Informative";
            break;
        default:
            assert(false && "SetScoredDialogue: unknown interaction level");
            break;
        }

        assert(!scoreDialogueKey.empty() && "SetScoredDialogue: scoreDialogueKey not set");
    }

    /// @brief Loads Twin inspection dialogues based on interaction level.
    /// @post  inspectStartDialogueKey, inspectMedicineDialogueKey,
    ///        inspectHatDialogueKey, inspectBagDialogueKey,
    ///        inspectEndDialogueKey are set.
    void SetInteractDialogue() override
    {
        InteractionLevel level = characterData->getInteractionLevel(GetCabin());

        string prefix;
        string startPath;
        string medicinePath;
        string hatPath;
        string bagPath;
        string endPath;

        switch (level)
        {
        case LOW:
            prefix = "Low";
            startPath = "Assets/Dialogue/Twin/Low/Twin_Low_Start.xml";
            medicinePath = "Assets/Dialogue/Twin/Low/Twin_Low_Medicine.xml";
            hatPath = "Assets/Dialogue/Twin/Low/Twin_Low_Hat.xml";
            bagPath = "Assets/Dialogue/Twin/Low/Twin_Low_Bag.xml";
            endPath = "Assets/Dialogue/Twin/Low/Twin_Low_End.xml";
            break;
        case AVERAGE:
            prefix = "Average";
            startPath = "Assets/Dialogue/Twin/Average/Twin_Average_Start.xml";
            medicinePath = "Assets/Dialogue/Twin/Average/Twin_Average_Medicine.xml";
            hatPath = "Assets/Dialogue/Twin/Average/Twin_Average_Hat.xml";
            bagPath = "Assets/Dialogue/Twin/Average/Twin_Average_Bag.xml";
            endPath = "Assets/Dialogue/Twin/Average/Twin_Average_End.xml";
            break;
        case INFORMATIVE:
            prefix = "Informative";
            startPath = "Assets/Dialogue/Twin/Informative/"
                        "Twin_Informative_Start.xml";
            medicinePath = "Assets/Dialogue/Twin/Informative/"
                           "Twin_Informative_Medicine.xml";
            hatPath = "Assets/Dialogue/Twin/Informative/"
                      "Twin_Informative_Hat.xml";
            bagPath = "Assets/Dialogue/Twin/Informative/"
                      "Twin_Informative_Bag.xml";
            endPath = "Assets/Dialogue/Twin/Informative/"
                      "Twin_Informative_End.xml";
            break;
        default:
            assert(false && "SetInteractDialogue: unknown interaction level");
            return;
        }

        const string startKey = "Inspect_" + prefix + "_Start";
        const string medicineKey = "Inspect_" + prefix + "_Medicine";
        const string hatKey = "Inspect_" + prefix + "_Hat";
        const string bagKey = "Inspect_" + prefix + "_Bag";
        const string endKey = "Inspect_" + prefix + "_End";

        dialogueManager->LoadDialogues(startKey, startPath);
        dialogueManager->LoadDialogues(medicineKey, medicinePath);
        dialogueManager->LoadDialogues(hatKey, hatPath);
        dialogueManager->LoadDialogues(bagKey, bagPath);
        dialogueManager->LoadDialogues(endKey, endPath);

        inspectStartDialogueKey = startKey;
        inspectMedicineDialogueKey = medicineKey;
        inspectHatDialogueKey = hatKey;
        inspectBagDialogueKey = bagKey;
        inspectEndDialogueKey = endKey;

        assert(!inspectStartDialogueKey.empty() && "SetInteractDialogue: inspectStartDialogueKey not set");
        assert(!inspectEndDialogueKey.empty() && "SetInteractDialogue: inspectEndDialogueKey not set");
    }

    // -----------------------------------------------------------------
    // Inspection
    // -----------------------------------------------------------------

    /// @brief Returns true when medicine, hat and bag have all been inspected.
    bool AllObjectsInspected() const override
    {
        return isMedicineInspected && isHatInspected && isBagInspected;
    }

    /// @brief Drives the medicine/hat/bag inspection loop each frame.
    /// @pre   medicineFront, hat, bag and their inspect buttons are non-null.
    void ManageInspectionState() override
    {
        assert(medicineFront != nullptr && "ManageInspectionState: medicineFront is null");
        assert(hat != nullptr && "ManageInspectionState: hat is null");
        assert(bag != nullptr && "ManageInspectionState: bag is null");
        assert(medicineFrontInspect != nullptr && "ManageInspectionState: medicineFrontInspect is null");
        assert(hatInspect != nullptr && "ManageInspectionState: hatInspect is null");
        assert(bagInspect != nullptr && "ManageInspectionState: bagInspect is null");

        if (AllObjectsInspected())
            return;

        HandleMedicineDialogue();
        HandleHatDialogue();
        HandleBagDialogue();
        SyncNormalObjects();
        SetInspectionObjectState();
    }

private:
    /// @brief Mirrors visibility of normal objects vs inspect buttons.
    void SyncNormalObjects()
    {
        medicineFront->setActiveStatus(!medicineFrontInspect->getActiveStatus());
        hat->setActiveStatus(!hatInspect->getActiveStatus());
        bag->setActiveStatus(!bagInspect->getActiveStatus());
    }

    /// @brief Shows or hides inspect buttons based on current inspection state.
    void SetInspectionObjectState()
    {
        if (!inspectingObject.empty())
        {
            SetInstruction("Press [Space] or  [Mouse] to continue");
            medicineFrontInspect->setActiveStatus(false);
            hatInspect->setActiveStatus(false);
            bagInspect->setActiveStatus(false);
            objectsParallaxManager->DisableParallaxEffect();
            dialogueManager->SetDialogueVisibility(true);
        }
        else
        {
            SetInstruction("Inspect an object  to start conversation");
            ShowInstruction();
            medicineFrontInspect->setActiveStatus(!isMedicineInspected);
            hatInspect->setActiveStatus(!isHatInspected);
            bagInspect->setActiveStatus(!isBagInspected);
            objectsParallaxManager->EnableParallaxEffect();
            dialogueManager->SetDialogueVisibility(false);
        }
    }

    /// @brief Starts and monitors the medicine inspection dialogue.
    void HandleMedicineDialogue()
    {
        if (!inspectMedicineDialogueSet && inspectingObject == "Medicine")
        {
            dialogueManager->SetDialogueSet(inspectMedicineDialogueKey);
            inspectMedicineDialogueSet = true;
        }
        if (inspectMedicineDialogueSet && dialogueManager->IsDialogueFinished(inspectMedicineDialogueKey))
        {
            inspectMedicineDialogueSet = false;
            inspectingObject.clear();
            isMedicineInspected = true;
            CheckForEndDialogue();
        }
    }

    /// @brief Starts and monitors the hat inspection dialogue.
    void HandleHatDialogue()
    {
        if (!inspectHatDialogueSet && inspectingObject == "Hat")
        {
            dialogueManager->SetDialogueSet(inspectHatDialogueKey);
            inspectHatDialogueSet = true;
        }
        if (inspectHatDialogueSet && dialogueManager->IsDialogueFinished(inspectHatDialogueKey))
        {
            inspectHatDialogueSet = false;
            inspectingObject.clear();
            isHatInspected = true;
            CheckForEndDialogue();
        }
    }

    /// @brief Starts and monitors the bag inspection dialogue.
    void HandleBagDialogue()
    {
        if (!inspectBagDialogueSet && inspectingObject == "Bag")
        {
            dialogueManager->SetDialogueSet(inspectBagDialogueKey);
            inspectBagDialogueSet = true;
        }
        if (inspectBagDialogueSet && dialogueManager->IsDialogueFinished(inspectBagDialogueKey))
        {
            inspectBagDialogueSet = false;
            inspectingObject.clear();
            isBagInspected = true;
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
    // Twin-specific members
    // -----------------------------------------------------------------

    // Inspectable objects
    UIElement *medicineFront = nullptr;
    UIButton *medicineFrontInspect = nullptr;
    UIElement *hat = nullptr;
    UIButton *hatInspect = nullptr;
    UIElement *bag = nullptr;
    UIButton *bagInspect = nullptr;

    // Sprite/icon maps for DialogueManager setup
    map<string, UIElement *> speakerSprites;
    map<string, UIElement *> secondarySpeakerSprites;
    map<string, UIElement *> speakerIcons;

    // Inspection state
    string inspectingObject;
    string inspectMedicineDialogueKey;
    string inspectHatDialogueKey;
    string inspectBagDialogueKey;
    bool inspectMedicineDialogueSet = false;
    bool inspectHatDialogueSet = false;
    bool inspectBagDialogueSet = false;
    bool isMedicineInspected = false;
    bool isHatInspected = false;
    bool isBagInspected = false;
};