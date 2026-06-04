#pragma once
#include <QWidget>
#include "models/product.h"

class QPushButton;
class QTableWidget;
class QLabel;

class ItemCardPage : public QWidget {
    Q_OBJECT
public:
    explicit ItemCardPage(QWidget *parent = nullptr);
    void loadProduct(const Product &p);

signals:
    void backClicked();

private:
    QLabel       *m_crumbCode    = nullptr;
    QLabel       *m_crumbName    = nullptr;


    QLabel       *m_lblCategory  = nullptr;
    QLabel       *m_lblCode      = nullptr;
    QLabel       *m_lblBarcode   = nullptr;
    QLabel       *m_lblLocation  = nullptr;
    QLabel       *m_lblQty       = nullptr;
    QLabel       *m_lblDate      = nullptr;
    QLabel       *m_lblMfr       = nullptr;
    QLabel       *m_lblDesc      = nullptr;


    QTableWidget *m_historyTable = nullptr;
};