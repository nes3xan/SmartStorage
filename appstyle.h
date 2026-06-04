#pragma once

// =============================================================================
//  appstyle.h — Централізоване сховище стилів SMARTSTORAGE
//
//  Містить:
//    • Кольорові константи (простір імен AppStyle::Color)
//    • Розмірні константи (AppStyle::Size)
//    • Типографіку        (AppStyle::Font)
//    • Глобальний QSS     (AppStyle::globalStyleSheet())
//    • Утилітарні функції (AppStyle::setupApplication)
//
//  Використання:
//    #include "appstyle.h"
//    ...
//    QApplication::setStyle(QStyleFactory::create("Fusion"));
//    qApp->setStyleSheet(AppStyle::globalStyleSheet());
//    AppStyle::setupFont(app);
// =============================================================================

#include <QString>
#include <QApplication>
#include <QFont>
#include <QStyleFactory>

namespace AppStyle
{

// ═════════════════════════════════════════════════════════════════════════════
//  КОЛЬОРИ
//  Джерело: pixel-аналіз макетів Figma (скріншоти 1–6)
// ═════════════════════════════════════════════════════════════════════════════
namespace Color
{
    // ── Ієрархія фонів (від найтемнішого до найсвітлішого) ──────────────────
    // Зовнішній фон вікна / контентна зона між панелями
    inline constexpr const char* BG_APP         = "#0F1523";
    // Ліва навігаційна панель та верхня смуга
    inline constexpr const char* BG_SIDEBAR     = "#131B30";
    // Картки, панелі («Останні дії», статкартки, таблиці)
    inline constexpr const char* BG_PANEL       = "#182032";
    // Поля вводу, комбобокси
    inline constexpr const char* BG_INPUT       = "#1C2840";
    // Чергові рядки таблиці (alternating rows)
    inline constexpr const char* BG_ROW_ALT     = "#1A2B44";
    // Комірки схеми складу — невибрані
    inline constexpr const char* BG_CELL        = "#1E3259";
    // Заставка (splash) — контейнер
    inline constexpr const char* BG_SPLASH_CARD = "#131B30";

    // ── Акценти ──────────────────────────────────────────────────────────────
    // Синій — активні елементи, первинні кнопки, вибрана nav-кнопка
    inline constexpr const char* ACCENT_BLUE    = "#2563EB";
    // Синій hover / натиснення
    inline constexpr const char* ACCENT_HOVER   = "#1D4ED8";
    // Блакитний — посилання, другорядні акценти
    inline constexpr const char* ACCENT_LINK    = "#3B82F6";
    // Помаранчевий — попередження («Низький запас»)
    inline constexpr const char* ACCENT_ORANGE  = "#F59E0B";

    // ── Статуси товарів ───────────────────────────────────────────────────────
    inline constexpr const char* STATUS_OK      = "#22C55E";   // В наявності
    inline constexpr const char* STATUS_WARN    = "#F59E0B";   // Низький запас
    inline constexpr const char* STATUS_ERR     = "#EF4444";   // Відсутній

    // ── Текст ─────────────────────────────────────────────────────────────────
    // Основний текст, значення
    inline constexpr const char* TEXT_PRIMARY   = "#E4E8F0";
    // Другорядний текст, підписи, заголовки таблиць
    inline constexpr const char* TEXT_DIM       = "#7B8EA8";
    // Чисто-білий (великі числа, активні кнопки)
    inline constexpr const char* TEXT_WHITE     = "#FFFFFF";

    // ── Межі ──────────────────────────────────────────────────────────────────
    // Субтильна межа карток і панелей
    inline constexpr const char* BORDER_PANEL   = "#1E2F4E";
    // Межа полів введення
    inline constexpr const char* BORDER_INPUT   = "#2A3D60";
    // Межа в стані фокусу / hover
    inline constexpr const char* BORDER_FOCUS   = "#2563EB";

