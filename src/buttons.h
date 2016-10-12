#ifndef BUTTONS_H
#define BUTTONS_H

#include <QDialog>

namespace Ui {
class Buttons;
}

class Buttons : public QDialog
{
    Q_OBJECT

public:
    explicit Buttons(QWidget *parent = 0);
    ~Buttons();

private slots:
    void on_pushButton_clicked();

public:
    Ui::Buttons *ui;
    QImage Img;
    unsigned short int bits;
    int width, height;
    bool update;
};

#endif // BUTTONS_H
