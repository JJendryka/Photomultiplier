#include <QScrollBar>

#include "terminaldock.h"
#include "ui_terminaldock.h"

TerminalDock::TerminalDock(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);

    connect(terminalSendButton, &QPushButton::clicked, this, &TerminalDock::buttonClicked);
    connect(terminalLineEdit, &QLineEdit::returnPressed, this, &TerminalDock::buttonClicked);
}

void TerminalDock::rawIncomingPacket(QString string) {
    terminalTextEdit->moveCursor(QTextCursor::End);
    terminalTextEdit->insertPlainText(">>> ");
    terminalTextEdit->insertPlainText(string);
    terminalTextEdit->verticalScrollBar()->setValue(terminalTextEdit->verticalScrollBar()->maximum());
}

void TerminalDock::rawOutgoingPacket(QString string) {
    terminalTextEdit->moveCursor(QTextCursor::End);
    terminalTextEdit->insertPlainText("<<< ");
    terminalTextEdit->insertPlainText(string);
    terminalTextEdit->verticalScrollBar()->setValue(terminalTextEdit->verticalScrollBar()->maximum());
}

void TerminalDock::buttonClicked() {
    auto text = terminalLineEdit->text();
    text.append("\n");
    sendRaw(text);
    terminalLineEdit->clear();
}
