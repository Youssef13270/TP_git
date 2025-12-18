#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <mariadb/conncpp.hpp>
#include <iostream>
#include <QWidget>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Création des widgets
    log = new QLineEdit("", this);
    log->setPlaceholderText("Login");
    log->setGeometry(50, 50, 200, 30);




    mdp = new QLineEdit("", this);
    mdp->setPlaceholderText("Mot de passe");
    mdp->setEchoMode(QLineEdit::Password);  // Masquer le mot de passe
    mdp->setGeometry(50, 90, 200, 30);

    bConnexion = new QPushButton("Connexion", this);
    bConnexion->setGeometry(50, 130, 200, 30);

    // Connecter le bouton à un slot
    connect(bConnexion, &QPushButton::clicked, this, &MainWindow::onConnexionClicked);
}

void MainWindow::onConnexionClicked()
{
    // Récupérer les valeurs des champs
    QString username = log->text();
    QString password = mdp->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs");
        return;
    }

    try {
        // Connexion à la base
        sql::Driver* driver = sql::mariadb::get_driver_instance();
        sql::SQLString url("jdbc:mariadb://172.20.30.253:3306/y.sbaa");
        sql::Properties properties({{"user", "y.sbaa"}, {"password", "y.sbaa"}});
        sql::Connection* conn = driver->connect(url, properties);

        if (conn) {
            cout << "Connexion MariaDB réussie!" << endl;
        }

        // Préparer la requête de vérification
        sql::PreparedStatement* pstmt = conn->prepareStatement(
            "SELECT COUNT(*) as count FROM Logins WHERE identifiant=? AND mdp=SHA2(?,256)"
            );

        // Convertir QString en std::string
        pstmt->setString(1, username.toStdString());
        pstmt->setString(2, password.toStdString());

        // Exécuter la requête
        sql::ResultSet* res = pstmt->executeQuery();

        if (res->next()) {
            int count = res->getInt("count");

            if (count > 0) {
                QMessageBox::information(this, "Succès", "Connexion réussie!");
                cout << "Utilisateur authentifié" << endl;

                pstmt = conn->prepareStatement("SELECT id, prenom FROM bts_ciel2");
                res = pstmt->executeQuery();
                nouvellefenetre(res);


            } else {
                QMessageBox::warning(this, "Erreur", "Login ou mot de passe incorrect");
                cout << "Échec d'authentification" << endl;
            }
        }

        // Libération de la mémoire
        delete res;
        delete pstmt;
        delete conn;

    } catch (sql::SQLException &e) {
        cerr << "Erreur MariaDB : " << e.what() << endl;
        cerr << "Code erreur : " << e.getErrorCode() << endl;
        cerr << "État SQL : " << e.getSQLState() << endl;

        QMessageBox::critical(this, "Erreur de connexion",
                              QString("Erreur : %1").arg(e.what()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete log;
    delete mdp;
    delete bConnexion;
}

QLabel * MainWindow::getLabel(){
    return lab;
}

void MainWindow::nouvellefenetre(sql::ResultSet* res){
    fenetre = new QWidget();
    t = new QTableWidget(fenetre);



    t->setRowCount(20);
    t->setColumnCount(2);

    QString a ;
    lab =new QLabel(fenetre);

    this->hide();
    int i=0;
    while (res->next()) {

        QString id;
        QString prenom;
        id=(res->getString("id")) ;
        prenom=(res->getString("prenom"));

        QTableWidgetItem *item1 = new QTableWidgetItem(id);
        t->setItem(i, 0, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(prenom);
        t->setItem(i, 1, item2);
        i=i+1;
        cout<<i;
        cout << "id : " << res->getString("id") << endl;
        cout << "prenom : " << res->getString("prenom") << endl;
        //a = res->getString("prenom");
        //lab->setText(a) ;
        fenetre->show();
    }

}
