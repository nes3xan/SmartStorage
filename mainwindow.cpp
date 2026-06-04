#include "mainwindow.h"
#include "appstyle.h"

#include "pages/dashboardpage.h"
#include "pages/itemspage.h"
#include "pages/itemcardpage.h"
#include "pages/employeespage.h"
#include "pages/warehousemappage.h"
#include "models/product.h"

#include <QApplication>
#include <QScreen>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QLabel>
#include <QFrame>
#include <QDate>
#include <QPixmap>
#include <QPainter>
#include <QSvgRenderer>
#include <QByteArray>

static constexpr int PAGE_DASHBOARD  = 0;
static constexpr int PAGE_ITEMS      = 1;
static constexpr int PAGE_ITEMCARD   = 2;
static constexpr int PAGE_EMPLOYEES  = 3;
static constexpr int PAGE_WAREHOUSE  = 4;


static QIcon svgIcon(const QByteArray &svgTemplate,
                     const QString &normalColor  = "#7B8EA8",
                     const QString &activeColor  = "#FFFFFF",
                     int size = 18)
{
    auto render = [&](const QString &color) -> QPixmap {
        QByteArray svg = svgTemplate;
        svg.replace("COLOR", color.toUtf8());
        QSvgRenderer renderer(svg);
        QPixmap px(size, size);
        px.fill(Qt::transparent);
        QPainter p(&px);
        renderer.render(&p);
        return px;
    };

    QIcon icon;
    icon.addPixmap(render(normalColor), QIcon::Normal,   QIcon::Off);
    icon.addPixmap(render(activeColor), QIcon::Normal,   QIcon::On);
    icon.addPixmap(render("#E4E8F0"),   QIcon::Active,   QIcon::Off);
    icon.addPixmap(render(activeColor), QIcon::Active,   QIcon::On);
    return icon;
}


static const QByteArray SVG_HOME = R"svg(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none"
     stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <path d="M3 9.5L12 3l9 6.5V20a1 1 0 0 1-1 1H4a1 1 0 0 1-1-1V9.5z"/>
  <polyline points="9 21 9 12 15 12 15 21"/>
</svg>)svg";

static const QByteArray SVG_BOX = R"svg(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none"
     stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <path d="M21 16V8a2 2 0 0 0-1-1.73L13 2.27a2 2 0 0 0-2 0L4 6.27A2 2 0 0 0 3 8v8a2 2 0 0 0 1 1.73l7 4a2 2 0 0 0 2 0l7-4A2 2 0 0 0 21 16z"/>
  <polyline points="3.27 6.96 12 12.01 20.73 6.96"/>
  <line x1="12" y1="22.08" x2="12" y2="12"/>
</svg>)svg";

static const QByteArray SVG_PEOPLE = R"svg(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none"
     stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <path d="M17 21v-2a4 4 0 0 0-4-4H5a4 4 0 0 0-4 4v2"/>
  <circle cx="9" cy="7" r="4"/>
  <path d="M23 21v-2a4 4 0 0 0-3-3.87"/>
  <path d="M16 3.13a4 4 0 0 1 0 7.75"/>
</svg>)svg";

static const QByteArray SVG_MAP = R"svg(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none"
     stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <polygon points="1 6 1 22 8 18 16 22 23 18 23 2 16 6 8 2 1 6"/>
  <line x1="8" y1="2" x2="8" y2="18"/>
  <line x1="16" y1="6" x2="16" y2="22"/>
</svg>)svg";

static const QByteArray SVG_EXIT = R"svg(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none"
     stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <path d="M9 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h4"/>
  <polyline points="16 17 21 12 16 7"/>
  <line x1="21" y1="12" x2="9" y2="12"/>
</svg>)svg";

static const QByteArray SVG_USER = R"svg(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none"
     stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <path d="M20 21v-2a4 4 0 0 0-4-4H8a4 4 0 0 0-4 4v2"/>
  <circle cx="12" cy="7" r="4"/>
