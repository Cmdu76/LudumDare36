#ifndef KE_SPRITECOMPONENT_HPP
#define KE_SPRITECOMPONENT_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../Core/PrimitiveComponent.hpp"

namespace ke
{

class SpriteComponent : public PrimitiveComponent
{
	public:
		SpriteComponent();

		void setTexture(std::string const& textureName, sf::IntRect const& rect = sf::IntRect());
		void setTexture(sf::Texture& texture, sf::IntRect const& rect = sf::IntRect());
		std::string getTextureName() const;

		void setTextureRect(sf::IntRect const& rect);
		sf::IntRect getTextureRect() const;

		void setColor(sf::Color const& color);
		sf::Color getColor() const;

		void render(sf::RenderTarget& target);
		sf::FloatRect getLocalBounds();
		sf::FloatRect getGlobalBounds();

	private:
		sf::Sprite mSprite;
		std::string mTexture;
};

} // namespace ke

#endif // KE_SPRITECOMPONENT_HPP
