#include "player.hh"

static int MAXACTIONS = 3;

Student::Player::Player(int Id, int totalPawns) : _playerId(Id),
    _pawnLeft(totalPawns)
{
    _playerId = Id;
    _pawnLeft = totalPawns;
    _actionsLeft = MAXACTIONS;
    _savedPawns = 1;
}

unsigned int Student::Player::getActionsLeft() const
{
    return _actionsLeft;
}

int Student::Player::getpawnsleft()
{
    return _pawnLeft;
}

int Student::Player::getSavedPawns()
{
    return _savedPawns;
}

void Student::Player::savePawn()
{
    _savedPawns++;
    _pawnLeft--;
}

void Student::Player::resetActions()
{  
    setActionsLeft(MAXACTIONS);
}

int Student::Player::getPlayerId() const
{
    return _playerId;
}

void Student::Player::setActionsLeft(unsigned int actionsLeft)
{
    _actionsLeft = actionsLeft;
}