    // ── Схема складу ──────────────────────────────────────────────────────────
    // Комірка — невибрана
    inline constexpr const char* CELL_DEFAULT   = "#1E3259";
    // Комірка — hover
    inline constexpr const char* CELL_HOVER     = "#263D72";
    // Комірка — вибрана (виділена синім)
    inline constexpr const char* CELL_SELECTED  = "#2563EB";

} // namespace Color


// ═════════════════════════════════════════════════════════════════════════════
//  РОЗМІРИ
// ═════════════════════════════════════════════════════════════════════════════
namespace Size
{
    inline constexpr int SIDEBAR_WIDTH      = 200;  // px — ширина лівої панелі
    inline constexpr int TOPBAR_HEIGHT      =  56;  // px — висота верхньої смуги
    inline constexpr int NAV_BTN_HEIGHT     =  42;  // px — висота nav-кнопки
    inline constexpr int NAV_BTN_RADIUS     =   8;  // px — скруглення nav-кнопки
    inline constexpr int CARD_RADIUS        =  10;  // px — скруглення карток
    inline constexpr int INPUT_RADIUS       =   6;  // px — скруглення полів вводу
    inline constexpr int BTN_RADIUS         =   6;  // px — скруглення кнопок
    inline constexpr int WIN_BTN_SIZE       =  28;  // px — розмір кнопок вікна
    inline constexpr int STAT_CARD_MIN_W    = 170;  // px — мін. ширина стат-картки
    inline constexpr int TABLE_ROW_HEIGHT   =  40;  // px — висота рядка таблиці
    inline constexpr int WAREHOUSE_CELL_MIN = 110;  // px — мін. розмір комірки складу
    inline constexpr int SPLASH_W           = 640;  // px — ширина splash-вікна
    inline constexpr int SPLASH_H           = 420;  // px — висота splash-вікна

} // namespace Size


// ═════════════════════════════════════════════════════════════════════════════
//  ТИПОГРАФІКА
// ═════════════════════════════════════════════════════════════════════════════
namespace Font
{
    inline constexpr int XS     = 11;   // Дуже маленький (версія, підписи)
    inline constexpr int SM     = 12;   // Маленький (заголовки таблиць, одиниці)
    inline constexpr int BASE   = 13;   // Базовий (основний текст інтерфейсу)
    inline constexpr int MD     = 15;   // Середній (заголовки секцій)
    inline constexpr int LG     = 18;   // Великий (назва сторінки)
    inline constexpr int STAT   = 28;   // Числа у стат-картках
    inline constexpr int SPLASH_TITLE = 26; // «SMARTSTORAGE» у заставці
    inline constexpr int CELL   = 16;   // Мітки комірок складу

} // namespace Font


// ═════════════════════════════════════════════════════════════════════════════
//  ГЛОБАЛЬНА QSS-ТАБЛИЦЯ СТИЛІВ
//  Охоплює: базовий QWidget, sidebar, topbar, картки, таблиці, кнопки,
//           комбобокс, скролбар, прогресбар, комірки складу.
// ═════════════════════════════════════════════════════════════════════════════
inline QString globalStyleSheet()
{
    return QStringLiteral(R"(

/* ──────────────────────────────────────────────────────────────────────────
    база
   ────────────────────────────────────────────────────────────────────────── */
QWidget {
    background-color: #0F1523;
    color: #E4E8F0;
    font-family: "Segoe UI", "Ubuntu", "Helvetica Neue", Arial, sans-serif;
    font-size: 13px;
}

QLabel {
    background: transparent;
    color: #E4E8F0;
}

/* ──────────────────────────────────────────────────────────────────────────
   сайдбар
   ────────────────────────────────────────────────────────────────────────── */
#sidePanel {
    background-color: #131B30;
    border-right: 1px solid #1E2F4E;
}

/* Лого */
#logoLabel {
    background: transparent;
    font-size: 18px;
    font-weight: 700;
    letter-spacing: 0.5px;
    padding-left: 4px;
}


#navSectionLabel {
    color: #3A4F6E;
    font-size: 10px;
    font-weight: 600;
    letter-spacing: 1.5px;
    background: transparent;
    padding-left: 14px;
    padding-top: 8px;
    padding-bottom: 2px;
}

/* кнопки навігації */
#navBtn {
    background-color: transparent;
    color: #7B8EA8;
    border: none;
    border-radius: 8px;
    padding: 0px 14px;
    text-align: left;
    font-size: 13px;
    min-height: 42px;
    max-height: 42px;
}
#navBtn:hover {
    background-color: #1C2840;
    color: #E4E8F0;
}
#navBtn:checked {
    background-color: #2563EB;
    color: #FFFFFF;
    font-weight: 600;
}
#navBtn:checked:hover {
    background-color: #1D4ED8;
}


