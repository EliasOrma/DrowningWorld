#ifndef DIALOGWIN_HH
#define DIALOGWIN_HH

#include <QDialog>

namespace Ui {
class DialogWIn;
}

class DialogWIn : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWIn();
    virtual ~DialogWIn();

    /**
     * @brief setEndmsg sets messafe to dialog
     * @param msg is the message
     */
    void setEndmsg(std::string msg);

    /**
     * @brief setWinner sets winner of the game
     * @param winnerPlayer ID of player that has won
     * @param savedpawns amount of pawns winner has saved
     */
    void setWinner(int winnerPlayer,int savedpawns);

private slots:
    /**
     * @brief openScoreboard opens new scoreboard dialog
     */
    void openScoreboard();
    /**
     * @brief submitWinner adds winners info to .txt
     */
    void submitWinner();

private:
    Ui::DialogWIn *ui;
    int _winner;
    int _savedpawns;
    bool _submittedWinner;
};

#endif // DIALOGWIN_HH
