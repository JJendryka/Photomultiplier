#ifndef NEWEXPERIMENTDIALOG_H
#define NEWEXPERIMENTDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>

class NewExperimentDialog : public QDialog
{
public:
    NewExperimentDialog(const QStringList& items)
    {
        setLayout(new QHBoxLayout());

        box = new QComboBox;
        box->addItems(items);
        layout()->addWidget(box);

        QPushButton* ok = new QPushButton("Create");
        layout()->addWidget(ok);
        connect(ok, &QPushButton::clicked, this, [this]()
        {
           accept();
        });
    }

private:
    QComboBox* box;
};

#endif // NEWEXPERIMENTDIALOG_H
