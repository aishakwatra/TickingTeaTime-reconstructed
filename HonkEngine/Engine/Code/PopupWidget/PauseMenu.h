#pragma once

#include "Interface.h"
#include "OptionsMenu.h"
#include "ExitMenu.h"
#include "BackToMainMenu.h"
#include "../GameObjects/Timer.h"

class PauseMenu : public Interface
{
public:
    PauseMenu()
        : Interface("PauseMenu"), shouldReopenPauseMenu(false), optionsMenu(new OptionsMenu()),
          exitConfirmation(new ExitMenu()), menuConfirmation(new BackToMenu())
    {
        UIButton *GreyBackground = new UIButton("GreyBackground2", "Assets/Images/Journal/BlackAlpha.png",
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(19.2f, 10.8f, 0.0f), true, false, "");
        GreyBackground->SetOnClickAction([this]() { EmptyFunction(); });

        GameObject *PauseBackground =
            new UIObject("ExitBackground", "Assets/Images/Interface/Interface_Background_PopUp.png", true);
        PauseBackground->SetScale(glm::vec3(9.08f * 0.8f, 8.24f * 0.8f, 0.0f));
        PauseBackground->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        Text *PauseText = new Text("PauseText", "Paused", "Assets/Fonts/jibril.ttf", true);
        PauseText->SetPosition(glm::vec3(0.0f, 1.7f, 0.0f));
        PauseText->SetColor(glm::vec3(0, 0, 0));
        PauseText->SetScale(1.0f);

        UIButton *ContinueButton = new UIButton("ContinueButton", "Assets/Images/MainMenu/MainMenu_Button_Play.png",
            glm::vec3(-0.1f, 0.9f, 0.0f), glm::vec3(3.15f * 0.9f, 0.88f * 0.9f, 0.0f), true, true,
            "Assets/Fonts/OverpassMono-SemiBold.ttf");
        ContinueButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Start_Hover.png");
        ContinueButton->SetButtonText("Continue");
        ContinueButton->SetTextSize(0.55);
        ContinueButton->SetTextPosition(glm::vec3(-0.05f, 0.82f, 0.0f));
        ContinueButton->SetOnClickAction([this]() { ClickContinue(); });

        UIButton *OptionsButton = new UIButton("OptionsButton", "Assets/Images/MainMenu/MainMenu_Button_Setting.png",
            glm::vec3(-0.1f, 0.0f, 0.0f), glm::vec3(3.15f * 0.9f, 0.88f * 0.9f, 0.0f), true, true,
            "Assets/Fonts/OverpassMono-SemiBold.ttf");
        OptionsButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Setting_Hover.png");
        OptionsButton->SetButtonText("Options");
        OptionsButton->SetTextSize(0.55);
        OptionsButton->SetTextPosition(glm::vec3(-0.05f, -0.08f, 0.0f));
        OptionsButton->SetOnClickAction([this]() { ShowOptions(); });

        UIButton *MainMenuButton = new UIButton("MainMenuButton", "Assets/Images/MainMenu/MainMenu_Button_Exit.png",
            glm::vec3(-0.1f, -0.9f, 0.0f), glm::vec3(3.15f * 0.9f, 0.88f * 0.9f, 0.0f), true, true,
            "Assets/Fonts/OverpassMono-SemiBold.ttf");
        MainMenuButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Exit_Hover.png");
        MainMenuButton->SetButtonText("Main Menu");
        MainMenuButton->SetTextSize(0.55);
        MainMenuButton->SetTextPosition(glm::vec3(-0.05f, -0.97f, 0.0f));
        MainMenuButton->SetOnClickAction([this]() { ShowMenuConfirmation(); });

        UIButton *ExitGameButton = new UIButton("MainMenuButton", "Assets/Images/MainMenu/MainMenu_Button_Play.png",
            glm::vec3(-0.1f, -1.8f, 0.0f), glm::vec3(3.15f * 0.9f, 0.88f * 0.9f, 0.0f), true, true,
            "Assets/Fonts/OverpassMono-SemiBold.ttf");
        ExitGameButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Start_Hover.png");
        ExitGameButton->SetButtonText("Exit Game");
        ExitGameButton->SetTextSize(0.55);
        ExitGameButton->SetTextPosition(glm::vec3(-0.05f, -1.88f, 0.0f));
        ExitGameButton->SetOnClickAction([this]() { ShowExitConfirmation(); });

        m_gameObjects.push_back(GreyBackground);
        m_gameObjects.push_back(PauseBackground);
        m_gameObjects.push_back(PauseText);
        m_gameObjects.push_back(ContinueButton);
        m_gameObjects.push_back(OptionsButton);
        m_gameObjects.push_back(MainMenuButton);
        m_gameObjects.push_back(ExitGameButton);
    }

