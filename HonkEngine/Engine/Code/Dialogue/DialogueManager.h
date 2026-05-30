#pragma once

#include "tinyxml2.h"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include "../UI/UIButton.h"
#include "../UI/UIElement.h"
#include "../Audio/AudioManager.h"

using namespace std;

struct TempOrderData
{
    std::string roomNumber;
    std::string teaOrder;
    std::string sandwichOrder;
    std::string pastryOrder;
};

struct ClueInfo
{
    Cabin clueID = CABIN_EMPTY; // Default to CABIN_EMPTY assuming it's defined in your enum
    int clueIndex = -1;
    bool isValid = false;
};


struct DialogueChoice
{
    string text;
    string nextDialogueId;
    vector<string> clueIDs; // Store as a vector of strings
    bool hasClue() const
    {
        return !clueIDs.empty();
    }
};


struct Dialogue
{
    string id;
    string speakerName;
    vector<string> text;
    vector<DialogueChoice> choices;
    string next;
    tinyxml2::XMLElement *orderElement = nullptr;
    bool hasOrderData = false; // Add this line
    TempOrderData *tempOrderData = nullptr;
};

inline Cabin GetCabinFromString(const string &cabinStr)
{
    static const map<string, Cabin> cabinMap = { { "CABIN1", CABIN1 }, { "CABIN21", CABIN21 }, { "CABIN22", CABIN22 },
        { "CABIN3", CABIN3 }, { "CABIN4", CABIN4 }, { "CABIN_EMPTY", CABIN_EMPTY } };
    auto it = cabinMap.find(cabinStr);
    return it != cabinMap.end() ? it->second : CABIN_EMPTY;
}

class DialogueManager
{
public:
    DialogueManager(
        const string &name, UIButton *dialogueBox, const string &defaultSprite, const string &defaultSprite2 = "")
        : currentDialogueIndex(0), currentLineIndex(0), currentDialogueButton(dialogueBox), choiceMade(false),
          defaultSpriteName(defaultSprite), defaultSpriteName2(defaultSprite2)
    {
    }

    ~DialogueManager()
    {
        if (currentDialogueButton)
        {
            currentDialogueButton->Clear();
        }
        for (auto &dialogue : dialogues)
        {
            delete dialogue.tempOrderData;
            dialogue.tempOrderData = nullptr; // Ensure pointer is set to nullptr after deletion
        }
    }




