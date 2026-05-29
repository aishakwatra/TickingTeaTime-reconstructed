#pragma once

#include "Scene.h"

#include "../GameObjects/RenderGameObject.h"
#include"../GameObjects/AnimateGameObject.h"	
#include "../Text/Text.h"

#include "../UI/UIButton.h"

#include "../PopupWidget/OptionsMenu.h"
#include "../PopupWidget/ExitMenu.h"
#include "../PopupWidget/PauseMenu.h"

#include "../Application.h"


class MainMenu : public Scene {

public:

	MainMenu() :audioManager(AudioManager::GetInstance()) {

		audioManager.LoadSound("menuMusic", "Assets/Sounds/Music/BGmusic_Cabin2.mp3", Music, 0.6f);
        audioManager.LoadSound("buttonClick2", "Assets/Sounds/SFX_ButtonClick.mp3", SFX, 9.0f);

        GameObject *MainMenuBackground =
            new UIObject("MainMenuBackground", "Assets/Images/MainMenu/MainMenu_BG.jpg", true);
        MainMenuBackground->SetScale(glm::vec3(19.2f, 10.8f, 0.0f));
        MainMenuBackground->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        // 1. CONTINUE BUTTON (Moved down from 1.1f to 0.4f)
        ContinueButton = new UIButton("ContinueButton", "Assets/Images/MainMenu/MainMenu_Button_Play.png",
            glm::vec3(-6.9f, 0.4f, 0.0f), glm::vec3(3.78f, 1.06f, 0.0f), true, true, "Assets/Fonts/Jibril.ttf");
        ContinueButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Start_Hover.png");
        ContinueButton->SetButtonText("Continue");
        ContinueButton->SetTextSize(0.65);
        ContinueButton->SetTextPosition(glm::vec3(-6.9f, 0.3f, 0.0f));
        ContinueButton->SetOnClickAction([this]() { clickContinue(); });

        // 2. NEW GAME BUTTON (Moved down from -0.1f to -0.8f)
        PlayButton = new UIButton("PlayButton", "Assets/Images/MainMenu/MainMenu_Button_Play.png",
            glm::vec3(-6.9f, -0.8f, 0.0f), glm::vec3(3.78f, 1.06f, 0.0f), true, true, "Assets/Fonts/Jibril.ttf");
        PlayButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Start_Hover.png");
        PlayButton->SetButtonText("New Game");
        PlayButton->SetTextSize(0.65);
        PlayButton->SetTextPosition(glm::vec3(-6.9f, -0.9f, 0.0f));
        PlayButton->SetOnClickAction([this]() { clickPlay(); });

        // 3. OPTIONS BUTTON (Moved down from -1.3f to -2.0f)
        OptionsButton = new UIButton("OptionsButton", "Assets/Images/MainMenu/MainMenu_Button_Setting.png",
            glm::vec3(-6.9f, -2.0f, 0.0f), glm::vec3(3.78f, 1.06f, 0.0f), true, true, "Assets/Fonts/Jibril.ttf");
        OptionsButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Setting_Hover.png");
        OptionsButton->SetButtonText("Options");
        OptionsButton->SetTextSize(0.65);
        OptionsButton->SetTextPosition(glm::vec3(-6.9f, -2.12f, 0.0f));
        OptionsButton->SetOnClickAction([this]() { clickOptions(); });

        // 4. QUIT BUTTON (Moved down from -2.5f to -3.2f)
        QuitButton = new UIButton("QuitButton", "Assets/Images/MainMenu/MainMenu_Button_Play.png",
            glm::vec3(-6.9f, -3.2f, 0.0f), glm::vec3(3.78f, 1.06f, 0.0f), true, true, "Assets/Fonts/Jibril.ttf");
        QuitButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Exit_Hover.png");
        QuitButton->SetButtonText("Quit");
        QuitButton->SetTextSize(0.65);
        QuitButton->SetTextPosition(glm::vec3(-6.9f, -3.33f, 0.0f));
        QuitButton->SetOnClickAction([this]() { clickExit(); });

        clickToBegin = new Text("clickToBegin", "Click anywhere to begin", "Assets/Fonts/mvboli.ttf");
        clickToBegin->SetPosition(glm::vec3(-8.42f, -3.6f, 0.0f));
        clickToBegin->SetColor(glm::vec3(1, 1, 1));
        clickToBegin->SetScale(0.7f);

        transitionObject = new UINormal(
            "Transition", "Assets/Images/black.png", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(25.0f, 20.0f, 0.0f), true);
        transitionEffects = std::make_unique<TransitionEffects>(transitionObject);

        // Push all game objects to layout rendering stack
        m_gameObjects.push_back(MainMenuBackground);
        m_gameObjects.push_back(ContinueButton);
        m_gameObjects.push_back(PlayButton);
        m_gameObjects.push_back(OptionsButton);
        m_gameObjects.push_back(QuitButton);
        m_gameObjects.push_back(clickToBegin);

        // Interfaces in Main Menu
        m_gameObjects.push_back(transitionObject);
        m_gameObjects.push_back(&optionsMenu);
        m_gameObjects.push_back(&exitMenu);

        setMainMenuState();

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
		JournalData::GetInstance()->ActivateClue(CABIN4, 6);


		JournalData::GetInstance()->ActivateClue(CABIN3, 8);
		JournalData::GetInstance()->ActivateClue(CABIN4, 7);*/

	}

