#pragma once
#include <QString>
#include <QList>

struct CellItem {
    QString productName;
    int     quantity;
};

struct WarehouseCell {
    QString          id;
    QString          section;
    int              row;

    QList<CellItem>  items;


    static QList<WarehouseCell> mockData() {

        return {
                {"A-01","A",1, {}},
                {"A-02","A",2, {}},
                {"A-03","A",3, {}},
                {"B-01","B",1, {}},
                {"B-02","B",2, {}},
                {"B-03","B",3, {}},
                {"C-01","C",1, {}},
                {"C-02","C",2, {}},
                {"C-03","C",3, {
                                     {"Папір офісний А3", 44},
                                     {"Одяг MN12",        52},
                                     {"Роутер PO24",       3},
                                     }},
                };
    }


    static QStringList allSectionIds() {
        return {"A-01","A-02","A-03","B-01","B-02","B-03","C-01","C-02","C-03"};
    }
};