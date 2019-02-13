#ifndef IPLAYER_HH
#define IPLAYER_HH

/**
 * @file
 * @brief Defines the interface for player.
 */

namespace Common {

/**
 * @brief Interface for player.
 */
class IPlayer {

  public:

    /**
     * @brief Default constructor, exists solely for documentation.
     */
    IPlayer() = default;

    /**
     * @brief Virtual destructor. Does nothing, since this is an interface class.
     */
    virtual ~IPlayer() = default;


    /**
     * @brief getPlayer gets the player id.
     * @return The identifier of the player.
     * @post Exception quarantee: nothrow
     */
    virtual int getPlayerId() const = 0;

    /**
     * @brief setActionsLeft sets the player's remaining number of actions.
     * @param actionsLeft The number of actions left.
     * @post Actions left set to parameter value. Exception quarantee: nothrow
     */
    virtual void setActionsLeft(unsigned int actionsLeft) = 0;

    /**
     * @brief getActionsLeft returns the amount of actions left for the player
     * @return amount of actions
     * @post Exception quarantee: nothrow
     */
    virtual unsigned int getActionsLeft() const = 0;

};

}

#endif
