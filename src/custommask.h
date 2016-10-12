#ifndef CUSTOMMASK_H
#define CUSTOMMASK_H

#include <QDialog>

namespace Ui {
class CustomMask;
}

class CustomMask : public QDialog
{
    Q_OBJECT

public:
    explicit CustomMask(QWidget *parent = 0);
    ~CustomMask();

    int getX() const;
    int getY() const;
    int getTam() const;
    float getT(int k, int c);

private slots:
    void on_CrearFiltro_accepted();

    void on_CrearFiltro_rejected();

    void on_SetData_clicked();

    void on_Tam_textChanged(const QString &arg1);

    void on_X_textChanged(const QString &arg1);

    void on_Y_textChanged(const QString &arg1);


public:
    Ui::CustomMask *ui;
    int tam, cX, cY;
};

#endif // CUSTOMMASK_H
