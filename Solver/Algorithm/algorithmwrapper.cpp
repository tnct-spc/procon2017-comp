#include "algorithmwrapper.h"

AlgorithmWrapper::AlgorithmWrapper()
{

}

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

int AlgorithmWrapper::searchSameLength(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2, Fit &fit1, Fit &fit2)
{
    /*許容誤差指定*/
    const double length_error = 0.05; // 単位CM
    const double angle_error = 0.017; //単位rad
    double comped;
    double comping;
    int maxloop;
    int repeat;
    int Eva = 0;
    int i=0;
    int j=0;
    fit1.start_dot_or_line=Dot;
    fit1.start_id=-1;
    fit2.start_dot_or_line=Dot;
    fit2.start_id=-1;
    fit1.end_dot_or_line=Dot;
    fit1.end_id=-1;
    fit2.end_dot_or_line=Dot;
    fit2.end_id=-1;

    maxloop = polygon1.getSize();
    repeat=polygon2.getSize();
    for (i=0; i<maxloop; ++i)       //角度のみ探索
    {
        comped=polygon1.getSideAngle()[i];
        for (j=0; j<repeat; ++j)
        {
            comping=polygon2.getSideAngle()[j];
            if ((M_PI * 2)-(angle_error * 2) < (comped + comping) && (comped + comping) < (M_PI * 2)+(angle_error * 2))
            {
                Eva++;
                goto exit;
            }
        }
    }
    return 0;

    exit:
    int start_polygon1 = i;
    int start_polygon2 = j;
    for (int k=0; k<maxloop; ++k)
    {
            start_polygon2--;
            if (start_polygon2 < 0)
            {
                start_polygon2=polygon2.getSize();
            }
            comped=polygon1.getSideLength()[start_polygon1];
            comping=polygon2.getSideLength()[start_polygon2];
            if (comped - (length_error*2) < comping && comping < comped + (length_error*2))
            {
                Eva++;
            } else {
                if (start_polygon2 = polygon2.getSize())
                {
                    start_polygon2=-1;
                }
                fit1.start_dot_or_line=Dot;
                fit1.start_id=start_polygon1;
                fit2.start_dot_or_line=Dot;
                fit2.start_id=start_polygon2+1;
                goto reexit;
            }


            start_polygon1++;
            if (start_polygon1 > polygon1.getSize())
            {
                start_polygon1=0;
            }
            comped=polygon1.getSideAngle()[start_polygon1];
            comping=polygon2.getSideAngle()[start_polygon2];
            if (comped - (length_error*2) < comping && comping < comped + (length_error*2))
            {
                Eva++;
            } else {
                if (start_polygon1 == 0)
                {
                    start_polygon1=polygon1.getSize()+1;
                }
                fit1.start_dot_or_line=Line;
                fit1.start_id=start_polygon1-1;
                fit2.start_dot_or_line=Line;
                fit2.start_id=start_polygon2;
                goto reexit;
            }
   }

   reexit:
    start_polygon1 = i;
    start_polygon2 = j;
    for (int k=0; k<maxloop; ++k)
    {
            start_polygon1--;
            if (start_polygon1 < 0)
            {
                start_polygon1=polygon1.getSize();
            }
            comped=polygon1.getSideLength()[start_polygon1];
            comping=polygon2.getSideLength()[start_polygon2];
            if (comped - (length_error*2) < comping && comping < comped + (length_error*2))
            {
                Eva++;
            } else {
                if (start_polygon1 == polygon1.getSize())
                {
                    start_polygon1=-1;
                }
                fit1.end_dot_or_line=Dot;
                fit1.end_id=start_polygon1+1;
                fit2.end_dot_or_line=Dot;
                fit2.end_id=start_polygon2;
                if (Eva == 1){return -1;}
                return Eva;
            }


            start_polygon2++;
            if (start_polygon2 > polygon2.getSize())
            {
                start_polygon2=0;
            }
            comped=polygon1.getSideAngle()[start_polygon1];
            comping=polygon2.getSideAngle()[start_polygon2];
            if (comped - (length_error*2) < comping && comping < comped + (length_error*2))
            {
                Eva++;
            } else {
                if (start_polygon2 == 0)
                {
                    start_polygon2=polygon2.getSize()+1;
                }
                fit1.end_dot_or_line=Line;
                fit1.end_id=start_polygon1;
                fit2.end_dot_or_line=Line;
                fit2.end_id=start_polygon2-1;
                if (Eva == 1){return -1;}
                return Eva;
            }
   }
}
