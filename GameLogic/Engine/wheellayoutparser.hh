#ifndef WHEELLAYOUTPARSER_HH
#define WHEELLAYOUTPARSER_HH


#include <QJsonArray>
#include <string>
#include <vector>
namespace Logic {
/**
 * @brief JSON parser for reading the spin wheel layout
 */
class WheelLayoutParser
{
public:
    WheelLayoutParser() = default;
    /**
     * @brief WheelLayoutParser
     * @exception IOException Could not open the given json for reading.
     * @exception FormatException Format of the given json is invalid.
     * @param filePath file path to layout file
     * @post Exception quarantee: basic
     */
    explicit WheelLayoutParser(std::string filePath);
    /**
     * @brief Reads the file in given string
     * @param filePath file path to layout file
     * @exception IOException Could not open the given json for reading.
     * @exception FormatException Format of the given json is invalid.
     * @post The contents of read file can be queried.
     * @post Exception quarantee: strog
     */
    void readJSON(std::string filePath);
    /**
     * @brief Returns the different possible things to mve
     * @return vector containing the different things we can move with the spinner
     */
    std::vector<std::string> getSections() const;
    /**
     * @brief returns the chances for a specific section
     * @pre section must be one of sections returned by getSections()
     * @return the chances of a specific section
     */
    std::vector<std::pair<std::string, unsigned>> getChancesForSection(std::string section) const;
    /**
     * @brief isFileRead
     * @return true if a file has been read.
     */
    bool isFileRead();
private:
    QJsonArray lastRead_;

};

}
#endif // WHEELLAYOUTPARSER_HH

