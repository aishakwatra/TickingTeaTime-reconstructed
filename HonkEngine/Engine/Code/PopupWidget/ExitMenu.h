#pragma once

#include "Interface.h"

class ExitMenu : public Interface
{

public:
    ExitMenu() : Interface("ExitMenu")
    {

        UIButton *GreyBackground = new UIButton("GreyBackground2", "Assets/Images/Journal/BlackAlpha.png",
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(19.2f, 10.8f, 0.0f), true, false, "");
        GreyBackground->SetOnClickAction([this]() { EmptyFunction(); });

        GameObject *ExitBackground =
            new UIObject("ExitBackground", "Assets/Images/Interface/Interface_Background_PopUp.png", true);
        ExitBackground->SetScale(glm::vec3(9.08f * 0.8f, 8.24f * 0.8f, 0.0f));
        ExitBackground->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        Text *QuestionText =
            new Text("QuestionText", "Are you sure you want to quit?  Your progress will not be saved.",
                "Assets/Fonts/OverpassMono-SemiBold.ttf", true);
        QuestionText->SetPosition(glm::vec3(0.1f, 0.9f, 0.0f));
        QuestionText->SetColor(glm::vec3(0, 0, 0));
        QuestionText->SetScale(0.55f);

        UIButton *ConfirmExitButton = new UIButton("ConfirmExitButton",
            "Assets/Images/MainMenu/MainMenu_Button_Setting.png", glm::vec3(-0.1f, -0.3f, 0.0f),
            glm::vec3(3.15f * 0.9f, 0.88f * 0.9f, 0.0f), true, true, "Assets/Fonts/OverpassMono-SemiBold.ttf");
        ConfirmExitButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Setting_Hover.png");
        ConfirmExitButton->SetButtonText("Yes");
        ConfirmExitButton->SetTextSize(0.55);
        ConfirmExitButton->SetTextPosition(glm::vec3(-0.05f, -0.4f, 0.0f));
        ConfirmExitButton->SetOnClickAction([this]() { ClickExit(); });

        UIButton *CancelButton = new UIButton("CloseExitButton", "Assets/Images/MainMenu/MainMenu_Button_Play.png",
            glm::vec3(-0.1f, -1.2f, 0.0f), glm::vec3(3.15f * 0.9f, 0.88f * 0.9f, 0.0f), true, true,
            "Assets/Fonts/OverpassMono-SemiBold.ttf");
        CancelButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Start_Hover.png");
        CancelButton->SetButtonText("Cancel");
        CancelButton->SetTextSize(0.55);
        CancelButton->SetTextPosition(glm::vec3(-0.05f, -1.3f, 0.0f));
        CancelButton->SetOnClickAction([this]() { Hide(); });



        m_gameObjects.push_back(GreyBackground);
        m_gameObjects.push_back(ExitBackground);
        m_gameObjects.push_back(QuestionText);
        m_gameObjects.push_back(CancelButton);
        m_gameObjects.push_back(ConfirmExitButton);
    }

    void EmptyFunction()
    {
        return;
    }

    void ClickExit()
    {
        JournalSaveSystem::GetInstance().SaveJournalData();
        Application::Get().exitGame();
    }


private:
};