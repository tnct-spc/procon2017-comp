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
    int frame_inner_pos = -1;
    bool is_start_straight = false;
    bool is_end_straight = false;

    Fit();
};

#endif // FIT_H
