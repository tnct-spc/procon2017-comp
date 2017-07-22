#ifndef NEOPOLYGONVIEWER_H
#define NEOPOLYGONVIEWER_H

#include <vector>
#include "neosinglepolygondisplay.h"

class NeoPolygonViewer : public QWidget
{
    Q_OBJECT

public:
    static NeoPolygonViewer& getInstance();
    void displayPolygon(polygon_i const& polygon, std::string wname = "");

private:
    explicit NeoPolygonViewer(QWidget *parent = 0);
    std::vector<std::unique_ptr<NeoSinglePolygonDisplay>> viewers;
};

#endif // NEOPOLYGONVIEWER_H
