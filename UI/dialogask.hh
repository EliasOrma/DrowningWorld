#ifndef DIALOGASK_HH
#define DIALOGASK_HH

#include <QDialog>

namespace Ui {
class Dialogask;
}

class Dialogask : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogask();
    virtual ~Dialogask();

private:
    Ui::Dialogask *ui;
};

#endif // DIALOGASK_HH
