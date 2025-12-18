#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnexionClicked();  // Slot pour gérer le clic du bouton

private:
    Ui::MainWindow *ui;
    QLineEdit *log;
    QLineEdit *mdp;
    QPushButton *bConnexion;
    QWidget *fenetre;

};

#endif // MAINWINDOW_H