    void LoadDialogues(const string &key, const string &filePath)
    {
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
        {
            cerr << "Failed to load dialogue file: " << filePath << endl;
            return;
        }

        tinyxml2::XMLElement *root = doc.FirstChildElement("Dialogues");
        vector<Dialogue> loadedDialogues;
        for (tinyxml2::XMLElement *element = root->FirstChildElement("Dialogue"); element != nullptr; element = element->NextSiblingElement("Dialogue"))
        {
            Dialogue dialogue;
            dialogue.id = element->Attribute("id");
            tinyxml2::XMLElement *speakerElement = element->FirstChildElement("Speaker");
            if (speakerElement)
            {
                dialogue.speakerName = speakerElement->Attribute("name");
                for (tinyxml2::XMLElement *lineElement = speakerElement->FirstChildElement(); lineElement != nullptr; lineElement = lineElement->NextSiblingElement())
                {
                    dialogue.text.push_back(lineElement->GetText());
                }
            }

            tinyxml2::XMLElement *choicesElement = element->FirstChildElement("Choices");
            if (choicesElement)
            {
                for (tinyxml2::XMLElement *choiceElement = choicesElement->FirstChildElement(); choiceElement != nullptr; choiceElement = choiceElement->NextSiblingElement())
                {
                    DialogueChoice choice;
                    choice.text = choiceElement->GetText();
                    choice.nextDialogueId = choiceElement->Attribute("next");
                    const char *clueIDAttr = choiceElement->Attribute("clueID");
                    if (clueIDAttr)
                    {
                        stringstream ss(clueIDAttr);
                        string clueID;
                        while (getline(ss, clueID, ','))
                        {
                            choice.clueIDs.push_back(clueID);
                        }
                    }
                    dialogue.choices.push_back(choice);
                }
            }

            const char *nextAttr = element->Attribute("next");
            if (nextAttr)
            {
                dialogue.next = nextAttr;
            }

            tinyxml2::XMLElement *orderElement = element->FirstChildElement("Order");
            if (orderElement)
            {
                dialogue.orderElement = orderElement;
                dialogue.hasOrderData = true;
                dialogue.tempOrderData = new TempOrderData();
                tinyxml2::XMLElement *roomNumberElement = orderElement->FirstChildElement("RoomNumber");
                if (roomNumberElement)
                    dialogue.tempOrderData->roomNumber = roomNumberElement->GetText();
                tinyxml2::XMLElement *teaElement = orderElement->FirstChildElement("Tea");
                if (teaElement)
                    dialogue.tempOrderData->teaOrder = teaElement->GetText();
                tinyxml2::XMLElement *sandwichElement = orderElement->FirstChildElement("Sandwich");
                if (sandwichElement)
                    dialogue.tempOrderData->sandwichOrder = sandwichElement->GetText();
                tinyxml2::XMLElement *pastryElement = orderElement->FirstChildElement("Pastry");
                if (pastryElement)
                    dialogue.tempOrderData->pastryOrder = pastryElement->GetText();
            }
            loadedDialogues.push_back(dialogue);
        }

        dialogueSets[key] = loadedDialogues; // Store loaded dialogues under the provided key
    }

    void SetDialogueSet(const string &key)
    {
        auto it = dialogueSets.find(key);
        if (it != dialogueSets.end())
        {
            dialogues = it->second;
            currentDialogueIndex = 0; // Reset to start of new dialogue set
            currentLineIndex = 0;
            if (!dialogues.empty() && !dialogues[0].text.empty())
            {
                currentDialogueButton->SetButtonText(dialogues[0].text[0]);
            }
        }
        else
        {
            cerr << "Dialogue set with key '" << key << "' not found." << endl;
        }
    }



    void AddSpeakerIcon(const string &speakerCode, UIElement *icon)
    {
        speakerIcons[speakerCode] = icon;
    }

    void AddSpeakerSprite(const string &speakerCode, UIElement *sprite)
    {
        speakerSprites[speakerCode] = sprite;
        // Activate the sprite if its name matches the default sprite name
        if (speakerCode == defaultSpriteName)
        {
            speakerSprites[speakerCode]->setActiveStatus(true);
        }
        else
        {
            speakerSprites[speakerCode]->setActiveStatus(false);
        }
    }

    void AddSpeakerSprite2(const string &speakerCode, UIElement *sprite)
    {
        speakerSprites2[speakerCode] = sprite;
        // Activate the sprite if its name matches the default sprite name
        if (speakerCode == defaultSpriteName2)
        {
            speakerSprites2[speakerCode]->setActiveStatus(true);
        }
        else
        {
            speakerSprites2[speakerCode]->setActiveStatus(false);
        }
    }

    void HandleClue(const string &clueID)
    {
        size_t underscorePos = clueID.find('_');
        if (underscorePos != string::npos)
        {
            string cabinStr = clueID.substr(0, underscorePos);
            int clueIndex = stoi(clueID.substr(underscorePos + 1));

            Cabin cabinEnum = GetCabinFromString(cabinStr);
            if (cabinEnum != CABIN_EMPTY)
            {
                JournalData::GetInstance()->ActivateClue(cabinEnum, clueIndex);
                cout << "Clue activated: " << clueID << endl;
            }
            else
            {
                cout << "Invalid cabin ID: " << cabinStr << endl;
            }
        }
        else
        {
            cout << "Invalid clue ID format: " << clueID << endl;
        }
    }



