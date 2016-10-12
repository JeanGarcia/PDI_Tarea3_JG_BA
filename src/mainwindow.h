#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "histogram.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionCargar_triggered();

    void on_actionGuardar_triggered();

    void ZoomNN (double zoom);

    void fillHist();

    void SendOp(int op);

    void DoOperations();

    void Negativo();

    void Ecualizar();

    void EspejoX();

    void EspejoY();

    void vIlumination(int light);

    void Escals();

    void Contraste();

    void Umbral();

    void Rotate(int Angle);

    void on_pushButton_clicked();

    void on_bRotate_clicked();

    void on_Equal_clicked();

    void on_bHist_clicked();

    void on_bZoomOut_clicked();

    void on_bZoomIn_clicked();

    void on_bMirrorX_clicked();

    void on_bMirrorY_clicked();

    void on_sIuminacion_valueChanged(int value);

    void on_sbContraste_valueChanged(double arg1);

    void on_sUmbral_valueChanged(int value);

    void on_bEscalar_clicked();

    void on_action3x3_triggered();

    void on_action15x15_triggered();

    void on_action5x5_triggered();

    void on_action7x7_triggered();

    void on_action9x9_triggered();

    void on_action13x13_triggered();

    void on_action11x11_triggered();

    void on_bReset_clicked();

    void on_action3x3_2_triggered();

    void on_action5x5_2_triggered();

    void on_action7x7_2_triggered();

    void on_action9x9_2_triggered();

    void on_action11x11_2_triggered();

    void on_action13x13_2_triggered();

    void on_action15x15_2_triggered();

    void on_action3x3_3_triggered();

    void on_action5x5_3_triggered();

    void on_action7x7_3_triggered();

    void on_action9x9_3_triggered();

    void on_action11x11_3_triggered();

    void on_action13x13_3_triggered();

    void on_action15x15_3_triggered();

    void on_action3x3_4_triggered();

    void on_action5x5_4_triggered();

    void on_action7x7_4_triggered();

    void on_action9x9_4_triggered();

    void on_action11x11_4_triggered();

    void on_action13x13_4_triggered();

    void on_action15x15_4_triggered();

    void on_bUndo_clicked();

    void on_bRehacer_clicked();

    void on_actionCrear_Filtro_triggered();

    void on_action3x3_5_triggered();

    void on_action5x5_5_triggered();

    void on_action7x7_5_triggered();

    void on_action9x9_5_triggered();

    void on_action11x11_5_triggered();

    void on_action13x13_5_triggered();

    void on_action15x15_5_triggered();

    void on_action3x3_6_triggered();

    void on_action5x5_6_triggered();

public:
    Ui::MainWindow *ui;
    Histogram hist;
};

#endif // MAINWINDOW_H
