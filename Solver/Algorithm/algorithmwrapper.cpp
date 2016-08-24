#include "algorithmwrapper.h"

AlgorithmWrapper::AlgorithmWrapper()
{

}

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

int AlgorithmWrapper::searchSameLength(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2, Fit fit1, Fit fit2)
{
    /*許容誤差指定*/
    const double length_error = 0.05; // 単位CM
    const double angle_error = 0.017; //単位rad
    double comped;
    double comping;
    int result;
    int maxloop;
    int repeat;
    int Eva;
    int i=0;
    int j=0;

    maxloop = polygon1.getSize();
    for (i=0; i<maxloop; ++i)
    {
        comped=polygon1.getSideAngle()[i];
        repeat=polygon2.getSize();
        for (j=0; j<repeat; ++j)
        {
            comping=polygon2.getSideAngle()[j];
            if ((M_PI * 2)-(angle_error * 2) < (comped + comping) && (comped + comping) < (M_PI * 2)+(angle_error * 2))
            {
                goto exit;
            }
        }
    }
    return 0;

    exit:
    for (int k=0; k<maxloop; ++k)
    {
            comped=polygon1.getSideLength()[i];
            comping=polygon2.getSideLength()[j];
            if (comped - (length_error*2) < comping && comping < comped + (length_error*2))
            {
                Eva++;
                result=1;
            } else {
                result=0;
            }
            if (result = 0)
            {
                fit1.start_dot_or_line=false;
                fit1.start_id=i-1;
                fit2.start_dot_or_line=false;
                fit2.start_id=j+1;
                goto reexit;
            }
            comped=polygon1.getSideAngle()[i];
            comping=polygon2.getSideAngle()[j];
            if (comped - (length_error*2) < comping && comping < comped + (length_error*2))
            {
                Eva++;
                result=1;
            } else {
                result=0;
            }
            if (result = 0)
            {
                fit1.start_dot_or_line=true;
                fit1.start_id=i-1;
                fit2.start_dot_or_line=true;
                fit2.start_id=j+1;
                goto reexit;
            }
            i++;
            j--;
            if (i > polygon1.getSize())
            {
                i=0;
            }
            if (j < 0)
            {
                j=polygon2.getSize();
            }
   }

   reexit:
   for (int k=0; k<maxloop; ++k)
   {
        comped=polygon1.getSideLength()[i];
        comping=polygon2.getSideLength()[j];
        if (comped - (length_error*2) < comping && comping < comped + (length_error*2))
        {
            Eva++;
            result=1;
        } else {
            result=0;
        }
            if (result = 1)
        {
            fit1.end_dot_or_line=false;
            fit1.end_id=i;
            fit2.end_dot_or_line=false;
            fit2.end_id=j;
            return 1;
        }
            comped=polygon1.getSideAngle()[i];
            comping=polygon2.getSideAngle()[j];
            if (comped - (length_error*2) < comping && comping < comped + (length_error*2))
        {
            Eva++;
            result=1;
        } else {
            result=0;
        }
        if (result = 1)
        {
            fit1.start_dot_or_line=true;
            fit1.start_id=i;
            fit2.start_dot_or_line=true;
            fit2.start_id=j;
            return 1;
         }
         i++;
         j--;
         if (i > polygon1.getSize())
         {
             i=0;
         }
         if (j < 0)
         {
             j=polygon2.getSize();
         }
    }
}
