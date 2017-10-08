#ifndef TRYNEXTSEARCH_H
#define TRYNEXTSEARCH_H

#include <QWidget>
#include <QGraphicsView>
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
    void zoom_In();
    void zoom_Out();

public:
    explicit TryNextSearch(QWidget *parent = 0);
    ~TryNextSearch();

    void setField(procon::NeoField field);

private:
    Ui::TryNextSearch *ui;
    SinglePolygonDock *dock;
    QPushButton *go_button;
    NeoAnswerBoard *board;
    double zoom = 1;

    QGraphicsView *view;

    procon::NeoField field;
    bool checkFloatPiece(std::vector<polygon_i> frames);


private slots:
    void clickedGoButton();
    void _zoom_In();
    void _zoom_Out();
};

#endif // TRYNEXTSEARCH_H
