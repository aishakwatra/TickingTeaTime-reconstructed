#include "JournalSaveSystem.h"
#include "../GameObjects/JournalData.h"
#include "../Application.h"
#include "../Dialogue/tinyxml2.h"
#include <iostream>


using namespace tinyxml2;

bool JournalSaveSystem::SaveJournalData(const std::string &filepath)
{
    JournalData *journal = JournalData::GetInstance();
    if (!journal)
        return false;

    XMLDocument doc;
    XMLDeclaration *decl = doc.NewDeclaration();
    doc.InsertFirstChild(decl);

    // Root node: <SaveGame>
    XMLElement *root = doc.NewElement("SaveGame");
    doc.InsertEndChild(root);

    // =================================================================
    // 1. PROGRESSION NODE: <Progression>
    // =================================================================
    XMLElement *progression = doc.NewElement("Progression");
    root->InsertEndChild(progression);

    XMLElement *gameState = doc.NewElement("CurrentGameState");
    gameState->SetText(0);
    progression->InsertEndChild(gameState);

    XMLElement *roomState = doc.NewElement("CurrentRoomState");
    roomState->SetText(0);
    progression->InsertEndChild(roomState);

    Camera &camera = Application::GetCamera();
    XMLElement *playerX = doc.NewElement("PlayerX");
    playerX->SetText(camera.GetPosX());
    progression->InsertEndChild(playerX);

    XMLElement *playerY = doc.NewElement("PlayerY");
    playerY->SetText(camera.GetPosY());
    progression->InsertEndChild(playerY);

    // =================================================================
    // 2. JOURNAL NODE: <Journal>
    // =================================================================
    XMLElement *journalElem = doc.NewElement("Journal");
    root->InsertEndChild(journalElem);

    // Get public reference to MainPageData
    MainPageData &mainPageRef = journal->GetMainPageData();

    // <MainPage>
    XMLElement *mainPage = doc.NewElement("MainPage");
    journalElem->InsertEndChild(mainPage);

    XMLElement *playerSpy = doc.NewElement("PlayerSpy");
    playerSpy->SetText(static_cast<int>(mainPageRef.player_Spy));
    mainPage->InsertEndChild(playerSpy);

    XMLElement *bombLocation = doc.NewElement("BombLocation");
    bombLocation->SetText(static_cast<int>(mainPageRef.player_BombLocation));
    mainPage->InsertEndChild(bombLocation);

    XMLElement *playerEvidence = doc.NewElement("PlayerEvidence");
    playerEvidence->SetText(mainPageRef.player_Evidence);
    mainPage->InsertEndChild(playerEvidence);

    // <ClueStates>
    XMLElement *clueStatesRoot = doc.NewElement("ClueStates");
    journalElem->InsertEndChild(clueStatesRoot);

    // Explicitly iterate through defined Cabins using public GetClueState method
    std::vector<Cabin> targetCabins = { CABIN1, CABIN21, CABIN22, CABIN3, CABIN4, FOODGUIDE };

    for (Cabin cabinId : targetCabins)
    {
        XMLElement *cabinElem = nullptr;

        // Loop through reasonable clue indexes (e.g., 0 to 20) to see if any are active
        for (int clueIdx = 0; clueIdx <= 20; ++clueIdx)
        {
            if (journal->GetClueState(cabinId, clueIdx))
            {
                if (!cabinElem)
                {
                    cabinElem = doc.NewElement("Cabin");
                    cabinElem->SetAttribute("id", static_cast<int>(cabinId));
                    clueStatesRoot->InsertEndChild(cabinElem);
                }

                XMLElement *clueElem = doc.NewElement("Clue");
                clueElem->SetAttribute("index", clueIdx);
                clueElem->SetText("true");
                cabinElem->InsertEndChild(clueElem);
            }
        }
    }

    // <ActiveEvidence>
    XMLElement *activeEvidenceRoot = doc.NewElement("ActiveEvidence");
    journalElem->InsertEndChild(activeEvidenceRoot);

    for (Cabin cabinId : targetCabins)
    {
        CabinPageData &cabinData = journal->GetCabinPageData(cabinId);
        if (!cabinData.activeEvidence.empty())
        {
            XMLElement *cabinElem = doc.NewElement("Cabin");
            cabinElem->SetAttribute("id", static_cast<int>(cabinId));
            activeEvidenceRoot->InsertEndChild(cabinElem);

            for (const auto &evidenceStr : cabinData.activeEvidence)
            {
                XMLElement *evElem = doc.NewElement("Evidence");
                evElem->SetText(evidenceStr.c_str());
                cabinElem->InsertEndChild(evElem);
            }
        }
    }

    // <BookStatus>
    XMLElement *bookStatus = doc.NewElement("BookStatus");
    journalElem->InsertEndChild(bookStatus);

    XMLElement *bookClue0 = doc.NewElement("BookClue");
    bookClue0->SetAttribute("index", 0);
    bookClue0->SetText(journal->getBookClueState(0) ? "true" : "false");
    bookStatus->InsertEndChild(bookClue0);

    XMLElement *bookClue1 = doc.NewElement("BookClue");
    bookClue1->SetAttribute("index", 1);
    bookClue1->SetText(journal->getBookClueState(1) ? "true" : "false");
    bookStatus->InsertEndChild(bookClue1);

    XMLElement *lockBook = doc.NewElement("LockBook");
    lockBook->SetText(journal->GetBookState() ? "true" : "false");
    bookStatus->InsertEndChild(lockBook);

    XMLElement *lastScene = doc.NewElement("LastScene");
    lastScene->SetText(journal->GetSceneState() ? "true" : "false");
    bookStatus->InsertEndChild(lastScene);

    return (doc.SaveFile(filepath.c_str()) == XML_SUCCESS);
}

