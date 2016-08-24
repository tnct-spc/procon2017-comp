#include "algorithmwrapper.h"

AlgorithmWrapper::AlgorithmWrapper()
{

}

procon::Field AlgorithmWrapper::run(procon::Field field)
{
    return field;
}

int AlgorithmWrapper::searchSameLength(procon::ExpandedPolygon polygon1, procon::ExpandedPolygon polygon2, int& ans1, int& ans2, int& ans3, int& ans4, double& length1, double& length2, double& angleb1, double& angleb2, double& anglea1, double& anglea2)
{
    ans1 = -1;
    ans2 = -1;
    ans3 = -1;
    ans4 = -1;
    length1 = -1;
    length2 = -1;
    angleb1 = -1;
    angleb2 = -1;
    anglea1 = -1;
    anglea2 = -1;
    const double length_error = 0.05; // 単位CM
    const double angle_error = 0.017; //単位rad
    int loopNumber1 = polygon1.getSize();
    int loopNumber2 = polygon2.getSize();
    for(int i=0; i < loopNumber1; ++i)
    {
        double Comped = polygon1.getSideLength()[i];
        for(int j=0; j < loopNumber2; ++j)
        {
            double Comping = polygon2.getSideLength()[j];
            if (Comped - (length_error*2) < Comping && Comping < Comped + (length_error*2))
            {
                angleb1=polygon1.getSideAngle()[i];
                angleb2=polygon2.getSideAngle()[j];
                if ((M_PI * 2)-(angle_error * 2) < (angleb1 + angleb2) && (angleb1 + angleb2) < (M_PI * 2)+(angle_error * 2))
                {
                    anglea1=polygon1.getSideAngle()[i+1];
                    anglea2=polygon2.getSideAngle()[j+1];
                    if ((M_PI * 2)-(angle_error * 2) < (anglea1 + anglea2) && (anglea1 + anglea2) < (M_PI * 2)+(angle_error * 2))
                    {
                        ans1 = i;
                        ans2 = i+1;
                        ans3 = j;
                        ans4 = j+1;
                        length1 = Comped;
                        length2 = Comping;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
