#include "transport.hh"
#include "hex.hh"
#include <memory>
#include <algorithm>

namespace Common{

Transport::Transport( int id ):
    capacity_(0),
    hex_(nullptr),
    id_(id)
{}

Transport::~Transport(){}

void Transport::addPawn(std::shared_ptr<Pawn> pawn )
{
    if ( getCapacity() > 0 ){
        pawns_.push_back(pawn);
    }
}

void Transport::removePawn(std::shared_ptr<Pawn> pawn)
{
    if (pawn != nullptr) {
        auto foundPawn = std::find(pawns_.begin(),pawns_.end(),pawn);
        if (foundPawn != pawns_.end()) {
            pawns_.erase(foundPawn);
        }
    }
}

int Transport::getCapacity() const
{
    return capacity_ - pawns_.size();
}

int Transport::getMaxCapacity() const
{
    return capacity_;
}

void Transport::addHex( std::shared_ptr<Common::Hex> hex )
{
    hex->addTransport(shared_from_this());
    if (hex_ != nullptr) {
        hex_->removeTransport(shared_from_this());
    }
    hex_ = hex;
}

std::shared_ptr<Hex> Transport::getHex()
{
    return hex_;
}

std::vector<std::shared_ptr<Pawn> > Transport::getPawnsInTransport()
{
    return pawns_;
}

bool Transport::isPawnInTransport(std::shared_ptr<Pawn> pawn)
{
    return (std::find(pawns_.begin(),pawns_.end(),pawn) != pawns_.end());
}

int Transport::getId()
{
    return id_;
}

void Transport::removePawns()
{
    pawns_.clear();
}

}