</svg>)svg";

static const QByteArray SVG_CALENDAR = R"svg(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none"
     stroke="COLOR" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
  <rect x="3" y="4" width="18" height="18" rx="2" ry="2"/>
  <line x1="16" y1="2" x2="16" y2="6"/>
  <line x1="8" y1="2" x2="8" y2="6"/>
  <line x1="3" y1="10" x2="21" y2="10"/>
</svg>)svg";


static QPixmap renderSvg(const QByteArray &svgData, const QString &color, int size)
{
    QByteArray svg = svgData;
    svg.replace("COLOR", color.toUtf8());
    QSvgRenderer renderer(svg);
    QPixmap px(size, size);
    px.fill(Qt::transparent);
    QPainter p(&px);
    renderer.render(&p);
    return px;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setMinimumSize(1024, 680);
    resize(1280, 800);

    if (QScreen *sc = QGuiApplication::primaryScreen())
        move(sc->availableGeometry().center() - QPoint(640, 400));

    buildUi();
    buildPages();
    switchPage(PAGE_DASHBOARD);
}

void MainWindow::buildUi()
{
    auto *central = new QWidget(this);
    central->setObjectName("centralWidget");
    setCentralWidget(central);

    auto *rootH = new QHBoxLayout(central);
    rootH->setContentsMargins(0, 0, 0, 0);
    rootH->setSpacing(0);

    m_sidePanel = new QWidget(central);
    m_sidePanel->setObjectName("sidePanel");
    m_sidePanel->setFixedWidth(AppStyle::Size::SIDEBAR_WIDTH);
    buildSidebar(m_sidePanel, nullptr);
    rootH->addWidget(m_sidePanel);

    auto *rightPanel = new QWidget(central);
    rightPanel->setObjectName("rightPanel");
    auto *rightV = new QVBoxLayout(rightPanel);
    rightV->setContentsMargins(0, 0, 0, 0);
    rightV->setSpacing(0);

    buildTopBar(rightPanel, rightV);

    m_stack = new QStackedWidget(rightPanel);
    rightV->addWidget(m_stack, 1);

    rootH->addWidget(rightPanel, 1);
}

