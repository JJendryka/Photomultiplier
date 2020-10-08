#ifndef TERMINALDOCK_H
#define TERMINALDOCK_H

#include <QtWidgets>

class TerminalDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit TerminalDock(QWidget *parent = nullptr);

private slots:
    void send();

};

#endif // TERMINALDOCK_H
