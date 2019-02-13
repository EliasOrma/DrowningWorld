TEMPLATE = subdirs

SUBDIRS += \
    UnitTests \
    Engine

UnitTests.depends = Engine
