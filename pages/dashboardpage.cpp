#include "dashboardpage.h"
#include "appstyle.h"
#include "models/activitylog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>



class DonutChartWidget : public QWidget {
public:
    explicit DonutChartWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(148, 148);
        setAttribute(Qt::WA_TranslucentBackground);
    }
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(), Qt::transparent);

        const QRectF arc(20, 20, 108, 108);
        QPen pen;
        pen.setWidth(22);
        pen.setCapStyle(Qt::FlatCap);


        const double k = 360.0 * 16.0 / 1363.0;

        struct Seg { QColor color; int count; };
        const Seg segs[] = {
            { QColor("#3B82F6"), 670 },
            { QColor("#F59E0B"),  32 },
            { QColor("#6B7280"), 321 },
            { QColor("#92400E"), 340 },
        };

        int startAngle = 90 * 16;
        for (const auto &seg : segs) {
            int span = qRound(seg.count * k);
            pen.setColor(seg.color);
            p.setPen(pen);
            p.drawArc(arc, startAngle, span);
            startAngle += span;
        }
    }
};



static QFrame* createStatCard(const QString &title, const QString &value,
                              const QString &unit,  const QString &tooltip,
                              bool warn = false)
{
    auto *card = new QFrame();
    card->setObjectName("statCard");
    card->setToolTip(tooltip);
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    card->setMinimumHeight(130);

    auto *v = new QVBoxLayout(card);
    v->setContentsMargins(20, 20, 20, 20);
    v->setSpacing(6);

    const char *titleColor = warn ? AppStyle::Color::ACCENT_ORANGE
                                  : AppStyle::Color::TEXT_DIM;
    const char *valColor   = warn ? AppStyle::Color::ACCENT_ORANGE
                                : AppStyle::Color::TEXT_PRIMARY;
    const char *unitColor  = warn ? AppStyle::Color::ACCENT_ORANGE
                                 : AppStyle::Color::TEXT_DIM;

    auto *lblTitle = new QLabel(title, card);
    lblTitle->setStyleSheet(
        QString("background:transparent; color:%1; font-size:13px; font-weight:500;")
            .arg(titleColor));

    auto *lblValue = new QLabel(value, card);
    lblValue->setStyleSheet(
        QString("background:transparent; color:%1; font-size:28px; font-weight:700;")
            .arg(valColor));

    auto *lblUnit = new QLabel(unit, card);
    lblUnit->setStyleSheet(
        QString("background:transparent; color:%1; font-size:12px;")
            .arg(unitColor));

    v->addWidget(lblTitle);
    v->addWidget(lblValue);
    v->addWidget(lblUnit);
    v->addStretch();

    return card;
}



