#pragma once

#include "GameObject.h"
#include "RenderGameObject.h"
#include "../Engine.h"
#include <glm/glm.hpp>
#include "../Input/Input.h"
#include "JournalData.h"
#include <iostream>
#include "../Application.h"
#include "../Scene/Scene.h"
#include "../Text/Text.h"

#include "MainPage.h"
#include "CabinPage1.h"
#include "CabinPage2.h"
#include "CabinPage3.h"
#include "CabinPage4.h"
#include "CabinPage5.h"
#include "FoodGuidePage.h"


enum currentPage
{
    MAIN_PAGE,
    CABIN1_PAGE,
    CABIN2_PAGE,
    CABIN3_PAGE,
    CABIN4_PAGE,
    CABIN5_PAGE,
    FOODGUIDE_PAGE
};

// FIGURE OUT A DATA STRUCTURE TO STORE PAGE DATA

class Book : public GameObject
{

public:
    AudioManager &audioManager;

public:
    GameObject *JournalCover;

    Book() : GameObject("Book"), audioManager(AudioManager::GetInstance())
    {

        audioManager.LoadSound("openJournal", "Assets/Sounds/Journal/SFX_OpenJournal.mp3", SFX, 0.3f);
        audioManager.LoadSound("pageSwitch", "Assets/Sounds/Journal/SFX_PageSwitch.mp3", SFX, 0.2f);

        JournalCover = new UIObject("JournalCover", "Assets/Images/Journal/Cover.png", true);
        JournalCover->SetScale(glm::vec3(14.36f, 8.24f, 0.0f));
        JournalCover->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        UIButton *Tab1 = new UIButton("Tab1", "Assets/Images/Journal/Bookmark_CaseSummary.png",
            glm::vec3(6.57f, 2.87f, 0.0f), glm::vec3(2.25f, 0.96f, 0.0f), true, false, "");
        Tab1->SetHoverTexture("Assets/Images/Journal/Bookmark_CaseSummary_Highlight.png");

        UIButton *Tab2 = new UIButton("Tab2", "Assets/Images/Journal/Bookmark_P1.png", glm::vec3(6.57f, 1.85f, 0.0f),
            glm::vec3(2.13f, 0.91f, 0.0f), true, false, "");
        Tab2->SetHoverTexture("Assets/Images/Journal/Bookmark_P1_Highlight.png");

        UIButton *Tab3 = new UIButton("Tab3", "Assets/Images/Journal/Bookmark_P2_1.png", glm::vec3(6.55f, 0.92f, 0.0f),
            glm::vec3(2.13f, 0.91f, 0.0f), true, false, "");
        Tab3->SetHoverTexture("Assets/Images/Journal/Bookmark_P2_1_Highlight.png");

        UIButton *Tab4 = new UIButton("Tab4", "Assets/Images/Journal/Bookmark_P2_2.png", glm::vec3(6.61f, 0.0f, 0.0f),
            glm::vec3(2.13f, 0.91f, 0.0f), true, false, "");
        Tab4->SetHoverTexture("Assets/Images/Journal/Bookmark_P2_2_Highlight.png");

        UIButton *Tab5 = new UIButton("Tab5", "Assets/Images/Journal/Bookmark_P3.png", glm::vec3(6.55f, -0.93f, 0.0f),
            glm::vec3(2.13f, 0.91f, 0.0f), true, false, "");
        Tab5->SetHoverTexture("Assets/Images/Journal/Bookmark_P3_Highlight.png");

        UIButton *Tab6 = new UIButton("Tab6", "Assets/Images/Journal/Bookmark_P4.png", glm::vec3(6.55f, -1.85f, 0.0f),
            glm::vec3(2.13f, 0.91f, 0.0f), true, false, "");
        Tab6->SetHoverTexture("Assets/Images/Journal/Bookmark_P4_Highlight.png");

        UIButton *Tab7 = new UIButton("Tab6", "Assets/Images/Journal/Bookmark_TeaGuide.png",
            glm::vec3(6.75f, -2.8f, 0.0f), glm::vec3(1.78f, 0.91f, 0.0f), true, false, "");
        Tab7->SetHoverTexture("Assets/Images/Journal/Bookmark_TeaGuide_Highlight.png");

        Tab1->SetOnClickAction([this]() { setActiveTab1(); });
        Tab2->SetOnClickAction([this]() { setActiveTab2(); });
        Tab3->SetOnClickAction([this]() { setActiveTab3(); });
        Tab4->SetOnClickAction([this]() { setActiveTab4(); });
        Tab5->SetOnClickAction([this]() { setActiveTab5(); });
        Tab6->SetOnClickAction([this]() { setActiveTab6(); });
        Tab7->SetOnClickAction([this]() { setActiveTab7(); });

        BackGround = new UIObject("JournalBG", "Assets/Images/Journal/BlackAlpha.png", true);
        BackGround->SetScale(glm::vec3(19.2f, 10.8f, 0.0f));
        BackGround->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        GameObject *BlankPage = new UIObject("BlankPage", "Assets/Images/Journal/BlankPage.png", true);
        BlankPage->SetScale(glm::vec3(12.68f, 7.45f, 1.0f));
        BlankPage->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        CloseJournalButton = new UIButton("CloseButton", "Assets/Images/Journal/Journal_CloseButton.png",
            glm::vec3(5.25f, 4.0f, 0.0f), glm::vec3(1.04f, 1.70f, 0.0f), true, false, "");
        CloseJournalButton->SetOnClickAction([this]() { closeJournal(); });
        CloseJournalButton->SetHoverTexture("Assets/Images/Journal/Journal_CloseButton_Highlight.png");

        UIDraggable *Cabin3Newspaper =
            new UIDraggable("Cabin3Newspaper", "Assets/Images/PaperClues/Archibald_Cabin3_Newspaper.png",
                glm::vec3(11.7f, 2.2f, 0.0f), glm::vec3(7.31f, 10.12f, 0.0f), true);
        Cabin3Newspaper->setDragBoundsByObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 20.0f, 0.0f));

        UIDraggable *Cabin4Pamphlet =
            new UIDraggable("Cabin4Pamphlet", "Assets/Images/PaperClues/Octavia_Cabin4_Pamphlet.png",
                glm::vec3(-10.2f, -3.5f, 0.0f), glm::vec3(5.34f, 7.37f, 0.0f), true);
        Cabin4Pamphlet->setDragBoundsByObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 20.0f, 0.0f));

        // Update bookmark icons
        UpdateIcon1 = new UIObject("JournalUpdateIcon", "Assets/Images/Journal/UpdateIcon.png", true);
        UpdateIcon1->SetScale(glm::vec3(0.62f / 2.5, 1.71f / 2.5, 0.0f));
        UpdateIcon1->SetPosition(glm::vec3(7.5f, 1.95f, 0.0f));

        UpdateIcon2 = new UIObject("JournalUpdateIcon", "Assets/Images/Journal/UpdateIcon.png", true);
        UpdateIcon2->SetScale(glm::vec3(0.62f / 2.5, 1.71f / 2.5, 0.0f));
        UpdateIcon2->SetPosition(glm::vec3(7.5f, 1.02f, 0.0f));

        UpdateIcon3 = new UIObject("JournalUpdateIcon", "Assets/Images/Journal/UpdateIcon.png", true);
        UpdateIcon3->SetScale(glm::vec3(0.62f / 2.5, 1.71f / 2.5, 0.0f));
        UpdateIcon3->SetPosition(glm::vec3(7.5f, 0.1f, 0.0f));

        UpdateIcon4 = new UIObject("JournalUpdateIcon", "Assets/Images/Journal/UpdateIcon.png", true);
        UpdateIcon4->SetScale(glm::vec3(0.62f / 2.5, 1.71f / 2.5, 0.0f));
        UpdateIcon4->SetPosition(glm::vec3(7.45f, -0.83f, 0.0f));

        UpdateIcon5 = new UIObject("JournalUpdateIcon", "Assets/Images/Journal/UpdateIcon.png", true);
        UpdateIcon5->SetScale(glm::vec3(0.62f / 3, 1.71f / 2.5, 0.0f));
        UpdateIcon5->SetPosition(glm::vec3(7.45f, -1.75f, 0.0f));

        m_gameObjects.push_back(BackGround);
        m_gameObjects.push_back(JournalCover);
        m_gameObjects.push_back(Tab1);
        m_gameObjects.push_back(Tab2);
        m_gameObjects.push_back(Tab3);
        m_gameObjects.push_back(Tab4);
        m_gameObjects.push_back(Tab5);
        m_gameObjects.push_back(Tab6);
        m_gameObjects.push_back(Tab7);
        m_gameObjects.push_back(CloseJournalButton);
        m_gameObjects.push_back(BlankPage);

        Page *mainPage = new MainPage();
        Page *cabinPage1 = new CabinPage1();
        Page *cabinPage2 = new CabinPage2();
        Page *cabinPage3 = new CabinPage3();
        Page *cabinPage4 = new CabinPage4();
        Page *cabinPage5 = new CabinPage5();
        Page *foodGuide = new FoodGuidePage();

        allPages.push_back(mainPage);
        allPages.push_back(cabinPage1);
        allPages.push_back(cabinPage2);
        allPages.push_back(cabinPage3);
        allPages.push_back(cabinPage4);
        allPages.push_back(cabinPage5);
        allPages.push_back(foodGuide);

        draggableClues.push_back(Cabin3Newspaper);
        draggableClues.push_back(Cabin4Pamphlet);

        updateIcons.push_back(UpdateIcon1);
        updateIcons.push_back(UpdateIcon2);
        updateIcons.push_back(UpdateIcon3);
        updateIcons.push_back(UpdateIcon4);
        updateIcons.push_back(UpdateIcon5);

        RegisterInstance(this);
    }

    ~Book()
    {
        UnregisterInstance(this);
    }

    //----------------------------------- OPEN & CLOSE ------------------------------------

    void drawBook()
    {

        audioManager.PlaySound("openJournal", false);

        openStatus = true;

        for (auto &object : m_gameObjects)
        {

            if (!object->getActiveStatus())
            { // IF NOT ACTIVE SET AS ACTIVE

                object->setActiveStatus(true);
            }
        }

        for (auto &object : allPages)
        {
            object->setActiveStatus(true);
        }

        for (int i = 0; i < 2; i++)
        {

            if (m_journal->getBookClueState(i))
            {
                draggableClues[i]->setActiveStatus(true);
            }
        }

        if (m_journal->GetSceneState())
        {
            BackGround->setActiveStatus(false);
        }

        updateIcons[0]->setActiveStatus(m_journal->UnopenedClueStatus(CABIN1));
        updateIcons[1]->setActiveStatus(m_journal->UnopenedClueStatus(CABIN21));
        updateIcons[2]->setActiveStatus(m_journal->UnopenedClueStatus(CABIN22));
        updateIcons[3]->setActiveStatus(m_journal->UnopenedClueStatus(CABIN3));
        updateIcons[4]->setActiveStatus(m_journal->UnopenedClueStatus(CABIN4));

        // std::cout << "OPEN BOOKK" << std::endl;
    }

    void closeBook()
    {

        // make all gameobjects in book inactive

        audioManager.PlaySound("openJournal", false);

        openStatus = false;
        activePage = currentPage::MAIN_PAGE;

        for (auto &object : m_gameObjects)
        {
            if (object->getActiveStatus())
            { // IF ACTIVE SET AS INACTIVE
                object->setActiveStatus(false);
            }
        }

        for (auto &object : allPages)
        {
            object->setActiveStatus(false);
        }

        for (auto &object : draggableClues)
        {
            object->setActiveStatus(false);
        }


        for (auto &object : updateIcons)
        {
            object->setActiveStatus(false);
        }
        // std::cout << "CLOSE BOOKK" << std::endl;
    }

    void SetKitchenDefaultPage()
    {
        activePage = currentPage::FOODGUIDE_PAGE;
    }

    bool isOpen()
    {
        return openStatus;
    }

    void EmptyFunction()
    {
        return;
    }


    //--------------------------------UPDATE & RENDER-----------------------------------------------------

    virtual void Render() override
    {
        if (openStatus == false)
        {
            return;
        }

        for (auto &object : m_gameObjects)
        {
            if (object->getActiveStatus())
            { // CHECK ACTIVE STATUS
                object->Render();
            }
        }


        allPages[activePage]->Render();

        for (int i = 0; i < 2; i++)
        {

            if (m_journal->getBookClueState(i))
            {
                draggableClues[i]->Render();
            }
        }

        if (m_journal->UnopenedClueStatus(CABIN1))
        {
            updateIcons[0]->Render();
        }
        if (m_journal->UnopenedClueStatus(CABIN21))
        {
            updateIcons[1]->Render();
        }
        if (m_journal->UnopenedClueStatus(CABIN22))
        {
            updateIcons[2]->Render();
        }
        if (m_journal->UnopenedClueStatus(CABIN3))
        {
            updateIcons[3]->Render();
        }
        if (m_journal->UnopenedClueStatus(CABIN4))
        {
            updateIcons[4]->Render();
        }
    }

    virtual void Update(float dt, long frame) override
    {

        if (openStatus == false)
        {
            return;
        }

        for (auto it = m_gameObjects.rbegin(); it != m_gameObjects.rend(); ++it)
        {
            GameObject *object = *it;
            if (object->getActiveStatus())
            { // CHECK ACTIVE STATUS
                object->Update(dt, frame);
            }
        }

        for (int i = 0; i < 2; i++)
        {

            if (m_journal->getBookClueState(i))
            {

                if (draggableClues[i]->getActiveStatus())
                { // CHECK ACTIVE STATUS

                    draggableClues[i]->Update(dt, frame);
                }
            }
        }

        if (m_journal->UnopenedClueStatus(CABIN1))
        {
            updateIcons[0]->Update(dt, frame);
        }
        if (m_journal->UnopenedClueStatus(CABIN21))
        {
            updateIcons[1]->Update(dt, frame);
        }
        if (m_journal->UnopenedClueStatus(CABIN22))
        {
            updateIcons[2]->Update(dt, frame);
        }
        if (m_journal->UnopenedClueStatus(CABIN3))
        {
            updateIcons[3]->Update(dt, frame);
        }
        if (m_journal->UnopenedClueStatus(CABIN4))
        {
            updateIcons[4]->Update(dt, frame);
        }

        if (m_journal->GetBookState())
        { // If book is locked
            CloseJournalButton->setActiveStatus(false);
        }


        allPages[activePage]->Update(dt, frame);
    }

    //-----------------------BOOK UI BUTTON FUNCTIONS-----------------------------------

    void setActiveTab1()
    {
        if (activePage != MAIN_PAGE)
        {
            activePage = currentPage::MAIN_PAGE;
        }
        audioManager.PlaySound("pageSwitch", false);
    };
    void setActiveTab2()
    {
        if (activePage != CABIN1_PAGE)
        {
            activePage = currentPage::CABIN1_PAGE;
        }
        audioManager.PlaySound("pageSwitch", false);
        m_journal->MarkClueAsOpened(CABIN1);
    };

    void setActiveTab3()
    {
        if (activePage != CABIN2_PAGE)
        {
            activePage = currentPage::CABIN2_PAGE;
        }
        audioManager.PlaySound("pageSwitch", false);
        m_journal->MarkClueAsOpened(CABIN21);
    }

    void setActiveTab4()
    {
        if (activePage != CABIN3_PAGE)
        {
            activePage = currentPage::CABIN3_PAGE;
        }
        audioManager.PlaySound("pageSwitch", false);
        m_journal->MarkClueAsOpened(CABIN22);
    }

    void setActiveTab5()
    {
        if (activePage != CABIN4_PAGE)
        {
            activePage = currentPage::CABIN4_PAGE;
        }
        audioManager.PlaySound("pageSwitch", false);
        m_journal->MarkClueAsOpened(CABIN3);
    }

    void setActiveTab6()
    {
        if (activePage != CABIN5_PAGE)
        {
            activePage = currentPage::CABIN5_PAGE;
        }
        audioManager.PlaySound("pageSwitch", false);
        m_journal->MarkClueAsOpened(CABIN4);
    }


    void setActiveTab7()
    {
        if (activePage != FOODGUIDE_PAGE)
        {
            activePage = currentPage::FOODGUIDE_PAGE;
        }
        audioManager.PlaySound("pageSwitch", false);
    };

    void closeJournal()
    {
        if (openStatus == true)
        {
            if (!m_journal->GetBookState())
            { // If book is not locked
                closeBook();
            }
        }
    }


    void ResetJournal()
    {

        m_journal->ResetJournalData();

        for (auto &page : allPages)
        {
            page->ResetPage();
        }
    }

    static void ResetAllJournals()
    {

        for (auto *book : allBooks)
        {
            book->ResetJournal();
        }
    }



protected:
    static std::vector<Book *> allBooks;

    std::vector<GameObject *> m_gameObjects;
    std::vector<GameObject *> draggableClues;
    std::vector<GameObject *> updateIcons;

    std::vector<Page *> allPages;

    currentPage activePage = currentPage::MAIN_PAGE;

    bool openStatus = false; // true - open, false - closed

    glm::vec2 mousePos;

    JournalData *m_journal = JournalData::GetInstance();

    GameObject *BackGround;
    bool lastScene = false;

    GameObject *UpdateIcon1;
    GameObject *UpdateIcon2;
    GameObject *UpdateIcon3;
    GameObject *UpdateIcon4;
    GameObject *UpdateIcon5;


    UIButton *CloseJournalButton;

    static void RegisterInstance(Book *instance)
    {
        allBooks.push_back(instance);
    }

    static void UnregisterInstance(Book *instance)
    {
        allBooks.erase(std::remove(allBooks.begin(), allBooks.end(), instance), allBooks.end());
    }
};

std::vector<Book *> Book::allBooks;