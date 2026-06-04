#include "settingspage.h"
#include <QLabel>
#include <QVBoxLayout>
SettingsPage::SettingsPage(QWidget *parent) : QWidget(parent) {
    auto *lbl = new QLabel("⚙  Налаштування — в розробці", this);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet("color:#7B8EA8; font-size:18px;");
    (new QVBoxLayout(this))->addWidget(lbl);
}