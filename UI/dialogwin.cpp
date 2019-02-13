#include "dialogwin.hh"
#include "ui_dialogwin.h"
#include "dialogscoreboard.hh"

#include <QString>
#include <QFile>
#include <QTextStream>

DialogWIn::DialogWIn() :
    QDialog(nullptr),
    ui(new Ui::DialogWIn)
{
    ui->setupUi(this);
    _winner = 0;
    _submittedWinner = true;
}

DialogWIn::~DialogWIn()
{
    delete ui;
}

void DialogWIn::setEndmsg(std::string msg)
{
    ui->label->setText(QString::fromStdString(msg));
}

void DialogWIn::setWinner(int winnerPlayer,int savedpawns)
{
    _winner = winnerPlayer;
    _savedpawns = savedpawns;
    _submittedWinner = false;
}


void DialogWIn::openScoreboard()
{
    DialogScoreboard scoreboard;
    scoreboard.exec();
}

void DialogWIn::submitWinner()
{
    // Adds winner if there is any
    if((_winner != 0) && (!_submittedWinner))
    {
        QFile file ("scoreboarddata.txt");
        if(file.open(QIODevice::ReadWrite  | QIODevice::Append))
        {
                QTextStream in(&file);
                QString text = ui->winnertext->toPlainText();
                std::string textToWrite = text.toStdString() + ':'
                + std::to_string(_savedpawns) + '\n';

                in << QString::fromStdString(textToWrite);
                file.close();
                _submittedWinner = true;
        }
    }
}
