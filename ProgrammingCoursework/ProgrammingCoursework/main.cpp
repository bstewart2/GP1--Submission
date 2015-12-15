#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "asteroidsGame.h"
#include "cButton.h"


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;



    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
    windowOGL theOGLWnd;
	
    //Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\enemyTank.png", "Images\\enemyTank.png", "Images\\enemyTank.png", "Images\\enemyTank.png", "Images\\shot.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[4] = { "Audio/backgroundmusic.wav", "Audio/shot.wav", "Audio/tankmovement.wav", "Audio /explosion.mp4"};

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Tank", gameSounds[2]);
	theSoundMgr->add("Explosion", gameSounds[3]);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[1] = { "Fonts/Army.ttf" };

	theFontMgr->addFont("Menu", gameFonts[0], 60);
	theFontMgr->addFont("Instructions", gameFonts[0], 18);
	theFontMgr->addFont("Game", gameFonts[0], 25);

	// Spawn enemies and determine positions

	for (int astro = 0; astro < 5; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(0 + (rand() % 1000), 0 - (rand() % 75)));
		theAsteroids[astro]->setSpriteTranslation(glm::vec2((rand() % 4 + 1), (rand() % 4 + 1)));
		theAsteroids[astro]->setSpriteRotation(180.f);
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
	}
	// Makes half the enemies spawn at the bottom of the screen
	for (int astro = 5; astro < 10; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(0 +(rand() % 1000), windowHeight + (rand() % 75)));
		theAsteroids[astro]->setSpriteTranslation(glm::vec2((rand() % 4 + 1), (rand() % 4 + 1)));
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
	}

	// Background texture array
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images\\background.png", "Images\\menubackground.png" };
	for (int tCount = 0; tCount < 2; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	//Button Texture array
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/exitBtn.png", "Images/Buttons/playBtn.png", };
	for (int tCount = 0; tCount < 2; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	// Set sprites for background, buttons and player
	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	cBkGround spriteStartBkgd;
	spriteStartBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteStartBkgd.setTexture(textureBkgList[1]->getTexture());
	spriteStartBkgd.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	cBkGround spriteEndBkgd;
	spriteEndBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteEndBkgd.setTexture(textureBkgList[1]->getTexture());
	spriteEndBkgd.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[0]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[1]->getTexture());
	playButton.setTextureDimensions(btnTextureList[1]->getTWidth(), btnTextureList[1]->getTHeight());

	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\playerTank.png");
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(512.0f, 380.0f));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	// Text for game including control instructions
	string outputMsg;
	string strMsg[] = { "Left and right arrows to move. Up and down to change shooting direction. ", "Tank Battle", "Game over", "E to stop moving, space to Shoot."};

	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);

	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;

	// Count enemies destroyed by player
	int enemyDestroyed = 0;

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// play theme music
		theSoundMgr->getSnd("Theme")->playAudio(AL_TRUE);

		vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();

		// Converts enemy destroyed to the players's score to be displayed 
		std::string score = std::to_string(enemyDestroyed);

		switch (theGameState)
		{
		case MENU:
			// render main menu background and buttons
			spriteStartBkgd.render();

			playButton.setSpritePos(glm::vec2(450.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(450.0f, 375.0f));
			playButton.render();
			exitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update();

			// Display game name and instructions
			outputMsg = strMsg[1];
			theFontMgr->getFont("Menu")->printText(outputMsg.c_str(), FTPoint(320.0f, 120, 0.0f));
			outputMsg = strMsg[0];
			theFontMgr->getFont("Instructions")->printText(outputMsg.c_str(), FTPoint(160.0f, 220, 0.0f));
			outputMsg = strMsg[3];
			theFontMgr->getFont("Instructions")->printText(outputMsg.c_str(), FTPoint(360.0f, 250, 0.0f));


			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			break;

		case PLAYING:
			// render background and player
			spriteBkgd.render();
			rocketSprite.render();
			rocketSprite.update(elapsedTime);

	
			// render button and reset clicked to false
			while (asteroidIterator != theAsteroids.end())
			{
				if ((*asteroidIterator)->isActive() == false)
				{
					asteroidIterator = theAsteroids.erase(asteroidIterator);
					enemyDestroyed ++;
				}
				else
				{
					(*asteroidIterator)->update(elapsedTime);
					(*asteroidIterator)->render();
					++asteroidIterator;
				}

			}
			// End game if player destroys all enemies
			if (enemyDestroyed > 9)
			{
				theGameState = END;
			}

			// Game ends if player his hit by enemy tank
			for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
			{
				if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), rocketSprite.getBoundingRect()))
				{
					theGameState = END;
				}
			}

			outputMsg = strMsg[1];
			theFontMgr->getFont("Game")->printText(outputMsg.c_str(), FTPoint(10, 25, 0.0f));
			outputMsg = "Score: " + score;
			theFontMgr->getFont("Game")->printText(outputMsg.c_str(), FTPoint(10.0f, 45.0f, 0.0f)); // Display score
			break;
		case END:
			// Render background and buttons for end screen
			spriteEndBkgd.render();

			playButton.setClicked(false);
			exitButton.setClicked(false);

			playButton.setSpritePos(glm::vec2(450.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(450.0f, 375.0f));
			playButton.render();
			exitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update();

			// Display game over text and score
			outputMsg = strMsg[2];
			theFontMgr->getFont("Menu")->printText(outputMsg.c_str(), FTPoint(350.0f, 170, 0.0f));
			outputMsg = "Score: " + score;
			theFontMgr->getFont("Menu")->printText(outputMsg.c_str(), FTPoint(373.0f, 250, 0.0f));

			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}

			break;
		}	

		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
