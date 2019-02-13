#include "gamestate.hh"

Student::GameState::GameState()
{
    gamePhaseId_ = Common::GamePhase::MOVEMENT;
    playerInTurn_ = 1;
    roundsPlayed_ = 0;
    gameCanEnd = false;
}

Common::GamePhase Student::GameState::currentGamePhase() const
{
    return gamePhaseId_;
}

int Student::GameState::currentPlayer() const
{
    return playerInTurn_;
}

void Student::GameState::changeGamePhase(Common::GamePhase nextPhase)
{
    // Adds round in counter if round is complete
    if((nextPhase == Common::GamePhase::MOVEMENT) && (playerInTurn_ == 1))
    {
        ++roundsPlayed_;
    }
    gamePhaseId_ = nextPhase;
}

void Student::GameState::changePlayerTurn(int nextPlayer)
{
    playerInTurn_ = nextPlayer;
}

int Student::GameState::getRoundsPlayed()
{
    return roundsPlayed_;
}
