#include "gameboard.hh"
#include "gamestate.hh"
#include "player.hh"
#include "mainwindow.hh"
#include "dialog.hh"
#include "initialize.hh"
#include "igamerunner.hh"

#include <memory>
#include <QApplication>
#include <vector>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    Dialog setupWindow;
    if (setupWindow.exec() == QDialog::Rejected)
    {
        return EXIT_FAILURE;
    }

    int playeramount = setupWindow.playerAmount();

    // Initing gameboard,gamestate and playervector for
    // Common::Initialiization
    std::shared_ptr<Common::IGameBoard> board =
            std::make_shared<Student::GameBoard>();

    std::shared_ptr<Common::IGameState> state =
            std::make_shared<Student::GameState>();

    std::vector<std::shared_ptr<Common::IPlayer>> playerVec;


    for(int a = 0; a != playeramount; ++a)
    {
        std::shared_ptr<Common::IPlayer> p =
                std::make_shared<Student::Player>(a+1,3);

        playerVec.push_back(p);
    }

    std::shared_ptr<Common::IGameRunner> runner =
            Common::Initialization::getGameRunner(board,state,playerVec);

    // Casting inited classes to inherted Student classes
    std::shared_ptr<Student::GameBoard> realBoard =
            std::dynamic_pointer_cast<Student::GameBoard>(board);

    std::shared_ptr<Student::GameState> realState =
            std::dynamic_pointer_cast<Student::GameState>(state);

    std::vector<std::shared_ptr<Student::Player>> realPlayerVec;
    std::vector<std::shared_ptr<Common::IPlayer>>::iterator stopCondition =
            playerVec.end();

    for(auto iter = playerVec.begin(); iter != stopCondition; ++iter)
    {
        std::shared_ptr<Student::Player> plr =
                std::dynamic_pointer_cast<Student::Player>(*iter);

        realPlayerVec.push_back(plr);
    }

    // Setting up mainwindow and it's needed objects
    MainWindow w;
    w.setObjects(realBoard,realState,realPlayerVec,runner);
    w.show();

    return a.exec();
}
