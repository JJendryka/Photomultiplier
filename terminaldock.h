#ifndef TERMINALDOCK_H
#define TERMINALDOCK_H

#include <QDockWidget>

#include "ui_terminaldock.h"

class TerminalDock : public QDockWidget, private Ui::TerminalDock
{
    Q_OBJECT

public:
    explicit TerminalDock(QWidget *parent = nullptr);
};

#endif // TERMINALDOCK_H
