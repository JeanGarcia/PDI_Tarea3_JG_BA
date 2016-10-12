#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>
#include <QImage>

namespace Ui {
class Histogram;
}

class Histogram : public QDialog
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = 0);
    ~Histogram();
    void LoadHist(int r[], int g[], int b[], QImage Image);

public:
    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
