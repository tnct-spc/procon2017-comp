#ifndef TRYNEXTSEARCH_H
#define TRYNEXTSEARCH_H

#include <QWidget>
#include "neoanswerboard.h"
#include "singlepolygondock.h"

namespace Ui {
class TryNextSearch;
}

class TryNextSearch : public QWidget
{
    Q_OBJECT

signals:
    void startBeamSearch(procon::NeoField next_field);

public:
    explicit TryNextSearch(QWidget *parent = 0);
    ~TryNextSearch();

    void setField(procon::NeoField field);

private:
    Ui::TryNextSearch *ui;
    SinglePolygonDock *dock;
    QPushButton *go_button;
    NeoAnswerBoard *board;

    procon::NeoField field;
    bool checkFloatPiece(procon::NeoField field , std::vector<procon::NeoExpandedPolygon> expolygon);


private slots:
    void clickedGoButton();
};

#endif // TRYNEXTSEARCH_H