	void Update(float dt, long frame)
	{
		Scene::Update(dt, frame);

		transitionEffects->Update(dt);

		Input& input = Application::GetInput();

		if (startOfGame) {
			
			if (input.Get().GetKeyDown(GLFW_KEY_SPACE) || input.Get().GetMouseButtonDown(0))
			{
				startOfGame = false;
				setMainMenuState();
				AudioManager::GetInstance().PlaySound("buttonClick2");
			}
		}		

	}

	void setMainMenuState() {

		if (startOfGame) {

			ContinueButton->setActiveStatus(false);
			PlayButton->setActiveStatus(false);
			QuitButton->setActiveStatus(false);
			OptionsButton->setActiveStatus(false); 
			clickToBegin->setActiveStatus(true);

		}
		else {
            ContinueButton->setActiveStatus(true);
			PlayButton->setActiveStatus(true);
			QuitButton->setActiveStatus(true);
			OptionsButton->setActiveStatus(true);
			clickToBegin->setActiveStatus(false);
		}

	}

	void OnEnter() override {
		if (!audioManager.IsSoundPlaying("menuMusic"))
			audioManager.PlaySound("menuMusic", true);
		transitioning = true;
		transitionEffects->FadeIn(3.0f, [this]() {
			transitioning = false;
			});
	}

    void OnExit() override {
		audioManager.StopSound("menuMusic");

	}

private:

	AudioManager& audioManager;

	//BUTTON FUNCTIONS

	void clickPlay() {
		if (!transitioning) {
			transitioning = true;
			//transitioning = true;
            JournalSaveSystem::GetInstance().DeleteSaveFile("savegame.xml");
			AudioManager::GetInstance().PlaySound("buttonClick2");
			transitionEffects->FadeOut(2.0f, [this]() {
				Application::Get().SetScene("OpenScene");
				//Application::Get().SetScene("JournalEntry");
				});
		}
	}

	void clickContinue()
    {
        if (!transitioning)
        {
            transitioning = true;
            // transitioning = true;
            AudioManager::GetInstance().PlaySound("buttonClick2");
            transitionEffects->FadeOut(2.0f,
                [this]()
                {
                    Application::Get().SetScene("Hallway");
                    // Application::Get().SetScene("JournalEntry");
                });
        }
    }

	void clickOptions() { 
		AudioManager::GetInstance().PlaySound("buttonClick2");
;		optionsMenu.Show();
		//ACTIVATE OPTIONS MENU
	}

	void clickExit() { 
		AudioManager::GetInstance().PlaySound("buttonClick2");

		exitMenu.Show();

		/*transitionEffects->FadeOut(3.0f, [this]() {
			Application::Get().exitGame();
		});*/

		//ACTIVATE EXIT MENU
	
	}


	bool startOfGame = true;
	bool transitioning = false;

	UIButton* PlayButton;
    UIButton *ContinueButton;
	UIButton* OptionsButton; 
	UIButton* QuitButton;

	Text* clickToBegin;

	UIElement* transitionObject;
	std::unique_ptr<TransitionEffects> transitionEffects;

	OptionsMenu optionsMenu;
	ExitMenu exitMenu;

};


