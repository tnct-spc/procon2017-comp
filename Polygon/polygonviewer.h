#ifndef POLYGONVIEWER_H
#define POLYGONVIEWER_H

#include <QWidget>
#include "field.h"
#include "singlepolygondisplay.h"

class PolygonViewer : public QWidget
{
    Q_OBJECT
public:
    static PolygonViewer& getInstance();
    void pushPolygon(procon::ExpandedPolygon& polygon, int scale = 30, std::string wname_ = "Polygon");
private:
    explicit PolygonViewer(QWidget *parent = 0);
    std::vector<std::unique_ptr<SinglePolygonDisplay>> Displays;

signals:

public slots:
};

#endif // POLYGONVIEWER_H
