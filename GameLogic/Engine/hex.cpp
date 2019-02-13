#include "hex.hh"
#include "pawn.hh"
#include "actor.hh"
#include "transport.hh"

namespace Common {

Hex::Hex(): piece_("")
{
    neighbourVector_ = { Common::CubeCoordinate(1,-1,0),
                         Common::CubeCoordinate(1,0,-1),
                         Common::CubeCoordinate(0,1,-1),
                         Common::CubeCoordinate(-1,1,0),
                         Common::CubeCoordinate(-1,0,1),
                         Common::CubeCoordinate(0,-1,1)
                       };
}

void Hex::setCoordinates(Common::CubeCoordinate newCoordinates)
{
    coord_ = newCoordinates;

    setNeighbourVector();

}

void Hex::setNeighbourVector()
{
    for(auto neighIt = neighbourVector_.begin();
        neighIt != neighbourVector_.end();
        ++neighIt){
        neighIt->x = neighIt->x + coord_.x;
        neighIt->y = neighIt->y + coord_.y;
        neighIt->z = neighIt->z + coord_.z;
    }
}

void Hex::setPieceType(std::string piece)
{
    piece_ = piece;
}

void Hex::addPawn( std::shared_ptr<Common::Pawn> pawn )
{
    if (pawn != nullptr) {
        pawnMap_[pawn->getId()] = pawn;
    }
}

void Hex::removePawn(std::shared_ptr<Pawn> pawn)
{
    if (pawn != nullptr) {
        pawnMap_.erase(pawn->getId());
    }
}


Common::CubeCoordinate Hex::getCoordinates() const
{
    return coord_;
}

std::string Hex::getPieceType() const
{
    return piece_;
}

std::vector<std::string> Hex::getActorTypes() const
{
    std::vector<std::string> actorTypes;

    for (const auto& pair: actorMap_)
    {
        actorTypes.push_back(pair.second->getActorType());
    }

    return actorTypes;
}

void Hex::addActor( std::shared_ptr<Common::Actor> actor )
{
    if (actor != nullptr) {
        actorMap_[actor->getId()] = actor;
    }
}

void Hex::removeActor( std::shared_ptr<Common::Actor> actor )
{
    if (actor != nullptr) {
        actorMap_.erase(actor->getId());
    }
}

void Hex::addTransport( std::shared_ptr<Common::Transport> transport )
{
    if (transport != nullptr) {
        transportMap_[transport->getId()] = transport;
    }
}

void Hex::removeTransport( std::shared_ptr<Common::Transport> transport )
{
    if (transport != nullptr) {
        transportMap_.erase(transport->getId());
    }
}

int Hex::getPawnAmount() const
{
    return pawnMap_.size();
}

bool Hex::isWaterTile() const
{
    return piece_ == "Water";
}

std::vector<Common::CubeCoordinate> Hex::getNeighbourVector() const
{
    return neighbourVector_;
}

std::shared_ptr<Common::Pawn> Hex::givePawn(int pawnId) const
{
    if (pawnMap_.find(pawnId) == pawnMap_.end()) {
        return nullptr;
    }
    return pawnMap_[pawnId];
}

std::shared_ptr<Common::Transport> Hex::giveTransport(int transportId) const
{
    if (transportMap_.find(transportId) == transportMap_.end()) {
        return nullptr;
    }
    return transportMap_[transportId];
}

std::shared_ptr<Common::Actor> Hex::giveActor(int actorId) const
{
    if (actorMap_.find(actorId) == actorMap_.end()) {
        return nullptr;
    }
    return actorMap_[actorId];
}


void Hex::clear(){
    actorMap_.clear();
    transportMap_.clear();
    pawnMap_.clear();
}

void Hex::clearPawnsFromTerrain()
{
    std::map<int, std::shared_ptr<Common::Pawn>>::iterator it;
    std::map<int, std::shared_ptr<Common::Transport>>::iterator it2;
    std::vector<std::shared_ptr<Common::Pawn>> removable;
    bool pawnIsInTransport;
    for( it = pawnMap_.begin(); it != pawnMap_.end(); ++it){
        pawnIsInTransport = false;
        for ( it2 = transportMap_.begin(); it2 != transportMap_.end(); ++it2){
            if ( it2->second->isPawnInTransport(it->second) ){
                pawnIsInTransport = true;
            }
        }
        if ( !pawnIsInTransport ){
            removable.push_back(it->second);
        }
    }
    std::vector<std::shared_ptr<Common::Pawn>>::iterator it3;
    for( it3 = removable.begin(); it3 != removable.end(); ++it3){
        removePawn(*it3);
    }
}

void Hex::clearTransports()
{
    transportMap_.clear();
}

void Hex::addNeighbour(std::shared_ptr<Common::Hex> hex)
{
    neighbourHexes_.push_back(hex);
}

void Hex::clearAllFromNeightbours()
{
    std::vector<std::shared_ptr<Common::Hex>>::const_iterator it;
    for ( it = neighbourHexes_.begin(); it != neighbourHexes_.end(); ++it){
        (*it)->clear();
    }
}

std::vector<std::shared_ptr<Actor> > Hex::getActors()
{
    std::vector<std::shared_ptr<Actor> > actors;

    for ( const auto& pair: actorMap_)
    {
        actors.push_back(pair.second);
    }

    return actors;
}

std::vector<std::shared_ptr<Pawn> > Hex::getPawns()
{
    std::vector<std::shared_ptr<Pawn> > pawns;
    for ( const auto& pair: pawnMap_)
    {
        pawns.push_back(pair.second);
    }

    return pawns;
}

std::vector<std::shared_ptr<Transport> > Hex::getTransports()
{
    std::vector<std::shared_ptr<Transport> > transports;
    for ( const auto& pair: transportMap_)
    {
        transports.push_back(pair.second);
    }

    return transports;
}
}
