#include <iostream>

#include "terminaldock.h"

TerminalDock::TerminalDock(QWidget *parent) : QDockWidget(parent)
{

}

void TerminalDock::send() {
    std::cout<<"SEND!";
}
