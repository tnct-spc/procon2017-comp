#ifndef THRESHOLD_H
#define THRESHOLD_H


class threshold
{
private:
    static bool read_default_config_flag;
public:
    threshold(/*QString config_file_path = "dynamic.yml"*/);
    int test_threshold_san = 3.1415926535;

    static double LSDthrehold;
};

#endif // THRESHOLD_H
