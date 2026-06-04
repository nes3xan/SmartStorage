#include "warehousemappage.h"
#include "appstyle.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>
#include <QScrollArea>

WarehouseMapPage::WarehouseMapPage(QWidget *parent) : QWidget(parent)
{
    m_cells = WarehouseCell::mockData();

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 24);
    mainLayout->setSpacing(20);


    auto *sectionRow = new QHBoxLayout();
    sectionRow->setSpacing(12);

    auto *sectionLbl = new QLabel("Вибрати секцію:", this);
    sectionLbl->setStyleSheet(
        QString("color:%1; font-size:13px; background:transparent;")
            .arg(AppStyle::Color::TEXT_PRIMARY));
    sectionRow->addWidget(sectionLbl);

    m_sectionCombo = new QComboBox(this);
    m_sectionCombo->setToolTip("Перейти до секції складу");
    for (const auto &id : WarehouseCell::allSectionIds())
        m_sectionCombo->addItem(id);
    sectionRow->addWidget(m_sectionCombo);
    sectionRow->addStretch(1);

    mainLayout->addLayout(sectionRow);


    auto *contentH = new QHBoxLayout();
    contentH->setSpacing(24);


    auto *gridFrame = new QFrame(this);
    gridFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    gridFrame->setObjectName("panelCard");
    auto *gridFrameV = new QVBoxLayout(gridFrame);
    gridFrameV->setContentsMargins(20, 20, 20, 16);
    gridFrameV->setSpacing(0);


    auto *colHeaders = new QHBoxLayout();
    colHeaders->setSpacing(0);
    colHeaders->addSpacing(0);

    const QStringList colLabels = {"A", "B", "C"};
    for (const auto &lbl : colLabels) {
        auto *h = new QLabel(lbl, gridFrame);
        h->setAlignment(Qt::AlignCenter);
        h->setStyleSheet(
            QString("color:%1; font-size:13px; font-weight:600;"
                    " background:transparent;")
                .arg(AppStyle::Color::TEXT_DIM));
        colHeaders->addWidget(h, 1);
    }
    gridFrameV->addLayout(colHeaders);
    gridFrameV->addSpacing(8);


    const int COLS = 3;
    const int ROWS = 3;

    auto *grid = new QGridLayout();
    grid->setSpacing(12);

    for (int col = 0; col < COLS; ++col) {
        for (int row = 0; row < ROWS; ++row) {
            int idx = col * ROWS + row;   // відповідає порядку в mockData
            if (idx >= m_cells.size()) break;

            auto *btn = new QPushButton(m_cells[idx].id, gridFrame);
            btn->setObjectName("warehouseCell");
            btn->setCheckable(true);
            btn->setMinimumSize(AppStyle::Size::WAREHOUSE_CELL_MIN,
                                AppStyle::Size::WAREHOUSE_CELL_MIN);
            btn->setCursor(Qt::PointingHandCursor);
            btn->setToolTip(QString("Секція %1").arg(m_cells[idx].id));
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            m_cellBtns.append(btn);

            const int capturedIdx = idx;
            connect(btn, &QPushButton::clicked,
                    this, [this, capturedIdx]() { selectCell(capturedIdx); });

            grid->addWidget(btn, row, col);
        }
    }

    for (int i = 0; i < ROWS; ++i) grid->setRowStretch(i, 1);
    for (int i = 0; i < COLS; ++i) grid->setColumnStretch(i, 1);

    gridFrameV->addLayout(grid, 1);
    gridFrameV->addSpacing(10);


    auto *entryLbl = new QLabel("Вхід/Вихід", gridFrame);
    entryLbl->setAlignment(Qt::AlignCenter);
    entryLbl->setStyleSheet(
        QString("color:%1; font-size:12px; background:transparent;")
            .arg(AppStyle::Color::TEXT_DIM));
    gridFrameV->addWidget(entryLbl);

    contentH->addWidget(gridFrame, 3);


    auto *rightFrame = new QFrame(this);
    rightFrame->setObjectName("panelCard");
    rightFrame->setMinimumWidth(240);
    auto *rightV = new QVBoxLayout(rightFrame);
    rightV->setContentsMargins(20, 20, 20, 20);
    rightV->setSpacing(16);


    auto *titleRow = new QHBoxLayout();
    auto *titleKey = new QLabel("Товари в секції", rightFrame);
    titleKey->setObjectName("panelTitle");
    titleRow->addWidget(titleKey);
    titleRow->addStretch(1);
    m_panelTitle = new QLabel("—", rightFrame);
    m_panelTitle->setStyleSheet(
        QString("color:%1; font-size:13px; font-weight:700;"
                " background:transparent;")
            .arg(AppStyle::Color::TEXT_PRIMARY));
    titleRow->addWidget(m_panelTitle);
    rightV->addLayout(titleRow);


    auto *sep = new QFrame(rightFrame);
    sep->setFrameShape(QFrame::HLine);
    rightV->addWidget(sep);


    auto *scrollArea = new QScrollArea(rightFrame);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("background:transparent;");

    m_itemsWidget = new QWidget(scrollArea);
    m_itemsWidget->setStyleSheet("background:transparent;");
    m_itemsLayout = new QVBoxLayout(m_itemsWidget);
    m_itemsLayout->setContentsMargins(0, 0, 0, 0);
    m_itemsLayout->setSpacing(12);
    m_itemsLayout->addStretch(1);

    scrollArea->setWidget(m_itemsWidget);
    rightV->addWidget(scrollArea, 1);


    auto *detailsBtn = new QPushButton("Переглянути деталі", rightFrame);
    detailsBtn->setObjectName("btnPrimary");
    detailsBtn->setCursor(Qt::PointingHandCursor);
    detailsBtn->setToolTip("Відкрити детальний перегляд секції");
    rightV->addWidget(detailsBtn);

    contentH->addWidget(rightFrame, 2);
    mainLayout->addLayout(contentH, 1);


    connect(m_sectionCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int comboIdx) {

                const QString targetId = m_sectionCombo->itemText(comboIdx);
                for (int i = 0; i < m_cells.size(); ++i) {
                    if (m_cells[i].id == targetId) {
                        selectCell(i);
                        return;
                    }
                }
            });


    const int defaultCombo = m_sectionCombo->findText("C-03");
    if (defaultCombo >= 0) {
        m_sectionCombo->blockSignals(true);
        m_sectionCombo->setCurrentIndex(defaultCombo);
        m_sectionCombo->blockSignals(false);
    }

    for (int i = 0; i < m_cells.size(); ++i) {
        if (m_cells[i].id == "C-03" && !m_cells[i].items.isEmpty()) {
            selectCell(i);
            break;
        }
    }
}



