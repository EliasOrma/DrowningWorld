#ifndef DIALOG_HH
#define DIALOG_HH

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{

public:
    explicit Dialog();
    virtual ~Dialog();

    /**
     * @brief playerAmount returns the amount of players that
     * user has set with the radiobuttons
     * @return amount of players selected in a dialog
     */
    int playerAmount();

private slots:
    /**
     * @brief scoreboard opens scoreboard dialog
     */
    void scoreboard();

private:
    Ui::Dialog *ui;

    Q_OBJECT
};
#endif // DIALOG_HH
