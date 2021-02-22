#pragma once
#ifndef __BUTTONS__
#define __BUTTONS__

enum Buttons {
	NONE = -1,
	START,
	BACK,
	PAUSE,
	RESUME,
	RESTART,
	EXIT
};

#endif // 

/* Start State

Button* m_pStartButton
Button* m_pExitButton

	// Start Button
	m_pStartButton = new Button("../Assets/textures/startButton.png", "startButton", START);
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 400.0f);

	// Exit Button
	m_pExitButton = new Button("../Assets/textures/exitButton.png", "exitButton", EXIT);
	m_pExitButton->getTransform()->position = glm::vec2(400.0f, 300.0f);

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheEngine::engineInstance()->changeSceneState(PLAY_SCENE);
	});

	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

*/

/* Pause State

Button* m_pResumeButton
Button* m_pBackButton
Button* m_pExitButton

*/

/* Play State

Button* m_pPauseButton
Button* m_pExitButton

*/

/* Exit State

Button* m_pRestartButton
Button* m_pExitButton

    // Restart Button
	m_pRestartButton = new Button("../Assets/textures/restartButton.png", "restartButton", RESTART);
	m_pRestartButton->getTransform()->position = glm::vec2(400.0f, 400.0f);

	// Exit Button
	m_pExitButton = new Button("../Assets/textures/exitButton.png", "exitButton", EXIT);
	m_pExitButton->getTransform()->position = glm::vec2(400.0f, 350.0f);

	m_pRestartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pRestartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});

	m_pRestartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pRestartButton->setAlpha(128);
	});

	m_pRestartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pRestartButton->setAlpha(255);
	});
	addChild(m_pRestartButton)
*/