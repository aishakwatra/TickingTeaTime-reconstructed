#pragma once

#include "Interface.h"

class SubmitMenu : public Interface
{

public:
    SubmitMenu() : Interface("SubmitMenu")
    {

        UIButton *GreyBackground = new UIButton("GreyBackground2", "Assets/Images/Journal/BlackAlpha.png",
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(19.2f, 10.8f, 0.0f), true, false, "");
        GreyBackground->SetOnClickAction([this]() { EmptyFunction(); });

        GameObject *SubmitBackground =
            new UIObject("SubmitBackground", "Assets/Images/Interface/Interface_Background_PopUp.png", true);
        SubmitBackground->SetScale(glm::vec3(9.08f * 0.8f, 8.24f * 0.8f, 0.0f));
        SubmitBackground->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        Text *QuestionText = new Text(
            "QuestionText", "Are you sure you want to submit?", "Assets/Fonts/OverpassMono-SemiBold.ttf", true);
        QuestionText->SetPosition(glm::vec3(0.1f, 0.9f, 0.0f));
        QuestionText->SetColor(glm::vec3(0, 0, 0));
        QuestionText->SetScale(0.55f);

        ConfirmSubmitButton = new UIButton("ConfirmSubmitButton", "Assets/Images/MainMenu/MainMenu_Button_Setting.png",
            glm::vec3(-0.1f, -0.3f, 0.0f), glm::vec3(3.15f * 0.9f, 0.88f * 0.9f, 0.0f), true, true,
            "Assets/Fonts/OverpassMono-SemiBold.ttf");
        ConfirmSubmitButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Setting_Hover.png");
        ConfirmSubmitButton->SetButtonText("Yes");
        ConfirmSubmitButton->SetTextSize(0.55);
        ConfirmSubmitButton->SetTextPosition(glm::vec3(-0.05f, -0.4f, 0.0f));
        // ConfirmSubmitButton->SetOnClickAction([this]() { ClickExit(); });

        UIButton *CancelButton = new UIButton("CloseSubmitButton", "Assets/Images/MainMenu/MainMenu_Button_Play.png",
            glm::vec3(-0.1f, -1.2f, 0.0f), glm::vec3(3.15f * 0.9f, 0.88f * 0.9f, 0.0f), true, true,
            "Assets/Fonts/OverpassMono-SemiBold.ttf");
        CancelButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Start_Hover.png");
        CancelButton->SetButtonText("Cancel");
        CancelButton->SetTextSize(0.55);
        CancelButton->SetTextPosition(glm::vec3(-0.05f, -1.3f, 0.0f));
        CancelButton->SetOnClickAction([this]() { Hide(); });


        m_gameObjects.push_back(GreyBackground);
        m_gameObjects.push_back(SubmitBackground);
        m_gameObjects.push_back(QuestionText);
        m_gameObjects.push_back(CancelButton);
        m_gameObjects.push_back(ConfirmSubmitButton);
    }

    void EmptyFunction()
    {
        return;
    }


    void SetExitAction(const std::function<void()> &action)
    {
        if (ConfirmSubmitButton)
        {
            ConfirmSubmitButton->SetOnClickAction(action);
        }
    }


private:
    UIButton *ConfirmSubmitButton;
};