bool JournalSaveSystem::LoadJournalData(const std::string &filepath)
{
    JournalData *journal = JournalData::GetInstance();
    if (!journal)
        return false;

    XMLDocument doc;
    if (doc.LoadFile(filepath.c_str()) != XML_SUCCESS)
        return false;

    XMLElement *root = doc.FirstChildElement("SaveGame");
    if (!root)
        return false;

    journal->ResetJournalData();

    // 1. Process Progression Elements
    XMLElement *progression = root->FirstChildElement("Progression");
    if (progression)
    {
        XMLElement *playerXElem = progression->FirstChildElement("PlayerX");
        XMLElement *playerYElem = progression->FirstChildElement("PlayerY");

        if (playerXElem && playerYElem)
        {
            float posX = playerXElem->FloatText();
            float posY = playerYElem->FloatText();
            Camera &camera = Application::GetCamera();
            // camera.SetPosition(posX, posY);
        }
    }

    XMLElement *journalElem = root->FirstChildElement("Journal");
    if (!journalElem)
        return true;

    // 2. Parse <MainPage> Data via public methods
    XMLElement *mainPage = journalElem->FirstChildElement("MainPage");
    if (mainPage)
    {
        XMLElement *spy = mainPage->FirstChildElement("PlayerSpy");
        XMLElement *bomb = mainPage->FirstChildElement("BombLocation");
        XMLElement *evidence = mainPage->FirstChildElement("PlayerEvidence");

        if (spy)
            journal->SetPlayerSpyChoiceInternal(static_cast<Cabin>(spy->IntText()));
        if (bomb)
            journal->SetPlayerBombLocationInternal(static_cast<Location>(bomb->IntText()));
        if (evidence)
            journal->GetMainPageData().player_Evidence = evidence->IntText();
    }

    // 3. Loop through Clue States via public method interface
    XMLElement *clueStatesRoot = journalElem->FirstChildElement("ClueStates");
    if (clueStatesRoot)
    {
        XMLElement *cabinElem = clueStatesRoot->FirstChildElement("Cabin");
        while (cabinElem)
        {
            Cabin cabinId = static_cast<Cabin>(cabinElem->IntAttribute("id"));

            XMLElement *clueElem = cabinElem->FirstChildElement("Clue");
            while (clueElem)
            {
                int index = clueElem->IntAttribute("index");
                bool val = clueElem->BoolText();

                if (val)
                {
                    journal->ActivateClue(cabinId, index);
                }

                clueElem = clueElem->NextSiblingElement("Clue");
            }
            cabinElem = cabinElem->NextSiblingElement("Cabin");
        }
    }

    // 4. Extract Active Evidence via public page reference
    XMLElement *activeEvidenceRoot = journalElem->FirstChildElement("ActiveEvidence");
    if (activeEvidenceRoot)
    {
        XMLElement *cabinElem = activeEvidenceRoot->FirstChildElement("Cabin");
        while (cabinElem)
        {
            Cabin cabinId = static_cast<Cabin>(cabinElem->IntAttribute("id"));
            CabinPageData &cabinData = journal->GetCabinPageData(cabinId);

            XMLElement *evElem = cabinElem->FirstChildElement("Evidence");
            while (evElem)
            {
                const char *text = evElem->GetText();
                if (text)
                {
                    cabinData.activeEvidence.push_back(std::string(text));
                }
                evElem = evElem->NextSiblingElement("Evidence");
            }
            cabinElem = cabinElem->NextSiblingElement("Cabin");
        }
    }

    // 5. Restore Book Status Variables via public methods
    XMLElement *bookStatus = journalElem->FirstChildElement("BookStatus");
    if (bookStatus)
    {
        XMLElement *bookClueElem = bookStatus->FirstChildElement("BookClue");
        while (bookClueElem)
        {
            int index = bookClueElem->IntAttribute("index");
            bool val = bookClueElem->BoolText();
            if (val)
            {
                // Simulates unlocking the draggable items securely
                journal->ActivateClue(index == 0 ? CABIN3 : CABIN4, index == 0 ? 10 : 7);
            }
            bookClueElem = bookClueElem->NextSiblingElement("BookClue");
        }

        XMLElement *lockBook = bookStatus->FirstChildElement("LockBook");
        if (lockBook)
            journal->SetBookState(lockBook->BoolText());

        XMLElement *lastScene = bookStatus->FirstChildElement("LastScene");
        if (lastScene && lastScene->BoolText())
            journal->ActivateLastScene();
    }

    // Synchronize UI updates safely
    journal->NotifyObservers();
    return true;
}

float JournalSaveSystem::GetSavedPlayerX(const std::string &filepath)
{
    tinyxml2::XMLDocument doc;

    // If the file fails to load or doesn't exist, return -5.0f (your default!)
    if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS)
    {
        return -5.0f;
    }

    // Navigate to <SaveGame> -> <Progression> -> <PlayerX>
    tinyxml2::XMLElement *root = doc.FirstChildElement("SaveGame");
    if (root)
    {
        tinyxml2::XMLElement *progression = root->FirstChildElement("Progression");
        if (progression)
        {
            tinyxml2::XMLElement *playerXElem = progression->FirstChildElement("PlayerX");
            if (playerXElem)
            {
                // Return the float value stored in the XML tag
                return playerXElem->FloatText(-5.0f);
            }
        }
    }

    // Fallback if the tags are missing or malformed
    return -5.0f;
}