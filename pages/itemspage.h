#pragma once
#include <QWidget>
#include "models/product.h"

class QTableWidget;
class QPushButton;
class QLabel;

class ItemsPage : public QWidget {
    Q_OBJECT
public:
    explicit ItemsPage(QWidget *parent = nullptr);

signals:
    void productSelected(const Product &p);

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void        refreshTable();
    void        addRowToTable(int row, const Product &pr);
    QPushButton* createPageBtn(const QString &text, bool active = false);

    QTableWidget   *m_table     = nullptr;
    QPushButton    *m_addBtn    = nullptr;
    QPushButton    *m_editBtn   = nullptr;
    QPushButton    *m_deleteBtn = nullptr;
    QPushButton    *m_filterBtn = nullptr;
    QLabel         *m_totalLbl  = nullptr;
    QList<Product>  m_products;
};