#pragma once

#include "../Text/Text.h"
#include "../UI/UIButtonEmpty.h"
#include <vector>
#include <map>
#include <string>
#include <functional>

using namespace std;

enum Location
{
    TOWNSQUARE,
    HOLYCHURCH,
    COUNCIL,
    SUPREMECOURT,
    LOCATION_EMPTY
};
enum Cabin
{
    CABIN1,
    CABIN21,
    CABIN22,
    CABIN3,
    CABIN4,
    CABIN_EMPTY,
    MAINPAGE,
    FOODGUIDE
};
enum Ending
{
    END1,
    END2,
    END3,
    END4,
    END5,
    END6
};

struct ClueData
{
    bool showClue;
    GameObject *clueObject = nullptr;
    bool isEvidence = false;
};

struct DeferredRenderObject
{
    bool showObject;
    GameObject *gameObj;
};

struct MainPageData
{
    Cabin player_Spy = CABIN_EMPTY;
    Location player_BombLocation = LOCATION_EMPTY;
    int player_Evidence = 0;
};

struct CabinPageData
{
    std::vector<ClueData *> textClues;
    std::vector<std::string> activeEvidence;
};

class JournalData
{
    friend class JournalSaveSystem;

public:
    using JournalObserver = std::function<void()>;

    static JournalData *GetInstance();

    void AddObserver(JournalObserver observer)
    {
        observers.push_back(observer);
    }

    void NotifyObservers()
    {
        for (auto &observer : observers)
        {
            observer();
        }
    }

    void incrementEvidence();
    void setCurrentEvidencetext(UIButton *evidenceButton);
    void SetPlayerSpyChoiceInternal(Cabin spyChoice);
    void SetPlayerSpyChoice(Cabin spyChoice);
    void SetPlayerBombLocationInternal(Location bombLocation);
    void SetPlayerBombLocation(Location bombLocation);
    void updateCurrentEvidenceOptions(DeferredRenderObject *buttonObj, bool resetEvidenceSelection);
    void resetCurrentEvidenceOptions(DeferredRenderObject *buttonObj);
    bool AllChoicesPicked();
    bool GetClueState(Cabin cabin, int clueIndex) const;
    void ActivateClue(Cabin cabin, int index);
    void addEvidenceToJournal(Cabin cabin, int index, const std::string &evidenceText);

    MainPageData &GetMainPageData()
    {
        return main_page;
    }
    CabinPageData &GetCabinPageData(Cabin cabin)
    {
        return allCabinData[cabin];
    }

    int checkMainPageEntry();
    bool getBookClueState(int index);
    bool GetSceneState();
    bool GetBookState();
    void SetBookState(bool status);
    void ActivateLastScene();
    void ResetJournalData();
    bool HasUnopenedClue() const;
    void MarkClueAsOpened(Cabin cabin);
    bool UnopenedClueStatus(Cabin cabin);

private:
    static JournalData *instance;
    JournalData();

    MainPageData main_page;
    std::map<Cabin, CabinPageData> allCabinData;
    std::map<Cabin, std::map<int, bool>> clueStates;
    std::map<Cabin, std::map<int, std::string>> evidenceMap;

    std::string mainPageEvidence[2] = { " - ", " - " };
    int no_of_Evidence = 0;

    bool BookClueState[2] = { false, false };
    bool LastScene = false;
    bool LockBook = false;

    std::vector<JournalObserver> observers;
    std::map<Cabin, bool> unopenedClues;
};

extern JournalData *instance;
