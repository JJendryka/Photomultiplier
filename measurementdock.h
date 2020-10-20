#ifndef MEASUREMENTDOCK_H
#define MEASUREMENTDOCK_H

#include <QDockWidget>
#include <ui_measurementdock.h>


class MeasurementDock : public QDockWidget, private Ui::MeasurementDock
{
    Q_OBJECT

public:
    explicit MeasurementDock(QWidget *parent = nullptr);
};

#endif // MEASUREMENTDOCK_H
