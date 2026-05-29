#pragma once

#include "GameObject.h"
#include "../Engine.h"
#include "../Input/Input.h"
#include <iostream>
#include "../Application.h"

#include "../Text/Text.h"
#include "../UI/UIButton.h"
#include "../UI/UIDraggable.h"
#include "../UI/UIButtonEmpty.h"

#include "JournalData.h"


class Page : public GameObject {

public:


	Page(Cabin cabinType) : GameObject("page"), cabinType(cabinType) {}


	virtual void Render() override
	{

		for (auto& object : m_gameObjects) {

			if (object->getActiveStatus()) { //CHECK ACTIVE STATUS

				object->Render();

			}

		}

		for (size_t i = 0; i < textClues.size(); ++i) {

			if (m_journalData->GetClueState(cabinType, i)) {

				textClues[i]->Render();

			}
		}

		for (auto& object : deffered_m_gameObjects) {

			if (object->showObject) { //CHECK ACTIVE STATUS

				object->gameObj->Render();

			}

		}


	}

	virtual void Update(float dt, long frame) override {

		//Update from last object to first
		//top layer to bottom

		for (auto it = m_gameObjects.rbegin(); it != m_gameObjects.rend(); ++it) {

			GameObject* object = *it;
			if (object->getActiveStatus()) { // CHECK ACTIVE STATUS
				object->Update(dt, frame);
			}

		}

		for (auto& object : textClues) {

			object->Update(dt, frame);

		}

		for (auto& object : deffered_m_gameObjects) {

			if (object->showObject) {
				object->gameObj->Update(dt, frame);
			}

		}

	}

	virtual void setActiveStatus(bool status) {

		for (auto& object : m_gameObjects) {
			object->setActiveStatus(status);
		}

		for (auto& object : textClues) {

			if (object) { //checks for nullptr		

			}
		}

		for (auto& object : deffered_m_gameObjects) {

			if (object->gameObj) { //checks for nullptr

				if (object->showObject) {
					object->gameObj->setActiveStatus(status); //sets status accordingly if clue is active
				}
				else {
					object->gameObj->setActiveStatus(false); //if inactive always set as false
				}

			}
		}

		active = status;

	}

	virtual void ResetPage()
    {

        // 1. Synchronize all deferred graphical game objects (checkmarks, stamps, images)
        for (size_t i = 0; i < deffered_m_gameObjects.size(); ++i)
        {
            if (deffered_m_gameObjects[i] && deffered_m_gameObjects[i]->gameObj)
            {

                // Ask JournalData if this specific indexed clue for this cabin is unlocked
                bool isClueUnlocked = m_journalData->GetClueState(cabinType, static_cast<int>(i));

                // Synchronize the render visibility flag
                deffered_m_gameObjects[i]->showObject = isClueUnlocked;

                // Match the core engine active status with whether the page itself is currently open
                if (this->active && isClueUnlocked)
                {
                    deffered_m_gameObjects[i]->gameObj->setActiveStatus(true);
                }
                else
                {
                    deffered_m_gameObjects[i]->gameObj->setActiveStatus(false);
                }
            }
        }

        // 2. Synchronize your Text clues active status fields if necessary
        for (size_t i = 0; i < textClues.size(); ++i)
        {
            if (textClues[i])
            {
                bool isClueUnlocked = m_journalData->GetClueState(cabinType, static_cast<int>(i));
                if (this->active && isClueUnlocked)
                {
                    textClues[i]->setActiveStatus(true);
                }
                else
                {
                    textClues[i]->setActiveStatus(false);
                }
            }
        }
    }


protected:

	Cabin cabinType;

	std::vector<GameObject*> m_gameObjects;
	std::vector<GameObject*> textClues;
	std::vector<DeferredRenderObject*> deffered_m_gameObjects;

	JournalData* m_journalData = JournalData::GetInstance();

};