    virtual void Show() override
    {

        if (!optionsMenu->IsVisible() && !exitConfirmation->IsVisible() && !menuConfirmation->IsVisible())
        {
            isVisible = true;
            setActiveStatus(true);
            Timer &timer = Timer::GetInstance();
            timer.pause();
            BellManager::GetInstance().PauseAllRinging(); // Pause all ringing bells
            AudioManager::GetInstance().PlaySound("SFX_PageSwitch");
        }
    }



    void EmptyFunction()
    {
        return;
    }

    void ClickContinue()
    {
        Hide();
        Timer &timer = Timer::GetInstance();
        timer.resume();
        AudioManager::GetInstance().PlaySound("buttonClick2");
        BellManager::GetInstance().ResumeAllRinging(); // Resume all previously ringing bells
        shouldReopenPauseMenu = false;
    }

    void ShowOptions()
    {
        optionsMenu->Show();
        if (IsVisible())
        {
            shouldReopenPauseMenu = true;
            AudioManager::GetInstance().PlaySound("buttonClick2");
            Hide();
        }
    }

    void MainMenu()
    {


        GameStateManager::GetInstance().Reset();
        Application::Get().SetScene("MainMenu");
        AudioManager::GetInstance().StopSound("hallwayMusic");
        AudioManager::GetInstance().StopSound("trainAmbience");
        AudioManager::GetInstance().StopSound("cabinMusic");
        Hide();
    }

    void ShowExitConfirmation()
    {
        exitConfirmation->Show();
        if (IsVisible())
        {
            shouldReopenPauseMenu = true;
            AudioManager::GetInstance().PlaySound("buttonClick2");
            Hide();
        }
    }

    void ShowMenuConfirmation()
    {
        menuConfirmation->Show();
        // Hide();
        // AudioManager::GetInstance().PlaySound("buttonClick2");

        if (IsVisible())
        {
            shouldReopenPauseMenu = true;
            AudioManager::GetInstance().PlaySound("buttonClick2");
            Hide();
        }
    }



    virtual void Render() override
    {
        Interface::Render();
        // Render other popups if needed
        if (optionsMenu->IsVisible())
        {
            optionsMenu->Render();
        }
        if (exitConfirmation->IsVisible())
        {
            exitConfirmation->Render();
        }
        if (menuConfirmation->IsVisible())
        {
            menuConfirmation->Render();
        }
    }

    virtual void Update(float dt, long frame) override
    {
        Interface::Update(dt, frame);
        // Update other popups if needed
        if (optionsMenu->IsVisible())
        {
            optionsMenu->Update(dt, frame);
        }
        if (exitConfirmation->IsVisible())
        {
            exitConfirmation->Update(dt, frame);
        }
        if (menuConfirmation->IsVisible())
        {
            menuConfirmation->Update(dt, frame);
        }

        // Reopen the pause menu if needed
        if (!optionsMenu->IsVisible() && !exitConfirmation->IsVisible() && !menuConfirmation->IsVisible() &&
            shouldReopenPauseMenu)
        {
            Show();
            shouldReopenPauseMenu = false;
        }
    }


private:
    std::unique_ptr<OptionsMenu> optionsMenu;
    std::unique_ptr<ExitMenu> exitConfirmation;
    std::unique_ptr<BackToMenu> menuConfirmation;
    bool shouldReopenPauseMenu;
    bool isPaused;
};