#ifndef TIMER_H
#define TIMER_H

//this will be used for async. execution, all variables here will be private by default

#include <thread>
#include <chrono>
#include <functional>


class Timer
{
    std::thread Thread; //this will be used for async. code execution
    bool Alive = false; //state of timer if its runing or not
    long CallNumber = -1L; //used to determine how many time a function is called
    long repeat_count = -1L; //count amount of time a certian function has been called
    std::chrono::milliseconds interval = std::chrono::milliseconds(0);
    std::function<void(void)> funct = nullptr;  //can accept a function , does not return anything.

    void SleepAndRun()
    {
        std::this_thread::sleep_for(interval);
        if(Alive)
            Function()(); //public method, represents a function,
    }

    void ThreadFunc()
    {
        //this ftn willl be passed to the thread
        if(CallNumber == INFINITE)
            while(Alive)
                SleepAndRun();
        else
            while(repeat_count--)
                SleepAndRun();
    }

public:
    static const long Infinite = -1L; // static so can be used with out creating an instance of this

    Timer(){} //used to create timer as a global variable, becose variable will die in the end of the ftn if they are local variables

    Timer(const std::function<void(void)> &f): funct (f) {}

    Timer(const std::function <void(void)> &f, const unsigned long &i, const long repeat = Timer::Infinite) : funct(f), interval(std::chrono::milliseconds(i)),CallNumber(repeat){}


    void Start(bool Async = true)
    {
        if(isAlive())
            return;
        Alive = true;
        repeat_count  = CallNumber;
        if(Async)
            Thread = std::thread(ThreadFunc,this);
        else
            this->ThreadFunc();
    }
    void Stop()
    {
        Alive = false;
        Thread.join();
    }
    void SetFunction(const std::function<void(void)> &f)
    {
        funct = f;
    }

    bool isAlive() const {return Alive;}

    void RepeatCount(const long r)
    {
        if(Alive)
            return;
        CallNumber = r;
    }

    long GetLeftCount() const {return repeat_count;}

    long RepeatCount() const {return CallNumber;}

    void SetInterval(const unsigned long &i)
    {

        if(Alive)
            return;
        interval = std::chrono::milliseconds(i);
    }

    unsigned long Interval() const {return interval.count();}

    const std::function<void(void)> &Function() const
    {
        return funct;
    }


};








#endif // TIMER_H
