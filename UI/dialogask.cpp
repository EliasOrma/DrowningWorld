#include "dialogask.hh"
#include "ui_dialogask.h"

Dialogask::Dialogask() :
    QDialog(nullptr),
    ui(new Ui::Dialogask)
{
    ui->setupUi(this);
}

Dialogask::~Dialogask()
{
    delete ui;
}
