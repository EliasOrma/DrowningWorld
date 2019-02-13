# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [3.3.0] 2018-11-21

### Added
- Added functions for Transports to IGameRunner
- Added getCurrentPlayer to IGameRunner
- Added getPawnsInTransport, removePawns and getMaximumCapacity to Transport

### Fixed
- Improved Actor's documentation.
- Improved IGameRunner's documentation.

### Changed
- Moving pawns now reduces players acition points.
- moving pawns and transports with normal movements uses player's action points
- moveActor now restores player's actions 

## [3.2.0] 2018-11-16

### Added
- Added getHex() to Actor.
- Added getHex() getTransportType() and removePawn() a to Transport.

### Fixed
- Improved the documentation of IGameRunner
- Improved the documentation of IGameBoard
- GameEngine's constructor no longer leaks Common::GameException
- GameEngine no longer tries to add boats to Hexes that are not water hexes.

## [3.0.1] 2018-11-14

### Fixed
- Added missing include directives.

## [3.0.0] 2018-11-13

### Changed
- IGameBoard has been expanded, update your implementation.

### Added
- Added functions playerAmount() and getSpinnerLayout() to IGameRunner.
- Added addPawn(int,int,CubeCoord) to IGameBoard.
- Added a constructor with parmeters for Pawn.

### Fixed
- Various clarifications and improvements to documentation

### Removed
- Removed changeOccupation() which no more had an implementation and redundant setWaterAttribute() from Hex.

## [2.1.0]

### Fixed
- Fixed bug related to island layer counter not working correctly.
- Fixed issues with some compilers trying to use copy constructor of singleton factories.
- Fixed bug in Transport's hex setting.

## [2.1.0]
### Changed
- IGameBoard has been expanded, update your implementation.

### Added
- Support for specifying own spinner layout with Assets/layout.json
- 2 new Transports, Boat and Dolphin
- 4 new Actors, Shark, Kraken, Seamunster and Vortex
- Added new functions to IGameBoard, addActor, addTransport, moveTransport and removeTransport. Update your own GameBoard implementation!

## [2.0.0]
### Changed
- Renamed Common::Vehicle to Common::Transport
- Wrapped Common::getGameRunner (and the whole initialization module) to namespace Initialization
- Common::Initialization::getGameRunner now handles players as std::vector<std::shared_ptr<Common::IPlayer>> instead of std::vector<Common::IPlayer*>
- 

### Added
- Added new services to IGameEngine.
- Added implementations for Actor's and Transport's member functions.
- Added new services to Hex


### Removed
- Removed unnecessary services from Hex.


## [1.0.0] - 2018-10-25
### Added
- Initial release of GameLogic
