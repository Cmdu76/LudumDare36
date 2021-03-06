#include "TextComponent.hpp"
#include "../Core/World.hpp"

namespace ke
{

TextComponent::TextComponent()
{
}

void TextComponent::render(sf::RenderTarget & target)
{
	target.draw(mText, getWorldTransform());
}

void TextComponent::setFont(std::string const& font)
{
	mText.setFont(getWorld().getResource<ke::Font>(font));
}

void TextComponent::setFont(sf::Font& font)
{
	mText.setFont(font);
}

void TextComponent::setSize(unsigned int size)
{
	mText.setCharacterSize(size);
}

void TextComponent::setString(std::string const& text)
{
	mText.setString(text);
}

void TextComponent::setFillColor(sf::Color const& color)
{
	mText.setFillColor(color);
}

void TextComponent::setOutlineColor(sf::Color const& color)
{
	mText.setOutlineColor(color);
}

void TextComponent::setOutlineThickness(float thickness)
{
	mText.setOutlineThickness(thickness);
}

unsigned int TextComponent::getSize() const
{
	return mText.getCharacterSize();
}

std::string TextComponent::getString() const
{
	return mText.getString().toAnsiString();
}

sf::Color TextComponent::getFillColor() const
{
	return mText.getFillColor();
}

sf::Color TextComponent::getOutlineColor() const
{
	return mText.getOutlineColor();
}

float TextComponent::getOutlineThickness() const
{
	return mText.getOutlineThickness();
}

} // namespace ke