void MainWindow::buildSidebar(QWidget *panel, QLayout *)
{
    auto *vbox = new QVBoxLayout(panel);
    vbox->setContentsMargins(12, 20, 12, 20);
    vbox->setSpacing(2);


    auto *logo = new QLabel(
        "<span style='color:#E4E8F0; font-weight:700; font-size:17px;"
        " letter-spacing:0.5px;'>SMART</span>"
        "<span style='color:#3B82F6; font-weight:700; font-size:17px;"
        " letter-spacing:0.5px;'>STORAGE</span>", panel);
    logo->setObjectName("logoLabel");
    logo->setTextFormat(Qt::RichText);
    logo->setContentsMargins(6, 0, 0, 12);
    vbox->addWidget(logo);


    m_navHome  = makeNavBtn(svgIcon(SVG_HOME),   "Головна",     "Перейти на головну панель");
    m_navItems = makeNavBtn(svgIcon(SVG_BOX),    "Товари",       "Перелік товарів складу");
    m_navEmp   = makeNavBtn(svgIcon(SVG_PEOPLE), "Працівники",   "Список співробітників");
    m_navMap   = makeNavBtn(svgIcon(SVG_MAP),    "Схема складу", "Схема секцій складу");

    for (auto *btn : {m_navHome, m_navItems, m_navEmp, m_navMap}) {
        btn->setAutoExclusive(true);
        vbox->addWidget(btn);
    }

    vbox->addStretch(1);

    auto *sep = new QFrame(panel);
    sep->setFrameShape(QFrame::HLine);
    sep->setObjectName("navSep");
    sep->setStyleSheet("color:#1E2F4E; background:#1E2F4E; max-height:1px; border:none;");
    vbox->addWidget(sep);
    vbox->addSpacing(4);

    auto *exitBtn = makeNavBtn(svgIcon(SVG_EXIT, "#7B8EA8", "#FF6B6B"), "Вихід", "Вийти з програми");
    exitBtn->setObjectName("exitBtn");
    exitBtn->setAutoExclusive(false);
    connect(exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
    vbox->addWidget(exitBtn);

    connect(m_navHome,  &QPushButton::clicked, this, [this]{ switchPage(PAGE_DASHBOARD); });
    connect(m_navItems, &QPushButton::clicked, this, [this]{ switchPage(PAGE_ITEMS); });
    connect(m_navEmp,   &QPushButton::clicked, this, [this]{ switchPage(PAGE_EMPLOYEES); });
    connect(m_navMap,   &QPushButton::clicked, this, [this]{ switchPage(PAGE_WAREHOUSE); });
}

void MainWindow::buildTopBar(QWidget *parent, QLayout *parentLayout)
{
    auto *bar = new QWidget(parent);
    bar->setObjectName("topBar");
    bar->setFixedHeight(AppStyle::Size::TOPBAR_HEIGHT);

    auto *h = new QHBoxLayout(bar);
    h->setContentsMargins(20, 0, 12, 0);
    h->setSpacing(10);


    m_search = new QLineEdit(bar);
    m_search->setObjectName("topSearchEdit");
    m_search->setPlaceholderText("Пошук...");
    m_search->setToolTip("Глобальний пошук по системі");
    m_search->setFixedWidth(260);
    h->addWidget(m_search);
    h->addStretch(1);


    auto *calIco = new QLabel(bar);
    calIco->setPixmap(renderSvg(SVG_CALENDAR, "#6B7280", 16));
    h->addWidget(calIco);

    auto *dateLbl = new QLabel(QDate::currentDate().toString("dd.MM.yyyy"), bar);
    dateLbl->setObjectName("dateLabel");
    dateLbl->setStyleSheet("color:#6B7280; font-size:13px; font-weight:500; background:transparent;");
    h->addWidget(dateLbl);

    auto *dateDivider = new QFrame(bar);
    dateDivider->setFrameShape(QFrame::VLine);
    dateDivider->setStyleSheet("color:#2A3D60; background:#2A3D60; max-width:1px; min-height:24px;");
    h->addWidget(dateDivider);


    auto *userIco = new QLabel(bar);
    userIco->setPixmap(renderSvg(SVG_USER, "#D1D5DB", 16));
    h->addWidget(userIco);

    auto *adminLbl = new QLabel("Адміністратор", bar);
    adminLbl->setObjectName("adminLabel");
    adminLbl->setToolTip("Поточний користувач системи");
    h->addWidget(adminLbl);

    auto *divider = new QFrame(bar);
    divider->setFrameShape(QFrame::VLine);
    divider->setStyleSheet("color:#2A3D60; background:#2A3D60; max-width:1px; min-height:24px;");
    h->addWidget(divider);


    static const QByteArray SVG_MIN = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><line x1="5" y1="12" x2="19" y2="12" stroke="COLOR" stroke-width="2" stroke-linecap="round"/></svg>)";
    static const QByteArray SVG_MAX = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><rect x="4" y="4" width="16" height="16" rx="2" stroke="COLOR" stroke-width="2" fill="none"/></svg>)";
    static const QByteArray SVG_X   = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24"><line x1="5" y1="5" x2="19" y2="19" stroke="COLOR" stroke-width="2" stroke-linecap="round"/><line x1="19" y1="5" x2="5" y2="19" stroke="COLOR" stroke-width="2" stroke-linecap="round"/></svg>)";

    auto makeWinBtn = [&](const QByteArray &svg, const QString &id,
                          const QString &tip, const QString &hoverColor = "#D1D5DB") -> QPushButton* {
        auto *btn = new QPushButton(bar);
        btn->setObjectName(id);
        btn->setToolTip(tip);
        btn->setFixedSize(AppStyle::Size::WIN_BTN_SIZE, AppStyle::Size::WIN_BTN_SIZE);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setIcon(svgIcon(svg, "#7B8EA8", hoverColor, 14));
        btn->setIconSize(QSize(14, 14));
        return btn;
    };

    auto *btnMin   = makeWinBtn(SVG_MIN, "winBtn",      "Згорнути");
    auto *btnMax   = makeWinBtn(SVG_MAX, "winBtn",      "Розгорнути / відновити");
    auto *btnClose = makeWinBtn(SVG_X,   "winCloseBtn", "Закрити", "#FF6B6B");

    connect(btnMin,   &QPushButton::clicked, this, &MainWindow::showMinimized);
    connect(btnMax,   &QPushButton::clicked, this, [this]{
        isMaximized() ? showNormal() : showMaximized();
    });
    connect(btnClose, &QPushButton::clicked, qApp, &QApplication::quit);

    h->addWidget(btnMin);
    h->addWidget(btnMax);
    h->addWidget(btnClose);

    static_cast<QVBoxLayout*>(parentLayout)->addWidget(bar);
}

