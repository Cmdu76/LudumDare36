#ifndef ENDSTATE_HPP
#define ENDSTATE_HPP

#include "../Keengine/Application/Application.hpp"

class EndState : public ke::State
{
	public:
		EndState();
		~EndState();

		bool handleEvent(sf::Event const& event);
		bool update(sf::Time dt);
		void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

	protected:
		sf::Text mInfo;
		sf::Text mScore;
		tgui::EditBox::Ptr mBox;
		tgui::Button::Ptr mSend;
		bool mSent;

		sf::Sprite mHide;
};

#endif // ENDSTATE_HPP
