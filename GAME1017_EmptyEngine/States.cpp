#include "States.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "PlatformPlayer.h"
#include "CollisionManager.h"
#include "Button3.h"

#include <iostream>

#include "MainPlayer.h"
#include "SoundManager.h"
#include "TitleSprite.h"
using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void State::Resume(){}

// Begin TitleState
TitleState::TitleState():m_titleSprite({ 0,0,768,576 }){}

void TitleState::Enter()
{
	cout << "Entering TitleState..." << endl;
	TEMA::Load("Img/button.png", "play");
	TEMA::Load("Img/TitleSprite.png", "title");
	
	//m_objects.emplace("title", new TitleSprite({ 0,0,768,576 }));
	m_objects.emplace("play", new PlayButton({ 0,0,300,100 }, { 180,370,390,90 }, "play"));
	//new TitleSprite({ 0,0,768,576 })
	
}

void TitleState::Update()
{
	//if (EVMA::KeyPressed(SDL_SCANCODE_N))
	//	STMA::ChangeState(new GameState());// Change to new GameState
	
	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
}

void TitleState::Render()
{
	//cout << "Rendering TitleState..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	/*SDL_Rect m_src = { 0,0,768,576 };
	SDL_Rect m_dst = m_src;
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("title"), &m_src, &m_dst);*/
	m_titleSprite.Render();
	for (auto const& i : m_objects)
		i.second->Render();
	State::Render();
}

void TitleState::Exit()
{
	cout << "Exiting TitleState..." << endl;
	TEMA::Unload("play");
	TEMA::Unload("title");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
}
// End TitleState

// Begin GameState
GameState::GameState(){}

void GameState::Enter() // Used for initialization.
{
	cout << "Entering GameState..." << endl;
	//TEMA::Load("Img/Tiles.png", "tiles");
	//TEMA::Load("Img/Player.png", "player");
	//m_objects.emplace("level", new TiledLevel(24, 32, 32, 32, "Dat/Tiledata.txt", "Dat/Level1.txt", "tiles"));
	//m_objects.emplace("player", new PlatformPlayer({ 0,0,128,128 }, { 288,480,64,64 }));
	//TEMA::Load("../Assets/textures/bomb.png", "bomb");
	
	TEMA::Load("Img/Enemy3.png", "skeleton");
	TEMA::Load("Img/Enemy1.png", "bat");
	TEMA::Load("Img/Enemy2.png", "fire");
	TEMA::Load("Img/PlatfEnemy.png", "platf");
	
	SoundManager::Instance().load("Audio/backgroundSong.wav", "BGSong", SOUND_MUSIC);
	SoundManager::Instance().playMusic("BGSong", -1, 0);
	SoundManager::Instance().setMusicVolume(40);

	SoundManager::Instance().load("Audio/hitSound.wav", "hit", SOUND_SFX);
	SoundManager::Instance().load("Audio/jumpSound.wav", "jump", SOUND_SFX);
	

	
	TEMA::Load("Img/MainPlayer.png", "mainPlayer");
	
	
	m_groundCol = { 0,384, WIDTH, 64 };
	m_objects.emplace("mainPlayer", new MainPlayer({0,0,64,96},{0,0,64,96}));

	m_EBvec.reserve(13);
	for (int i = 0; i < 13; i++)
		m_EBvec.push_back(new EmptyBox({ 64 * i,256,64,64 }));

	m_BGvec.push_back(new Background(0, { 0,0,768,576 }));
	m_BGvec.push_back(new Background(0, { 768,0,768,576 }));
	m_BGvec.push_back(new Background(1, { 0,0,768,576 }));
	m_BGvec.push_back(new Background(1, { 768,0,768,576 }));
	m_BGvec.push_back(new Background(2, { 0,0,768,576 }));
	m_BGvec.push_back(new Background(2, { 768,0,768,576 }));
}

