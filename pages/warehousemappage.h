#pragma once
#include <QWidget>
#include "models/warehousecell.h"

class QComboBox;
class QPushButton;
class QLabel;
class QVBoxLayout;

class WarehouseMapPage : public QWidget {
    Q_OBJECT
public:
    explicit WarehouseMapPage(QWidget *parent = nullptr);

private:
    void selectCell(int index);
    void updateSectionPanel(const WarehouseCell &cell);

    QComboBox              *m_sectionCombo  = nullptr;
    QList<QPushButton*>     m_cellBtns;
    QList<WarehouseCell>    m_cells;
    int                     m_selectedIndex = -1;

    // Права панель
    QLabel                 *m_panelTitle    = nullptr;
    QVBoxLayout            *m_itemsLayout   = nullptr;
    QWidget                *m_itemsWidget   = nullptr;
};