#exitBtn {
    background-color: transparent;
    color: #7B8EA8;
    border: none;
    border-radius: 8px;
    padding: 0px 14px;
    text-align: left;
    font-size: 13px;
    min-height: 40px;
    max-height: 40px;
}
#exitBtn:hover {
    background-color: rgba(239, 68, 68, 0.15);
    color: #EF4444;
}


#topBar {
    background-color: #131B30;
    border-bottom: 1px solid #1E2F4E;
    min-height: 56px;
    max-height: 56px;
}

/* пошукове поле */
#topSearchEdit {
    background-color: #1C2840;
    color: #E4E8F0;
    border: 1px solid #2A3D60;
    border-radius: 6px;
    padding: 6px 14px;
    font-size: 13px;
    min-width: 220px;
    max-width: 320px;
    min-height: 34px;
    max-height: 34px;
}
#topSearchEdit:focus {
    border-color: #2563EB;
}


#adminLabel {
    color: #7B8EA8;
    font-size: 13px;
    background: transparent;
}


#winBtn {
    background-color: transparent;
    color: #7B8EA8;
    border: none;
    border-radius: 4px;
    font-size: 14px;
    min-width: 28px;
    max-width: 28px;
    min-height: 28px;
    max-height: 28px;
}
#winBtn:hover {
    background-color: #253358;
    color: #E4E8F0;
}


#winCloseBtn {
    background-color: transparent;
    color: #7B8EA8;
    border: none;
    border-radius: 4px;
    font-size: 14px;
    min-width: 28px;
    max-width: 28px;
    min-height: 28px;
    max-height: 28px;
}
#winCloseBtn:hover {
    background-color: #EF4444;
    color: #FFFFFF;
}


#panelCard {
    background-color: #182032;
    border: 1px solid #1E2F4E;
    border-radius: 10px;
}

#panelTitle {
    color: #E4E8F0;
    font-size: 15px;
    font-weight: 600;
    background: transparent;
}


#statCard {
    background-color: #182032;
    border: 1px solid #1E2F4E;
    border-radius: 10px;
    min-width: 170px;
}


#statCardTitle { color: #7B8EA8; font-size: 13px; background: transparent; }
#statCardValue { color: #E4E8F0; font-size: 28px; font-weight: 700; background: transparent; }
#statCardUnit  { color: #7B8EA8; font-size: 12px; background: transparent; }


#statCardTitleWarn { color: #F59E0B; font-size: 13px; background: transparent; }
#statCardValueWarn { color: #F59E0B; font-size: 28px; font-weight: 700; background: transparent; }
#statCardUnitWarn  { color: #F59E0B; font-size: 12px; background: transparent; }


QTableWidget {
    background-color: transparent;
    color: #E4E8F0;
    gridline-color: transparent;
    border: none;
    outline: none;
    font-size: 13px;
    alternate-background-color: #1A2B44;
    selection-background-color: #1E3A66;
    selection-color: #FFFFFF;
}

QTableWidget::item {
    padding: 0px 12px;
    border: none;
    border-bottom: 1px solid #1A2840;
}

QTableWidget::item:selected {
    background-color: #1E3A66;
    color: #FFFFFF;
}

QTableWidget::item:hover {
    background-color: #1C2E55;
}


QHeaderView {
    background-color: transparent;
}
QHeaderView::section {
    background-color: #182032;
    color: #7B8EA8;
    font-size: 12px;
    font-weight: 600;
    text-transform: uppercase;
    letter-spacing: 0.8px;
    border: none;
    border-bottom: 1px solid #1E2F4E;
    padding: 10px 12px;
}
QHeaderView::section:first {
    border-top-left-radius: 0;
}
QHeaderView::section:last {
    border-top-right-radius: 0;
}


