#include "ioexception.hh"
#include "formatexception.hh"
#include "piecefactory.hh"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

namespace Logic {

QString const PIECEDATA = ("Assets/pieces.json");

PieceFactory::PieceFactory() {}

PieceFactory& PieceFactory::getInstance()
{

    static PieceFactory instance;
    return instance;

}

void PieceFactory::readJSON()
{

    QFile file (PIECEDATA);

    if (!file.open(QFile::ReadOnly))
    {
        throw Common::IoException("Could not read file");
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    if (json.isNull()) {
        throw Common::FormatException("JSON parsing failed for input file");
    }

    json_ = json.object();

}

std::vector<std::pair<std::string,int>> PieceFactory::getGamePieces() const
{

    std::vector<std::pair<std::string,int>> gamePieces;
    QJsonArray common = json_["Common"].toArray();
    for (int i = 0; i < common.size(); ++i) {
        auto piecesData = std::make_pair(common[i].toObject().value("name").toString().toStdString(),common[i].toObject().value("layers").toInt());
        gamePieces.push_back(piecesData);
    }
    return gamePieces;

}

}
