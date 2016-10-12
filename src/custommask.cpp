#include "custommask.h"
#include "ui_custommask.h"

CustomMask::CustomMask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomMask)
{
    ui->setupUi(this);
}

CustomMask::~CustomMask()
{
    delete ui;
}

void CustomMask::on_CrearFiltro_accepted()
{
    accept();
}

void CustomMask::on_CrearFiltro_rejected()
{
    reject();
}

int CustomMask::getX() const{
    return (ui->X->text()).toInt();
}
int CustomMask::getY() const{
    return (ui->Y->text()).toInt();
}
int CustomMask::getTam() const{
    return (ui->Tam->text()).toInt();
}


void CustomMask::on_SetData_clicked()
{
    tam = getTam();
    cX = getX();
    cY = getY();
    if(tam > 0 && (tam%2) && cX >= 0 && cY >= 0 &&  cX < tam && cY < tam){
        ui->CrearFiltro->setEnabled(true);
        ui->Table->setColumnCount(tam);
        ui->Table->setRowCount(tam);
        ui->Table->resizeColumnsToContents();
    }else
        ui->Status->setText("El tamaño debe ser Impar y mayor a 0 / El centro no debe salirse de los limites de la matriz");


}

void CustomMask::on_Tam_textChanged(const QString &arg1)
{
    ui->CrearFiltro->setEnabled(false);
}

void CustomMask::on_X_textChanged(const QString &arg1)
{
    ui->CrearFiltro->setEnabled(false);
}

void CustomMask::on_Y_textChanged(const QString &arg1)
{
    ui->CrearFiltro->setEnabled(false);
}

float CustomMask::getT(int k, int c){
    return (ui->Table->item(k,c)->text()).toFloat();
}
