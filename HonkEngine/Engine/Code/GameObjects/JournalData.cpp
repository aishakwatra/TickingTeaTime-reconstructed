#include "JournalData.h"
#include <iostream>

JournalData *JournalData::instance = nullptr;

JournalData::JournalData()
{
    main_page.player_Spy = CABIN_EMPTY;
    main_page.player_BombLocation = Location::LOCATION_EMPTY;
    main_page.player_Evidence = 0;
}

JournalData *JournalData::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new JournalData();
    }
    return instance;
}

void JournalData::incrementEvidence()
{
    main_page.player_Evidence = (main_page.player_Evidence + 1) % 2;
}

void JournalData::setCurrentEvidencetext(UIButton *evidenceButton)
{
    no_of_Evidence = allCabinData[main_page.player_Spy].activeEvidence.size();
    if (no_of_Evidence == 0)
    {
        evidenceButton->SetButtonText("EMPTY");
    }
    else if (no_of_Evidence == 1)
    {
        evidenceButton->SetButtonText(mainPageEvidence[0]);
    }
    else
    {
        evidenceButton->SetButtonText(mainPageEvidence[main_page.player_Evidence]);
    }
}

void JournalData::SetPlayerSpyChoiceInternal(Cabin spyChoice)
{
    main_page.player_Spy = spyChoice;
}

void JournalData::SetPlayerSpyChoice(Cabin spyChoice)
{
    SetPlayerSpyChoiceInternal(spyChoice);
    NotifyObservers();
}

void JournalData::SetPlayerBombLocationInternal(Location bombLocation)
{
    main_page.player_BombLocation = bombLocation;
}

void JournalData::SetPlayerBombLocation(Location bombLocation)
{
    SetPlayerBombLocationInternal(bombLocation);
    NotifyObservers();
}

void JournalData::resetCurrentEvidenceOptions(DeferredRenderObject *buttonObj)
{
    Cabin spy_choice = main_page.player_Spy;
    no_of_Evidence = allCabinData[spy_choice].activeEvidence.size();

    if (no_of_Evidence == 0)
    {
        buttonObj->showObject = false;
        buttonObj->gameObj->setActiveStatus(false);
        mainPageEvidence[0] = " - ";
        mainPageEvidence[1] = " - ";
    }
    else if (no_of_Evidence == 1)
    {
        buttonObj->showObject = true;
        buttonObj->gameObj->setActiveStatus(true);
        mainPageEvidence[0] = allCabinData[spy_choice].activeEvidence.at(0);
        mainPageEvidence[1] = " - ";
    }
    else
    {
        buttonObj->showObject = true;
        buttonObj->gameObj->setActiveStatus(true);
        mainPageEvidence[0] = allCabinData[spy_choice].activeEvidence.at(0);
        mainPageEvidence[1] = allCabinData[spy_choice].activeEvidence.at(1);
    }
    main_page.player_Evidence = 0;
}

bool JournalData::AllChoicesPicked()
{
    return (main_page.player_Spy != CABIN_EMPTY && main_page.player_BombLocation != LOCATION_EMPTY);
}

bool JournalData::GetClueState(Cabin cabin, int clueIndex) const
{
    auto cabinIt = clueStates.find(cabin);
    if (cabinIt != clueStates.end())
    {
        auto clueIt = cabinIt->second.find(clueIndex);
        if (clueIt != cabinIt->second.end())
        {
            return clueIt->second;
        }
    }
    return false;
}

void JournalData::ActivateClue(Cabin cabin, int index)
{
    if (cabin == CABIN3 && index == 10)
    {
        BookClueState[0] = true;
        return;
    }
    if (cabin == CABIN4 && index == 7)
    {
        BookClueState[1] = true;
        return;
    }

    clueStates[cabin][index] = true;

    if (evidenceMap.find(cabin) != evidenceMap.end() && evidenceMap[cabin].find(index) != evidenceMap[cabin].end())
    {
        std::string evidenceText = evidenceMap[cabin][index];
        allCabinData[cabin].activeEvidence.push_back(evidenceText);
    }
    unopenedClues[cabin] = true;
}

void JournalData::addEvidenceToJournal(Cabin cabin, int index, const std::string &evidenceText)
{
    evidenceMap[cabin][index] = evidenceText;
}

int JournalData::checkMainPageEntry()
{
    Cabin spy_choice = main_page.player_Spy;
    Location location_choice = main_page.player_BombLocation;
    int evidence_choice = main_page.player_Evidence;

    bool spyCorrect = false;
    bool locationCorrect = false;
    bool evidenceCorrect = false;

    if (spy_choice == CABIN21)
        spyCorrect = true;

    if (spyCorrect)
    {
        string evidenceChoice = "";
        if (mainPageEvidence->size() > 1)
        {
            evidenceChoice = mainPageEvidence[evidence_choice];
        }
        else if (mainPageEvidence->size() == 1)
        {
            evidenceChoice = mainPageEvidence[0];
        }
        if (evidenceChoice == "Met his sister at National Day Event")
            evidenceCorrect = true;
    }

    if (location_choice == TOWNSQUARE)
        locationCorrect = true;

    if (spyCorrect)
    {
        if (evidenceCorrect && locationCorrect)
            return END1;
        if (evidenceCorrect && !locationCorrect)
            return END2;
        if (!evidenceCorrect && locationCorrect)
            return END3;
        return END4;
    }
    else
    {
        return locationCorrect ? END5 : END6;
    }
}

bool JournalData::getBookClueState(int index)
{
    return BookClueState[index];
}

bool JournalData::GetSceneState()
{
    return LastScene;
}

bool JournalData::GetBookState()
{
    return LockBook;
}

void JournalData::SetBookState(bool status)
{
    LockBook = status;
}

void JournalData::ActivateLastScene()
{
    SetBookState(true);
    LastScene = true;
}

void JournalData::ResetJournalData()
{
    main_page.player_Spy = CABIN_EMPTY;
    main_page.player_BombLocation = LOCATION_EMPTY;
    main_page.player_Evidence = 0;

    allCabinData.clear();
    clueStates.clear();
    evidenceMap.clear();

    mainPageEvidence[0] = " - ";
    mainPageEvidence[1] = " - ";
    no_of_Evidence = 0;

    BookClueState[0] = false;
    BookClueState[1] = false;

    LockBook = false;
    LastScene = false;
    unopenedClues.clear();

    NotifyObservers();
}

bool JournalData::HasUnopenedClue() const
{
    for (const auto &pair : unopenedClues)
    {
        if (pair.second)
        {
            return true;
        }
    }
    return false;
}

void JournalData::MarkClueAsOpened(Cabin cabin)
{
    unopenedClues[cabin] = false;
}

bool JournalData::UnopenedClueStatus(Cabin cabin)
{
    return unopenedClues[cabin];
}