QScrollBar:vertical {
    background: transparent;
    width: 6px;
    margin: 4px 0;
}
QScrollBar::handle:vertical {
    background: #2A3D60;
    border-radius: 3px;
    min-height: 24px;
}
QScrollBar::handle:vertical:hover  { background: #3A5A90; }
QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical      { height: 0; background: none; }
QScrollBar::add-page:vertical,
QScrollBar::sub-page:vertical      { background: none; }

QScrollBar:horizontal {
    background: transparent;
    height: 6px;
    margin: 0 4px;
}
QScrollBar::handle:horizontal {
    background: #2A3D60;
    border-radius: 3px;
    min-width: 24px;
}
QScrollBar::handle:horizontal:hover { background: #3A5A90; }
QScrollBar::add-line:horizontal,
QScrollBar::sub-line:horizontal     { width: 0; background: none; }
QScrollBar::add-page:horizontal,
QScrollBar::sub-page:horizontal     { background: none; }




#btnPrimary {
    background-color: #2563EB;
    color: #FFFFFF;
    border: none;
    border-radius: 6px;
    padding: 0px 20px;
    font-size: 13px;
    font-weight: 600;
    min-height: 36px;
}
#btnPrimary:hover   { background-color: #1D4ED8; }
#btnPrimary:pressed { background-color: #1E40AF; }
#btnPrimary:disabled {
    background-color: #1E2F4E;
    color: #3A4F6E;
}


#btnSecondary {
    background-color: transparent;
    color: #E4E8F0;
    border: 1px solid #2A3D60;
    border-radius: 6px;
    padding: 0px 20px;
    font-size: 13px;
    min-height: 36px;
}
#btnSecondary:hover   { background-color: #1C2840; border-color: #3B82F6; color: #FFFFFF; }
#btnSecondary:pressed { background-color: #1A2B4A; }
#btnSecondary:disabled { color: #3A4F6E; border-color: #1E2F4E; }


#btnLink {
    background-color: transparent;
    color: #3B82F6;
    border: none;
    font-size: 13px;
    padding: 4px 0px;
    text-align: left;
}
#btnLink:hover { color: #60A5FA; }


#btnOutlinePlus {
    background-color: transparent;
    color: #E4E8F0;
    border: 1px solid #2A3D60;
    border-radius: 6px;
    padding: 0px 16px;
    font-size: 13px;
    min-height: 36px;
}
#btnOutlinePlus:hover   { background-color: #1C2840; border-color: #3B82F6; }
#btnOutlinePlus:pressed { background-color: #1A2B4A; }


#btnPage {
    background-color: transparent;
    color: #7B8EA8;
    border: 1px solid #1E2F4E;
    border-radius: 6px;
    font-size: 13px;
    min-width: 36px;
    max-width: 36px;
    min-height: 36px;
    max-height: 36px;
}
#btnPage:hover { background-color: #1C2840; color: #E4E8F0; }


#btnPageActive {
    background-color: #2563EB;
    color: #FFFFFF;
    border: none;
    border-radius: 6px;
    font-size: 13px;
    font-weight: 600;
    min-width: 36px;
    max-width: 36px;
    min-height: 36px;
    max-height: 36px;
}


QComboBox {
    background-color: #1C2840;
    color: #E4E8F0;
    border: 1px solid #2A3D60;
    border-radius: 6px;
    padding: 6px 14px;
    font-size: 13px;
    min-height: 36px;
    min-width: 140px;
}
QComboBox:focus  { border-color: #2563EB; }
QComboBox:hover  { border-color: #3B82F6; }

QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: right center;
    width: 30px;
    border: none;
}

QComboBox::down-arrow {
    image: none;
    width: 0;
}

QComboBox QAbstractItemView {
    background-color: #1C2840;
    color: #E4E8F0;
    border: 1px solid #2A3D60;
    border-radius: 4px;
    selection-background-color: #2563EB;
    selection-color: #FFFFFF;
    outline: none;
    padding: 4px;
}
QComboBox QAbstractItemView::item {
    min-height: 32px;
    padding-left: 10px;
}


#splashProgress {
    background-color: #1C2840;
    border: none;
    border-radius: 4px;
    text-align: right;
    color: transparent;
    min-height: 8px;
    max-height: 8px;
}
#splashProgress::chunk {
    background-color: #3B82F6;
    border-radius: 4px;
}


#warehouseCell {
    background-color: #1E3259;
    color: #E4E8F0;
    border: none;
    border-radius: 10px;
    font-size: 16px;
    font-weight: 700;
    min-width: 110px;
    min-height: 110px;
}
#warehouseCell:hover {
    background-color: #263D72;
}
#warehouseCell:checked {
    background-color: #2563EB;
    color: #FFFFFF;
}


#employeeCard {
    background-color: #182032;
    border: 1px solid #1E2F4E;
    border-radius: 10px;
}


