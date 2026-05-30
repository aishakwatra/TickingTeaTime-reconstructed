#pragma once

#include "Scene.h"

#include "../GameObjects/RenderGameObject.h"
#include "../GameObjects/AnimateGameObject.h"
#include "../Effects/TransitionEffects.h"

#include "../UI/UIButton.h"
#include "../UI/UIDraggable.h"
#include "../UI/UIButtonEmpty.h"

#include "KitchenData.h"
#include "../GameStateManager.h"

#include "../PopupWidget/PauseMenu.h"

class Kitchen : public Scene
{

public:
    Book *Journal;

    UIButtonEmpty *teaDropArea =
        new UIButtonEmpty("teaDropArea", glm::vec3(-3.4f, -0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), true, false, "");
    UIButtonEmpty *sandwichDropArea = new UIButtonEmpty(
        "sandwichDropArea", glm::vec3(-6.35f, -0.6f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), true, false, "");
    UIButtonEmpty *dessertDropArea = new UIButtonEmpty(
        "dessertDropArea", glm::vec3(-4.85f, 1.57f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), true, false, "");
    UIButtonEmpty *optionalDropArea = new UIButtonEmpty(
        "optionalDropArea", glm::vec3(-4.6f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), true, false, "");

    Kitchen() : audioManager(AudioManager::GetInstance())
    {

        /*--------------------------------------------------------------LOAD
         * AUDDIO-------------------------------------------------------------------------------------------------------
         */
        audioManager.LoadSound("kitchenAmbience", "Assets/Sounds/Ambience/Ambience_Kitchen.mp3", SFX, 0.5f);

        audioManager.LoadSound("plateSound1", "Assets/Sounds/Kitchen/SFX_MealSelect1.mp3", SFX, 1.0f);
        audioManager.LoadSound("plateSound2", "Assets/Sounds/Kitchen/SFX_MealSelect2.mp3", SFX, 1.0f);
        audioManager.LoadSound("plateSound3", "Assets/Sounds/Kitchen/SFX_MealSelect3.mp3", SFX, 1.0f);
        audioManager.LoadSound("plateSound4", "Assets/Sounds/Kitchen/SFX_MealSelect4.mp3", SFX, 1.0f);

        audioManager.LoadSound("servingBellRing", "Assets/Sounds/Kitchen/SFX_ServingBell.mp3", SFX, 0.5f);

        /*--------------------------------------------------------------CREATE
         * GAMEOBJECT-------------------------------------------------------------------------------------------------------
         */

        GameObject *KitchenBackground =
            new UIObject("KitchenBackground", "Assets/Images/Kitchen/Kitchen_Background.png", true);
        KitchenBackground->SetScale(glm::vec3(19.2f, 10.8f, 0.0f));
        KitchenBackground->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        GameObject *Tray = new UIObject("Tray", "Assets/Images/Kitchen/Tray.png", true);
        Tray->SetScale(glm::vec3(7.13f * 0.8f, 3.45f * 0.8f, 0.0f));
        Tray->SetPosition(glm::vec3(-4.9f, -0.2f, 0.0f));

        GameObject *Note = new UIObject("Note", "Assets/Images/Kitchen/Kitchen_Note.png", true);
        Note->SetScale(glm::vec3(4.04f, 2.05f, 0.0f));
        Note->SetPosition(glm::vec3(2.46f, 4.175f, 0.0f));

        float sd = 0.85f;

        GameObject *TeaSign = new UIObject("TeaSign", "Assets/Images/Kitchen/Kitchen_Sign_Tea.png", true);
        TeaSign->SetScale(glm::vec3(2.16f * sd, 0.65f * sd, 0.0f));
        TeaSign->SetPosition(glm::vec3(1.95f, 1.95f, 0.0f));

        GameObject *OptionalSign =
            new UIObject("OptionalSign", "Assets/Images/Kitchen/Kitchen_Sign_Optional.png", true);
        OptionalSign->SetScale(glm::vec3(2.16f * sd, 0.65f * sd, 0.0f));
        OptionalSign->SetPosition(glm::vec3(7.6f, 1.95f, 0.0f));

        GameObject *SandwichSign =
            new UIObject("SandwichSign", "Assets/Images/Kitchen/Kitchen_Sign_Sandwich.png", true);
        SandwichSign->SetScale(glm::vec3(2.16f * sd, 0.65f * sd, 0.0f));
        SandwichSign->SetPosition(glm::vec3(4.7f, -0.4f, 0.0f));

        GameObject *PastrySign = new UIObject("PastrySign", "Assets/Images/Kitchen/Kitchen_Sign_Pastry.png", true);
        PastrySign->SetScale(glm::vec3(2.16f * sd, 0.65f * sd, 0.0f));
        PastrySign->SetPosition(glm::vec3(4.7f, -2.44f, 0.0f));


        SandwichArrow = new UIObject("SandwichArrow", "Assets/Images/Kitchen/Kitchen_Arrow_Sandwich.png", true);
        SandwichArrow->SetScale(glm::vec3(1.6f / 2, 1.35f / 2, 0.0f));
        SandwichArrow->SetPosition(glm::vec3(-7.8f, 0.23f, 0.0f));
        SandwichArrow->setActiveStatus(false);

        TeaArrow = new UIObject("TeaArrow", "Assets/Images/Kitchen/Kitchen_Arrow_Tea.png", true);
        TeaArrow->SetScale(glm::vec3(1.58f / 2, 1.35f / 2, 0.0f));
        TeaArrow->SetPosition(glm::vec3(-2.1f, 0.9f, 0.0f));
        TeaArrow->setActiveStatus(false);

        OptionalArrow = new UIObject("OptionalArrow", "Assets/Images/Kitchen/Kitchen_Arrow_Tea.png", true);
        OptionalArrow->SetScale(glm::vec3(1.58f / 2, 1.35f / 2, 0.0f));
        OptionalArrow->SetPosition(glm::vec3(-3.6f, 0.0f, 0.0f));
        OptionalArrow->setActiveStatus(false);

        DessertArrow = new UIObject("DessertArrow", "Assets/Images/Kitchen/Kitchen_Arrow_Pastry.png", true);
        DessertArrow->SetScale(glm::vec3(1.74f / 2, 1.63f / 2, 0.0f));
        DessertArrow->SetPosition(glm::vec3(-3.7f, 2.5f, 0.0f));
        DessertArrow->setActiveStatus(false);

        orderPaper = new UINormal("OrderPaper", "Assets/Images/UI/OrderPaper.png", glm::vec3(-7.72f, 4.1f, 0.0f),
            glm::vec3(3.55f, 2.54f, 0.0f), true);
        timerUI = new UINormal(
            "TimerUI", "Assets/Images/UI/Timer.png", glm::vec3(6.85f, 4.9f, 0.0f), glm::vec3(4.37f, 3.13f, 0.0f), true);

        Journal = new Book();

        /*--------------------------------------------------------------CREATE
         * GAMEOBJECT-------------------------------------------------------------------------------------------------------
         */

        orderNoText = new Text("orderNo", "", "Assets/Fonts/mvboli.ttf", true);
        teaOrderText = new Text("TeaOrder", "", "Assets/Fonts/mvboli.ttf", true);
        sandwichOrderText = new Text("sandwichOrder", "", "Assets/Fonts/mvboli.ttf", true);
        pastryOrderText = new Text("PastryOrder", "", "Assets/Fonts/mvboli.ttf", true);


        orderNoText->SetPosition(glm::vec3(-8.8f, 4.5f, 0.0f));
        orderNoText->SetColor(glm::vec3(0.5, 0, 0));
        teaOrderText->SetPosition(glm::vec3(-7.2f, 4.6f, 0.0f));
        teaOrderText->SetScale(0.65f);
        sandwichOrderText->SetPosition(glm::vec3(-7.63f, 3.95f, 0.0f));
        sandwichOrderText->SetScale(0.55f);
        pastryOrderText->SetPosition(glm::vec3(-7.63f, 3.32f, 0.0f));
        pastryOrderText->SetScale(0.55f);


        // OrderData to manager Order Text
        OrderData &orderData = OrderData::GetInstance();
        orderData.Initialize(orderNoText, teaOrderText, sandwichOrderText, pastryOrderText);
        // orderData.SetOrderPaper(orderPaper);
        //  Add observer to update UI on change

        // In Kitchen Constructor
        Timer &timer = Timer::GetInstance();
        timerText = new Text("timerText", "", "Assets/Fonts/Jibril.ttf", true);

        timerText->SetPosition(glm::vec3(6.35f, 3.9f, 0.0f));
        timerText->SetColor(glm::vec3(0.78039, 0.72549, 0.44314));
        timerText->SetScale(1.45f);

        timerUI->setActiveStatus(false);

        timer.Initialize(timerText);
        // timer.SetTimerUI(this->timerUI);

        timer.AddObserver([this, &timer]() { this->UpdateTimerDisplay(); });

        // m_gameObjects.push_back(timerUI); // Add the timer UI to the game object list
        // m_gameObjects.push_back(timerText); // Add the timer text to the game object list


        orderData.AddObserver([this]() { this->UpdateOrderDisplay(); });


        transitionObject = new UINormal(
            "Transition", "Assets/Images/black.png", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(25.0f, 20.0f, 0.0f), true);
        transitionEffects = std::make_unique<TransitionEffects>(transitionObject);

        /*--------------------------------------------------------------CREATE
         * BUTTONS-------------------------------------------------------------------------------------------------------
         */

        ServeBellButton = new UIButton("ServeBellButton", "Assets/Images/Kitchen/ServeButton_servable_default.png",
            glm::vec3(-4.9f, -2.6f, 0.0f), glm::vec3(2.02f * 0.85f, 1.45f * 0.85f, 0.0f), true, false);
        ServeBellButton->SetHoverTexture("Assets/Images/Kitchen/ServeButton_servable_Highlight.png");
        ServeBellButton->setActiveStatus(false);
        ServeBellButton->SetOnClickAction([this]() { Serve(); });

        ServeBellGrey =
            new UIObject("ServeBellGrey", "Assets/Images/Kitchen/ServeButton_notServable_default.png", true);
        ServeBellGrey->SetPosition(glm::vec3(-4.9f, -2.6f, 0.0f));
        ServeBellGrey->SetScale(glm::vec3(2.02f * 0.85f, 1.45f * 0.85f, 0.0f));


        UIButton *journalButton = new UIButton("JournalButton", "Assets/Images/UI/JournalButton.png",
            glm::vec3(-8.32f, -4.8f, 0.0f), glm::vec3(3.0f, 3.0f, 0.0f), true, false, "");
        journalButton->SetHoverTexture("Assets/Images/UI/JournalButton_Highlight.png");
        journalButton->SetOnClickAction(
            [this]()
            {
                Journal->SetKitchenDefaultPage();
                Journal->drawBook();
            });

        journalUpdateIcon = new UINormal("JournalUpdateIcon", "Assets/Images/Journal/UpdateIcon.png",
            glm::vec3(-7.38f, -3.65f, 0.0f), glm::vec3(0.62f / 2, 1.71f / 2, 0.0f), true);

        UIButton *ResetButton =
            new UIButton("PlayButton", "Assets/Images/Kitchen/Button_ResetMeal.png", glm::vec3(-1.75f, 4.75f, 0.0f),
                glm::vec3(3.19f * 0.8f, 0.92f * 0.8f, 0.0f), true, true, "Assets/Fonts/jibril.ttf");
        ResetButton->SetHoverTexture("Assets/Images/Kitchen/Button_ResetMeal_Highlight.png");
        ResetButton->SetButtonText("Reset");
        ResetButton->SetTextSize(0.54f);
        ResetButton->SetTextPosition(glm::vec3(-1.7f, 4.67f, 0.0f));
        ResetButton->SetOnClickAction([this]() { clearPlate(); });


        /*--------------------------------------------------------------CREATE FOOD
         * DRAGGABLES-------------------------------------------------------------------------------------------------------
         */

        float tsm = 0.19;

        AssamBlackTea = new UIDraggable("AssamBlackTea", "Assets/Images/Kitchen/Food/Tea/TeaBox_Assam.png",
            glm::vec3(1.2f, 0.59f, 0.0f), glm::vec3(5.56f * tsm, 7.04f * tsm, 0.0f), true);
        ChamomileTea = new UIDraggable("ChamomileTea", "Assets/Images/Kitchen/Food/Tea/TeaBox_Chamomile.png",
            glm::vec3(2.45f, 0.695f, 0.0f), glm::vec3(4.78f * tsm, 8.32f * tsm, 0.0f), true);
        EarlGreyTea = new UIDraggable("EarlGreyTea", "Assets/Images/Kitchen/Food/Tea/TeaBox_Earl.png",
            glm::vec3(3.72f, 0.65f, 0.0f), glm::vec3(6.02f * tsm, 7.58f * tsm, 0.0f), true);
        GreenTea = new UIDraggable("GreenTea", "Assets/Images/Kitchen/Food/Tea/TeaBox_Green.png",
            glm::vec3(5.05f, 0.625f, -0.1f), glm::vec3(6.04f * tsm, 7.36f * tsm, 0.0f), true);

        SalmonSandwich =
            new UIDraggable("SalmonSandwich", "Assets/Images/Kitchen/Food/Sandwich/Sandwich_Icon_Salmon.png",
                glm::vec3(1.62f, -1.6f, 0.0f), glm::vec3(2.08f * 0.95f, 1.15f * 0.95f, 0.0f), true);
        EggSandwich = new UIDraggable("EggSandwich", "Assets/Images/Kitchen/Food/Sandwich/Sandwich_Icon_Egg.png",
            glm::vec3(3.8f, -1.75f, 0.0f), glm::vec3(1.38f * 1.3f, 0.58f * 1.3f, 0.0f), true);
        CucumberSandwich =
            new UIDraggable("CucumberSandwich", "Assets/Images/Kitchen/Food/Sandwich/Sandwich_Icon_Cucumber.png",
                glm::vec3(5.8f, -1.68f, 0.0f), glm::vec3(1.86f * 0.87f, 1.03f * 0.87f, 0.0f), true);
        BeefSandwich = new UIDraggable("BeefSandwich", "Assets/Images/Kitchen/Food/Sandwich/Sandwich_Icon_Beef.png",
            glm::vec3(7.9f, -1.67f, 0.0f), glm::vec3(2.0f, 0.9f, 0.0f), true);

        Eclair = new UIDraggable("Eclair", "Assets/Images/Kitchen/Food/Dessert/Dessert_Icon_Eclair.png",
            glm::vec3(1.6f, -3.78f, 0.0f), glm::vec3(2.0f * 0.85f, 0.7f * 0.85f, 0.0f), true);
        LemonTart = new UIDraggable("LemonTart", "Assets/Images/Kitchen/Food/Dessert/Dessert_Icon_LemonTart.png",
            glm::vec3(3.67f, -3.67f, 0.0f), glm::vec3(1.45f, 0.83f, 0.0f), true);
        Scone = new UIDraggable("Scone", "Assets/Images/Kitchen/Food/Dessert/Dessert_Icon_Scone.png",
            glm::vec3(5.46f, -3.7f, 0.0f), glm::vec3(1.04f * 1.5f, 0.52f * 1.5f, 0.0f), true);
        Macaron = new UIDraggable("Macaron", "Assets/Images/Kitchen/Food/Dessert/Dessert_Icon_Macaron.png",
            glm::vec3(7.6f, -3.74f, 0.0f), glm::vec3(2.0f * 0.9f, 0.77f * 0.9f, 0.0f), true);

        Milk = new UIDraggable("Milk", "Assets/Images/Kitchen/Food/Tea/Tea_MilkBottle.png",
            glm::vec3(7.6f, 0.74f, 0.0f), glm::vec3(9.9f * 0.15f, 10.88f * 0.15f, 0.0f), true);


        /*--------------------------------------------------------------SET DRAGGABLE FOOD
         * FUNCTIONS-------------------------------------------------------------------------------------------------------
         */

        AssamBlackTea->SetOnReleaseAction([this]() { releaseAssam(); });
        EarlGreyTea->SetOnReleaseAction([this]() { releaseEarlGrey(); });
        GreenTea->SetOnReleaseAction([this]() { releaseGreenTea(); });
        ChamomileTea->SetOnReleaseAction([this]() { releaseChamomileTea(); });

        AssamBlackTea->SetOnDragAction([this]() { dragTea(); });
        EarlGreyTea->SetOnDragAction([this]() { dragTea(); });
        GreenTea->SetOnDragAction([this]() { dragTea(); });
        ChamomileTea->SetOnDragAction([this]() { dragTea(); });

        SalmonSandwich->SetOnReleaseAction([this]() { releaseSalmonSandwich(); });
        EggSandwich->SetOnReleaseAction([this]() { releaseEggSandwich(); });
        CucumberSandwich->SetOnReleaseAction([this]() { releaseCucumberSandwich(); });
        BeefSandwich->SetOnReleaseAction([this]() { releaseBeefSandwich(); });

        SalmonSandwich->SetOnDragAction([this]() { dragSandwich(); });
        EggSandwich->SetOnDragAction([this]() { dragSandwich(); });
        CucumberSandwich->SetOnDragAction([this]() { dragSandwich(); });
        BeefSandwich->SetOnDragAction([this]() { dragSandwich(); });

        Eclair->SetOnReleaseAction([this]() { releaseEclair(); });
        LemonTart->SetOnReleaseAction([this]() { releaseLemonTart(); });
        Scone->SetOnReleaseAction([this]() { releaseScone(); });
        Macaron->SetOnReleaseAction([this]() { releaseMacaron(); });

        Eclair->SetOnDragAction([this]() { dragDessert(); });
        LemonTart->SetOnDragAction([this]() { dragDessert(); });
        Scone->SetOnDragAction([this]() { dragDessert(); });
        Macaron->SetOnDragAction([this]() { dragDessert(); });

        Milk->SetOnReleaseAction([this]() { releaseMilk(); });
        Milk->SetOnDragAction([this]() { dragOptional(); });

        /*-------------------------------------------------------------- CREATE PLATED FOOD
         * ------------------------------------------------------------------------------------------------------- */

        float sm = 1.1f;
        float sm_tea = 2.0f;

        GameObject *AssamBlack_dish =
            new UIObject("AssamBlackDish", "Assets/Images/Kitchen/Food/Tea/TeaPot_Assam.png", true);
        AssamBlack_dish->SetPosition(TeaDishPosition);
        AssamBlack_dish->SetScale(glm::vec3(1.4f * sm_tea, 0.9f * sm_tea, 0.0f));

        GameObject *EarlGreyTea_dish =
            new UIObject("EarlGreyDish", "Assets/Images/Kitchen/Food/Tea/TeaPot_Earl.png", true);
        EarlGreyTea_dish->SetPosition(TeaDishPosition);
        EarlGreyTea_dish->SetScale(glm::vec3(1.4f * sm_tea, 0.9f * sm_tea, 0.0f));

        GameObject *GreenTea_dish =
            new UIObject("GreenTeaDish", "Assets/Images/Kitchen/Food/Tea/TeaPot_Green.png", true);
        GreenTea_dish->SetPosition(TeaDishPosition);
        GreenTea_dish->SetScale(glm::vec3(1.2f * sm_tea, 0.92f * sm_tea, 0.0f));

        GameObject *ChamomileTea_dish =
            new UIObject("ChamomileTeaDish", "Assets/Images/Kitchen/Food/Tea/TeaPot_Chamomile1.png", true);
        ChamomileTea_dish->SetPosition(TeaDishPosition);
        ChamomileTea_dish->SetScale(glm::vec3(1.462f * sm_tea, 0.855f * sm_tea, 0.0f));

        GameObject *SalmonDish =
            new UIObject("SalmonDish", "Assets/Images/Kitchen/Food/Sandwich/Sandwich_Salmon.png", true);
        SalmonDish->SetPosition(SandwichDishPosition);
        SalmonDish->SetScale(glm::vec3(2.35f * sm, 1.28f * sm, 0.0f));

        GameObject *EggDish = new UIObject("EggDish", "Assets/Images/Kitchen/Food/Sandwich/Sandwich_Egg.png", true);
        EggDish->SetPosition(SandwichDishPosition);
        EggDish->SetScale(glm::vec3(1.672f * 1.6f, 0.826f * 1.6f, 0.0f));

        GameObject *CucumberDish =
            new UIObject("CucumberDish", "Assets/Images/Kitchen/Food/Sandwich/Sandwich_Cucumber.png", true);
        CucumberDish->SetPosition(SandwichDishPosition);
        CucumberDish->SetScale(glm::vec3(2.32f, 1.3f, 0.0f));

        GameObject *BeefDish = new UIObject("BeefDish", "Assets/Images/Kitchen/Food/Sandwich/Sandwich_Beef.png", true);
        BeefDish->SetPosition(SandwichDishPosition);
        BeefDish->SetScale(glm::vec3(1.437 * 1.6f, 0.826f * 1.6f, 0.0f));

        GameObject *EclairDish =
            new UIObject("EclairDish", "Assets/Images/Kitchen/Food/Dessert/Dessert_Eclair.png", true);
        EclairDish->SetPosition(DessertDishPosition);
        EclairDish->SetScale(glm::vec3(2.6f, 0.8f, 0.0f));

        GameObject *LemonTartDish =
            new UIObject("LemonTartDish", "Assets/Images/Kitchen/Food/Dessert/Dessert_LemonTart.png", true);
        LemonTartDish->SetPosition(DessertDishPosition);
        LemonTartDish->SetScale(glm::vec3(2.08f * 1.15f, 0.97f * 1.15f, 0.0f));

        GameObject *SconeDish = new UIObject("SconeDish", "Assets/Images/Kitchen/Food/Dessert/Dessert_Scone.png", true);
        SconeDish->SetPosition(DessertDishPosition);
        SconeDish->SetScale(glm::vec3(1.95f * 1.5f, 0.58f * 1.5f, 0.0f));

        GameObject *MacaronDish =
            new UIObject("MacaronDish", "Assets/Images/Kitchen/Food/Dessert/Dessert_Macaron.png", true);
        MacaronDish->SetPosition(DessertDishPosition);
        MacaronDish->SetScale(glm::vec3(13.4f * 0.17f, 4.89f * 0.17f, 0.0f));

        GameObject *MilkDish = new UIObject("MilkDish", "Assets/Images/Kitchen/Food/Tea/Tea_MilkCup.png", true);
        MilkDish->SetPosition(OptionalDishPosition);
        MilkDish->SetScale(glm::vec3(2.42f * 0.35f, 1.98f * 0.35f, 0.0f));

        /*--------------------------------------------------------------PUSH
         * BACK-------------------------------------------------------------------------------------------------------
         */

        m_gameObjects.push_back(KitchenBackground);
        m_gameObjects.push_back(ServeBellButton);
        m_gameObjects.push_back(ServeBellGrey);
        m_gameObjects.push_back(orderPaper);
        m_gameObjects.push_back(timerUI);

        m_gameObjects.push_back(orderNoText);
        m_gameObjects.push_back(teaOrderText);
        m_gameObjects.push_back(sandwichOrderText);
        m_gameObjects.push_back(pastryOrderText);

        m_gameObjects.push_back(timerText);
        m_gameObjects.push_back(ResetButton);

        // drop area
        m_gameObjects.push_back(teaDropArea);
        m_gameObjects.push_back(sandwichDropArea);
        m_gameObjects.push_back(dessertDropArea);
        m_gameObjects.push_back(optionalDropArea);


        // PLATED FOOD GAMEOBJECTS
        platedTea[ASSAMTEA] = AssamBlack_dish;
        platedTea[EARLGREYTEA] = EarlGreyTea_dish;
        platedTea[GREENTEA] = GreenTea_dish;
        platedTea[CHAMOMILETEA] = ChamomileTea_dish;

        platedSandwich[SALMON] = SalmonDish;
        platedSandwich[EGG] = EggDish;
        platedSandwich[CUCUMBER] = CucumberDish;
        platedSandwich[BEEF] = BeefDish;

        platedDessert[ECLAIR] = EclairDish;
        platedDessert[TART] = LemonTartDish;
        platedDessert[SCONE] = SconeDish;
        platedDessert[MACARON] = MacaronDish;

        platedMilk = MilkDish;

        m_gameObjects.push_back(EclairDish);
        m_gameObjects.push_back(LemonTartDish);
        m_gameObjects.push_back(SconeDish);
        m_gameObjects.push_back(MacaronDish);

        m_gameObjects.push_back(AssamBlack_dish);
        m_gameObjects.push_back(EarlGreyTea_dish);
        m_gameObjects.push_back(GreenTea_dish);
        m_gameObjects.push_back(ChamomileTea_dish);

        m_gameObjects.push_back(SalmonDish);
        m_gameObjects.push_back(EggDish);
        m_gameObjects.push_back(CucumberDish);
        m_gameObjects.push_back(BeefDish);

        m_gameObjects.push_back(Tray);
        m_gameObjects.push_back(MilkDish);
        m_gameObjects.push_back(Note);


        // DRAGGABLE FOOD
        m_gameObjects.push_back(AssamBlackTea);
        m_gameObjects.push_back(EarlGreyTea);
        m_gameObjects.push_back(GreenTea);
        m_gameObjects.push_back(ChamomileTea);

        m_gameObjects.push_back(SalmonSandwich);
        m_gameObjects.push_back(EggSandwich);
        m_gameObjects.push_back(CucumberSandwich);
        m_gameObjects.push_back(BeefSandwich);

        m_gameObjects.push_back(Eclair);
        m_gameObjects.push_back(LemonTart);
        m_gameObjects.push_back(Scone);
        m_gameObjects.push_back(Macaron);

        m_gameObjects.push_back(Milk);

        // Signs
        m_gameObjects.push_back(TeaSign);
        m_gameObjects.push_back(OptionalSign);
        m_gameObjects.push_back(SandwichSign);
        m_gameObjects.push_back(PastrySign);

        // visual clues
        m_gameObjects.push_back(SandwichArrow);
        m_gameObjects.push_back(TeaArrow);
        m_gameObjects.push_back(OptionalArrow);
        m_gameObjects.push_back(DessertArrow);

        // Journal
        m_gameObjects.push_back(journalButton);
        m_gameObjects.push_back(journalUpdateIcon);
        m_gameObjects.push_back(Journal);

        m_gameObjects.push_back(&pauseMenu);
        m_gameObjects.push_back(transitionObject);

        // set all plate gameobjects as inactive
        clearPlate();
    }

