#include "employeespage.h"
#include "appstyle.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QFrame>

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QTextEdit>

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

static QPixmap makeAvatarPixmap(const QString &/*name*/, int size = 80)
{
    QPixmap pm(size, size);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);

    const qreal s  = size;
    const qreal r  = s * 0.18;
    const qreal cx = s / 2.0;

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0x25, 0x33, 0x58));
    p.drawRoundedRect(QRectF(0, 0, s, s), r, r);


    const qreal headR  = s * 0.20;
    const qreal headCY = s * 0.36;
    p.setBrush(QColor(0x7B, 0x8E, 0xA8));
    p.drawEllipse(QPointF(cx, headCY), headR, headR);


    const qreal bodyRX = s * 0.42;
    const qreal bodyRY = s * 0.22;
    const qreal bodyCY = s * 0.88;

    QPainterPath clip;
    clip.addRect(QRectF(0, 0, s, s));
    p.setClipPath(clip);

    p.drawEllipse(QPointF(cx, bodyCY), bodyRX, bodyRY);

    p.end();
    return pm;
}

EmployeesPage::EmployeesPage(QWidget *parent) : QWidget(parent)
{
    m_employees = Employee::mockData();

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 24);
    mainLayout->setSpacing(20);


    auto *topBar = new QHBoxLayout();
    topBar->setSpacing(12);

    auto *addBtn = new QPushButton("+  Додати працівника", this);
    addBtn->setObjectName("btnOutlinePlus");
    addBtn->setCursor(Qt::PointingHandCursor);
    addBtn->setToolTip("Додати нового працівника");
    topBar->addWidget(addBtn);

    topBar->addStretch(1);

    auto *editBtn = new QPushButton("Редагувати", this);
    editBtn->setObjectName("btnPrimary");
    editBtn->setCursor(Qt::PointingHandCursor);
    editBtn->setToolTip("Редагувати дані обраного працівника");
    topBar->addWidget(editBtn);

    mainLayout->addLayout(topBar);


    auto *tableFrame = new QFrame(this);
    tableFrame->setObjectName("panelCard");
    auto *frameLayout = new QVBoxLayout(tableFrame);
    frameLayout->setContentsMargins(0, 0, 0, 0);
    frameLayout->setSpacing(0);

    m_table = new QTableWidget(tableFrame);
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({
        "ПІБ", "Посада", "Зона відповідальності", "Телефон", "E-mail"
    });

    m_table->setFrameShape(QFrame::NoFrame);
    m_table->setShowGrid(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(AppStyle::Size::TABLE_ROW_HEIGHT);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    for (int i = 0; i < m_employees.size(); ++i) {
        const auto &e = m_employees[i];
        m_table->insertRow(i);

        auto makeItem = [](const QString &txt) {
            auto *it = new QTableWidgetItem(txt);
            it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            return it;
        };

        m_table->setItem(i, 0, makeItem(e.name));
        m_table->setItem(i, 1, makeItem(e.position));
        m_table->setItem(i, 2, makeItem(e.zone));
        m_table->setItem(i, 3, makeItem(e.phone));
        m_table->setItem(i, 4, makeItem(e.email));
    }

    frameLayout->addWidget(m_table);
    mainLayout->addWidget(tableFrame);


    m_card = new QWidget(this);
    m_card->setObjectName("employeeCard");
    m_card->setVisible(false);

    auto *cardH = new QHBoxLayout(m_card);
    cardH->setContentsMargins(24, 20, 24, 20);
    cardH->setSpacing(24);


    m_cardAvatar = new QLabel(m_card);
    m_cardAvatar->setObjectName("avatarFrame");
    m_cardAvatar->setAlignment(Qt::AlignCenter);
    m_cardAvatar->setFixedSize(80, 80);
    cardH->addWidget(m_cardAvatar, 0, Qt::AlignVCenter);


    auto *nameBlock = new QVBoxLayout();
    nameBlock->setSpacing(4);
    m_cardName = new QLabel(m_card);
    m_cardName->setObjectName("employeeNameLabel");
    m_cardPos  = new QLabel(m_card);
    m_cardPos->setObjectName("employeePositionLabel");
    nameBlock->addStretch(1);
    nameBlock->addWidget(m_cardName);
    nameBlock->addWidget(m_cardPos);
    nameBlock->addStretch(1);
    cardH->addLayout(nameBlock);


    auto *vline = new QFrame(m_card);
    vline->setFrameShape(QFrame::VLine);
    vline->setStyleSheet("color:#1E2F4E; background:#1E2F4E; max-width:1px;");
    cardH->addWidget(vline);


    auto *detailsGrid = new QVBoxLayout();
    detailsGrid->setSpacing(8);

    auto makeDetailRow = [&](QLabel *&valLabel,
                             const QString &keyText) -> QHBoxLayout* {
        auto *row = new QHBoxLayout();
        row->setSpacing(8);
        auto *key = new QLabel(keyText + ":", m_card);
        key->setObjectName("employeeDetailKey");
        key->setFixedWidth(90);
        valLabel = new QLabel(m_card);
        valLabel->setObjectName("employeeDetailVal");
        valLabel->setWordWrap(true);
        row->addWidget(key);
        row->addWidget(valLabel, 1);
        return row;
    };

    detailsGrid->addLayout(makeDetailRow(m_cardPhone, "Телефон"));
    detailsGrid->addLayout(makeDetailRow(m_cardEmail, "E-mail"));
    detailsGrid->addLayout(makeDetailRow(m_cardZone,  "Зона відповідальності"));
    detailsGrid->addLayout(makeDetailRow(m_cardNote,  "Примітка"));
    cardH->addLayout(detailsGrid, 1);

    mainLayout->addWidget(m_card);

    connect(addBtn,  &QPushButton::clicked, this, &EmployeesPage::onAddEmployeeClicked);
    connect(editBtn, &QPushButton::clicked, this, &EmployeesPage::onEditEmployeeClicked);

    connect(m_table, &QTableWidget::cellClicked,
            this, [this](int row, int) { showEmployee(row); });

    if (!m_employees.isEmpty()) {
        m_table->selectRow(0);
        showEmployee(0);
    }
}