    void HandleChoice(int choiceIndex)
    {
        if (choiceIndex >= 0 && choiceIndex < dialogues[currentDialogueIndex].choices.size())
        {
            const auto &choice = dialogues[currentDialogueIndex].choices[choiceIndex];
            audioManager.PlaySound("buttonClick", false);
            std::cout << "Player has chosen: " << choice.text << std::endl; // Add this line to print the chosen choice
            // Activating each clue
            for (const auto &clueID : choice.clueIDs)
            {
                HandleClue(clueID);
            }

            // Find the dialogue with the corresponding nextDialogueId
            auto it = std::find_if(
                dialogues.begin(), dialogues.end(), [&](const Dialogue &d) { return d.id == choice.nextDialogueId; });
            if (it != dialogues.end())
            {
                currentDialogueIndex = std::distance(dialogues.begin(), it);
                currentLineIndex = 0; // Start from the first line of the next dialogue
                // Update the dialogue button text
                currentDialogueButton->SetButtonText(dialogues[currentDialogueIndex].text[currentLineIndex]);
                HideChoices(); // Hide choices for the next dialogue
                isButtonClicked = true;
                choiceMade = true;
            }
        }
    }

    bool isChoicesClicked()
    {
        return isButtonClicked;
    }

    void setChoicesClicked(bool clicked)
    {
        isButtonClicked = clicked;
    }

    void AddChoiceButton(UIButton *choiceButton)
    {
        int choiceIndex = choiceButtons.size(); // Get the index for the new choice button
        choiceButtons.push_back(choiceButton);
        choiceButton->SetOnClickAction(
            [this, choiceIndex]()
            {
                HandleChoice(choiceIndex); // Set the OnClick action to handle the choice
            });
        HideChoices(); // Hide the choices initially
    }

    bool IsCurrentDialogueQuestion()
    {
        return !dialogues[currentDialogueIndex].id.empty() && dialogues[currentDialogueIndex].id[2] == 'Q';
    }


    string GetSpeakerCodeFromId(const string &id)
    {
        if (id.size() >= 2)
        {
            return id.substr(1, 1); // Assuming the speaker code is the second character in the ID
        }
        return ""; // Return empty string if the ID is too short
    }


    void DisplayChoices()
    {
        if (IsCurrentDialogueQuestion())
        {
            choiceMade = false;
            std::cout << "Displaying choices for dialogue " << dialogues[currentDialogueIndex].id << std::endl;
            for (size_t i = 0; i < choiceButtons.size() && i < dialogues[currentDialogueIndex].choices.size(); ++i)
            {
                choiceButtons[i]->SetButtonText(dialogues[currentDialogueIndex].choices[i].text);
                choiceButtons[i]->setActiveStatus(true);
            }
        }
        else
        {
            HideChoices();
        }
    }

    void HideChoices()
    {
        // std::cout << "Hiding choices for dialogue " << dialogues[currentDialogueIndex].id << std::endl;
        for (size_t i = 0; i < choiceButtons.size(); ++i)
        {
            choiceButtons[i]->setActiveStatus(false); // Set the button to inactive
            // std::cout << "Setting choice button " << i << " to inactive." << std::endl;
        }
    }

    void Update(float dt, long frame)
    {
        UpdateSpeakerSprite();
        UpdateSpeakerSprite2();
        UpdateSpeakerIcon();

        if (currentDialogueButton)
        {
            currentDialogueButton->setActiveStatus(true);
        }

        // Get the speaker code for the current dialogue
        string speakerCode = GetSpeakerCodeFromId(dialogues[currentDialogueIndex].id);

        // Render speaker icons
        if (speakerIcons.find(speakerCode) != speakerIcons.end())
        {
            speakerIcons[speakerCode]->setActiveStatus(true);
        }
    }

    void Render()
    {
    }


