#pragma once

#include <QComboBox>

class ComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboBox(QWidget *parent = 0);

    void wheelEvent(QWheelEvent *e) Q_DECL_OVERRIDE;
};

