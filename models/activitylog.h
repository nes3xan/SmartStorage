#pragma once
#include <QString>
#include <QList>

struct ActivityLogEntry {
    QString time;
    QString action;
    QString productName;

    QString displayText() const {
        return action + " \"" + productName + "\"";
    }

    static QList<ActivityLogEntry> mockData() {
        return {
                {"15:43", "Надходження товару",  "Монітор N198k"},
                {"16:23", "Переміщення товару",  "Миша L3987"},
                {"16:40", "Надходження товару",  "Ноутбук Teno S300"},
                {"18:51", "Списання товару",     "Гарнітура LGD0013"},
                {"19:33", "Додано товар",        "Телефон Is 14max"},
                {"19:47", "Надходження товару",  "Папір офісний А3"},
                {"20:01", "Переміщення товару",  "Роутер MiniRR22"},
                };
    }
};