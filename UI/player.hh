#ifndef PLAYER_HH
#define PLAYER_HH

#include "iplayer.hh"


namespace Student {

class Player : public Common::IPlayer
{
public:
    Player() = default;
    Player(int Id,int pawnLeft);
    virtual ~Player() = default;


    /**
     * @brief getPlayer gets the player id.
     * @return The identifier of the player.
     * @post Exception quarantee: nothrow
     */
    virtual int getPlayerId() const;

    /**
     * @brief setActionsLeft sets the player's remaining number of actions.
     * @param actionsLeft The number of actions left.
     * @post Actions left set to parameter value. Exception quarantee: nothrow
     */
    virtual void setActionsLeft(unsigned int actionsLeft);

    /**
     * @brief getActionsLeft returns the amount of actions left for the player
     * @return amount of actions
     * @post Exception quarantee: nothrow
     */
    virtual unsigned int getActionsLeft() const;

    /**
     * @brief getpawnsleft returns the amount of pawns left for the player
     * @return amount of pawns left
     * @post Expection quarantee: nothrow
     */
    int getpawnsleft();

    /**
     * @brief getSavedPawns
     * @return returns players saved pawns
     */
    int getSavedPawns();

    /**
     * @brief savePawn adds new saved pawn and deletes it from pawns from
     * pawns left
     */
    void savePawn();

    /**
     * @brief resetActions resets players acrions back to 3
     */
    void resetActions();


private:
    int _playerId;
    unsigned int _actionsLeft;
    int _pawnLeft;
    int _savedPawns;
};

}

#endif // PLAYER_HH
