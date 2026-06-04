#pragma once
#include <QWidget>
#include <QTimer>
#include <QProgressBar>
#include <QLabel>

class SplashScreen : public QWidget
{
    Q_OBJECT
public:
    explicit SplashScreen(QWidget *parent = nullptr);
    void startProgress();

signals:
    void finished();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onTick();

private:
    void buildUi();
    void drawWarehouseIcon(QPainter &p, int cx, int cy);

    QProgressBar *m_bar     = nullptr;
    QLabel       *m_percent = nullptr;
    QTimer       *m_timer   = nullptr;
    int           m_value   = 0;
};