#avatarFrame {
    background-color: transparent;
    min-width: 80px;
    max-width: 80px;
    min-height: 80px;
    max-height: 80px;
}

#employeeNameLabel {
    color: #E4E8F0;
    font-size: 16px;
    font-weight: 700;
    background: transparent;
}

#employeePositionLabel {
    color: #7B8EA8;
    font-size: 13px;
    background: transparent;
}

#employeeDetailKey {
    color: #7B8EA8;
    font-size: 13px;
    background: transparent;
    min-width: 80px;
}

#employeeDetailVal {
    color: #E4E8F0;
    font-size: 13px;
    background: transparent;
}


#employeeEmailLink {
    color: #3B82F6;
    font-size: 13px;
    background: transparent;
}


#breadcrumbContainer {
    background: transparent;
}

#breadcrumbBtn {
    background-color: transparent;
    color: #7B8EA8;
    border: none;
    font-size: 13px;
    padding: 4px 6px;
}
#breadcrumbBtn:hover { color: #E4E8F0; }

#breadcrumbSep {
    color: #3A4F6E;
    background: transparent;
    font-size: 13px;
}

#breadcrumbCurrent {
    color: #E4E8F0;
    font-size: 13px;
    font-weight: 600;
    background: transparent;
}


#itemDetailKey {
    color: #7B8EA8;
    font-size: 13px;
    background: transparent;
    min-width: 160px;
}

#itemDetailVal {
    color: #E4E8F0;
    font-size: 13px;
    background: transparent;
}


#imagePlaceholder {
    background-color: #182032;
    border: 1px solid #1E2F4E;
    border-radius: 8px;
    color: #3A4F6E;
    font-size: 48px;
    min-width: 200px;
    min-height: 180px;
    max-width: 250px;
}


QFrame[frameShape="4"] {
    color: #1E2F4E;
    background-color: #1E2F4E;
    max-height: 1px;
    border: none;
}


QStackedWidget {
    background-color: #0F1523;
}


QToolTip {
    background-color: #253358;
    color: #E4E8F0;
    border: 1px solid #2A3D60;
    border-radius: 4px;
    padding: 5px 8px;
    font-size: 12px;
}



QLineEdit,
QSpinBox,
QTextEdit {
    background-color: #1C2840;
    color: #E4E8F0;
    border: 1px solid #2A3D60;
    border-radius: 6px;
    padding: 5px 10px;
    font-size: 13px;
    min-height: 30px;
}

QLineEdit:focus,
QSpinBox:focus,
QTextEdit:focus {
    border-color: #2563EB;
}

QSpinBox::up-button,
QSpinBox::down-button {
    background-color: #253358;
    border: none;
    width: 16px;
    border-radius: 3px;
}



QDialog {
    background-color: #131B30;
    color: #E4E8F0;
}

QDialog QLabel {
    color: #7B8EA8;
    font-size: 13px;
    background: transparent;
}



QMessageBox {
    background-color: #131B30;
}

QMessageBox QLabel {
    color: #E4E8F0;
    font-size: 13px;
    background: transparent;
}

QMessageBox QPushButton {
    background-color: #2563EB;
    color: #FFFFFF;
    border: none;
    border-radius: 6px;
    padding: 6px 16px;
    font-size: 13px;
    min-width: 80px;
    min-height: 30px;
}

QMessageBox QPushButton:hover {
    background-color: #1D4ED8;
}

QMessageBox QPushButton:pressed {
    background-color: #1E40AF;
}
)");
}




inline void setupApplication(QApplication& app)
{

    app.setStyle(QStyleFactory::create("Fusion"));


    QFont f;
    f.setFamily("Segoe UI");
    f.setPointSize(10);
    f.setHintingPreference(QFont::PreferFullHinting);
    app.setFont(f);


    app.setStyleSheet(AppStyle::globalStyleSheet());
}


inline QString statusLabelStyle(const QString& status)
{
    if (status == QStringLiteral("В наявності"))
        return QStringLiteral("color: #22C55E; font-weight: 600; background: transparent;");
    if (status == QStringLiteral("Низький запас"))
        return QStringLiteral("color: #F59E0B; font-weight: 600; background: transparent;");
    if (status == QStringLiteral("Відсутній"))
        return QStringLiteral("color: #EF4444; font-weight: 600; background: transparent;");
    return QStringLiteral("color: #7B8EA8; background: transparent;");
}

}