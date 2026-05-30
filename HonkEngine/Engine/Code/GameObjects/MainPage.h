#pragma once

#include "Page.h"

class MainPage : public Page {

private:

	AudioManager& audioManager;

public:

	MainPage() :Page(MAINPAGE), audioManager(AudioManager::GetInstance()) {

		audioManager.LoadSound("EvidenceWrite", "Assets/Sounds/Journal/SFX_EvidenceWrite1.mp3", SFX, 3.0f);
		audioManager.LoadSound("BombUnderline", "Assets/Sounds/Journal/SFX_BombUnderline3.mp3", SFX, 4.0f);
		audioManager.LoadSound("SpyCircle", "Assets/Sounds/Journal/SFX_SpyCircle.mp3", SFX, 2.5f);

		// DRAGGABLE PAPERS
		UIDraggable* CaseNews = new UIDraggable("CaseNews", "Assets/Images/Journal/CaseSummary_News.png", glm::vec3(-3.2f, 0.0f, 0.0f), glm::vec3(5.34f, 6.85f, 0.0f), true);
		CaseNews->setDragBoundsByObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(12.68f, 7.45f, 1.0f));

		UIDraggable* Ticket = new UIDraggable("Ticket", "Assets/Images/Journal/CaseSummary_Ticket.png", glm::vec3(-3.6f, -2.5f, 1.0f), glm::vec3(4.18f, 1.65f, 0.0f), true);
		Ticket->setDragBoundsByObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(12.68f, 7.45f, 1.0f));

		//JOURNAL PART
		GameObject* JournalSleeve = new UIObject("JounalSleeve", "Assets/Images/Journal/CaseSummary_Sleeve.png", true);
		JournalSleeve->SetScale(glm::vec3(2.68f, 8.09f, 0.0f));
		JournalSleeve->SetPosition(glm::vec3(-5.8f, 0.0f, 0.0f));

		// HEADINGS
		Text* TheSpy = new Text("TheSpy", "The Spy", "Assets/Fonts/mvboli.ttf");
		TheSpy->SetPosition(glm::vec3(0.8f, 2.8f, 0.0f));
		TheSpy->SetColor(glm::vec3(0, 0, 0));
		TheSpy->SetScale(0.85f);

		Text* Evidence = new Text("Evidence", "Evidence", "Assets/Fonts/mvboli.ttf");
		Evidence->SetPosition(glm::vec3(0.8f, -0.5f, 0.0f));
		Evidence->SetColor(glm::vec3(0, 0, 0));
		Evidence->SetScale(0.85f);

		Text* BombLocation = new Text("BombLocation", "Bomb Location", "Assets/Fonts/mvboli.ttf");
		BombLocation->SetPosition(glm::vec3(0.8f, -1.75f, 0.0f));
		BombLocation->SetColor(glm::vec3(0, 0, 0));
		BombLocation->SetScale(0.85f);


		//SPY BUTTONS

		UIButton* Suspect1 = new UIButton("Suspect1", "Assets/Images/Journal/CaseSummary_Icon_Martha.png", glm::vec3(1.3f, 2.05f, 0.0f), glm::vec3(1.05f, 0.99f, 0.0f), true, false, "");
		Suspect1->SetHoverTexture("Assets/Images/Journal/CaseSummary_Icon_Martha_Hover.png");
		Suspect1->SetOnClickAction([this]() { setSpy1(); });

		Text* Cabin1 = new Text("Cabin1", "Cabin 1", "Assets/Fonts/ESA-m.ttf");
		Cabin1->SetPosition(glm::vec3(0.87f, 1.5f, 0.0f));
		Cabin1->SetColor(glm::vec3(0, 0, 0));
		Cabin1->SetScale(0.55f);


		UIButton* Suspect2 = new UIButton("Suspect2", "Assets/Images/Journal/CaseSummary_Icon_Nathaniel.png", glm::vec3(2.75f, 2.02f, 0.0f), glm::vec3(1.05f, 0.99f, 0.0f), true, false, "");
		Suspect2->SetHoverTexture("Assets/Images/Journal/CaseSummary_Icon_Nathaniel_Hover.png");
		Suspect2->SetOnClickAction([this]() { setSpy21(); });

		Text* Cabin21 = new Text("Cabin21", "Cabin 2/1", "Assets/Fonts/ESA-m.ttf");
		Cabin21->SetPosition(glm::vec3(2.25f, 1.5f, 0.0f));
		Cabin21->SetColor(glm::vec3(0, 0, 0));
		Cabin21->SetScale(0.5f);


		UIButton* Suspect3 = new UIButton("Suspect3", "Assets/Images/Journal/CaseSummary_Icon_Evelyn.png", glm::vec3(4.25f, 2.02f, 0.0f), glm::vec3(1.05f, 0.99f, 0.0f), true, false, "");
		Suspect3->SetHoverTexture("Assets/Images/Journal/CaseSummary_Icon_Evelyn_Hover.png");
		Suspect3->SetOnClickAction([this]() { setSpy22(); });

		Text* Cabin22 = new Text("Cabin22", "Cabin 2/2", "Assets/Fonts/ESA-m.ttf");
		Cabin22->SetPosition(glm::vec3(3.75f, 1.5f, 0.0f));
		Cabin22->SetColor(glm::vec3(0, 0, 0));
		Cabin22->SetScale(0.5f);


		UIButton* Suspect4 = new UIButton("Suspect4", "Assets/Images/Journal/CaseSummary_Icon_Archibald.png", glm::vec3(1.4f, 0.75f, 0.0f), glm::vec3(1.05f, 0.99f, 0.0f), true, false, "");
		Suspect4->SetHoverTexture("Assets/Images/Journal/CaseSummary_Icon_Archibald_Hover.png");
		Suspect4->SetOnClickAction([this]() { setSpy3(); });

		Text* Cabin3 = new Text("Cabin3", "Cabin 3", "Assets/Fonts/ESA-m.ttf");
		Cabin3->SetPosition(glm::vec3(0.9f, 0.2f, 0.0f));
		Cabin3->SetColor(glm::vec3(0, 0, 0));
		Cabin3->SetScale(0.5f);


		UIButton* Suspect5 = new UIButton("Suspect5", "Assets/Images/Journal/CaseSummary_Icon_Octavia.png", glm::vec3(2.75f, 0.75f, 0.0f), glm::vec3(1.05f, 0.99f, 0.0f), true, false, "");
		Suspect5->SetHoverTexture("Assets/Images/Journal/CaseSummary_Icon_Octavia_Hover.png");
		Suspect5->SetOnClickAction([this]() { setSpy4(); });

		Text* Cabin4 = new Text("Cabin4", "Cabin 4", "Assets/Fonts/ESA-m.ttf");
		Cabin4->SetPosition(glm::vec3(2.3f, 0.2f, 0.0f));
		Cabin4->SetColor(glm::vec3(0, 0, 0));
		Cabin4->SetScale(0.5f);

		RedCircle = new UIObject("RedCircle", "Assets/Images/Journal/Journal_CaseSummary_Icon_Red_Circle_3.png", true);
		RedCircle->setActiveStatus(false);
		RedCircle->SetScale(glm::vec3(1.44f, 0.44f, 0.0f));
		RedCircle->SetPosition(glm::vec3(1.15f, 1.55f, 0.0f));

		redCircle = new DeferredRenderObject();
		redCircle->showObject = false;
		redCircle->gameObj = RedCircle;

		// BOMB LOCATIONS

		UIButton* TownSquare = new UIButton("TownSquare", "Assets/Images/Journal/CaseSummary_Underline.png", glm::vec3(1.64f, -2.03f, 0.0f), glm::vec3(2.33f * 0.95f, 0.61f * 0.95f, 0.0f), true, true, "Assets/Fonts/ESA-m.ttf");
		TownSquare->SetHoverTexture("Assets/Images/Journal/CaseSummary_Underline_Hover.png");
		TownSquare->SetButtonText("Town Square");
		TownSquare->SetTextSize(0.6f);
		TownSquare->SetTextPosition(glm::vec3(1.57f, -2.25f, 0.0f));
		TownSquare->SetOnClickAction([this]() { setLocationTownSquare(); });

		UIButton* TheHolyChurch = new UIButton("TheHolyChurch", "Assets/Images/Journal/CaseSummary_Underline.png", glm::vec3(4.05f, -2.05f, 0.0f), glm::vec3(2.33f * 0.95f, 0.61f * 0.95f, 0.0f), true, true, "Assets/Fonts/ESA-m.ttf");
		TheHolyChurch->SetHoverTexture("Assets/Images/Journal/CaseSummary_Underline_Hover.png");
		TheHolyChurch->SetButtonText("The Holy Church");
		TheHolyChurch->SetTextPosition(glm::vec3(3.75f, -2.25f, 0.0f));
		TheHolyChurch->SetTextSize(0.6f);
		TheHolyChurch->SetOnClickAction([this]() { setLocationHolyChurch(); });

		UIButton* TheCouncil = new UIButton("TheCouncil", "Assets/Images/Journal/CaseSummary_Underline.png", glm::vec3(1.48f, -2.65f, 0.0f), glm::vec3(2.33f * 0.95f, 0.61f * 0.95f, 0.0f), true, true, "Assets/Fonts/ESA-m.ttf");
		TheCouncil->SetHoverTexture("Assets/Images/Journal/CaseSummary_Underline_Hover.png");
		TheCouncil->SetButtonText("The Council");
		TheCouncil->SetTextSize(0.6f);
		TheCouncil->SetTextPosition(glm::vec3(1.48f, -2.75f, 0.0f));
		TheCouncil->SetOnClickAction([this]() { setLocationCouncil(); });

		UIButton* SupremeCourt = new UIButton("SupremeCourt", "Assets/Images/Journal/CaseSummary_Underline.png", glm::vec3(3.95f, -2.65f, 0.0f), glm::vec3(2.33f * 0.95f, 0.61f * 0.95f, 0.0f), true, true, "Assets/Fonts/ESA-m.ttf");
		SupremeCourt->SetHoverTexture("Assets/Images/Journal/CaseSummary_Underline_Hover.png");
		SupremeCourt->SetButtonText("Supreme Court");
		SupremeCourt->SetTextSize(0.6f);
		SupremeCourt->SetTextPosition(glm::vec3(3.75f, -2.75f, 0.0f));
		SupremeCourt->SetOnClickAction([this]() { setLocationSupremeCourt(); });

		RedUnderline = new UIObject("RedUnderline", "Assets/Images/Journal/CaseSummary_Icon_Red_Underline.png", true);
		RedUnderline->setActiveStatus(false);
		RedUnderline->SetScale(glm::vec3(1.77f, 0.13f, 0.0f));
		RedUnderline->SetPosition(glm::vec3(1.1f, -2.0f, 0.0f));

		redUnderline = new DeferredRenderObject();
		redUnderline->showObject = false;
		redUnderline->gameObj = RedUnderline;

		// EVIDENCE TEXT

		/*EvidenceButton = new UIButtonEmpty("EvidenceText", glm::vec3(3.2f, -1.0f, 0.0f), glm::vec3(5.0f, 0.5f, 0.0f), true, true, "Assets/Fonts/ESA-smb.ttf");
		EvidenceButton->SetTextSize(0.55f);
		EvidenceButton->SetTextColor(glm::vec3(0.58, 0.04, 0.00));
		EvidenceButton->SetOnClickAction([this]() { clickEvidenceButton(); });*/

		EvidenceButton = new UIButton("EvidenceText", "Assets/Images/Journal/CaseSummary_Evidence.png", glm::vec3(3.18f, -0.47f, 0.0f), glm::vec3(4.75f * 0.95f, 0.92f * 0.95f, 0.0f), true, true, "Assets/Fonts/ESA-smb.ttf");
		EvidenceButton->SetTextSize(0.55f);
		EvidenceButton->SetTextColor(glm::vec3(0.58, 0.04, 0.00));
		EvidenceButton->SetTextPosition(glm::vec3(3.2f, -1.0f, 0.0f));
		EvidenceButton->SetOnClickAction([this]() { clickEvidenceButton(); });
		EvidenceButton->SetHoverTexture("Assets/Images/Journal/CaseSummary_Evidence_Hover.png");

		evidenceButtonDeferred = new DeferredRenderObject();
		evidenceButtonDeferred->showObject = false;
		evidenceButtonDeferred->gameObj = EvidenceButton;

		m_gameObjects.push_back(TheSpy);

		m_gameObjects.push_back(Cabin1);
		m_gameObjects.push_back(Suspect1);

		m_gameObjects.push_back(Cabin21);
		m_gameObjects.push_back(Suspect2);

		m_gameObjects.push_back(Cabin22);
		m_gameObjects.push_back(Suspect3);

		m_gameObjects.push_back(Cabin3);
		m_gameObjects.push_back(Suspect4);

		m_gameObjects.push_back(Cabin4);
		m_gameObjects.push_back(Suspect5);

		m_gameObjects.push_back(Evidence);

		m_gameObjects.push_back(BombLocation);

		m_gameObjects.push_back(TownSquare);
		m_gameObjects.push_back(TheHolyChurch);
		m_gameObjects.push_back(TheCouncil);
		m_gameObjects.push_back(SupremeCourt);

		m_gameObjects.push_back(CaseNews);
		m_gameObjects.push_back(Ticket);
		m_gameObjects.push_back(JournalSleeve);


		//push back deferred rendering

		deffered_m_gameObjects.push_back(redUnderline);
		deffered_m_gameObjects.push_back(redCircle);
		deffered_m_gameObjects.push_back(evidenceButtonDeferred);

		JournalData::GetInstance()->AddObserver([this]() { UpdateFromJournalData(); });

		updateEvidenceButtonText();

	}

	virtual void Render() override
	{

		// Render the first 17 objects
		for (size_t i = 0; i < 17 && i < m_gameObjects.size(); ++i) {
			if (m_gameObjects[i]->getActiveStatus()) {
				m_gameObjects[i]->Render();
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

		// Render the last 4 objects
		size_t startIdx = m_gameObjects.size() > 4 ? m_gameObjects.size() - 4 : 0;
		for (size_t i = startIdx; i < m_gameObjects.size(); ++i) {
			if (m_gameObjects[i]->getActiveStatus()) {
				m_gameObjects[i]->Render();
			}
		}


	}


	void UpdateFromJournalData() {

		MainPageData& data = m_journalData->GetMainPageData();

		// Update spy choice UI
		UpdateSpyChoice(data.player_Spy);

		// Update bomb location UI
		UpdateBombLocation(data.player_BombLocation);

		m_journalData->updateCurrentEvidenceOptions(evidenceButtonDeferred, false);
		m_journalData->setCurrentEvidencetext(EvidenceButton);

	}

	void UpdateSpyChoice(Cabin cabin) {
		switch (cabin) {
		case CABIN1:
			SetSpyChoiceUI(CABIN1, glm::vec3(1.15f, 1.55f, 0.0f));
			break;
		case CABIN21:
			SetSpyChoiceUI(CABIN21, glm::vec3(2.65f, 1.55f, 0.0f));
			break;
		case CABIN22:
			SetSpyChoiceUI(CABIN22, glm::vec3(4.15f, 1.55f, 0.0f));
			break;
		case CABIN3:
			SetSpyChoiceUI(CABIN3, glm::vec3(1.15f, 0.25f, 0.0f));
			break;
		case CABIN4:
			SetSpyChoiceUI(CABIN4, glm::vec3(2.65f, 0.25f, 0.0f));
			break;
		default:
			break;
		}
	}

	void UpdateBombLocation(Location location) {
		switch (location) {
		case TOWNSQUARE:
			SetBombLocationUI(TOWNSQUARE, glm::vec3(1.65f, -2.3f, 0.0f));
			break;
		case HOLYCHURCH:
			SetBombLocationUI(HOLYCHURCH, glm::vec3(3.6f, -2.36f, 0.0f));
			break;
		case COUNCIL:
			SetBombLocationUI(COUNCIL, glm::vec3(1.64f, -2.83f, 0.0f));
			break;
		case SUPREMECOURT:
			SetBombLocationUI(SUPREMECOURT, glm::vec3(3.62f, -2.85f, 0.0f));
			break;
		default:
			break;
		}
	}

	// Separate functions to update the UI without causing recursion
	void SetSpyChoiceUI(Cabin spyChoice, glm::vec3 position) {
		if (redCircle->showObject == false) {
			redCircle->showObject = true;
		}
		redCircle->gameObj->SetPosition(position);
	}

	void SetBombLocationUI(Location bombLocation, glm::vec3 position) {
		if (redUnderline->showObject == false) {
			redUnderline->showObject = true;
		}
		redUnderline->gameObj->SetPosition(position);
	}

	// In your button click functions, just set the state
	void setSpy1() {
		m_journalData->SetPlayerSpyChoice(CABIN1);
		updateEvidenceButtonText();
		audioManager.PlaySound("SpyCircle");
	}

	void setSpy21() {
		m_journalData->SetPlayerSpyChoice(CABIN21);
		updateEvidenceButtonText();
		audioManager.PlaySound("SpyCircle");
	}

	void setSpy22() {
		m_journalData->SetPlayerSpyChoice(CABIN22);
		updateEvidenceButtonText();
		audioManager.PlaySound("SpyCircle");
	}

	void setSpy3() {
		m_journalData->SetPlayerSpyChoice(CABIN3);
		updateEvidenceButtonText();
		audioManager.PlaySound("SpyCircle");
	}

	void setSpy4() {
		m_journalData->SetPlayerSpyChoice(CABIN4);
		updateEvidenceButtonText();
		audioManager.PlaySound("SpyCircle");
	}

	void setLocationTownSquare() {
		m_journalData->SetPlayerBombLocation(TOWNSQUARE);
		audioManager.PlaySound("BombUnderline");
	}

	void setLocationHolyChurch() {
		m_journalData->SetPlayerBombLocation(HOLYCHURCH);
		audioManager.PlaySound("BombUnderline");
	}

	void setLocationCouncil() {
		m_journalData->SetPlayerBombLocation(COUNCIL);
		audioManager.PlaySound("BombUnderline");
	}

	void setLocationSupremeCourt() {
		m_journalData->SetPlayerBombLocation(SUPREMECOURT);
		audioManager.PlaySound("BombUnderline");
	}

	//--------------------- SUSPECT BUTTON FUNCTIONS -----------------------

	void updateEvidenceButtonText() {

		//every time spy is rechosen
		//reset the options
		//reset button text

		m_journalData->resetCurrentEvidenceOptions(evidenceButtonDeferred);
		m_journalData->setCurrentEvidencetext(EvidenceButton);

	}

	void clickEvidenceButton() {

		//increment choice
		//set new text for button

		std::cout << "CLICKING BUTTON" << std::endl;

		m_journalData->incrementEvidence();
		m_journalData->setCurrentEvidencetext(EvidenceButton);
		audioManager.PlaySound("EvidenceWrite");


	}



	virtual void Update(float dt, long frame) override {
		Page::Update(dt, frame);


	}


private:

	UIObject* RedCircle;
	UIObject* RedUnderline;

	DeferredRenderObject* redUnderline;
	DeferredRenderObject* redCircle;
	DeferredRenderObject* evidenceButtonDeferred;

	//UIButtonEmpty* EvidenceButton;
	UIButton* EvidenceButton;

};
