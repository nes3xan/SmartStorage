#include "itemspage.h"
#include "appstyle.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QFrame>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QMessageBox>


static bool execProductDialog(Product &p, QWidget *parent, const QString &title)
{
    QDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setMinimumWidth(420);

    auto *form = new QFormLayout(&dlg);
    form->setSpacing(10);
    form->setContentsMargins(20, 20, 20, 16);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    auto mkLine = [&](const QString &val, const QString &tip = {}) {
        auto *e = new QLineEdit(val, &dlg);
        if (!tip.isEmpty()) e->setToolTip(tip);
        return e;
    };

    auto *codeEdit    = mkLine(p.code,         "Артикул, напр. TOB-012");
    auto *nameEdit    = mkLine(p.name,          "Повна назва товару");
    auto *catEdit     = mkLine(p.category,      "Категорія, напр. Монітори");
    auto *locEdit     = mkLine(p.location,      "Код місця, напр. A2-B1-C3");
    auto *barcodeEdit = mkLine(p.barcode);
    auto *dateEdit    = mkLine(p.arrivalDate);
    dateEdit->setPlaceholderText("ДД.ММ.РРРР");
    auto *mfrEdit     = mkLine(p.manufacturer);
    auto *descEdit    = mkLine(p.description);

    auto *qtyBox = new QSpinBox(&dlg);
    qtyBox->setRange(0, 99999);
    qtyBox->setValue(p.quantity);
    qtyBox->setToolTip("Кількість одиниць на складі");

    auto *statusCombo = new QComboBox(&dlg);
    statusCombo->addItems({"В наявності", "Низький запас", "Відсутній"});
    statusCombo->setCurrentText(p.status.isEmpty() ? "В наявності" : p.status);
    statusCombo->setToolTip("Поточний статус наявності товару");

    form->addRow("Код товару:",       codeEdit);
    form->addRow("Назва:",            nameEdit);
    form->addRow("Категорія:",        catEdit);
    form->addRow("Кількість:",        qtyBox);
    form->addRow("Місце зберігання:", locEdit);
    form->addRow("Статус:",           statusCombo);
    form->addRow("Штрих-код:",        barcodeEdit);
    form->addRow("Дата надходження:", dateEdit);
    form->addRow("Виробник:",         mfrEdit);
    form->addRow("Опис:",             descEdit);

    auto *btns = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    btns->button(QDialogButtonBox::Ok)->setText("Зберегти");
    btns->button(QDialogButtonBox::Ok)->setObjectName("btnPrimary");
    btns->button(QDialogButtonBox::Cancel)->setText("Скасувати");
    btns->button(QDialogButtonBox::Cancel)->setObjectName("btnSecondary");
    form->addRow(btns);

    QObject::connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return false;

    p.code         = codeEdit->text().trimmed();
    p.name         = nameEdit->text().trimmed();
    p.category     = catEdit->text().trimmed();
    p.quantity     = qtyBox->value();
    p.location     = locEdit->text().trimmed();
    p.status       = statusCombo->currentText();
    p.barcode      = barcodeEdit->text().trimmed();
    p.arrivalDate  = dateEdit->text().trimmed();
    p.manufacturer = mfrEdit->text().trimmed();
    p.description  = descEdit->text().trimmed();
    return true;
}


QPushButton* ItemsPage::createPageBtn(const QString &text, bool active)
{
    auto *btn = new QPushButton(text, this);
    btn->setObjectName(active ? "btnPageActive" : "btnPage");
    btn->setFixedSize(32, 32);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setToolTip(active
                        ? QString("Поточна сторінка: %1").arg(text)
                        : QString("Перейти на сторінку %1").arg(text));
    return btn;
}


void ItemsPage::addRowToTable(int row, const Product &pr)
{
    m_table->insertRow(row);

    auto mkItem = [](const QString &txt) {
        auto *it = new QTableWidgetItem(txt);
        it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        return it;
    };

    m_table->setItem(row, 0, mkItem(pr.code));
    m_table->setItem(row, 1, mkItem(pr.name));
    m_table->setItem(row, 2, mkItem(pr.category));
    m_table->setItem(row, 3, mkItem(QString::number(pr.quantity)));
    m_table->setItem(row, 4, mkItem(pr.location));

    // Статус — кольоровий текст без фонового бейджа (відповідає макету Figma)
    auto *statusItem = mkItem(pr.status);
    if (pr.status == "В наявності")
        statusItem->setForeground(QColor(AppStyle::Color::STATUS_OK));
    else if (pr.status == "Низький запас")
        statusItem->setForeground(QColor(AppStyle::Color::STATUS_WARN));
    else
        statusItem->setForeground(QColor(AppStyle::Color::STATUS_ERR));
    m_table->setItem(row, 5, statusItem);
}


void ItemsPage::refreshTable()
{
    m_table->setRowCount(0);
    for (int i = 0; i < m_products.size(); ++i)
        addRowToTable(i, m_products[i]);

    if (m_totalLbl)
        m_totalLbl->setText(
            QString("Всього позицій: %1").arg(m_products.size()));
}


void ItemsPage::onAddClicked()
{
    Product p;
    if (execProductDialog(p, this, "Додати товар")) {
        if (p.code.isEmpty() || p.name.isEmpty()) {
            QMessageBox::warning(this, "Помилка", "Код та назва товару є обов'язковими.");
            return;
        }
        m_products.append(p);
        refreshTable();
        m_table->selectRow(m_products.size() - 1);
    }
}

