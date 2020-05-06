#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <SFML/Graphics.hpp>

class TileMap : public sf::Drawable, public sf::Transformable {

	public:
        bool load_map();

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            // apply the transform
            states.transform *= getTransform();

            // apply the tileset texture
            states.texture = &mapTexture;

            // draw the vertex array
            target.draw(mapVertices, states);
        }

        sf::VertexArray mapVertices;
        sf::Texture mapTexture;
};

#endif