    void OnEnter() override
    {
        Scene::OnEnter(); // Call base class if there's relevant logic
        // audioManager.PlaySound("hallwayMusic", true);
        transitionEffects->FadeIn(1.0f, [this]() {});
        audioManager.PlaySound("kitchenAmbience", true);
        AudioManager::GetInstance().PlaySound("doorSlide");
        if (GameStateManager::GetInstance().getRoomState() != RoomState::Prepare)
        {
            clearPlate();
        }

        journalUpdateIcon->setActiveStatus(JournalData::GetInstance()->HasUnopenedClue());
    }

    void Update(float dt, long frame) override
    {

        Scene::Update(dt, frame);
        gameStateManager.Update();

        Timer &timer = Timer::GetInstance();
        timer.Update(dt);

        BellManager &bellManager = BellManager::GetInstance();
        bellManager.Update(dt, frame);

        transitionEffects->Update(dt);


        processInput();
        updateServeButton();
        UpdateOrderDisplay();
        UpdatePlateDisplay();
        journalUpdateIcon->setActiveStatus(JournalData::GetInstance()->HasUnopenedClue());
    }

    void processInput()
    {
        if (input.Get().GetKeyDown(GLFW_KEY_R))
        {
            clearPlate();
        }
        if (input.Get().GetKeyDown(GLFW_KEY_TAB))
        {
            if (Journal->isOpen())
            {
                Journal->closeBook();
            }
            else if (!Journal->isOpen())
            {
                Journal->SetKitchenDefaultPage();
                Journal->drawBook();
            }
        }
        if (input.Get().GetKeyDown(GLFW_KEY_ESCAPE))
        {
            pauseMenu.Show();
        }
    }

