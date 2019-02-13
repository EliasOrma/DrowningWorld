#include "wheellayoutparser.hh"

#include "ioexception.hh"
#include "formatexception.hh"

#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
namespace Logic {
using ChanceVector = std::vector<std::pair<std::string, unsigned>>;

WheelLayoutParser::WheelLayoutParser(std::string filePath)
{
    readJSON(filePath);
}

void WheelLayoutParser::readJSON(std::string filePath)
{
    QFile file(QString::fromStdString(filePath));

    if (!file.open(QFile::ReadOnly))
    {
        throw Common::IoException("Could not read file");
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    if (json.isNull()) {
        throw Common::FormatException("JSON parsing failed for input file");
    }

    lastRead_ = json.array();

}

std::vector<std::string> WheelLayoutParser::getSections() const
{
    std::vector<std::string> sections;

    for (auto i = 0; i < lastRead_.size(); ++i){
        sections.push_back(lastRead_[i].toObject().value("name").toString().toStdString());
    }
    return sections;
}

ChanceVector WheelLayoutParser::getChancesForSection(std::string section) const
{
    std::vector<std::pair<std::string, unsigned>> chances;

    for (auto i = 0; i < lastRead_.size(); ++i) {
        auto currentSection = lastRead_[i].toObject().value("name").toString().toStdString();
        if ( currentSection == section ) {
            auto chancesObject = lastRead_[i].toObject().value("chances").toObject();
            for(auto key: chancesObject.keys()){
                auto value = chancesObject.value(key);
                chances.push_back(std::make_pair<std::string, unsigned>(key.toStdString(),
                                                                        (unsigned) value.toInt()));
            }
        }
    }
    return chances;
}

bool WheelLayoutParser::isFileRead()
{
    return !lastRead_.empty();
}
}