void EmployeesPage::showEmployee(int index)
{
    if (index < 0 || index >= m_employees.size())
        return;

    const auto &e = m_employees[index];

    m_cardName->setText(e.name);
    m_cardPos->setText(e.position);
    m_cardPhone->setText(e.phone);

    m_cardAvatar->setPixmap(makeAvatarPixmap(e.name, 80));

    m_cardEmail->setText(e.email);
    m_cardEmail->setObjectName("employeeEmailLink");
    m_cardEmail->style()->unpolish(m_cardEmail);
    m_cardEmail->style()->polish(m_cardEmail);

    m_cardZone->setText(e.zone);
    m_cardNote->setText(e.note);

    m_card->setVisible(true);
}

static bool execEmployeeDialog(Employee &e, QWidget *parent, const QString &title)
{
    QDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setMinimumWidth(400);

    auto *form = new QFormLayout(&dlg);
    form->setSpacing(10);
    form->setContentsMargins(20, 20, 20, 16);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    auto mk = [&](const QString &val, const QString &tip = {}) {
        auto *e = new QLineEdit(val, &dlg);
        if (!tip.isEmpty()) e->setToolTip(tip);
        return e;
    };

    auto *nameEdit  = mk(e.name,     "Прізвище та ініціали, напр. Іванов А.С.");
    auto *posEdit   = mk(e.position, "Посада, напр. Вантажник або Керівник");
    auto *zoneEdit  = mk(e.zone,     "Зони відповідальності, напр. Секції: А, В");
    auto *phoneEdit = mk(e.phone,    "Номер телефону");
    auto *emailEdit = mk(e.email,    "Корпоративна або особиста пошта");

    auto *noteEdit = new QTextEdit(&dlg);
    noteEdit->setPlainText(e.note);
    noteEdit->setToolTip("Коротка примітка про обов'язки або особливості");
    noteEdit->setFixedHeight(70);

    form->addRow("ПІБ:",                  nameEdit);
    form->addRow("Посада:",               posEdit);
    form->addRow("Зона відповідальності:", zoneEdit);
    form->addRow("Телефон:",              phoneEdit);
    form->addRow("E-mail:",               emailEdit);
    form->addRow("Примітка:",             noteEdit);

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

    e.name     = nameEdit->text().trimmed();
    e.position = posEdit->text().trimmed();
    e.zone     = zoneEdit->text().trimmed();
    e.phone    = phoneEdit->text().trimmed();
    e.email    = emailEdit->text().trimmed();
    e.note     = noteEdit->toPlainText().trimmed();
    return true;
}

void EmployeesPage::refreshEmployeeTable()
{
    m_table->setRowCount(0);
    for (int i = 0; i < m_employees.size(); ++i) {
        const auto &e = m_employees[i];
        m_table->insertRow(i);

        auto makeItem = [](const QString &txt) {
            auto *it = new QTableWidgetItem(txt);
            it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            return it;
        };
        m_table->setItem(i, 0, makeItem(e.name));
        m_table->setItem(i, 1, makeItem(e.position));
        m_table->setItem(i, 2, makeItem(e.zone));
        m_table->setItem(i, 3, makeItem(e.phone));
        m_table->setItem(i, 4, makeItem(e.email));
    }
}

void EmployeesPage::onAddEmployeeClicked()
{
    Employee e;
    if (execEmployeeDialog(e, this, "Додати працівника")) {
        if (e.name.isEmpty()) {
            QMessageBox::warning(this, "Помилка", "ПІБ є обов'язковим полем.");
            return;
        }
        m_employees.append(e);
        refreshEmployeeTable();
        m_table->selectRow(m_employees.size() - 1);
        showEmployee(m_employees.size() - 1);
    }
}

void EmployeesPage::onEditEmployeeClicked()
{
    const int row = m_table->currentRow();
    if (row < 0 || row >= m_employees.size()) {
        QMessageBox::information(this, "Редагування", "Оберіть рядок у таблиці.");
        return;
    }
    Employee e = m_employees[row];
    if (execEmployeeDialog(e, this, "Редагувати працівника")) {
        m_employees[row] = e;
        refreshEmployeeTable();
        m_table->selectRow(row);
        showEmployee(row);
    }
}