    void UpdatePlateDisplay()
    {
        if (Kitchen_Data->getTea() == TEA_EMPTY)
        {
            for (int i = 0; i < 4; i++)
            {
                platedTea[i]->setActiveStatus(false);
            }
        }

        if (Kitchen_Data->getSandwich() == SANDWICH_EMPTY)
        {
            for (int i = 0; i < 4; i++)
            {
                platedSandwich[i]->setActiveStatus(false);
            }
        }

        if (Kitchen_Data->getDessert() == DESSERT_EMPTY)
        {
            for (int i = 0; i < 4; i++)
            {
                platedDessert[i]->setActiveStatus(false);
            }
        }

        if (Kitchen_Data->getOptional() == OPTIONAL_EMPTY)
        {
            platedMilk->setActiveStatus(false);
        }
    }
    void UpdateOrderDisplay()
    {

        OrderData &orderData = OrderData::GetInstance();


        if (orderNoText && teaOrderText && sandwichOrderText && pastryOrderText && orderPaper)
        {
            orderNoText->SetContent(orderData.GetRoomNumber());
            teaOrderText->SetContent(orderData.GetTeaOrder());
            sandwichOrderText->SetContent(orderData.GetSandwichOrder());
            pastryOrderText->SetContent(orderData.GetPastryOrder());
            orderPaper->setActiveStatus(orderData.GetOrderPaperVisibility());


            // Debug: Confirm the update visually
            // orderNoText->SetColor(glm::vec3(1.0f, 0.0f, 0.0f)); // Set text color to red for visibility
        }
        else
        {
            std::cout << "One or more text objects are null." << std::endl;
        }
    }