void WarehouseMapPage::selectCell(int index)
{
    if (index < 0 || index >= m_cells.size()) return;


    for (auto *btn : m_cellBtns)
        btn->setChecked(false);


    if (index < m_cellBtns.size())
        m_cellBtns[index]->setChecked(true);

    m_selectedIndex = index;


    const int comboIdx = m_sectionCombo->findText(m_cells[index].id);
    if (comboIdx >= 0 && m_sectionCombo->currentIndex() != comboIdx) {
        m_sectionCombo->blockSignals(true);
        m_sectionCombo->setCurrentIndex(comboIdx);
        m_sectionCombo->blockSignals(false);
    }

    updateSectionPanel(m_cells[index]);
}



void WarehouseMapPage::updateSectionPanel(const WarehouseCell &cell)
{
    m_panelTitle->setText(cell.id);


    while (m_itemsLayout->count() > 1)
        delete m_itemsLayout->takeAt(0)->widget();

    if (cell.items.isEmpty()) {
        auto *emptyLbl = new QLabel("— порожня секція —", m_itemsWidget);
        emptyLbl->setStyleSheet(
            QString("color:%1; font-size:13px; background:transparent;")
                .arg(AppStyle::Color::TEXT_DIM));
        emptyLbl->setAlignment(Qt::AlignCenter);
        m_itemsLayout->insertWidget(0, emptyLbl);
        return;
    }

    for (int i = 0; i < cell.items.size(); ++i) {
        const auto &item = cell.items[i];

        auto *row = new QWidget(m_itemsWidget);
        row->setStyleSheet("background:transparent;");
        auto *h = new QHBoxLayout(row);
        h->setContentsMargins(0, 0, 0, 0);
        h->setSpacing(8);

        auto *nameLbl = new QLabel(item.productName, row);
        nameLbl->setStyleSheet(
            QString("color:%1; font-size:13px; background:transparent;")
                .arg(AppStyle::Color::TEXT_PRIMARY));
        nameLbl->setWordWrap(true);

        auto *qtyLbl = new QLabel(
            QString("%1 шт.").arg(item.quantity), row);
        qtyLbl->setStyleSheet(
            QString("color:%1; font-size:12px; background:transparent;")
                .arg(AppStyle::Color::TEXT_DIM));
        qtyLbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        h->addWidget(nameLbl, 1);
        h->addWidget(qtyLbl);


        auto *itemV = new QVBoxLayout();
        itemV->setSpacing(8);
        itemV->setContentsMargins(0, 0, 0, 0);
        itemV->addWidget(row);
        if (i < cell.items.size() - 1) {
            auto *div = new QFrame(m_itemsWidget);
            div->setFrameShape(QFrame::HLine);
            itemV->addWidget(div);
        }

        auto *wrapper = new QWidget(m_itemsWidget);
        wrapper->setStyleSheet("background:transparent;");
        wrapper->setLayout(itemV);
        m_itemsLayout->insertWidget(i, wrapper);
    }
}