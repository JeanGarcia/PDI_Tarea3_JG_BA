#include "buttons.h"
#include "ui_buttons.h"

Buttons::Buttons(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Buttons)
{
    ui->setupUi(this);
}

Buttons::~Buttons()
{
    delete ui;
}

void Buttons::on_pushButton_clicked()
{
    if(this->bits == 24){
        for(int r=0; r < (this->height)/3; r++){
                    for(int c=0; c<this->width/3; c++){
                        QColor clrCurrent(this->Img.pixel(r, c));

                        QRgb value = qRgb(255 - clrCurrent.red(), 255 - clrCurrent.green() , 255 - clrCurrent.blue());
                        this->Img.setPixel(r, c, value);
                    }
        }
        this->update = true;
    }else if(this->bits < 24){

    }
}