    void UpdateTimerDisplay()
    {
        Timer &timer = Timer::GetInstance();
        if (timerText && timerUI)
        {
            timerText->SetContent(timer.GetTime());
            timerUI->setActiveStatus(timer.isTimerRunning());
        }
        else
        {
            std::cout << "One or more text objects are null." << std::endl;
        }
    }



    void clearPlate()
    {
        Kitchen_Data->clearPlate();
    }


    // UPDATE FOOD ON TRAY

    void updateSandwichObjects()
    {

        for (int i = 0; i < 4; i++)
        {
            if (i == Kitchen_Data->getSandwich())
            {
                platedSandwich[i]->setActiveStatus(true);
            }
            else
            {
                platedSandwich[i]->setActiveStatus(false);
            }
        }
    }

    void updateDessertObjects()
    {


        for (int i = 0; i < 4; i++)
        {
            if (i == Kitchen_Data->getDessert())
            {
                platedDessert[i]->setActiveStatus(true);
            }
            else
            {
                platedDessert[i]->setActiveStatus(false);
            }
        }
    }

    void updateTeaObjects()
    {

        for (int i = 0; i < 4; i++)
        {
            if (i == Kitchen_Data->getTea())
            {
                platedTea[i]->setActiveStatus(true);
            }
            else
            {
                platedTea[i]->setActiveStatus(false);
            }
        }
    }

