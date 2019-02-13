#include "dialog.hh"
#include "ui_dialog.h"

#include "dialogscoreboard.hh"

Dialog::Dialog() :
    QDialog(nullptr),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

int Dialog::playerAmount()
{
    int players;

    if(ui->radioButton2->isChecked())
    {
        players = 2;
    }
    else if(ui->radioButton3->isChecked())
    {
        players = 3;
    }
    else
    {
        players = 4;
    }
    return players;
}

void Dialog::scoreboard()
{
    DialogScoreboard win;
    win.exec();
}
