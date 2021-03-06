#ifndef KE_LAYERCOMPONENT_HPP
#define KE_LAYERCOMPONENT_HPP

#include <map>

#include <SFML/Graphics/VertexArray.hpp>

#include "../Application/PropertiesHolder.hpp"
#include "../Application/ResourceManager.hpp"
#include "../Core/PrimitiveComponent.hpp"
#include "../ExtLibs/pugixml.hpp"
#include "../System/Compression.hpp"
#include "../System/Variant.hpp"

namespace ke
{

class LayerComponent : public PrimitiveComponent, public PropertiesHolder
{
	public:
		LayerComponent();
		LayerComponent(Tileset* tileset, sf::Vector2i const& size = sf::Vector2i(), sf::Vector2i const& tileSize = sf::Vector2i(), std::string const& orientation = "orthogonal", std::string const& staggerAxis = "y", std::string const& staggerIndex = "odd", unsigned int hexSideLength = 0);

		sf::Vector2i worldToCoords(sf::Vector2f const& world);
		sf::Vector2f coordsToWorld(sf::Vector2i const& coords);

		void render(sf::RenderTarget& target);

		bool loadFromNode(pugi::xml_node const& node, Tileset* tileset, sf::Vector2i const& size, sf::Vector2i const& tileSize, std::string const& orientation, std::string const& staggerAxis, std::string const& staggerIndex, unsigned int hexSideLength);
		void saveToNode(pugi::xml_node& node);

		unsigned int getTileId(sf::Vector2i const& coords);
		void setTileId(sf::Vector2i const& coords, unsigned int id);

		std::string getCode();
		bool loadFromCode(std::string const& code);

		void create(Tileset* tileset = nullptr, sf::Vector2i const& size = sf::Vector2i(), sf::Vector2i const& tileSize = sf::Vector2i(), std::string const& orientation = "orthogonal", std::string const& staggerAxis = "y", std::string const& staggerIndex = "odd", unsigned int hexSideLength = 0);

		Tileset* getTileset() const;
		void setTileset(Tileset* tileset);

		float getOpacity() const;
		void setOpacity(float opacity);

		std::string getName() const;
		void setName(std::string const& name);

		const std::string& getEncoding() const;
		void setEncoding(std::string const& encoding);

		const std::string& getCompression() const;
		void setCompression(std::string const& compression);

		const sf::Vector2i& getSize() const;
		void setSize(sf::Vector2i const& size);

		const sf::Vector2i& getTileSize() const;
		void setTileSize(sf::Vector2i const& tileSize);

		const std::string& getOrientation() const;
		void setOrientation(std::string const& orientation);

		const std::string& getStaggerAxis() const;
		void setStaggerAxis(std::string const& axis);

		const std::string& getStaggerIndex() const;
		void setStaggerIndex(std::string const& index);

		unsigned int getHexSizeLength() const;
		void setHexSideLength(unsigned int hexSideLength);

		void updateRender();
		sf::Vector2f getVertexPosition(sf::Vector2i const& coords);

	private:
		sf::VertexArray mVertices;

		Tileset* mTileset;
		float mOpacity;
		std::string mName;
		std::string mEncoding;
		std::string mCompression;

		sf::Vector2i mSize;
		sf::Vector2i mTileSize;
		std::string mOrientation;
		std::string mStaggerAxis;
		std::string mStaggerIndex;
		unsigned int mHexSideLength;
};

} // namespace ke

#endif // KE_LAYERCOMPONENT_HPP