DashboardPage::DashboardPage(QWidget *parent) : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 24);
    mainLayout->setSpacing(20);


    auto *pageTitle = new QLabel("Огляд складу", this);
    pageTitle->setStyleSheet(
        QString("background:transparent; color:%1; font-size:%2px; font-weight:700;")
            .arg(AppStyle::Color::TEXT_PRIMARY)
            .arg(AppStyle::Font::LG));
    mainLayout->addWidget(pageTitle);


    auto *cardsRow = new QHBoxLayout();
    cardsRow->setSpacing(20);
    cardsRow->addWidget(createStatCard("Всього товарів",    "1450", "позицій",
                                       "Загальна кількість позицій на складі"));
    cardsRow->addWidget(createStatCard("Низький запас",     "32",   "позиції",
                                       "Товари з критичним рівнем запасу", /*warn=*/true));
    cardsRow->addWidget(createStatCard("Працівників",       "20",   "осіб",
                                       "Кількість активних співробітників"));
    cardsRow->addWidget(createStatCard("Операцій сьогодні", "14",   "дій",
                                       "Кількість складських операцій за сьогодні"));
    mainLayout->addLayout(cardsRow);


    auto *bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(20);


    auto *leftPanel = new QFrame(this);
    leftPanel->setObjectName("panelCard");
    auto *leftV = new QVBoxLayout(leftPanel);
    leftV->setContentsMargins(24, 20, 24, 16);
    leftV->setSpacing(0);

    auto *actTitle = new QLabel("Останні дії", leftPanel);
    actTitle->setObjectName("panelTitle");
    leftV->addWidget(actTitle);
    leftV->addSpacing(16);


    const auto logs = ActivityLogEntry::mockData();
    for (const auto &entry : logs) {
        auto *row = new QWidget(leftPanel);
        row->setStyleSheet("background:transparent;");
        auto *h = new QHBoxLayout(row);
        h->setContentsMargins(0, 0, 0, 0);
        h->setSpacing(12);

        auto *textLbl = new QLabel(entry.displayText(), row);
        textLbl->setStyleSheet(
            QString("color:%1; font-size:13px; background:transparent;")
                .arg(AppStyle::Color::TEXT_PRIMARY));

        auto *timeLbl = new QLabel(entry.time, row);
        timeLbl->setStyleSheet(
            QString("color:%1; font-size:13px; background:transparent;")
                .arg(AppStyle::Color::TEXT_DIM));
        timeLbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        timeLbl->setFixedWidth(40);

        h->addWidget(textLbl, 1);
        h->addWidget(timeLbl);

        leftV->addWidget(row);


        auto *div = new QFrame(leftPanel);
        div->setFrameShape(QFrame::HLine);
        div->setStyleSheet("background:#1A2840; max-height:1px; border:none;");
        leftV->addWidget(div);
    }

    leftV->addSpacing(8);


    auto *allOpsBtn = new QPushButton("Переглянути список усіх операцій", leftPanel);
    allOpsBtn->setObjectName("btnLink");
    allOpsBtn->setCursor(Qt::PointingHandCursor);
    allOpsBtn->setToolTip("Відкрити повний журнал операцій");
    leftV->addWidget(allOpsBtn, 0, Qt::AlignLeft);

    bottomRow->addWidget(leftPanel, 2);


    auto *rightPanel = new QFrame(this);
    rightPanel->setObjectName("panelCard");
    auto *rightV = new QVBoxLayout(rightPanel);
    rightV->setContentsMargins(24, 20, 24, 20);
    rightV->setSpacing(20);

    auto *chartTitle = new QLabel("Товари за статусом", rightPanel);
    chartTitle->setObjectName("panelTitle");
    rightV->addWidget(chartTitle);


    auto *chartRow = new QHBoxLayout();
    chartRow->setSpacing(20);
    chartRow->setAlignment(Qt::AlignVCenter);

    chartRow->addWidget(new DonutChartWidget(rightPanel), 0, Qt::AlignVCenter);

    auto *legendV = new QVBoxLayout();
    legendV->setSpacing(14);
    legendV->setAlignment(Qt::AlignVCenter);

    auto addLegend = [&](const QString &hex, const QString &label, const QString &count) {
        auto *row = new QWidget(rightPanel);
        row->setStyleSheet("background:transparent;");
        auto *h = new QHBoxLayout(row);
        h->setContentsMargins(0, 0, 0, 0);
        h->setSpacing(10);

        auto *dot = new QLabel(row);
        dot->setFixedSize(12, 12);
        dot->setStyleSheet(
            QString("background:%1; border-radius:2px;").arg(hex));

        auto *lbl = new QLabel(label, row);
        lbl->setStyleSheet(
            QString("color:%1; font-size:13px; background:transparent;")
                .arg(AppStyle::Color::TEXT_PRIMARY));

        auto *cnt = new QLabel(count, row);
        cnt->setStyleSheet(
            QString("color:%1; font-size:13px; font-weight:700; background:transparent;")
                .arg(AppStyle::Color::TEXT_DIM));
        cnt->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        h->addWidget(dot);
        h->addWidget(lbl, 1);
        h->addWidget(cnt);

        legendV->addWidget(row);
    };

    addLegend("#3B82F6", "У наявності",   "670");
    addLegend("#F59E0B", "Низький запас",  "32");
    addLegend("#6B7280", "Відсутні",       "321");
    addLegend("#92400E", "Доставляється",  "340");

    chartRow->addLayout(legendV, 1);
    rightV->addLayout(chartRow);
    rightV->addStretch();

    bottomRow->addWidget(rightPanel, 1);

    mainLayout->addLayout(bottomRow, 1);
}