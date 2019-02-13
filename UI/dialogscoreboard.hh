#ifndef DIALOGSCOREBOARD_HH
#define DIALOGSCOREBOARD_HH

#include <QDialog>


namespace Ui {
class DialogScoreboard;
}

class DialogScoreboard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogScoreboard();
    virtual ~DialogScoreboard();

private:
    Ui::DialogScoreboard *ui;

    /**
     * @brief split splits string
     * @param str string of text
     * @param divider divides the string
     * @return vector of splitted string
     */
    std::vector< std::string > split(std::string str,char divider);

    /**
     * @brief readScoreboard reads .txt file and sets the text to dialog label
     */
    void readScoreboard();

    /**
     * @brief sortingFunction function for std::sort
     * @param first iterator
     * @param second iterator
     * @return iter1 > iter2
     */
    static bool sortingFunction(std::pair<std::string,int> iter1,
                         std::pair<std::string,int> iter2);
};

#endif // DIALOGSCOREBOARD_HH
