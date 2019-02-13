TEMPLATE = subdirs

SUBDIRS += \
    Tests \
    UI \
    GameLogic

UI.depends = GameLogic
