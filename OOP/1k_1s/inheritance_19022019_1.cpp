class Clock {
    private:
        float time;
    public:
        float gettime() { return time; }
        void settime(float tm) {
            if (tm >= 0 && tm <24) time = tm;
        }
};

class Alarm: public Clock { //наслідування від Clock
    private:
        float alarm;
    public:
        void setalarm(float alarm) {
            if (alarm >= 0 && alarm <24) this->alarm = alarm;
        }
};


int main(int argc, char const *argv[])
{
    Clock C;
    Alarm A;
    C.settime(12.5);
    A.settime(14.0); //наслідований
    A.setalarm(6); //власний метод


    return 0;
}