    void updateOptionalObjects()
    {

        if (Kitchen_Data->getOptional() == Optional::MILK)
        {
            platedMilk->setActiveStatus(true);
        }
        else
        {
            platedMilk->setActiveStatus(false);
        }
    }

    // ON DRAG FUNCTIONS

    void dragSandwich()
    {
        SandwichArrow->setActiveStatus(true);
    }

    void dragTea()
    {
        TeaArrow->setActiveStatus(true);
    }

    void dragDessert()
    {
        DessertArrow->setActiveStatus(true);
    }

    void dragOptional()
    {
        OptionalArrow->setActiveStatus(true);
    }

    // ON RELEASE FUNCTIONS

    void releaseAssam()
    {

        TeaArrow->setActiveStatus(false);

        if (AssamBlackTea->withinRage(teaDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setTea(ASSAMTEA);
            updateTeaObjects();
            audioManager.PlaySound("plateSound3");
        }
    }

    void releaseEarlGrey()
    {

        TeaArrow->setActiveStatus(false);

        // std::cout << "EARL GREY RELEASED" << std::endl;
        if (EarlGreyTea->withinRage(teaDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setTea(EARLGREYTEA);
            updateTeaObjects();
            audioManager.PlaySound("plateSound3");
        }
    }

    void releaseGreenTea()
    {

        TeaArrow->setActiveStatus(false);

        // std::cout << "GREEN TEA RELEASED" << std::endl;
        if (GreenTea->withinRage(teaDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setTea(GREENTEA);
            updateTeaObjects();
            audioManager.PlaySound("plateSound3");
        }
    }

    void releaseChamomileTea()
    {

        TeaArrow->setActiveStatus(false);

        if (ChamomileTea->withinRage(teaDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setTea(CHAMOMILETEA);
            updateTeaObjects();
            audioManager.PlaySound("plateSound3");
        }
    }

    void releaseSalmonSandwich()
    {

        SandwichArrow->setActiveStatus(false);

        if (SalmonSandwich->withinRage(sandwichDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setSandwich(SALMON);
            updateSandwichObjects();
            audioManager.PlaySound("plateSound2");
        }
    }

    void releaseEggSandwich()
    {

        SandwichArrow->setActiveStatus(false);

        if (EggSandwich->withinRage(sandwichDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setSandwich(EGG);
            updateSandwichObjects();
            audioManager.PlaySound("plateSound2");
        }
    }

    void releaseCucumberSandwich()
    {

        SandwichArrow->setActiveStatus(false);

        if (CucumberSandwich->withinRage(sandwichDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setSandwich(CUCUMBER);
            updateSandwichObjects();
            audioManager.PlaySound("plateSound2");
        }
    }

    void releaseBeefSandwich()
    {

        SandwichArrow->setActiveStatus(false);

        if (BeefSandwich->withinRage(sandwichDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setSandwich(BEEF);
            updateSandwichObjects();
            audioManager.PlaySound("plateSound2");
        }
    }


    void releaseEclair()
    {

        DessertArrow->setActiveStatus(false);

        if (Eclair->withinRage(dessertDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setDessert(ECLAIR);
            updateDessertObjects();
            audioManager.PlaySound("plateSound4");
        }
    }

    void releaseLemonTart()
    {

        DessertArrow->setActiveStatus(false);

        if (LemonTart->withinRage(dessertDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setDessert(TART);
            updateDessertObjects();
            audioManager.PlaySound("plateSound4");
        }
    }

    void releaseScone()
    {

        DessertArrow->setActiveStatus(false);

        if (Scone->withinRage(dessertDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setDessert(SCONE);
            updateDessertObjects();
            audioManager.PlaySound("plateSound4");
        }
    }

    void releaseMacaron()
    {

        DessertArrow->setActiveStatus(false);

        if (Macaron->withinRage(dessertDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setDessert(MACARON);
            updateDessertObjects();
            audioManager.PlaySound("plateSound4");
        }
    }

    void releaseMilk()
    {

        OptionalArrow->setActiveStatus(false);

        if (Milk->withinRage(optionalDropArea->GetOnscreenPosition(), snapThreshold))
        {
            Kitchen_Data->setOptional(MILK);
            updateOptionalObjects();
            audioManager.PlaySound("plateSound1");
        }
    }



    void Serve()
    {
        std::cout << "SERVE FOOD" << std::endl;
        audioManager.PlaySound("servingBellRing");
        Application::Get().SetTimer(
            1000, [this]() { transitionEffects->FadeOut(1.0f, [this]() { Application::Get().SetScene("Hallway"); }); },
            false);
        // Assuming GameStateManager and DoorManager are accessible globally or passed to this scene.
        GameState currentGameState = gameStateManager.getGameState();
        RoomState currentRoomState = gameStateManager.getRoomState();
        std::string doorName = gameStateNameToDoorName(currentGameState);

        if (!doorName.empty())
        {
            Door *door = DoorManager::GetInstance().GetDoorByName(doorName);
            if (door != nullptr)
            {
                door->setPermission(true);
                std::cout << doorName << " unlocked." << std::endl;
            }
            else
            {
                std::cout << "Door not found: " << doorName << std::endl;
            }
        }
        else
        {
            std::cout << "No valid door associated with current game state." << std::endl;
        }
    }

    void updateServeButton()
    {

        if (Kitchen_Data->checkCompletePlate())
        {
            ServeBellButton->setActiveStatus(true);
            ServeBellGrey->setActiveStatus(false);
        }
        else
        {
            ServeBellButton->setActiveStatus(false);
            ServeBellGrey->setActiveStatus(true);
        }
    }

    void OnExit() override
    {
        Scene::OnExit(); // Call base class if there's relevant logic
        // audioManager.PauseSound("hallwayMusic");
        AudioManager::GetInstance().PlaySound("doorSlide");
        audioManager.StopSound("kitchenAmbience");
    }


private:
    GameStateManager &gameStateManager = GameStateManager::GetInstance();
    AudioManager &audioManager;
    Input &input = Application::GetInput();

    KitchenData *Kitchen_Data = KitchenData::GetInstance();

    const glm::vec3 tea_platePos;
    const glm::vec3 Sandwich_platePos;
    const glm::vec3 dessert_platePos;

    UIDraggable *AssamBlackTea;
    UIDraggable *EarlGreyTea;
    UIDraggable *GreenTea;
    UIDraggable *ChamomileTea;

    UIDraggable *SalmonSandwich;
    UIDraggable *EggSandwich;
    UIDraggable *CucumberSandwich;
    UIDraggable *BeefSandwich;

    UIDraggable *Eclair;
    UIDraggable *LemonTart;
    UIDraggable *Scone;
    UIDraggable *Macaron;

    UIDraggable *Milk;
    UIDraggable *Champagne;

    UIButton *ServeBellButton;
    GameObject *ServeBellGrey;


    // Array of plated game objects
    GameObject *platedSandwich[4];
    GameObject *platedDessert[4];
    GameObject *platedTea[4];
    GameObject *platedMilk;
    GameObject *platedOptional[2];

    // Plated dish positions
    const glm::vec3 TeaDishPosition = glm::vec3(-3.4f, -0.35f, 0.0f);
    const glm::vec3 SandwichDishPosition = glm::vec3(-6.35f, -0.6f, 0.0f);
    const glm::vec3 DessertDishPosition = glm::vec3(-4.85f, 1.57f, 0.0f);
    const glm::vec3 OptionalDishPosition = glm::vec3(-4.6f, -0.87f, 0.0f);

    const float snapThreshold = 2.0f;


    Text *orderNoText;
    Text *teaOrderText;
    Text *sandwichOrderText;
    Text *pastryOrderText;

    Text *timerText;
    UIElement *timerUI;
    UIElement *orderPaper;

    UIElement *journalUpdateIcon;


    GameObject *SandwichArrow;
    GameObject *TeaArrow;
    GameObject *OptionalArrow;
    GameObject *DessertArrow;

    UIElement *transitionObject;
    std::unique_ptr<TransitionEffects> transitionEffects;

    PauseMenu pauseMenu;
};
