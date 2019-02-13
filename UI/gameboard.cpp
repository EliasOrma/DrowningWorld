#include "gameboard.hh"
#include "pawn.hh"
#include "transport.hh"
#include "actor.hh"


using cubCord = Common::CubeCoordinate;

void Student::GameBoard::addPawn(int playerId, int pawnId,
                                 Common::CubeCoordinate coord)
{
    std::shared_ptr<Common::Pawn> newPawn = std::make_shared<Common::Pawn>();
    newPawn->setId(pawnId,playerId);
    newPawn->setCoordinates(coord);

    hexMap_[coord]->addPawn(newPawn);
    pawnMap_[pawnId] = newPawn;

}

void Student::GameBoard::addPawn(int playerId, int pawnId)
{
    int maxPlayers = 4;
    if(playerId > maxPlayers)
    {
        return;
    }
    // Used for setting starting coordinates
    int rad = 10;
    std::shared_ptr<Common::Pawn> newPawn = std::make_shared<Common::Pawn>();
    newPawn->setId(pawnId,playerId);

    // Pawn spawning coordinates for each player
    cubCord coordAr [4] = {cubCord(-rad,0,rad),cubCord(rad,0,-rad),
                          cubCord(0,rad,-rad),cubCord(0,-rad,rad)};

    cubCord coord = coordAr[playerId-1];
    newPawn->setCoordinates(coord);
    hexMap_[coord]->addPawn(newPawn);
    pawnMap_[pawnId] = newPawn;

}

int Student::GameBoard::checkTileOccupation
(Common::CubeCoordinate tileCoord) const

{
    if(hexMap_.find(tileCoord) == hexMap_.end())
    {
        return -1;
    }
    return hexMap_.at(tileCoord)->getPawnAmount();
}

bool Student::GameBoard::isWaterTile(Common::CubeCoordinate tileCoord) const
{
    return hexMap_.at(tileCoord)->isWaterTile();
}

std::shared_ptr<Common::Hex> Student::GameBoard::getHex
(Common::CubeCoordinate coord) const
{
    if(hexMap_.find(coord)!=hexMap_.end())
    {
        return hexMap_.at(coord);

    }
    else
    {
        return nullptr;
    }
}

void Student::GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{
    if(hexMap_.find(pawnCoord) != hexMap_.end())
    {
        hexMap_[pawnMap_[pawnId]->getCoordinates()]->
                removePawn(pawnMap_[pawnId]);
        hexMap_[pawnCoord]->addPawn(pawnMap_[pawnId]);

        // Does action if there is actor in end coordinate
        if(hexMap_[pawnCoord]->getActors().size() != 0)
        {
            hexMap_[pawnCoord]->getActors().at(0)->doAction();
            std::string actorType = hexMap_[pawnCoord]->
                    getActors().at(0)->getActorType();
            if((actorType == "shark")|| (actorType == "seamunster"))
            {
                removePawn(pawnId);
                return;
            }
        }
        pawnMap_[pawnId]->setCoordinates(pawnCoord);
    }
}

void Student::GameBoard::removePawn(int pawnId)
{
    if(pawnMap_.find(pawnId) != pawnMap_.end())
    {
        hexMap_[pawnMap_[pawnId]->getCoordinates()]->
                removePawn(pawnMap_[pawnId]);
        pawnMap_.erase(pawnId);
    }
}

void Student::GameBoard::addActor(std::shared_ptr<Common::Actor> actor,
                                  Common::CubeCoordinate actorCoord)
{
    actor->addHex(hexMap_[actorCoord]);
    hexMap_[actorCoord]->addActor(actor);
    actorMap_[actor->getId()] = actor;
}

