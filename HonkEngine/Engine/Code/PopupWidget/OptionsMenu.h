#pragma once

#include "Interface.h"

#include "../UI/UISlider.h"


class OptionsMenu : public Interface
{

public:
    OptionsMenu() : Interface("OptionsMenu")
    {

        UIButton *GreyBackground = new UIButton("GreyBackground", "Assets/Images/Journal/BlackAlpha.png",
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(19.2f, 10.8f, 0.0f), true, false, "");
        GreyBackground->SetOnClickAction([this]() { EmptyFunction(); });

        GameObject *OptionsBackground =
            new UIObject("OptionsBackground", "Assets/Images/Interface/Interface_Background2.png", true);
        OptionsBackground->SetScale(glm::vec3(12.64f, 8.24f, 0.0f));
        OptionsBackground->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        UIButtonEmpty *BackButton = new UIButtonEmpty("BackButton", glm::vec3(4.8f, -3.0f, 0.0f),
            glm::vec3(1.5f, 3.0f, 0.0f), true, true, "Assets/Fonts/jibril.ttf");
        BackButton->SetButtonText("Back");
        BackButton->SetTextSize(0.6);
        BackButton->SetOnClickAction([this]() { Hide(); });

        // VOLUME

        Text *AudioText = new Text("AudioText", "Audio", "Assets/Fonts/jibril.ttf", true);
        AudioText->SetPosition(glm::vec3(2.9f, 2.4f, 0.0f));
        AudioText->SetColor(glm::vec3(0, 0, 0));
        AudioText->SetScale(0.9f);

        Text *MasterVolumeText =
            new Text("MasterVolumeText", "Master Volume", "Assets/Fonts/OverpassMono-SemiBold.ttf", true);
        MasterVolumeText->SetPosition(glm::vec3(2.9f, 1.4f, 0.0f));
        MasterVolumeText->SetColor(glm::vec3(0, 0, 0));
        MasterVolumeText->SetScale(0.6f);

        MasterVolumeSlider = new UISlider("MasterVolumeSlider", "Assets/Images/Interface/Slider_Handle.png",
            glm::vec3(2.85f, 1.1f, 0.0f), glm::vec3(0.45f * 0.8f, 0.61f * 0.8f, 0.0f), true, MasterVolume);
        MasterVolumeSlider->SetOnValueChangedCallback(
            [this](float value) { AudioManager::GetInstance().SetMasterVolume(value); });

        Text *SFXVolumeText = new Text("SFXVolumeText", "SFX Volume", "Assets/Fonts/OverpassMono-SemiBold.ttf", true);
        SFXVolumeText->SetPosition(glm::vec3(2.9f, 0.3f, 0.0f));
        SFXVolumeText->SetColor(glm::vec3(0, 0, 0));
        SFXVolumeText->SetScale(0.6f);

        SFXVolumeSlider = new UISlider("SFXVolumeSlider", "Assets/Images/Interface/Slider_Handle.png",
            glm::vec3(2.85f, 0.0f, 0.0f), glm::vec3(0.45f * 0.8f, 0.61f * 0.8f, 0.0f), true, SFXVolume);
        SFXVolumeSlider->SetOnValueChangedCallback(
            [this](float value) { AudioManager::GetInstance().SetSFXVolumeMultiplier(value); });

        Text *MusicVolumeText =
            new Text("MusicVolumeText", "Music Volume", "Assets/Fonts/OverpassMono-SemiBold.ttf", true);
        MusicVolumeText->SetPosition(glm::vec3(2.9f, -0.9f, 0.0f));
        MusicVolumeText->SetColor(glm::vec3(0, 0, 0));
        MusicVolumeText->SetScale(0.6f);

        MusicVolumeSlider = new UISlider("MusicVolumeSlider", "Assets/Images/Interface/Slider_Handle.png",
            glm::vec3(2.85f, -1.2f, 0.0f), glm::vec3(0.45f * 0.8f, 0.61f * 0.8f, 0.0f), true, MusicVolume);
        MusicVolumeSlider->SetOnValueChangedCallback(
            [this](float value) { AudioManager::GetInstance().SetMusicVolumeMultiplier(value); });

        // SCREEN MODE

        Text *GeneralText = new Text("GeneralText", "General", "Assets/Fonts/jibril.ttf", true);
        GeneralText->SetPosition(glm::vec3(-2.9f, 2.4f, 0.0f));
        GeneralText->SetColor(glm::vec3(0, 0, 0));
        GeneralText->SetScale(0.9f);

        Text *WindowMode = new Text("WindowModeText", "Window Mode", "Assets/Fonts/OverpassMono-SemiBold.ttf", true);
        WindowMode->SetPosition(glm::vec3(-2.9f, 1.4f, 0.0f));
        WindowMode->SetColor(glm::vec3(0, 0, 0));
        WindowMode->SetScale(0.6f);

        UIButton *FullScreenButton = new UIButton("FullScreenButton", "Assets/Images/MainMenu/MainMenu_Button_Play.png",
            glm::vec3(-2.95f, 0.7f, 0.0f), glm::vec3(3.15f, 0.88f, 0.0f), true, true,
            "Assets/Fonts/OverpassMono-SemiBold.ttf");
        FullScreenButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Start_Hover.png");
        FullScreenButton->SetButtonText("Full Screen");
        FullScreenButton->SetTextSize(0.55);
        FullScreenButton->SetTextPosition(glm::vec3(-2.88f, 0.62f, 0.0f));
        FullScreenButton->SetOnClickAction([this]() { FullScreen(); });

        UIButton *WindowedButton = new UIButton("WindowedButton", "Assets/Images/MainMenu/MainMenu_Button_Play.png",
            glm::vec3(-2.95f, -0.3f, 0.0f), glm::vec3(3.15f, 0.88f, 0.0f), true, true,
            "Assets/Fonts/OverpassMono-SemiBold.ttf");
        WindowedButton->SetHoverTexture("Assets/Images/MainMenu/MainMenu_Button_Start_Hover.png");
        WindowedButton->SetButtonText("Windowed");
        WindowedButton->SetTextSize(0.55);
        WindowedButton->SetTextPosition(glm::vec3(-2.88f, -0.38f, 0.0f));
        WindowedButton->SetOnClickAction([this]() { Windowed(); });

        // PUSH BACK OBJECTS

        m_gameObjects.push_back(GreyBackground);
        m_gameObjects.push_back(OptionsBackground);
        m_gameObjects.push_back(BackButton);

        m_gameObjects.push_back(AudioText);
        m_gameObjects.push_back(MasterVolumeText);
        m_gameObjects.push_back(SFXVolumeText);
        m_gameObjects.push_back(MusicVolumeText);

        m_gameObjects.push_back(MasterVolumeSlider);
        m_gameObjects.push_back(SFXVolumeSlider);
        m_gameObjects.push_back(MusicVolumeSlider);

        m_gameObjects.push_back(GeneralText);
        m_gameObjects.push_back(WindowMode);
        m_gameObjects.push_back(FullScreenButton);
        m_gameObjects.push_back(WindowedButton);
    }

    void EmptyFunction()
    {
        return;
    }

    void Show() override
    {
        Interface::Show();
        MasterVolumeSlider->UpdateKnobPosition();
        SFXVolumeSlider->UpdateKnobPosition();
        MusicVolumeSlider->UpdateKnobPosition();
    }

    void FullScreen()
    {
        Application::Get().SetFullScreen();
        AudioManager::GetInstance().PlaySound("buttonClick2");
        return;
    }

    void Windowed()
    {
        Application::Get().SetWindowedMode();
        AudioManager::GetInstance().PlaySound("buttonClick2");
        return;
    }



protected:
    UISlider *MasterVolumeSlider;
    UISlider *SFXVolumeSlider;
    UISlider *MusicVolumeSlider;
};