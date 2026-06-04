#include "itemcardpage.h"
#include "appstyle.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QFrame>


static QHBoxLayout* makeFieldRow(const QString &fieldName, QLabel *&valueLabel,
                                 QWidget *parent)
{
    auto *row = new QHBoxLayout();
    row->setSpacing(8);

    auto *keyLbl = new QLabel(fieldName + ":", parent);
    keyLbl->setStyleSheet(
        QString("color:%1; font-size:13px; font-weight:600; background:transparent;"
                "min-width:160px;")
            .arg(AppStyle::Color::TEXT_DIM));
    keyLbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    valueLabel = new QLabel("—", parent);
    valueLabel->setStyleSheet(
        QString("color:%1; font-size:13px; background:transparent;")
            .arg(AppStyle::Color::TEXT_PRIMARY));
    valueLabel->setWordWrap(true);

    row->addWidget(keyLbl);
    row->addWidget(valueLabel, 1);
    return row;
}


ItemCardPage::ItemCardPage(QWidget *parent) : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 16, 24, 16);
    mainLayout->setSpacing(12);


    auto *breadcrumb = new QHBoxLayout();
    breadcrumb->setSpacing(6);

    auto *backBtn = new QPushButton("← Товари", this);
    backBtn->setObjectName("btnSecondary");
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setToolTip("Повернутись до списку товарів");
    breadcrumb->addWidget(backBtn);

    auto *sep = new QLabel("/", this);
    sep->setStyleSheet(
        QString("color:%1; background:transparent;").arg(AppStyle::Color::TEXT_DIM));
    breadcrumb->addWidget(sep);

    m_crumbCode = new QLabel("—", this);
    m_crumbCode->setStyleSheet(
        QString("color:%1; background:transparent; font-weight:600;")
            .arg(AppStyle::Color::TEXT_DIM));
    breadcrumb->addWidget(m_crumbCode);

    auto *sep2 = new QLabel("/", this);
    sep2->setStyleSheet(
        QString("color:%1; background:transparent;").arg(AppStyle::Color::TEXT_DIM));
    breadcrumb->addWidget(sep2);

    m_crumbName = new QLabel("—", this);
    m_crumbName->setStyleSheet(
        QString("color:%1; background:transparent; font-weight:600;")
            .arg(AppStyle::Color::TEXT_PRIMARY));
    breadcrumb->addWidget(m_crumbName);
    breadcrumb->addStretch();

    mainLayout->addLayout(breadcrumb);


    auto *infoCard = new QFrame(this);
    infoCard->setObjectName("panelCard");
    auto *infoLayout = new QVBoxLayout(infoCard);
    infoLayout->setContentsMargins(24, 20, 24, 20);
    infoLayout->setSpacing(10);

    auto *cardTitle = new QLabel("Картка товару", infoCard);
    cardTitle->setStyleSheet(
        QString("color:%1; font-size:%2px; font-weight:700; background:transparent;")
            .arg(AppStyle::Color::TEXT_PRIMARY).arg(AppStyle::Font::MD));
    infoLayout->addWidget(cardTitle);

    auto *divider = new QFrame(infoCard);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet(
        QString("border:none; border-top:1px solid %1;").arg(AppStyle::Color::BORDER_PANEL));
    infoLayout->addWidget(divider);


    infoLayout->addLayout(makeFieldRow("Категорія",       m_lblCategory, infoCard));
    infoLayout->addLayout(makeFieldRow("Код товару",      m_lblCode,     infoCard));
    infoLayout->addLayout(makeFieldRow("Штрих-код",       m_lblBarcode,  infoCard));
    infoLayout->addLayout(makeFieldRow("Місце зберігання",m_lblLocation, infoCard));
    infoLayout->addLayout(makeFieldRow("Кількість",       m_lblQty,      infoCard));
    infoLayout->addLayout(makeFieldRow("Дата надходження",m_lblDate,     infoCard));
    infoLayout->addLayout(makeFieldRow("Виробник",        m_lblMfr,      infoCard));
    infoLayout->addLayout(makeFieldRow("Опис",            m_lblDesc,     infoCard));

    mainLayout->addWidget(infoCard);


    auto *histFrame = new QFrame(this);
    histFrame->setObjectName("panelCard");
    auto *histLayout = new QVBoxLayout(histFrame);
    histLayout->setContentsMargins(0, 0, 0, 0);
    histLayout->setSpacing(0);

    auto *histTitle = new QLabel("  Історія операцій", histFrame);
    histTitle->setStyleSheet(
        QString("color:%1; font-size:%2px; font-weight:600; background:transparent;"
                "padding: 12px 0 8px 0;")
            .arg(AppStyle::Color::TEXT_PRIMARY).arg(AppStyle::Font::MD));
    histLayout->addWidget(histTitle);

    m_historyTable = new QTableWidget(histFrame);
    m_historyTable->setColumnCount(4);
    m_historyTable->setHorizontalHeaderLabels(
        {"Дата", "Операція", "Кількість", "Виконавець"});
    m_historyTable->setFrameShape(QFrame::NoFrame);
    m_historyTable->setShowGrid(false);
    m_historyTable->setAlternatingRowColors(true);
    m_historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_historyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_historyTable->verticalHeader()->setVisible(false);
    m_historyTable->verticalHeader()->setDefaultSectionSize(
        AppStyle::Size::TABLE_ROW_HEIGHT);
    m_historyTable->horizontalHeader()->setStretchLastSection(true);
    m_historyTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_historyTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_historyTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_historyTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_historyTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    histLayout->addWidget(m_historyTable);
    mainLayout->addWidget(histFrame, 1);


    connect(backBtn, &QPushButton::clicked, this, &ItemCardPage::backClicked);
}


void ItemCardPage::loadProduct(const Product &p)
{
    m_crumbCode->setText(p.code);
    m_crumbName->setText(p.name);

    m_lblCategory->setText(p.category);
    m_lblCode->setText(p.code);
    m_lblBarcode->setText(p.barcode.isEmpty() ? "—" : p.barcode);
    m_lblLocation->setText(p.location);
    m_lblQty->setText(QString::number(p.quantity) + " шт.");
    m_lblDate->setText(p.arrivalDate.isEmpty() ? "—" : p.arrivalDate);
    m_lblMfr->setText(p.manufacturer.isEmpty() ? "—" : p.manufacturer);
    m_lblDesc->setText(p.description.isEmpty() ? "—" : p.description);


    m_historyTable->setRowCount(0);
    for (const auto &ch : p.changes) {
        const int row = m_historyTable->rowCount();
        m_historyTable->insertRow(row);

        auto mkItem = [](const QString &txt) {
            auto *it = new QTableWidgetItem(txt);
            it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            return it;
        };

        m_historyTable->setItem(row, 0, mkItem(ch.date));
        m_historyTable->setItem(row, 1, mkItem(ch.operation));
        m_historyTable->setItem(row, 2, mkItem(QString::number(ch.quantity)));
        m_historyTable->setItem(row, 3, mkItem(ch.executor));
    }
}