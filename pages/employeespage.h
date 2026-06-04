#pragma once
#include <QWidget>
#include "models/employee.h"

class QTableWidget;
class QPushButton;
class QLabel;
class QFrame;

class EmployeesPage : public QWidget {
    Q_OBJECT
public:
    explicit EmployeesPage(QWidget *parent = nullptr);

private slots:
    void onAddEmployeeClicked();
    void onEditEmployeeClicked();

private:
    void refreshEmployeeTable();
    void showEmployee(int index);

    QTableWidget    *m_table       = nullptr;
    QList<Employee>  m_employees;

    QWidget         *m_card        = nullptr;
    QLabel          *m_cardAvatar  = nullptr;
    QLabel          *m_cardName    = nullptr;
    QLabel          *m_cardPos     = nullptr;
    QLabel          *m_cardPhone   = nullptr;
    QLabel          *m_cardEmail   = nullptr;
    QLabel          *m_cardZone    = nullptr;
    QLabel          *m_cardNote    = nullptr;
};