void Student::GameBoard::moveActor(int actorId,
                                   Common::CubeCoordinate actorCoord)
{
    if(hexMap_.find(actorCoord) != hexMap_.end())
    {
        actorMap_[actorId]->move(hexMap_[actorCoord]);
        std::string actorType = actorMap_[actorId]->getActorType();
        int endPawnAmount = hexMap_[actorCoord]->getPawnAmount();
        int endVehAmount = hexMap_[actorCoord]->getTransports().size();
        if(actorType == "seamunster")
        {
            if(endPawnAmount != 0)
            {
                std::vector<std::shared_ptr<Common::Pawn>> pawns =
                        hexMap_[actorCoord]->getPawns();
                for(auto pawn : pawns)
                {
                    removePawn(pawn->getId());
                }
            }
            if(endVehAmount != 0)
            {
                // There can be only one transport in one hex
                removeTransport(hexMap_[actorCoord]->
                                getTransports().at(0)->getId());
            }
            actorMap_[actorId]->doAction();
            return;
        }
        if( actorType == "shark")
        {
            if(endPawnAmount != 0)
            {
                std::vector<std::shared_ptr<Common::Pawn>> pawns =
                        hexMap_[actorCoord]->getPawns();
                for(auto pawn : pawns)
                {
                    removePawn(pawn->getId());
                }
            }
            actorMap_[actorId]->doAction();
            return;
        }
        if( actorType == "kraken")
        {
            if(endVehAmount != 0)
            {
                if(hexMap_[actorCoord]->
                        getTransports().at(0)->getTransportType() != "boat")
                {
                    return;
                }
                // There can be only one transport in one hex
                removeTransport(hexMap_[actorCoord]->
                                getTransports().at(0)->getId());
            }
            actorMap_[actorId]->doAction();
            return;
        }
    }
}

void Student::GameBoard::removeActor(int actorId)
{
    actorMap_[actorId]->getHex()->removeActor(actorMap_[actorId]);
    actorMap_.erase(actorId);
}

void Student::GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    hexMap_[newHex->getCoordinates()] = newHex;
}

void Student::GameBoard::addTransport(std::shared_ptr<Common::Transport>
                                      transport, Common::CubeCoordinate coord)
{
    transport->addHex(hexMap_[coord]);
    hexMap_[coord]->addTransport(transport);
    transportMap_[transport->getId()] = transport;
}

void Student::GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{
    if(hexMap_.find(coord) != hexMap_.end())
    {
        transportMap_[id]->move(hexMap_[coord]);
    }
}

void Student::GameBoard::removeTransport(int id)
{
    transportMap_[id]->getHex()->removeTransport(transportMap_[id]);
    transportMap_.erase(id);
}

std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>>
Student::GameBoard::getHexMap()
{
    return hexMap_;
}

int Student::GameBoard::checkPawnPropertyPlace(Common::CubeCoordinate coord,
                                               int playerId)
{
    if(hexMap_.find(coord)== hexMap_.end())
    {
        return -1;
    }
    std::vector<std::shared_ptr<Common::Pawn>> allpawns =
            hexMap_.at(coord)->getPawns();
    int index = 0;

    std::vector<std::shared_ptr<Common::Pawn>>::iterator iter;
    std::vector<std::shared_ptr<Common::Pawn>>::iterator iterStopCondition =
                                                        allpawns.end();

    for(iter = allpawns.begin(); iter != iterStopCondition; ++iter)
    {
        if(iter->get()->getPlayerId() == playerId)
        {
            return index;
        }
        ++index;
    }
    return -1;
}

bool Student::GameBoard::checkActorExistanse(std::string actorType)
{
    if(actorType == "dolphin")
    {
        for(auto veh : transportMap_)
        {
            if(veh.second->getTransportType() == actorType)
            {
                return true;
            }
        }
    }
    else
    {
        for(auto actor : actorMap_)
        {
            if(actor.second->getActorType() == actorType)
            {
                return true;
            }
        }
    }
    return false;
}

bool Student::GameBoard::noPawnsLeft()
{
    return pawnMap_.size() == 0;
}
