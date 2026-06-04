#include "splashscreen.h"
#include "appstyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QDate>

SplashScreen::SplashScreen(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(840, 560);


    if (QScreen *sc = QGuiApplication::primaryScreen())
        move(sc->availableGeometry().center() - rect().center());

    buildUi();
}

void SplashScreen::buildUi()
{

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(140, 90, 140, 90);
    root->setSpacing(0);


    root->addSpacing(64);

    auto *titleLbl = new QLabel(
        "<span style='color:#E4E8F0; font-weight:700; font-size:26px;"
        " letter-spacing:1px;'>SMART</span>"
        "<span style='color:#3B82F6; font-weight:700; font-size:26px;"
        " letter-spacing:1px;'>STORAGE</span>", this);
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setTextFormat(Qt::RichText);
    root->addWidget(titleLbl);
    root->addSpacing(8);


    auto *subLbl = new QLabel("Warehouse Management System", this);
    subLbl->setAlignment(Qt::AlignCenter);
    subLbl->setStyleSheet("color:#7B8EA8; font-size:14px; background:transparent;");
    root->addWidget(subLbl);
    root->addSpacing(28);


    auto *loadLbl = new QLabel("Завантаження...", this);
    loadLbl->setAlignment(Qt::AlignCenter);
    loadLbl->setStyleSheet("color:#5A7090; font-size:12px; background:transparent;");
    root->addWidget(loadLbl);
    root->addSpacing(8);


    auto *progRow = new QHBoxLayout();
    progRow->setSpacing(10);

    m_bar = new QProgressBar(this);
    m_bar->setObjectName("splashProgress");
    m_bar->setRange(0, 100);
    m_bar->setValue(0);
    m_bar->setTextVisible(false);
    m_bar->setFixedHeight(8);
    progRow->addWidget(m_bar, 1);

    m_percent = new QLabel("0%", this);
    m_percent->setFixedWidth(36);
    m_percent->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_percent->setStyleSheet("color:#7B8EA8; font-size:12px; background:transparent;");
    progRow->addWidget(m_percent);

    root->addLayout(progRow);
    root->addStretch(1);


    int year = QDate::currentDate().year();
    QString footer = QString(
                         "<span style='color:#3A4F6E;'>Розробник: Тітов І. В.</span>"
                         "&nbsp;&nbsp;&nbsp;&nbsp;"
                         "<span style='color:#3A4F6E;'>Версія: 1.0.0</span>"
                         "&nbsp;&nbsp;&nbsp;&nbsp;"
                         "<span style='color:#3A4F6E;'>%1</span>").arg(year);

    auto *footerLbl = new QLabel(footer, this);
    footerLbl->setTextFormat(Qt::RichText);
    footerLbl->setAlignment(Qt::AlignCenter);
    footerLbl->setStyleSheet("background:transparent; font-size:11px;");
    root->addWidget(footerLbl);
}

void SplashScreen::startProgress()
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SplashScreen::onTick);
    m_timer->start(30);
}

void SplashScreen::onTick()
{
    ++m_value;
    m_bar->setValue(m_value);
    m_percent->setText(QString::number(m_value) + "%");

    if (m_value >= 100) {
        m_timer->stop();
        emit finished();
    }
}

void SplashScreen::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);


    p.fillRect(rect(), QColor("#0C1120"));


    const QRect card(100, 60, 640, 440);
    QPainterPath path;
    path.addRoundedRect(card, 16, 16);
    p.fillPath(path, QColor("#131B30"));


    p.setPen(QPen(QColor("#1E2F4E"), 1));
    p.drawPath(path);


    const int iconCX = card.center().x();
    const int iconCY = card.top() + 58;
    drawWarehouseIcon(p, iconCX, iconCY);
}

void SplashScreen::drawWarehouseIcon(QPainter &p, int cx, int cy)
{

    QPen pen(QColor("#3B82F6"), 2.2);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);


    p.drawRect(cx - 46, cy - 24, 44, 40);
    p.drawLine(cx - 46, cy - 4,  cx - 46 + 44, cy - 4);
    p.drawLine(cx - 24, cy - 24, cx - 24,       cy - 4);


    p.drawRect(cx + 4, cy - 12, 32, 28);


    p.drawRect(cx + 36, cy - 2, 14, 18);
}