#ifndef PIECEFACTORY_HH
#define PIECEFACTORY_HH

#include <QJsonObject>
#include <string>
#include <vector>

/**
 * @file
 * @brief Singleton class that creates pieces.
 */

namespace Logic {

/**
 * @brief Singleton class for creating pieces.
 *
 * The factory is requested to read JSON file, after which it will requested to
 * return a data structure, which containts the read pieces.
 */
class PieceFactory {

  public:

    /**
     * @return A reference to the factory.
     */
    static PieceFactory& getInstance();

    /**
     * @brief readJSON reads pieces a JSON file.
     * @exception IOException Could not open the file Assets/pieces.json for reading.
     * @exception FormatException Format of the file Assets/pieces.json is invalid.
     * @post Exception quarantee: basic
     */
    void readJSON();

    /**
     * @brief Gets the pieces used in the game
     * @return The pieces read from the JSON file. If the file is not read, or the actors did not exist, will return an empty vector.
     * @post Exception quarantee: basic
     */
    std::vector<std::pair<std::string,int>> getGamePieces() const;

  private:

    PieceFactory();

    QJsonObject json_;

};

}

#endif