void GameState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new TitleState()); // Change to new TitleState
		return;
	}
	else if (EVMA::KeyPressed(SDL_SCANCODE_P))
		STMA::PushState(new PauseState()); // Add new PauseState

	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}

	for (unsigned int i = 0; i < m_BGvec.size(); i++)
		m_BGvec[i]->Update();
	
	// Check if first column of main vector goes out of bounds.
	if (m_EBvec[0]->GetPos().x <= -m_EBvec[0]->GetPos().w)
	{
		// Pop the first vector element/column off.
		delete m_EBvec[0]; // Deallocate Box via pointer.
		m_EBvec[0] = nullptr; // Optional wrangle.
		m_EBvec.erase(m_EBvec.begin()); // Destroys first element of vector.
		
		// Add a new sprite inside the box base.
		m_EBvec.push_back(new EmptyBox({ WIDTH,320,64,64 }, (m_cGap++ % m_maxGap == 0 ? true:false)));
	}
	// Scroll the boxes.
	for (unsigned int i = 0; i < m_EBvec.size(); i++)
		m_EBvec[i]->Update();

	// check if the player has been hit by an obstacle.
	//
	for (unsigned int i = 0; i < m_EBvec.size(); i++)
	{
		if (m_EBvec[i]->getHasSprite())
		{
			if (COMA::AABBCheck(static_cast<MainPlayer*>(m_objects["mainPlayer"])->GetPlayerPos(), m_EBvec[i]->GetSpritePos()))
			{
				//std::cout << "Player hit and obstacle!!!!!" << std::endl;
				SoundManager::Instance().playSound("hit", 1);
				STMA::PushState(new LoseState());
			}
		}
	}
	
	
	// Check collision.
	static_cast<MainPlayer*>(m_objects["mainPlayer"])->checkPlayerAndObjColl(m_groundCol);
	// if obstacle has a platf -> mainplayer.checkPlayerAndObj ( obstWithPlatf )
	for (unsigned int i = 0; i < m_EBvec.size(); i++)
		if(m_EBvec[i]->getHasSprite())
			if (m_EBvec[i]->checkObstHasPlatf())
				static_cast<MainPlayer*>(m_objects["mainPlayer"])->checkPlayerAndObjColl(m_EBvec[i]->GetPlatfSpritePos());
}

void GameState::Render()
{
	cout << "Rendering GameState..." << endl;
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	
	// render a ground 
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 225, 0, 0);
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &m_groundCol);

	for (unsigned int i = 0; i < m_BGvec.size(); i++)
		m_BGvec[i]->Render();

	for (unsigned int i = 0; i < m_EBvec.size(); i++)
		m_EBvec[i]->Render();
	
	for (auto const& i : m_objects)
		i.second->Render();
	if ( dynamic_cast<GameState*>(STMA::GetStates().back()) ) // Check to see if current state is of type GameState
		State::Render();
}

void GameState::Exit()
{
	cout << "Exiting GameState..." << endl;
	TEMA::Unload("tiles");
	TEMA::Unload("player");
	SoundManager::Instance().stopMusic();
	SoundManager::Instance().unload("BGSong", SOUND_MUSIC);
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();

	for (unsigned int i = 0; i < m_EBvec.size(); i++)
	{
		delete m_EBvec[i];
		m_EBvec[i] = nullptr;
	}
	m_EBvec.clear(); // Clears out vector.
	m_EBvec.shrink_to_fit(); // Optional.
}

void GameState::Resume()
{
	cout << "Resuming GameState..." << endl;
}
// End GameState


// Begin PauseState
PauseState::PauseState(){}

void PauseState::Enter()
{
	cout << "Entering PauseState..." << endl;
	cout << "Entering LoseState..." << endl;
	TEMA::Load("Img/resume.png", "resume");
	m_objects.emplace("resume", new ResumeButton({ 0,0,300,100 }, { 180,370,390,90 }, "resume"));
}

void PauseState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_R))
		STMA::PopState();
	
	for (auto const& i : m_objects)
	{
		i.second->Update();
	}
}

void PauseState::Render()
{
	cout << "Rendering PauseState..." << endl;
	// First render the GameState
	STMA::GetStates().front()->Render();
	// Now render rest of PauseState
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 128);
	SDL_Rect rect = { 2*64, 2*64, 8*64, 6*64 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	for (auto const& i : m_objects)
		i.second->Render();
	State::Render();
}

void PauseState::Exit()
{
	cout << "Exiting PauseState..." << endl;
	TEMA::Unload("resume");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
}
// End PauseState


// Begin LoseState
LoseState::LoseState() {}

void LoseState::Enter()
{
	cout << "Entering LoseState..." << endl;
	TEMA::Load("Img/exit.png", "title");
	m_objects.emplace("title", new TitleButton({ 0,0,300,100 }, { 180,370,390,90 }, "title"));
}

void LoseState::Update()
{
	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
}

void LoseState::Render()
{
	cout << "Rendering LoseState..." << endl;
	// First render the GameState
	STMA::GetStates().front()->Render();
	// Now render rest of PauseState
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 100, 0, 0, 128);
	SDL_Rect rect = { 2 * 64, 2 * 64, 8 * 64, 6 * 64 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	for (auto const& i : m_objects)
		i.second->Render();
	State::Render();
}

void LoseState::Exit()
{
	cout << "Exiting LoseState..." << endl;
	TEMA::Unload("title");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
}
// End PauseState
