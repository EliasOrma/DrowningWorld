#include "dolphin.hh"
#include "hex.hh"

#include <algorithm>

namespace Common {

Dolphin::Dolphin(int id):
    Transport(id)
{
    capacity_ = 1;
}

std::string Dolphin::getTransportType()
{
    return "dolphin";
}

void Dolphin::move(std::shared_ptr<Hex> to) {
    std::vector<std::shared_ptr<Common::Pawn>>::iterator i;
    for( i = pawns_.begin(); i != pawns_.end(); ++i){
        to->addPawn(*i);
        hex_->removePawn(*i);
        (*i)->setCoordinates(to->getCoordinates());
    }
    addHex(to);
}

bool Dolphin::canMove( int playerId ) const {
    // Calculating frequencies of pawns for each player
    // who has a pawn in the transport
    std::map<int,int> frequencies;
    Transport::PawnVector::const_iterator it;
    for( it = pawns_.begin(); it != pawns_.end(); ++it){
        frequencies[(*it)->getPlayerId()]++;
    }
    // Number of pawns that given player has in transport
    int numberOfPawns = frequencies[playerId];

    std::map<int,int>::iterator it2;
    for( it2 = frequencies.begin(); it2 != frequencies.end(); ++it2){
        if ( it2->second > numberOfPawns){
            return false;
        }
    }
    return true;
}

}
