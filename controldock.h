#ifndef CONTROLDOCK_H
#define CONTROLDOCK_H

#include <QDockWidget>

#include "ui_controldock.h"

class ControlDock : public QDockWidget, private Ui::ControlDock
{
    Q_OBJECT

public:
    explicit ControlDock(QWidget *parent = nullptr);
};

#endif // ControlDock_H
