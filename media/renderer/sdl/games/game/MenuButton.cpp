#include "MenuButton.h"
#include "InputHandler.h"



MenuButton::MenuButton(const LoadParams* pParams, void (*callback)()) : SDLGameObject(pParams), 
						m_callback(callback), m_bClicked(false)
{
	m_currentFrame = MOUSE_OUT;
}


MenuButton::~MenuButton()
{
}

void MenuButton::draw()
{
	SDLGameObject::draw();
}

void MenuButton::update()
{
	Vector2D* pMousePos = InputHandler::Instance()->getMousePosition();

	if (pMousePos->getX() < m_position.getX() + m_width &&
		pMousePos->getX() > m_position.getX() &&
		pMousePos->getY() < m_position.getY() + m_height &&
		pMousePos->getY() > m_position.getY()) {
		

		if (InputHandler::Instance()->getMousButtonState(InputHandler::LEFT) && !m_bClicked) {
			m_currentFrame = MOUSE_CLICKED;
			m_bClicked = true;
		} else if (!InputHandler::Instance()->getMousButtonState(InputHandler::LEFT)) {
			m_currentFrame = MOUSE_OVER;
			if (m_bClicked) {
				m_callback();
				m_bClicked = false;
			}
			
		}
	} else {
		m_currentFrame = MOUSE_OUT;
		m_bClicked = false;
	}
}

void MenuButton::clean()
{
	SDLGameObject::clean();
}
