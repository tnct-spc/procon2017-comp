#ifndef FIT_H
#define FIT_H


class Fit
{
public:
    enum DotORLine { Dot , Line };
    DotORLine start_dot_or_line = Dot;
    int start_id = -1;
    DotORLine end_dot_or_line = Dot;
    int end_id = -1;
    Fit();
};

#endif // FIT_H