void ItemsPage::onEditClicked()
{
    const int row = m_table->currentRow();
    if (row < 0 || row >= m_products.size()) {
        QMessageBox::information(this, "Редагування", "Оберіть рядок у таблиці.");
        return;
    }
    Product p = m_products[row];
    if (execProductDialog(p, this, "Редагувати товар")) {
        m_products[row] = p;
        refreshTable();
        m_table->selectRow(row);
    }
}

void ItemsPage::onDeleteClicked()
{
    const int row = m_table->currentRow();
    if (row < 0 || row >= m_products.size()) {
        QMessageBox::information(this, "Видалення", "Оберіть рядок у таблиці.");
        return;
    }
    const QString name = m_products[row].name;
    auto reply = QMessageBox::question(
        this, "Підтвердження видалення",
        QString("Видалити товар «%1»?").arg(name),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_products.removeAt(row);
        refreshTable();
        if (!m_products.isEmpty())
            m_table->selectRow(qMin(row, m_products.size() - 1));
    }
}


ItemsPage::ItemsPage(QWidget *parent) : QWidget(parent)
{
    m_products = Product::mockData();

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 16, 24, 14);
    mainLayout->setSpacing(12);


    auto *title = new QLabel("Товари", this);
    title->setStyleSheet(
        QString("background:transparent; color:%1; font-size:%2px; font-weight:700;")
            .arg(AppStyle::Color::TEXT_PRIMARY).arg(AppStyle::Font::LG));
    mainLayout->addWidget(title);


    auto *topBar = new QHBoxLayout(); topBar->setSpacing(10);

    m_addBtn = new QPushButton("+ Додати", this);
    m_addBtn->setObjectName("btnPrimary");
    m_addBtn->setCursor(Qt::PointingHandCursor);
    m_addBtn->setToolTip("Додати новий товар до бази складу");

    m_editBtn = new QPushButton("Редагувати", this);
    m_editBtn->setObjectName("btnSecondary");
    m_editBtn->setCursor(Qt::PointingHandCursor);
    m_editBtn->setToolTip("Редагувати обраний товар");

    m_deleteBtn = new QPushButton("Видалити", this);
    m_deleteBtn->setObjectName("btnSecondary");
    m_deleteBtn->setCursor(Qt::PointingHandCursor);
    m_deleteBtn->setToolTip("Видалити обраний товар зі складу");

    topBar->addWidget(m_addBtn);
    topBar->addWidget(m_editBtn);
    topBar->addWidget(m_deleteBtn);
    topBar->addStretch();

    m_filterBtn = new QPushButton("Фільтр ▾", this);
    m_filterBtn->setObjectName("btnSecondary");
    m_filterBtn->setCursor(Qt::PointingHandCursor);
    m_filterBtn->setToolTip("Фільтрувати список за категорією або статусом");
    topBar->addWidget(m_filterBtn);

    mainLayout->addLayout(topBar);


    auto *tableFrame = new QFrame(this);
    tableFrame->setObjectName("panelCard");
    auto *frameV = new QVBoxLayout(tableFrame);
    frameV->setContentsMargins(0, 0, 0, 0);
    frameV->setSpacing(0);

    m_table = new QTableWidget(tableFrame);
    m_table->setColumnCount(6);
    m_table->setHorizontalHeaderLabels(
        {"Код", "Назва товару", "Категорія", "Кількість", "Місце зберіг.", "Статус"});
    m_table->setToolTip("Клік — переглянути картку товару. Використовуйте кнопки вище для CRUD-операцій.");
    m_table->setFrameShape(QFrame::NoFrame);
    m_table->setShowGrid(false);
    m_table->setAlternatingRowColors(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(AppStyle::Size::TABLE_ROW_HEIGHT);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    frameV->addWidget(m_table);
    mainLayout->addWidget(tableFrame, 1);

    refreshTable();


    connect(m_table, &QTableWidget::cellClicked, this, [this](int row, int) {
        if (row >= 0 && row < m_products.size())
            emit productSelected(m_products[row]);
    });
    connect(m_addBtn,    &QPushButton::clicked, this, &ItemsPage::onAddClicked);
    connect(m_editBtn,   &QPushButton::clicked, this, &ItemsPage::onEditClicked);
    connect(m_deleteBtn, &QPushButton::clicked, this, &ItemsPage::onDeleteClicked);


    auto *bottomBar = new QHBoxLayout(); bottomBar->setSpacing(6);

    m_totalLbl = new QLabel(this);
    m_totalLbl->setStyleSheet(
        QString("color:%1; font-size:13px; background:transparent;")
            .arg(AppStyle::Color::TEXT_DIM));
    m_totalLbl->setToolTip("Загальна кількість позицій у поточній вибірці");
    bottomBar->addWidget(m_totalLbl);
    bottomBar->addStretch();

    auto *pagRow = new QHBoxLayout(); pagRow->setSpacing(5);
    pagRow->addWidget(createPageBtn("‹"));
    pagRow->addWidget(createPageBtn("1", true));
    pagRow->addWidget(createPageBtn("2"));
    pagRow->addWidget(createPageBtn("3"));
    auto *dots = new QLabel("...", this);
    dots->setStyleSheet(QString("color:%1; background:transparent; padding: 0 2px;")
                            .arg(AppStyle::Color::TEXT_DIM));
    pagRow->addWidget(dots);
    pagRow->addWidget(createPageBtn("94"));
    pagRow->addWidget(createPageBtn("›"));
    bottomBar->addLayout(pagRow);

    mainLayout->addLayout(bottomBar);


    m_totalLbl->setText(QString("Всього позицій: %1").arg(m_products.size()));
}