#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPoint>
#include <QIcon>

class DashboardPage;
class ItemsPage;
class ItemCardPage;
class EmployeesPage;
class WarehouseMapPage;
struct Product;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    void openItemCard(const Product &p);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    void buildUi();
    void buildSidebar(QWidget *parent, QLayout *parentLayout);
    void buildTopBar(QWidget *parent, QLayout *parentLayout);
    void buildPages();
    void switchPage(int index);
    QPushButton *makeNavBtn(const QIcon &icon, const QString &text, const QString &tip);

    QWidget     *m_sidePanel   = nullptr;
    QPushButton *m_navHome     = nullptr;
    QPushButton *m_navItems    = nullptr;
    QPushButton *m_navEmp      = nullptr;
    QPushButton *m_navMap      = nullptr;

    QLineEdit      *m_search   = nullptr;
    QStackedWidget *m_stack    = nullptr;

    DashboardPage    *m_dashboard    = nullptr;
    ItemsPage        *m_items        = nullptr;
    ItemCardPage     *m_itemCard     = nullptr;
    EmployeesPage    *m_employees    = nullptr;
    WarehouseMapPage *m_warehouseMap = nullptr;

    QPoint m_dragPos;
    bool   m_dragging = false;
};