    bool IsConsequenceDialogue(size_t index)
    {
        // A dialogue is considered a consequence if it's the 'next' dialogue of any choice in the previous dialogues
        for (size_t i = 0; i < index; i++)
        {
            for (const auto &choice : dialogues[i].choices)
            {
                if (choice.nextDialogueId == dialogues[index].id)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void PlayNextDialogue()
    {

        if (currentDialogueIndex >= dialogues.size())
        {
            std::cerr << "No more dialogues available at index: " << currentDialogueIndex << std::endl;
            return; // Safely handle end of dialogues
        }

        // Check if the current dialogue is a question and if a choice has been made
        if (IsCurrentDialogueQuestion() && !choiceMade)
        {
            std::cout << "Please make a choice before continuing." << std::endl;
            return; // Do not advance to the next dialogue
        }

        // Advance to the next line or dialogue
        if (currentLineIndex + 1 < dialogues[currentDialogueIndex].text.size())
        {
            currentLineIndex++;
            audioManager.PlaySound("dialoguePlay");
        }
        else
        {
            // Find the next dialogue based on the 'next' attribute or skip to the next non-consequence dialogue
            size_t nextDialogueIndex = currentDialogueIndex + 1;
            if (!dialogues[currentDialogueIndex].next.empty())
            {
                auto it = std::find_if(dialogues.begin(), dialogues.end(),
                    [&](const Dialogue &d) { return d.id == dialogues[currentDialogueIndex].next; });
                if (it != dialogues.end())
                {
                    nextDialogueIndex = std::distance(dialogues.begin(), it);
                }
            }
            else
            {
                while (nextDialogueIndex < dialogues.size() && IsConsequenceDialogue(nextDialogueIndex))
                {
                    nextDialogueIndex++;
                }
            }

            if (nextDialogueIndex < dialogues.size())
            {
                if (dialogues[currentDialogueIndex].hasOrderData)
                {
                    // Apply the order data
                    SetOrderDataForCurrentDialogue();
                }
                currentDialogueIndex = nextDialogueIndex;
                currentLineIndex = 0;
                choiceMade = false;
            }
            else
            {
                std::cout << "No more dialogues available." << std::endl;

                return;
            }
            audioManager.PlaySound("dialoguePlay");
        }

        // Update the dialogue button text
        if (currentDialogueIndex < dialogues.size() && currentLineIndex < dialogues[currentDialogueIndex].text.size())
        {
            std::cout << "Playing dialogue: " << dialogues[currentDialogueIndex].id << std::endl;
            currentDialogueButton->SetButtonText(dialogues[currentDialogueIndex].text[currentLineIndex]);

            // Display choices if the current dialogue is a question
            if (IsCurrentDialogueQuestion())
            {
                DisplayChoices();
            }
            else
            {
                HideChoices();
            }
        }
        else
        {
            std::cerr << "Error: Invalid dialogue or line index!" << std::endl;
        }
    }

    bool IsDialogueFinished(const string &key) const
    {
        auto it = dialogueSets.find(key);
        if (it != dialogueSets.end() && !it->second.empty())
        {
            const auto &dialogues = it->second;
            bool atLastDialogue = currentDialogueIndex >= dialogues.size() - 1;
            bool atLastLine = currentLineIndex >= dialogues[currentDialogueIndex].text.size() - 1;
            return atLastDialogue && atLastLine;
        }
        return false; // Safely default to false if the dialogue set is not found
    }



    bool IsDialogueSequenceFinished(const string &key)
    {
        if (waitingForPlayerToAcknowledgeEnd)
        {
            return true;
        }
        auto it = dialogueSets.find(key);
        if (it != dialogueSets.end() && !it->second.empty())
        {
            const auto &dialogues = it->second;
            // Check if we are on the last dialogue and the last line of that dialogue
            if (currentDialogueIndex == dialogues.size() - 1 &&
                currentLineIndex == dialogues[currentDialogueIndex].text.size() - 1)
            {
                waitingForPlayerToAcknowledgeEnd = true; // Set the flag when end is reached
                return false;
            }
        }
        return false;
    }

    void PlayerAcknowledgedDialogueEnd()
    {
        waitingForPlayerToAcknowledgeEnd = false; // Reset flag when player moves to next dialogue set
    }


    void SetDialogueVisibility(bool isVisible)
    {
        isDialogueVisible = isVisible; // Update the visibility status

        if (currentDialogueButton)
        {
            currentDialogueButton->setActiveStatus(isVisible);
        }

        if (isVisible && IsCurrentDialogueQuestion())
        {
            DisplayChoices();
        }
        else
        {
            HideChoices();
        }

        UpdateSpeakerIcon(); // Update speaker icons based on the new visibility state
    }



    void PlayNextDialogueSet(const string &nextSetKey)
    {
        if (IsDialogueFinished(nextSetKey))
        {
            SetDialogueSet(nextSetKey);
        }
    }


    void SetDialoguePosition(float x, float y)
    {
        if (currentDialogueButton)
        {
            currentDialogueButton->SetTextPosition(glm::vec3(x, y, 0.0f));
        }
    }

    void SetDialogueBoxPosition(const glm::vec3 &position)
    {
        if (currentDialogueButton)
        {
            currentDialogueButton->SetButtonPosition(position);
        }
    }

    void SetDialogueBoxScale(const glm::vec3 &scale)
    {
        if (currentDialogueButton)
        {
            currentDialogueButton->SetButtonScale(scale);
        }
    }

    void SetDialogueScale(float scale)
    {
        if (currentDialogueButton)
        {
            currentDialogueButton->SetTextSize(scale);
        }
    }

    void SetChoicePosition(float x, float y)
    {
        for (auto &choiceButton : choiceButtons)
        {
            if (choiceButton)
            {
                choiceButton->SetTextPosition(glm::vec3(x, y, 0.0f));
            }
        }
    }

    void SetChoiceScale(float scale)
    {
        for (auto &choiceButton : choiceButtons)
        {
            if (choiceButton)
            {
                choiceButton->SetTextSize(scale);
            }
        }
    }

    void SetOrderDataForCurrentDialogue()
    {
        const Dialogue &currentDialogue = dialogues[currentDialogueIndex];
        if (!currentDialogue.tempOrderData)
        {
            std::cerr << "Error: tempOrderData is null for dialogue index " << currentDialogueIndex << std::endl;
            return;
        }

        OrderData &orderData = OrderData::GetInstance();
        if (!currentDialogue.tempOrderData->roomNumber.empty())
        {
            orderData.SetRoomNumber(currentDialogue.tempOrderData->roomNumber);
            std::cout << "Room Number set to: " << currentDialogue.tempOrderData->roomNumber << std::endl;
        }
        if (!currentDialogue.tempOrderData->teaOrder.empty())
        {
            orderData.SetTeaOrder(currentDialogue.tempOrderData->teaOrder);
            std::cout << "Tea Order set to: " << currentDialogue.tempOrderData->teaOrder << std::endl;
        }
        if (!currentDialogue.tempOrderData->sandwichOrder.empty())
        {
            orderData.SetSandwichOrder(currentDialogue.tempOrderData->sandwichOrder);
            std::cout << "Sandwich Order set to: " << currentDialogue.tempOrderData->sandwichOrder << std::endl;
        }
        if (!currentDialogue.tempOrderData->pastryOrder.empty())
        {
            orderData.SetPastryOrder(currentDialogue.tempOrderData->pastryOrder);
            std::cout << "Pastry Order set to: " << currentDialogue.tempOrderData->pastryOrder << std::endl;
        }
    }

    void HideAllDialogueUI()
    {
        if (currentDialogueButton)
        {
            currentDialogueButton->setActiveStatus(false); // Hide the main dialogue box
        }
        for (auto &choiceButton : choiceButtons)
        {
            choiceButton->setActiveStatus(false); // Hide all choice buttons
        }
        for (auto &pair : speakerIcons)
        {
            pair.second->setActiveStatus(false); // Hide all speaker icons
        }
    }

    void UpdateSpeakerIcon()
    {
        if (!isDialogueVisible)
        {
            // If the dialogue is not visible, ensure all icons are deactivated
            for (auto &pair : speakerIcons)
            {
                pair.second->setActiveStatus(false);
            }
            return; // Exit the function early if the dialogue is not visible
        }

        string speakerCode = GetSpeakerCodeFromId(dialogues[currentDialogueIndex].id);

        // Deactivate all speaker icons first
        for (auto &pair : speakerIcons)
        {
            pair.second->setActiveStatus(false); // Set all to inactive
        }

        // Then activate only the current speaker's icon
        if (speakerIcons.find(speakerCode) != speakerIcons.end())
        {
            speakerIcons[speakerCode]->setActiveStatus(true);
        }
    }


    void Reset()
    {
        currentDialogueIndex = 0;
        currentLineIndex = 0;
        choiceMade = false;
        HideChoices();
    }



private:
    void UpdateSpeakerSprite()
    {
        if (dialogues.empty() || currentDialogueIndex >= dialogues.size())
            return; // Safeguard against empty dialogues or invalid index

        const string &currentSpeaker = dialogues[currentDialogueIndex].speakerName;


        // Check if the current speaker has a specific sprite
        auto spriteIt = speakerSprites.find(currentSpeaker);

        if (spriteIt != speakerSprites.end())
        {
            // Deactivate all sprites first
            for (auto &pair : speakerSprites)
            {
                pair.second->setActiveStatus(false); // Set all to inactive
            }
            // If found, activate the sprite for the current speaker
            spriteIt->second->setActiveStatus(true);
            lastActiveSpeakerSprite = spriteIt->second; // Update last active sprite
        }
        else if (lastActiveSpeakerSprite)
        {
            // If no new speaker sprite is found, continue using the last active sprite
            lastActiveSpeakerSprite->setActiveStatus(true);
        }
    }

    void UpdateSpeakerSprite2()
    {
        if (dialogues.empty() || currentDialogueIndex >= dialogues.size())
            return; // Safeguard against empty dialogues or invalid index

        const string &currentSpeaker = dialogues[currentDialogueIndex].speakerName;


        // Check if the current speaker has a specific sprite
        auto spriteIt = speakerSprites2.find(currentSpeaker);

        if (spriteIt != speakerSprites2.end())
        {
            // Deactivate all sprites first
            for (auto &pair : speakerSprites2)
            {
                pair.second->setActiveStatus(false); // Set all to inactive
            }
            // If found, activate the sprite for the current speaker
            spriteIt->second->setActiveStatus(true);
            lastActiveSpeakerSprite = spriteIt->second; // Update last active sprite
        }
        else if (lastActiveSpeakerSprite)
        {
            // If no new speaker sprite is found, continue using the last active sprite
            lastActiveSpeakerSprite->setActiveStatus(true);
        }
    }




    bool isDialogueVisible = true;
    bool isButtonClicked = false;
    UIElement *lastActiveSpeakerSprite = nullptr; // Pointer to the last active speaker sprite
    map<string, vector<Dialogue>> dialogueSets;
    string defaultSpriteName;
    string defaultSpriteName2;
    vector<Dialogue> dialogues;
    UIButton *currentDialogueButton;
    map<string, UIElement *> speakerIcons;
    map<string, UIElement *> speakerSprites;
    map<string, UIElement *> speakerSprites2;
    vector<UIButton *> choiceButtons;
    bool choiceMade;
    size_t currentDialogueIndex;
    size_t currentLineIndex;
    string fontPath;
    bool waitingForPlayerToAcknowledgeEnd = false;
    AudioManager &audioManager = AudioManager::GetInstance();
};