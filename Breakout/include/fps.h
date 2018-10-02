#ifndef FPS_H
#define FPS_H


class fps
{
    public:
        fps();
        virtual ~fps();

        void update_timer();
        void update_ctr();
        float avgfps();

        void delay_time(int ms);

    protected:

    private:
        float ctr;
        float tt;
        float avg;
        int lastTime;
        int currentTime;
};

#endif // FPS_H