void MainWindow::buildPages()
{
    m_dashboard    = new DashboardPage(this);
    m_items        = new ItemsPage(this);
    m_itemCard     = new ItemCardPage(this);
    m_employees    = new EmployeesPage(this);
    m_warehouseMap = new WarehouseMapPage(this);

    m_stack->addWidget(m_dashboard);
    m_stack->addWidget(m_items);
    m_stack->addWidget(m_itemCard);
    m_stack->addWidget(m_employees);
    m_stack->addWidget(m_warehouseMap);

    connect(m_items, &ItemsPage::productSelected,
            this, &MainWindow::openItemCard);
    connect(m_itemCard, &ItemCardPage::backClicked,
            this, [this]() { switchPage(PAGE_ITEMS); });
}

void MainWindow::switchPage(int index)
{
    m_stack->setCurrentIndex(index);

    const int navIndex = (index == PAGE_ITEMCARD) ? PAGE_ITEMS : index;
    m_navHome->setChecked(navIndex  == PAGE_DASHBOARD);
    m_navItems->setChecked(navIndex == PAGE_ITEMS);
    m_navEmp->setChecked(navIndex   == PAGE_EMPLOYEES);
    m_navMap->setChecked(navIndex   == PAGE_WAREHOUSE);

    static const QString hints[] = {
        "Пошук...", "Пошук товарів...", "Монітор N198k",
        "Пошук...", "Пошук...", "Пошук..."
    };
    if (index < 6) m_search->setPlaceholderText(hints[index]);
}

void MainWindow::openItemCard(const Product &p)
{
    m_itemCard->loadProduct(p);
    switchPage(PAGE_ITEMCARD);
}

QPushButton *MainWindow::makeNavBtn(const QIcon &icon,
                                    const QString &text,
                                    const QString &tip)
{
    auto *btn = new QPushButton(text, m_sidePanel);
    btn->setObjectName("navBtn");
    btn->setCheckable(true);
    btn->setToolTip(tip);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFixedHeight(AppStyle::Size::NAV_BTN_HEIGHT);
    btn->setIcon(icon);
    btn->setIconSize(QSize(18, 18));

    btn->setStyleSheet("text-align: left; padding-left: 12px;");
    return btn;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPos = e->globalPosition().toPoint() - frameGeometry().topLeft();
    }
    QMainWindow::mousePressEvent(e);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (m_dragging && (e->buttons() & Qt::LeftButton))
        move(e->globalPosition().toPoint() - m_dragPos);
    QMainWindow::mouseMoveEvent(e);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragging = false;
    QMainWindow::mouseReleaseEvent(e);
}