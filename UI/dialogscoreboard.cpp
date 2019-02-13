#include "dialogscoreboard.hh"
#include "ui_dialogscoreboard.h"

#include <vector>
#include <algorithm>

#include <QFile>
#include <QTextStream>
#include <QFileInfo>


DialogScoreboard::DialogScoreboard() :
    QDialog(nullptr),
    ui(new Ui::DialogScoreboard)
{
    ui->setupUi(this);
    readScoreboard();
}

DialogScoreboard::~DialogScoreboard()
{
    delete ui;
}

std::vector<std::string> DialogScoreboard::split(std::string str, char divider)
{
    std::vector< std::string > splitted;
    std::string row = "";

    for (char& letter : str)
    {
        if (letter == divider)
        {
            splitted.push_back(row);
            row = "";
        }
        else
        {
            row += letter;
        }
    }
    splitted.push_back(row);
    return splitted;
}


bool DialogScoreboard::sortingFunction(std::pair<std::string, int> iter1,
                                       std::pair<std::string, int> iter2)
{
    return iter1.second > iter2.second;
}

void DialogScoreboard::readScoreboard()
{
    QFile file ("scoreboarddata.txt");

    // Doesn't write anything on dialog if file doesn't exist
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QTextStream in(&file);
    std::string line = "";
    std::vector<std::pair<std::string,int>> scorevec;

    // Reads data from file and makes vector
    while (!in.atEnd()) {
        QString line1 = in.readLine();
        std::string line = line1.toStdString();
        std::vector<std::string> nameAndScore = split(line, ':');

        // For checking if there is any info in readed line
        long unsigned int ifinfo = 2;
        if(nameAndScore.size() == ifinfo)
        {
            int num = std::stoi(nameAndScore.at(1));
            std::string name = std::string(nameAndScore.at(0));
            std::pair<std::string,int> a = {name,num};
            scorevec.push_back(a);
        }
    }

    // Sorts the vector in right order
    std::sort(scorevec.begin(),scorevec.end(),sortingFunction);

    std::vector<std::pair<std::string,int>>::iterator iter;
    std::vector<std::pair<std::string,int>>::iterator iterEnd = scorevec.end();
    int length = 0;
    int scoreboardLength = 10;

    // Prints data in score order
    for(iter = scorevec.begin(); iter != iterEnd; ++iter)
    {
        if(length == scoreboardLength)
        {
            break;
        }
        line += std::to_string(length + 1) + "." + iter->first +
                " : " + std::to_string(iter->second) + '\n';
        ++length;
    }
    QString msg = QString::fromStdString(line);
    ui->scores->setText(msg);
}


