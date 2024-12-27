#ifndef PATH_H
#define PATH_H

#include "SFML/Graphics/VertexArray.hpp"


class Path {
    sf::VertexArray nodePath;

    public:

        Path() = default;

        void addToPath(const sf::Vertex &point) {
            nodePath.append(point);
        }

        sf::VertexArray getNodePath() {
            return nodePath;
        }
};



#endif //PATH_H
