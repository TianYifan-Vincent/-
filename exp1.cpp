#include <iostream>
#include <queue>
#include <string>
#include <time.h>
#define num 5
using namespace std;

struct PCB //PCB structure
{
    int id = 0;                         //进程标识号
    int priority = 0;                   //进程优先数
    int cpuTime = 0;                    //进程已经运行时间
    int allTime = 0;                    //进程还需运行时间
    int status = 1;                     //进程状态  0:RUN 1:WAIT 2:FINISH
    friend bool operator<(PCB A, PCB B) //优先队列的排队顺序，大顶堆
    {
        return A.priority < B.priority;
    }
    void setID(int ID) { id = ID; }
    void setPriority(int Priority) { priority = Priority; }
    void setcpuTime(int CPUTime) { cpuTime = CPUTime; }
    void setallTime(int AllTime) { allTime = AllTime; }
    void setStatus(int Status) { status = Status; }
} Array[num + 1]; //进程队列

//全局变量
queue<PCB> ready_queueRR;        //RR的就绪队列
priority_queue<PCB> ready_queue; //PSA的就绪队列
string mode;                     //算法模式
int RUN;                         //当前正在运行的进程号
int length;                      //队列长度

int random(double start, double end)
{
    return (int)(start + (end - start) * rand() / (RAND_MAX + 1.0));
}

//创建进程
void CreatProcess()
{
    if (mode == "1")
    {
        for (int i = 1; i <= num; i++)
        {
            Array[i].setID(i);
            Array[i].setPriority(random(5, 50));
            Array[i].cpuTime = 0;
            Array[i].setallTime(random(1, 10));
            Array[i].status = 1;
            ready_queue.push(Array[i]);
        }
        RUN = ready_queue.top().id;
        ready_queue.pop();
    }
    else if (mode == "2")
    {
        for (int i = 1; i <= num; i++)
        {
            Array[i].setID(i);
            Array[i].setPriority(random(2, 7));
            Array[i].cpuTime = 0;
            Array[i].setallTime(random(1, 10));
            Array[i].status = 1;
            ready_queueRR.push(Array[i]);
        }
        RUN = ready_queueRR.front().id;
        ready_queueRR.pop();
    }
}

void Display()
{
    queue<PCB> temp; //另外的队列用来打印队列顺序
    if (mode == "1")
    {
        length = ready_queue.size();
        while (!ready_queue.empty())
        {
            temp.push(ready_queue.top());
            ready_queue.pop();
        }
    }
    else if (mode == "2")
    {
        length = ready_queueRR.size();
        while (!ready_queueRR.empty())
        {
            temp.push(ready_queueRR.front());
            ready_queueRR.pop();
        }
    }
    cout << "OUTPUT OF " << mode << endl;
    cout << "==================================================" << endl;
    cout << "RUNNING PROC.\t\t\tWAITING QUEUE" << endl;
    cout << Array[RUN].id << "\t\t";
    for (int i = 1; i <= length; i++)
    {
        cout << temp.front().id << "\t";
        temp.push(temp.front());
        temp.pop();
    }
    cout << endl;
    cout << "==================================================" << endl;
    cout << "ID\t\t1\t2\t3\t4\t5" << endl;
    cout << "PRIORITY\t";
    for (int i = 1; i <= num; i++)
    {
        cout << Array[i].priority << "\t";
    }
    cout << endl;
    cout << "CPUTIME\t\t";
    for (int i = 1; i <= num; i++)
    {
        cout << Array[i].cpuTime << "\t";
    }
    cout << endl;
    cout << "ALLTIME\t\t";
    for (int i = 1; i <= num; i++)
    {
        cout << Array[i].allTime << "\t";
    }
    cout << endl;
    cout << "STATUS\t\t";
    for (int i = 1; i <= num; i++)
    {
        if (Array[i].status == 0)
        {
            cout << "R"
                 << "\t";
        }
        else if (Array[i].status == 1)
        {
            cout << "W"
                 << "\t";
        }
        else if (Array[i].status == 2)
        {
            cout << "F"
                 << "\t";
        }
    }
    cout << endl;
    cout << "==================================================" << endl;
    if (mode == "1")
    {
        while (!temp.empty())
        {
            ready_queue.push(temp.front());
            temp.pop();
        }
    }
    else if (mode == "2")
    {
        while (!temp.empty())
        {
            ready_queueRR.push(temp.front());
            temp.pop();
        }
    }
}

void PriorityProcess()
{
    Array[RUN].setStatus(0);
    Display();
    //优先数法
    while (RUN != 0)
    {
        Array[RUN].setallTime(Array[RUN].allTime - 1);
        Array[RUN].setPriority(Array[RUN].priority - 3);
        Array[RUN].setcpuTime(Array[RUN].cpuTime + 1);
        if (Array[RUN].allTime == 0)
        {
            //此进程结束
            Array[RUN].status = 2;
            if (!ready_queue.empty())
            {
                RUN = ready_queue.top().id;
                Array[RUN].status = 0;
                ready_queue.pop();
            }
            else
            {
                Array[0].id = Array[RUN].id;
                RUN = 0;
            }
        }
        else
        {
            if (!ready_queue.empty() && Array[RUN].priority <= ready_queue.top().priority)
            {
                // 如果不为空就放入队列，排序后接着运行，如果为空就直接运行
                Array[RUN].status = 1;
                ready_queue.push(Array[RUN]);
                RUN = ready_queue.top().id;
                Array[RUN].status = 0;
                ready_queue.pop();
            }
        }
        Display();
    }
}

void RoundRobinProcess()
{
    Array[RUN].setStatus(0);
    Display();
    int count = Array[RUN].priority;
    while (RUN != 0)
    {
        Array[RUN].allTime -= 1;
        Array[RUN].cpuTime += 1;
        count--;
        if (Array[RUN].allTime == 0 && count >= 0)
        {
            Array[RUN].status = 2;
            if (!ready_queueRR.empty())
            {
                RUN = ready_queueRR.front().id;
                count = Array[RUN].priority;
                Array[RUN].status = 0;
                ready_queueRR.pop();
            }
            else
            {
                Array[0].id = Array[RUN].id;
                RUN = 0;
            }
        }
        else if (count == 0)
        {
            if (!ready_queueRR.empty())
            {
                Array[RUN].status = 1;
                ready_queueRR.push(Array[RUN]);
                RUN = ready_queueRR.front().id;
                count = Array[RUN].priority;
                Array[RUN].status = 0;
                ready_queueRR.pop();
            }
            else
                count = Array[RUN].priority;
        }
        Display();
    }
}

int main()
{
    srand((unsigned)time(NULL));

    cout << "TYPE THE ALGORITHM PSA(1) or RR(2) or EXIT(0):" << endl;
    while (cin >> mode)
    {
        CreatProcess();
        if (mode == "1")
        {
            PriorityProcess();
        }
        else if (mode == "2")
        {
            RoundRobinProcess();
        }
        else if (mode == "0")
        {
            break;
        }
        cout << "\nTYPE THE ALGORITHM PSA(1) or RR(2) or EXIT(0):";
    }
    return 0;
}
