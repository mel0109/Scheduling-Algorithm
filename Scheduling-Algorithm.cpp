#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <windows.h>
using namespace std;

class process
{
    int procNum;
    int burst;
    int arrival;
    int priority;
    int terminate_time;
    vector<int> execute_time;
    vector<int> interrupts;

public:
    process(){};
    process(int procNum,int arrival,int burst)
    {
        this->procNum = procNum;
        this->arrival = arrival;
        this->burst = burst;
        vector<int> interrupts;
    }
    process(int procNum,int arrival,int burst,int priority)
    {
        this->procNum = procNum;
        this->arrival =arrival;
        this->burst = burst;
        this->priority = priority;
        vector<int> interrupts;
    }

    int get_procNum()
    {
        return procNum;
    }

    int get_burst()
    {
        return burst;
    }

    int get_arrival()
    {
        return arrival;
    }

    int get_priority()
    {
        return priority;
    }
    int get_terminate_time()
    {
        return terminate_time;
    }

    vector<int> get_interrupts()
    {
        return interrupts;
    }
    vector<int> get_execute_time()
    {
        return execute_time;
    }

    void deduct_burst()
    {
        this->burst = burst -1;
    }

    void set_interrupt(int time)
    {
      interrupts.push_back(time);
    }

    void set_execute_time(int time)
    {
      execute_time.push_back(time);
    }

    void set_terminate(int time)
    {
      terminate_time = time;
    }





    /**Overloads sort to compare burst time in process queue**/
    bool operator < (const process& procs) const
    {
        return (burst < procs.burst);
    }
};

void swap(vector<process> & data, int i, int j)
{
        process tmp = data[i];
        data[i]= data[j];
        data[j]= tmp;
}

void SortByPriority(vector<process>& x)
{
        int vectorsize = x.size();
        for(int i=0; i < vectorsize;i++){
            bool swapped = false;
            for (int j = 0; j < vectorsize -(i+1); ++j){
                if(x[j].get_priority()> x[j+1].get_priority()){
                    swap(x, j, j+1);
                    swapped = true;
                }
            }
            if(!swapped) break;
        }
}

void CycleVector(vector<process>& x){
    process Temp;
    Temp =x[0];
    x.erase(x.begin());
    x.push_back(Temp);
}

void SortAscending(vector<int>& x){
    int hold;
    for(int i=0; i<x.size(); i++)
    {
        for(int j=0; j <x.size()-1; j++)
        {
            if(x[i] < x[j])
            {
                hold = x[i];
                x[i] =x[j];
                x[j] = hold;
            }
        }
    }
}
void PrintGanttChart(vector<process>& p, int n){
    vector<int> AllInterrupts;
    vector<int> AllExecutes;
    vector<int>Terminate;
    vector<int> check;

    int largestTerminate;
    int hold;
    int Pnum;

    for(int i=0; i < n; i++)
    {
        Terminate.push_back(p[i].get_terminate_time());
    }
    SortAscending(Terminate);
    /*
    cout << "---------Terminate times----------"<<endl;
    for(int i=0; i < Terminate.size(); i++)
    {
        cout << Terminate[i] <<endl;
    }*/

    for(int i =0; i < n; i++)
    {
        vector<int>temp = p[i].get_interrupts();
        vector<int>temp2 = p[i].get_execute_time();
        for (int j = 0 ; j < temp.size(); j++)
        {
            AllInterrupts.push_back(temp[j]);
        }
        for (int j = 0 ; j < temp2.size(); j++)
        {
            AllExecutes.push_back(temp2[j]);
        }
    }
    AllInterrupts.push_back(Terminate.back());

    SortAscending(AllExecutes);
    SortAscending(AllInterrupts);
    /*
    cout << "---------Execute times----------"<<endl;
    for(int i=0; i < AllExecutes.size(); i++)
    {
        cout << AllExecutes[i] <<endl;
    }

    cout <<"----------Interrupt times----------" <<endl;
    for(int i=0; i < AllInterrupts.size(); i++)
    {
        cout << AllInterrupts[i] <<endl;
    }*/

    //cout << "execute vector size: "<<AllExecutes.size() <<endl;
    //cout << "Interrupt vector size: "<<AllInterrupts.size() <<endl;
    //top bar
    cout << " ";
    for(int i = 0; i < AllExecutes.size(); i++)
    {
        for(int j = AllExecutes[i]; j < AllInterrupts[i]; j++)
        {
            cout << "--";
        }
        cout << " ";
    }
    cout << endl << "|";





    //mid section

    for (int i = 0; i < AllExecutes.size(); i++)
    {
        for(int j = AllExecutes[i]; j < AllInterrupts[i] -1; j++)
        {
            cout <<  " ";
        }
        for(int j = 0; j < n; j++){
            check = p[j].get_execute_time();
            for(int k = 0; k < check.size(); k++){
                if(AllExecutes[i] == check[k]){
                    Pnum = p[j].get_procNum();
                }
            }
        }

        cout << "P" << Pnum;
        for(int j = AllExecutes[i]; j < AllInterrupts[i] -1; j++)
        {
            cout <<  " ";
        }
        cout << "|";
    }


    cout << endl << " ";

    //bottom bar
    for(int i = 0; i < AllExecutes.size(); i++)
    {
        for(int j = AllExecutes[i]; j < AllInterrupts[i]; j++)
        {
            cout << "--";
        }
        cout << " ";
    }

    //printing the timeline
    cout << endl<< "0";
    for(int i=0; i < AllExecutes.size(); i++)
    {
        for(int j = AllExecutes[i]; j < AllInterrupts[i] ; j++)
        {
            cout<<"  ";
        }
        if(AllInterrupts[i] >= 10)
        {
            cout << "\b" ; //helps to remove a space.
        }
        cout << AllInterrupts[i];
    }
    cout <<"\n\n";






}

vector<int>calcTT(vector<process>& p, vector<int>& TT)
{
    int tt=0;
    for(int i=0; i < p.size(); i++)
    {
        tt = p[i].get_terminate_time() - p[i].get_arrival();
        TT.push_back(tt);
    }
    return TT;
}

vector<int> calcWT(vector<process>& p, vector<int>& TT, vector<int>& WT)
{
    int wt;
    for(int i=0; i<p.size(); i++)
    {
        wt = TT[i] - p[i].get_burst();
        WT.push_back(wt);
    }
    return WT;
}

void getProcesses(vector<process>& allprocs, vector<process>& backup, int n ){
    int arrivaltime=0;
    int bursttime=0;
    int priority=0;
    for (int i =1; i <= n; i++)
    {
        cout << "Enter information for Process" << i << endl;
        cout << "Arrival Time: ";
        cin >> arrivaltime;
        cout << "Burst Time:";
        cin >> bursttime;
        cout << "Priority:" ;
        cin >> priority;
        allprocs.push_back(process( i, arrivaltime, bursttime, priority));
        backup.push_back(process( i, arrivaltime, bursttime, priority));
    }
}
int main()
{
    bool loop=true;
    do{
        vector<process> AllProcs;
        vector<process> ProcsQueue;
        vector<process> Temp;
        vector<process>lvl3q1;
        vector<process>lvl3q2;
        vector<process>lvl3q3;
        vector<process>backup;
        int timeline = 0;
        int currentProc =0;
        int lastProc = 0;
        int menu, counter1=0;
        int quantum = 0;
        int curdeleted =0;
        int temploopsize;
        int TotalTT=0, TotalWT=0;
        double avgTT=0, avgWT=0;
        int numbOfProcess =0;

        vector<int> FCFS_TT;
        vector<int> FCFS_WT;
        vector<int> RR_TT;
        vector<int> RR_WT;
        vector<int> lvl3_TT;
        vector<int> lvl3_WT;
        vector<int> SJF_TT;
        vector<int> SJF_WT;

        int lvl3avgTT=0;
        int lvl3avgWT=0;


        cout << "choose function 1-4 :" <<endl;
        cout << "1. Preemptive Priority FCFS" << endl;
        cout << "2. Round Robin Scheduling" << endl;
        cout << "3. Three level queue Scheduling " << endl;
        cout << "4. Shortest Remaining Time Next (SRTN/Preemptive SJF) scheduling " << endl;
        cin >> menu;

        switch(menu)
        {
            case 1:
                {
                    system("cls");
                        cout << "_________________________________________________ " << endl
                             << "       *** Preemptive Priority FCFS *** "           << endl
                             << "_________________________________________________ " << endl;

                    cout << "How many processes would you like?"<<endl;
                    cin >> numbOfProcess;


                    getProcesses(AllProcs, backup, numbOfProcess);


                    for(int i = 0; i < AllProcs.size(); i++)  //GET TOTAL BURST TIME
                    {
                        timeline = timeline + AllProcs[i].get_burst();
                    }

                    for(int i=0;i<timeline+1;i++)   //total burst time
                    {
                        for(int j=0; j< AllProcs.size() ; j++)  //total processes
                        {
                            if ( i == AllProcs[j].get_arrival() )
                            {
                                ProcsQueue.push_back(AllProcs[j]); //puts process into queue

                                cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                            }
                        }
                        if(ProcsQueue[0].get_burst() ==0 )   //Check when the process is finished
                        {
                            for (int j =0; j < AllProcs.size() ; j++)
                            {
                                if(ProcsQueue[0].get_procNum() == AllProcs[j].get_procNum())
                                {
                                    AllProcs[j].set_terminate(i);   //record terminate time
                                }
                            }
                            ProcsQueue.erase(ProcsQueue.begin());   //remove from queue
                        }

                        SortByPriority(ProcsQueue);

                        ProcsQueue[0].deduct_burst();
                        currentProc = ProcsQueue[0].get_procNum();

                        if(lastProc != currentProc && lastProc!= 0)
                        {
                            for (int j =0; j < AllProcs.size() ; j++)
                            {
                                if(lastProc == AllProcs[j].get_procNum())
                                {
                                    AllProcs[j].set_interrupt(i);
                                }
                            }
                        }
                        if(lastProc != currentProc)
                        {
                            for (int j =0; j < AllProcs.size() ; j++)
                            {
                                if(currentProc == AllProcs[j].get_procNum())
                                {
                                    AllProcs[j].set_execute_time(i);
                                }
                            }
                        }
                        lastProc = currentProc;
                    }

                    /*
                    for(int i=0;i<AllProcs.size();i++)
                    {
                        vector<int> temp = AllProcs[i].get_interrupts();
                        vector<int> temp2 = AllProcs[i].get_execute_time();

                        for(int j=0; j <temp.size(); j++)
                        {
                            cout << "Process "<< AllProcs[i].get_procNum()<< " interrupt time = " << temp[j] << endl;
                        }

                        for(int j=0; j <temp2.size(); j++)
                        {
                            cout << "Process "<< AllProcs[i].get_procNum()<< " Exceute time = " << temp2[j] << endl;
                        }

                        cout << "Process "<< AllProcs[i].get_procNum() << " Terminate time = "  <<AllProcs[i].get_terminate_time() <<"\n\n";

                    }
                    */
                    PrintGanttChart(AllProcs, AllProcs.size());
                    calcTT(AllProcs, FCFS_TT);
                    calcWT(AllProcs, FCFS_TT, FCFS_WT);
                    for(int i=0; i < FCFS_TT.size(); i++)
                    {
                        cout << "Process" << i+1 << " Turnaround Time = " << FCFS_TT[i] << ", Waiting Time =" << FCFS_WT[i] <<endl;
                    }
                    TotalTT=0;
                    TotalWT=0;
                    for (int i=0; i < FCFS_TT.size(); i++)
                    {
                        TotalTT = TotalTT + FCFS_TT[i];
                    }

                    avgTT = double(TotalTT)/double(FCFS_TT.size());

                    cout << "The Average Turnaround Time for FCFS (First Come First Served)-based pre-emptive Priority:" << avgTT << endl;
                    for (int i=0; i < FCFS_WT.size(); i++)
                    {
                        TotalWT = TotalWT + FCFS_WT[i];
                    }
                    avgWT= double(TotalWT)/double(FCFS_WT.size());
                    cout << "The Average Turnaround Time for FCFS (First Come First Served)-based pre-emptive Priority:" << avgWT << endl;






                    break;
                }
            case 2:
                {
                    system("cls");
                        cout << "_________________________________________________ " << endl
                             << "            *** Round Robin *** "                   << endl
                             << "_________________________________________________ " << endl;


                    /*AllProcs.push_back(process(1, 0, 5, 1));
                    AllProcs.push_back(process(2, 4, 6, 4));
                    AllProcs.push_back(process(3, 0, 8, 2));
                    AllProcs.push_back(process(4, 7, 2, 5));
                    AllProcs.push_back(process(5, 9, 4, 3));*/

                    cout << "How many processes would you like?"<<endl;
                    cin >> numbOfProcess;


                    getProcesses(AllProcs, backup, numbOfProcess);

                    cout << "What is the desired time quantum??" <<endl;
                    cin >> quantum;



                    for(int i = 0; i < AllProcs.size(); i++)  //GET TOTAL BURST TIME
                    {
                        timeline = timeline + AllProcs[i].get_burst();
                    }
                    //cout << timeline << "timeline size"<< endl;

                    for(int i=0;i<timeline;)
                    {
                        if(i == 0){
                            counter1 =0 ;

                            for(int j=0; j< AllProcs.size() ; j++)  //total processes
                            {
                                if ( i == AllProcs[j].get_arrival() )
                                {
                                    counter1++;
                                    //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                    //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                }
                            }
                            /*if(i == 0){
                                cout << counter1 << "counter\n";
                            }*/

                            for(int j=0; j< AllProcs.size() ; j++)
                            {
                                if(counter1> 1){
                                    if(i == AllProcs[j].get_arrival())
                                        {
                                        Temp.push_back(AllProcs[j]);
                                        SortByPriority(Temp);
                                        }
                                }
                                else if(counter1 == 1)
                                {
                                    if(i == AllProcs[j].get_arrival())
                                    {
                                        ProcsQueue.push_back(AllProcs[j]);
                                    }
                                }
                            }

                            if(Temp.empty() != 1)
                            {
                                for(int j=0; j < Temp.size(); j++)
                                {
                                    ProcsQueue.push_back(Temp[j]);
                                }
                            }
                            Temp.clear();

                        }

                        currentProc = ProcsQueue[0].get_procNum();

                            if(lastProc != currentProc && lastProc!= 0)
                            {
                                for (int j =0; j < AllProcs.size() ; j++)
                                {
                                    if(lastProc == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_interrupt(i);
                                    }
                                }
                            }
                            if(lastProc != currentProc)
                            {
                                for (int j =0; j < AllProcs.size() ; j++)
                                {
                                    if(currentProc == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_execute_time(i);
                                    }
                                }
                            }
                        lastProc = currentProc;


                        if(ProcsQueue[0].get_burst()!=0)
                        {
                            if(ProcsQueue[0].get_burst() >= quantum)
                            {
                                //cout << "error 1\n";
                                for(int j=0; j < quantum ; j++)
                                {
                                    ProcsQueue[0].deduct_burst();
                                    i++;
                                    //cout << "The time is:" << i << endl;
                                    //cout << "Process Number: "<< ProcsQueue[0].get_procNum() <<endl;
                                    counter1 =0;
                                    for(int k=0; k< AllProcs.size() ; k++)  //total processes
                                    {
                                        if ( i == AllProcs[k].get_arrival() )
                                        {
                                            counter1++;
                                            //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                            //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                        }
                                    }
                                    //cout << counter1 << "counter\n";
                                    for(int k=0; k< AllProcs.size() ; k++)
                                    {
                                        if(counter1> 1){
                                            if(i == AllProcs[k].get_arrival())
                                                {
                                                Temp.push_back(AllProcs[k]);
                                                SortByPriority(Temp);
                                                }
                                        }
                                        else if(counter1 == 1)
                                        {
                                            if(i == AllProcs[k].get_arrival())
                                            {
                                                ProcsQueue.push_back(AllProcs[k]);

                                            }
                                        }
                                    }

                                    if(Temp.empty() != 1)
                                    {
                                        for(int k=0; k < Temp.size(); k++)
                                        {
                                            ProcsQueue.push_back(Temp[k]);
                                        }
                                    }
                                    Temp.clear();

                                    //cout << "error 2\n";
                                }
                            }
                            else if(ProcsQueue[0].get_burst() < quantum)
                            {
                                //cout << "error 3\n";
                                temploopsize = 0;
                                temploopsize = ProcsQueue[0].get_burst();
                                for (int j=0; j < temploopsize; j++)
                                {
                                    ProcsQueue[0].deduct_burst();
                                    i++;
                                    //cout << "The time is:" << i << endl;
                                    //cout << "Process Number: "<< ProcsQueue[0].get_procNum() <<endl;
                                    counter1=0;

                                    for(int k=0; k< AllProcs.size() ; k++)  //total processes
                                    {
                                        if ( i == AllProcs[k].get_arrival() )
                                        {
                                            counter1++;
                                            //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                            //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                        }
                                    }
                                    //cout << counter1 << "counter\n";
                                    for(int k=0; k< AllProcs.size() ; k++)
                                    {
                                        if(counter1> 1){
                                            if(i == AllProcs[k].get_arrival())
                                                {
                                                Temp.push_back(AllProcs[k]);
                                                SortByPriority(AllProcs);
                                                }
                                        }
                                        else if(counter1 == 1)
                                        {
                                            if(i == AllProcs[k].get_arrival())
                                            {
                                                ProcsQueue.push_back(AllProcs[k]);
                                            }
                                        }
                                    }

                                    if(Temp.empty() != 1)
                                    {
                                        for(int k=0; k < Temp.size(); j++)
                                        {
                                            ProcsQueue.push_back(Temp[j]);
                                        }
                                    }
                                    Temp.clear();
                                    //cout << "error 4\n";
                                }
                            }
                        }
                        //cout << "im stuck here!!" << i  << endl;

                        if(ProcsQueue[0].get_burst() ==0 )   //Check when the process is finished
                        {
                            for (int j =0; j < AllProcs.size() ; j++)
                            {
                                if(ProcsQueue[0].get_procNum() == AllProcs[j].get_procNum())
                                {
                                    AllProcs[j].set_terminate(i);   //record terminate time
                                }
                                currentProc = ProcsQueue[0].get_procNum();

                            if(lastProc != currentProc && lastProc!= 0)
                            {
                                for (int j =0; j < AllProcs.size() ; j++)
                                {
                                    if(lastProc == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_interrupt(i);
                                    }
                                }
                            }
                            if(lastProc != currentProc)
                            {
                                for (int j =0; j < AllProcs.size() ; j++)
                                {
                                    if(currentProc == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_execute_time(i);
                                    }
                                }
                            }
                            lastProc = currentProc;
                            }
                            ProcsQueue.erase(ProcsQueue.begin());   //remove from queue
                            curdeleted = 1;
                        }
                        if(curdeleted != 1){
                            CycleVector(ProcsQueue);
                        }
                        curdeleted =0;
                        //cout << "Loop completed" <<endl;

                    }


                    /*for (int i=0; i < ProcsQueue.size(); i++){
                        cout << ProcsQueue[i].get_procNum() << endl;
                    }*/
                    /*
                    for(int i=0;i<AllProcs.size();i++)
                    {
                        vector<int> temp = AllProcs[i].get_interrupts();
                        vector<int> temp2 = AllProcs[i].get_execute_time();

                        for(int j=0; j <temp.size(); j++)
                        {
                            cout << "Process "<< AllProcs[i].get_procNum()<< " interrupt time = " << temp[j] << endl;
                        }

                        for(int j=0; j <temp2.size(); j++)
                        {
                            cout << "Process "<< AllProcs[i].get_procNum()<< " Exceute time = " << temp2[j] << endl;
                        }

                        cout << "Process "<< AllProcs[i].get_procNum() << " Terminate time = "  <<AllProcs[i].get_terminate_time() <<"\n\n";

                    }*/

                    PrintGanttChart(AllProcs, AllProcs.size());
                    calcTT(AllProcs, RR_TT);
                    calcWT(AllProcs, RR_TT, RR_WT);
                    for(int i=0; i < RR_TT.size(); i++)
                    {
                        cout << "Process" << i+1 << " Turnaround Time = " << RR_TT[i] << ", Waiting Time =" << RR_WT[i] <<endl;
                    }
                    TotalTT=0;
                    TotalWT=0;
                    for (int i=0; i < RR_TT.size(); i++)
                    {
                        TotalTT = TotalTT + RR_TT[i];
                    }

                    avgTT = double(TotalTT)/double(RR_TT.size());

                    cout << "The Average Turnaround Time for Round Robin Scheduling:" << avgTT << endl;
                    for (int i=0; i < RR_WT.size(); i++)
                    {
                        TotalWT = TotalWT + RR_WT[i];
                    }
                    avgWT= double(TotalWT)/double(RR_WT.size());
                    cout << "The Average Waiting Time for Round Robin Scheduling:" << avgWT << endl;



                    break;
                }
            case 3:
                {
                    system("cls");
                        cout << "_________________________________________________ " << endl
                             << "            *** 3 Level Queue  *** "                << endl
                             << "_________________________________________________ " << endl;

                    /*AllProcs.push_back(process(1, 0, 5, 1));
                    AllProcs.push_back(process(2, 4, 6, 4));
                    AllProcs.push_back(process(3, 0, 8, 2));
                    AllProcs.push_back(process(4, 7, 2, 5));
                    AllProcs.push_back(process(5, 9, 4, 3));*/
                    /*
                    AllProcs.push_back(process(1, 28, 8, 2));
                    AllProcs.push_back(process(2, 0, 15, 5));
                    AllProcs.push_back(process(3, 0, 9, 2));
                    AllProcs.push_back(process(4, 0, 5, 1));
                    AllProcs.push_back(process(5, 0, 13, 4));
                    AllProcs.push_back(process(6, 0, 6, 1));*/

                    /*AllProcs.push_back(process(1, 0, 8, 2));
                    AllProcs.push_back(process(2, 4, 15, 5));
                    AllProcs.push_back(process(3, 7, 9, 3));
                    AllProcs.push_back(process(4, 13, 5, 1));
                    AllProcs.push_back(process(5, 9, 13, 4));
                    AllProcs.push_back(process(6, 0, 6, 1));*/

                    cout << "How many processes would you like?"<<endl;
                    cin >> numbOfProcess;

                    getProcesses(AllProcs, backup, numbOfProcess);


                    cout << "What is the desired time quantum??" <<endl;
                    cin >> quantum;

                    for(int i = 0; i < AllProcs.size(); i++)  //GET TOTAL BURST TIME
                    {
                        timeline = timeline + AllProcs[i].get_burst();
                    }
                    //cout << "timeline: " << timeline << endl;

                    for(int i=0; i < timeline;)
                    {



                        if(i ==0)
                        {
                            for(int j=0; j < AllProcs.size();j++)
                            {
                                if(i == AllProcs[j].get_arrival())
                                {
                                    if( AllProcs[j].get_priority() > 0 && AllProcs[j].get_priority() < 3)
                                    {
                                        lvl3q1.push_back(AllProcs[j]);
                                        //cout << AllProcs[j].get_procNum() << "Has been pushed in q1.\n";
                                    }
                                    else if(AllProcs[j].get_priority() > 2 && AllProcs[j].get_priority() < 5 )
                                    {
                                        lvl3q2.push_back(AllProcs[j]);
                                        //cout << AllProcs[j].get_procNum() << "Has been pushed in q2.\n";
                                    }
                                    else
                                    {
                                        lvl3q3.push_back(AllProcs[j]);
                                        //cout << AllProcs[j].get_procNum() << "Has been pushed in q3.\n";
                                    }
                                }
                            }
                        }


    //-----------------------------------------------------------------------------------------------------
                        if(!(lvl3q1.empty()))       // round robin code
                        {
                            if(i == 0)
                            {
                                counter1 = 0 ;

                                for(int j=0; j< lvl3q1.size() ; j++)  //total processes
                                {
                                    if ( i == lvl3q1[j].get_arrival() )
                                    {
                                        counter1++;
                                        //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                        //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                    }
                                }
                                if(i == 0)
                                {
                                    //cout << counter1 << "counter\n";
                                }

                                for(int j=0; j < lvl3q1.size() ; j++)
                                {
                                    if(counter1> 1)
                                    {
                                        if(i == lvl3q1[j].get_arrival())
                                            {
                                            Temp.push_back(lvl3q1[j]);
                                            SortByPriority(Temp);
                                            }
                                    }
                                    else if(counter1 == 1)
                                    {
                                        if(i == lvl3q1[j].get_arrival())
                                        {
                                            ProcsQueue.push_back(lvl3q1[j]);
                                        }
                                    }
                                }
                                if(Temp.empty() != 1)
                                {
                                    for(int j=0; j < Temp.size(); j++)
                                    {
                                        ProcsQueue.push_back(Temp[j]);
                                    }
                                }
                                Temp.clear();
                            }

                            currentProc = ProcsQueue[0].get_procNum();

                                if(lastProc != currentProc && lastProc!= 0)
                                {
                                    for (int j =0; j < AllProcs.size() ; j++)
                                    {
                                        if(lastProc == AllProcs[j].get_procNum())
                                        {
                                            AllProcs[j].set_interrupt(i);
                                        }
                                    }
                                }
                                if(lastProc != currentProc)
                                {
                                    for (int j =0; j < AllProcs.size() ; j++)
                                    {
                                        if(currentProc == AllProcs[j].get_procNum())
                                        {
                                            AllProcs[j].set_execute_time(i);
                                        }
                                    }
                                }
                            lastProc = currentProc;




                            if(ProcsQueue[0].get_burst()!=0)
                            {
                                if(ProcsQueue[0].get_burst() >= quantum)
                                {
                                    //cout << "error 1\n";
                                    for(int j=0; j < quantum ; j++)
                                    {
                                        ProcsQueue[0].deduct_burst();
                                        i++;


                                        //cout << "The time is:" << i << endl;
                                        //cout << "Process Number: "<< ProcsQueue[0].get_procNum() <<endl;
                                        counter1 =0;
                                        for(int k=0; k< lvl3q1.size() ; k++)  //total processes
                                        {
                                            if ( i == lvl3q1[k].get_arrival() )
                                            {
                                                counter1++;
                                                //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                                //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                            }
                                        }
                                        //cout << counter1 << "counter\n";
                                        for(int k=0; k< lvl3q1.size() ; k++)
                                        {
                                            if(counter1> 1)
                                            {
                                                if(i == lvl3q1[k].get_arrival())
                                                {
                                                    Temp.push_back(lvl3q1[k]);
                                                    SortByPriority(Temp);
                                                }
                                            }
                                            else if(counter1 == 1)
                                            {
                                                if(i == lvl3q1[k].get_arrival())
                                                {
                                                    ProcsQueue.push_back(lvl3q1[k]);
                                                }
                                            }
                                        }

                                        if(Temp.empty() != 1)
                                        {
                                            for(int k=0; k < Temp.size(); k++)
                                            {
                                                ProcsQueue.push_back(Temp[k]);
                                            }
                                        }
                                        Temp.clear();
                                        //cout << "error 2\n";

                                        for(int k=0; k< AllProcs.size();k++)
                                        {
                                            if(i == AllProcs[k].get_arrival())
                                            {
                                                if( AllProcs[k].get_priority() > 0 && AllProcs[k].get_priority() < 3)
                                                {
                                                    lvl3q1.push_back(AllProcs[k]);
                                                    //cout << AllProcs[k].get_procNum() << "Has been pushed in q1.\n";
                                                }
                                                else if(AllProcs[k].get_priority() > 2 && AllProcs[k].get_priority() < 5 )
                                                {
                                                    lvl3q2.push_back(AllProcs[k]);
                                                    //cout << AllProcs[k].get_procNum() << "Has been pushed in q2.\n";
                                                }
                                                else
                                                {
                                                    lvl3q3.push_back(AllProcs[k]);
                                                    //cout << AllProcs[k].get_procNum() << "Has been pushed in q3.\n";
                                                }
                                            }
                                        }

                                        for(int k=0; k< lvl3q1.size() ; k++)  //total processes
                                        {
                                            if ( i == lvl3q1[k].get_arrival() )
                                            {
                                                counter1++;
                                                //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                                //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                            }
                                        }
                                        if(i == 0)
                                        {
                                            //cout << counter1 << "counter\n";
                                        }

                                        for(int k=0; k < lvl3q1.size() ; k++)
                                        {
                                            if(counter1> 1)
                                            {
                                                if(i == lvl3q1[k].get_arrival())
                                                    {
                                                    Temp.push_back(lvl3q1[j]);
                                                    SortByPriority(Temp);
                                                    }
                                            }
                                            else if(counter1 == 1)
                                            {
                                                if(i == lvl3q1[k].get_arrival())
                                                {
                                                    ProcsQueue.push_back(lvl3q1[k]);
                                                }
                                            }
                                        }
                                        if(Temp.empty() != 1)
                                        {
                                            for(int k=0; k < Temp.size(); k++)
                                            {
                                                ProcsQueue.push_back(Temp[k]);
                                            }
                                        }
                                        Temp.clear();
                                        //cout << "Queue 1 size: " << lvl3q1.size() << endl;
                                        //cout << "Queue 2 size: " << lvl3q2.size() << endl;
                                        //cout << "Queue 3 size: " << lvl3q3.size() << endl;
                                    }
                                }
                                else if(ProcsQueue[0].get_burst() < quantum)
                                {
                                    //cout << "error 3\n";
                                    temploopsize = 0;
                                    temploopsize = ProcsQueue[0].get_burst();
                                    for (int j=0; j < temploopsize; j++)
                                    {
                                        ProcsQueue[0].deduct_burst();
                                        i++;

                                        //cout << "The time is:" << i << endl;
                                        //cout << "Process Number: "<< ProcsQueue[0].get_procNum() <<endl;
                                        counter1=0;

                                        for(int k=0; k< lvl3q1.size() ; k++)  //total processes
                                        {
                                            if ( i == lvl3q1[k].get_arrival() )
                                            {
                                                counter1++;
                                                //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                                //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                            }
                                        }
                                        //cout << counter1 << "counter\n";
                                        for(int k=0; k < lvl3q1.size() ; k++)
                                        {
                                            if(counter1> 1){
                                                if(i == lvl3q1[k].get_arrival())
                                                    {
                                                    Temp.push_back(lvl3q1[k]);
                                                    SortByPriority(Temp);
                                                    }
                                            }
                                            else if(counter1 == 1)
                                            {
                                                if(i == lvl3q1[k].get_arrival())
                                                {
                                                    ProcsQueue.push_back(lvl3q1[k]);
                                                }
                                            }
                                        }

                                        if(Temp.empty() != 1)
                                        {
                                            for(int k=0; k < Temp.size(); j++)
                                            {
                                                ProcsQueue.push_back(Temp[j]);
                                            }
                                        }
                                        Temp.clear();

                                        for(int k=0; k< AllProcs.size();k++)
                                        {
                                            if(i == AllProcs[k].get_arrival())
                                            {
                                                if( AllProcs[k].get_priority() >0 && AllProcs[k].get_priority() < 3)
                                                {
                                                    lvl3q1.push_back(AllProcs[k]);
                                                    //cout << AllProcs[k].get_procNum() << "Has been pushed in q1.\n";
                                                }
                                                else if(AllProcs[k].get_priority() > 2 && AllProcs[k].get_priority() < 5 )
                                                {
                                                    lvl3q2.push_back(AllProcs[k]);
                                                    //cout << AllProcs[k].get_procNum() << "Has been pushed in q2.\n";
                                                }
                                                else
                                                {
                                                    lvl3q3.push_back(AllProcs[k]);
                                                    //cout << AllProcs[k].get_procNum() << "Has been pushed in q3.\n";
                                                }
                                            }
                                        }

                                        for(int k=0; k< lvl3q1.size() ; k++)  //total processes
                                        {
                                            if ( i == lvl3q1[k].get_arrival() )
                                            {
                                                counter1++;
                                                //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                                //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                            }
                                        }
                                        if(i == 0)
                                        {
                                            //cout << counter1 << "counter\n";
                                        }

                                        for(int k=0; k < lvl3q1.size() ; k++)
                                        {
                                            if(counter1> 1)
                                            {
                                                if(i == lvl3q1[k].get_arrival())
                                                    {
                                                    Temp.push_back(lvl3q1[j]);
                                                    SortByPriority(Temp);
                                                    }
                                            }
                                            else if(counter1 == 1)
                                            {
                                                if(i == lvl3q1[k].get_arrival())
                                                {
                                                    ProcsQueue.push_back(lvl3q1[k]);
                                                }
                                            }
                                        }
                                        if(Temp.empty() != 1)
                                        {
                                            for(int k=0; k < Temp.size(); k++)
                                            {
                                                ProcsQueue.push_back(Temp[k]);
                                            }
                                        }
                                        Temp.clear();

                                        //cout << "Queue 1 size: " << lvl3q1.size() << endl;
                                        //cout << "Queue 2 size: " << lvl3q2.size() << endl;
                                        //cout << "Queue 3 size: " << lvl3q3.size() << endl;
                                    }
                                }
                            }
                            //cout << "im stuck here!!" << i  << endl;

                            if(ProcsQueue[0].get_burst() == 0 )   //Check when the process is finished
                            {
                                for (int k =0; k < AllProcs.size() ; k++)
                                {
                                    if(ProcsQueue[0].get_procNum() == AllProcs[k].get_procNum())
                                    {
                                        AllProcs[k].set_terminate(i+1);   //record terminate time
                                    }
                                    currentProc = ProcsQueue[0].get_procNum();

                                if(lastProc != currentProc && lastProc!= 0)
                                {
                                    for (int k =0; k < AllProcs.size() ; k++)
                                    {
                                        if(lastProc == AllProcs[k].get_procNum())
                                        {
                                            AllProcs[k].set_interrupt(i);
                                        }
                                    }
                                }
                                if(lastProc != currentProc)
                                {
                                    for (int k =0; k < AllProcs.size() ; k++)
                                    {
                                        if(currentProc == AllProcs[k].get_procNum())
                                        {
                                            AllProcs[k].set_execute_time(i);
                                        }
                                    }
                                }
                                lastProc = currentProc;
                                }
                                for (int j = 0; j < lvl3q1.size(); j++ )
                                {
                                    if (ProcsQueue[0].get_procNum() == lvl3q1[j].get_procNum())
                                    {
                                        lvl3q1.erase(lvl3q1.begin()+j);
                                    }
                                }
                                ProcsQueue.erase(ProcsQueue.begin());   //remove from queue

                                curdeleted = 1;
                            }
                            if(curdeleted != 1)
                            {
                                CycleVector(ProcsQueue);
                            }
                            curdeleted =0;
                            //cout << "this is i: " << i << endl;
                            //cout << "Loop completed" <<endl;
                        }
    //end of round robin coding
    //----------------------------------------------------------------------------------------------
    //2nd and 3rd queue FCFS
                        else if((lvl3q1.empty()) && (!(lvl3q2.empty())))
                        {
                            SortByPriority(lvl3q2);
                            lvl3q2[0].deduct_burst();

                            /*for(int j=0; j< lvl3q2.size() ; j++)  //total processes
                            {
                                ProcsQueue.push_back(lvl3q2[j]); //puts process into queue
                                //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                            }*/
                            if(lvl3q2[0].get_burst() == 0 )   //Check when the process is finished
                            {
                                for (int j=0; j < AllProcs.size() ; j++)
                                {
                                    if(lvl3q2[0].get_procNum() == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_terminate(i+1);   //record terminate time
                                    }
                                }
                                /*for (int j = 0; j < lvl3q2.size(); j++ )
                                {
                                    if (ProcsQueue[0].get_procNum() == lvl3q2[j].get_procNum()){
                                        lvl3q2.erase(lvl3q2.begin()+j);
                                    }
                                }*/
                                lvl3q2.erase(lvl3q2.begin());   //remove from queue
                            }

                            SortByPriority(lvl3q2);

                            //lvl3q2[0].deduct_burst();
                            currentProc = lvl3q2[0].get_procNum();

                            if(lastProc != currentProc && lastProc!= 0)
                            {
                                for (int j =0; j < AllProcs.size() ; j++)
                                {
                                    if(lastProc == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_interrupt(i);
                                    }
                                }
                            }
                            if(lastProc != currentProc)
                            {
                                for (int j =0; j < AllProcs.size() ; j++)
                                {
                                    if(currentProc == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_execute_time(i);
                                    }
                                }
                            }
                            lastProc = currentProc;
                            i++;

                            for(int k=0; k< AllProcs.size();k++)
                            {
                                if(i == AllProcs[k].get_arrival())
                                {
                                    if( AllProcs[k].get_priority() >0 && AllProcs[k].get_priority() < 3)
                                    {
                                        lvl3q1.push_back(AllProcs[k]);
                                        //cout << AllProcs[k].get_procNum() << "Has been pushed in q1.\n";
                                    }
                                    else if(AllProcs[k].get_priority() > 2 && AllProcs[k].get_priority() < 5 )
                                    {
                                        lvl3q2.push_back(AllProcs[k]);
                                        //cout << AllProcs[k].get_procNum() << "Has been pushed in q2.\n";
                                    }
                                    else
                                    {
                                        lvl3q3.push_back(AllProcs[k]);
                                        //cout << AllProcs[k].get_procNum() << "Has been pushed in q3.\n";
                                    }
                                }
                            }

                            for(int k=0; k< lvl3q1.size() ; k++)  //total processes
                            {
                                if ( i == lvl3q1[k].get_arrival() )
                                {
                                    counter1++;
                                    //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                    //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                }
                            }
                            if(i == 0)
                            {
                                //cout << counter1 << "counter\n";
                            }

                            for(int k=0; k < lvl3q1.size() ; k++)
                            {
                                if(counter1> 1)
                                {
                                    if(i == lvl3q1[k].get_arrival())
                                    {
                                        Temp.push_back(lvl3q1[k]);
                                        SortByPriority(Temp);
                                    }
                                }
                                else if(counter1 == 1)
                                {
                                    if(i == lvl3q1[k].get_arrival())
                                    {
                                        ProcsQueue.push_back(lvl3q1[k]);
                                    }
                                }
                            }
                            if(Temp.empty() != 1)
                            {
                                for(int k=0; k < Temp.size(); k++)
                                {
                                    ProcsQueue.push_back(Temp[k]);
                                }
                            }
                            Temp.clear();
                            //cout << "The time is: " << i << endl;
                            //cout << "Process Number: "<< lvl3q2[0].get_procNum() <<endl;
                            //cout << "Burst time remaining  " << lvl3q2[0].get_burst() << endl;
                            //cout << "Queue 1 size: " << lvl3q1.size() << endl;
                            //cout << "Queue 2 size: " << lvl3q2.size() << endl;
                            //cout << "Queue 3 size: " << lvl3q3.size() << endl;
                        }
    //------------------------------------------------------------------------------------------------------------------Third queue

                        else if((lvl3q1.empty()) && (lvl3q2.empty()) && (!(lvl3q3.empty())) )
                        {
                            SortByPriority(lvl3q3);
                            lvl3q3[0].deduct_burst();


                            /*for(int j=0; j< lvl3q2.size() ; j++)  //total processes
                            {
                                ProcsQueue.push_back(lvl3q2[j]); //puts process into queue
                                //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                            }*/
                            if(lvl3q3[0].get_burst() == 0 )   //Check when the process is finished
                            {
                                for (int j=0; j < AllProcs.size() ; j++)
                                {
                                    if(lvl3q3[0].get_procNum() == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_terminate(i+1);   //record terminate time
                                    }
                                }
                                /*for (int j = 0; j < lvl3q2.size(); j++ )
                                {
                                    if (ProcsQueue[0].get_procNum() == lvl3q2[j].get_procNum()){
                                        lvl3q2.erase(lvl3q2.begin()+j);
                                    }
                                }*/
                                lvl3q3.erase(lvl3q3.begin());   //remove from queue
                            }

                            SortByPriority(lvl3q3);

                            //lvl3q3[0].deduct_burst();
                            currentProc = lvl3q3[0].get_procNum();

                            if(lastProc != currentProc && lastProc!= 0)
                            {
                                for (int j =0; j < AllProcs.size() ; j++)
                                {
                                    if(lastProc == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_interrupt(i);
                                    }
                                }
                            }
                            if(lastProc != currentProc)
                            {
                                for (int j =0; j < AllProcs.size() ; j++)
                                {
                                    if(currentProc == AllProcs[j].get_procNum())
                                    {
                                        AllProcs[j].set_execute_time(i);
                                    }
                                }
                            }
                            lastProc = currentProc;
                            i++;
                            for(int k=0; k< AllProcs.size();k++)
                            {
                                if(i == AllProcs[k].get_arrival())
                                {
                                    if( AllProcs[k].get_priority() >0 && AllProcs[k].get_priority() < 3)
                                    {
                                        lvl3q1.push_back(AllProcs[k]);
                                        //cout << AllProcs[k].get_procNum() << "Has been pushed in q1.\n";
                                    }
                                    else if(AllProcs[k].get_priority() > 2 && AllProcs[k].get_priority() < 5 )
                                    {
                                        lvl3q2.push_back(AllProcs[k]);
                                        //cout << AllProcs[k].get_procNum() << "Has been pushed in q2.\n";
                                    }
                                    else
                                    {
                                        lvl3q3.push_back(AllProcs[k]);
                                        //cout << AllProcs[k].get_procNum() << "Has been pushed in q3.\n";
                                    }
                                }
                            }

                            for(int k=0; k< lvl3q1.size() ; k++)  //total processes
                            {
                                if ( i == lvl3q1[k].get_arrival() )
                                {
                                    counter1++;
                                    //ProcsQueue.push_back(AllProcs[j]); //puts process into queue
                                    //cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                                }
                            }
                            if(i == 0)
                            {
                                //cout << counter1 << "counter\n";
                            }

                            for(int k=0; k < lvl3q1.size() ; k++)
                            {
                                if(counter1> 1)
                                {
                                    if(i == lvl3q1[k].get_arrival())
                                    {
                                        Temp.push_back(lvl3q1[k]);
                                        SortByPriority(Temp);
                                    }
                                }
                                else if(counter1 == 1)
                                {
                                    if(i == lvl3q1[k].get_arrival())
                                    {
                                        ProcsQueue.push_back(lvl3q1[k]);
                                    }
                                }
                            }
                            if(Temp.empty() != 1)
                            {
                                for(int k=0; k < Temp.size(); k++)
                                {
                                    ProcsQueue.push_back(Temp[k]);
                                }
                            }
                            Temp.clear();
                            //cout << "The time is: " << i << endl;
                            //cout << "Process Number: "<< lvl3q3[0].get_procNum() <<endl;
                            //cout << "Burst time remaining  " << lvl3q3[0].get_burst() << endl;
                            //cout << "Queue 1 size: " << lvl3q1.size() << endl;
                            //cout << "Queue 2 size: " << lvl3q2.size() << endl;
                            //cout << "Queue 3 size: " << lvl3q3.size() << endl;
                        }
                    }
    /*
                    for(int i=0;i<AllProcs.size();i++)
                    {
                        vector<int> temp = AllProcs[i].get_interrupts();
                        vector<int> temp2 = AllProcs[i].get_execute_time();

                        for(int j=0; j <temp.size(); j++)
                        {
                            cout << "Process "<< AllProcs[i].get_procNum()<< " interrupt time = " << temp[j] << endl;
                        }

                        for(int j=0; j <temp2.size(); j++)
                        {
                            cout << "Process "<< AllProcs[i].get_procNum()<< " Exceute time = " << temp2[j] << endl;
                        }

                        cout << "Process "<< AllProcs[i].get_procNum() << " Terminate time = "  <<AllProcs[i].get_terminate_time() <<"\n\n";

                    }
    */
                    PrintGanttChart(AllProcs, AllProcs.size());
                    calcTT(AllProcs, lvl3_TT);
                    calcWT(AllProcs, lvl3_TT, lvl3_WT);
                    for(int i=0; i < lvl3_TT.size(); i++)
                    {
                        cout << "Process" << i+1 << " Turnaround Time = " << lvl3_TT[i] << ", Waiting Time =" << lvl3_WT[i] <<endl;
                    }
                    TotalTT=0;
                    TotalWT=0;
                    for (int i=0; i < lvl3_TT.size(); i++)
                    {
                        TotalTT = TotalTT + lvl3_TT[i];
                    }

                    avgTT = double(TotalTT)/double(lvl3_TT.size());
                    lvl3avgTT = avgTT;

                    cout << "The Average Turnaround Time for FCFS (First Come First Served)-based pre-emptive Priority:" << lvl3avgTT << endl;
                    for (int i=0; i < lvl3_WT.size(); i++)
                    {
                        TotalWT = TotalWT + lvl3_WT[i];
                    }
                    avgWT= double(TotalWT)/double(lvl3_WT.size());
                    lvl3avgWT = avgWT;

                    cout << "The Average Turnaround Time for FCFS (First Come First Served)-based pre-emptive Priority:" << lvl3avgWT << endl;

                    for (int i = 0; i < lvl3q3.size(); i++){
                        cout << lvl3q3[i].get_procNum() << " Process Number" << endl;
                        cout << lvl3q3[i].get_burst() << " Burst time remaining" << endl;
                    }





                    break;
                }
            case 4:
                {

                    system("cls");
                        cout << "_________________________________________________ " << endl
                             << "            *** Preemptive SJF *** "                << endl
                             << "_________________________________________________ " << endl;

                    /*AllProcs.push_back(process(1,0,12));
                    AllProcs.push_back(process(2,3,2));
                    AllProcs.push_back(process(3,4,3));
                    AllProcs.push_back(process(4,6,1));
                    AllProcs.push_back(process(5,8,5));*/

                    /*AllProcs.push_back(process(1, 0, 8, 2));
                    AllProcs.push_back(process(2, 4, 15, 5));
                    AllProcs.push_back(process(3, 7, 9, 3));
                    AllProcs.push_back(process(4, 13, 5, 1));
                    AllProcs.push_back(process(5, 9, 13, 4));
                    AllProcs.push_back(process(6, 0, 6, 1));*/

                    cout << "How many processes would you like?"<<endl;
                    cin >> numbOfProcess;


                    getProcesses(AllProcs, backup, numbOfProcess);


                    for(int i = 0; i < AllProcs.size(); i++)  //GET TOTAL BURST TIME
                    {
                        timeline = timeline + AllProcs[i].get_burst();
                    }

                    for(int i=0;i<timeline+1;i++)   //total burst time
                    {
                        for(int j=0; j< AllProcs.size() ; j++)  //total processes
                        {
                            if ( i == AllProcs[j].get_arrival() )
                            {
                                ProcsQueue.push_back(AllProcs[j]); //puts process into queue

                                cout << "process " << AllProcs[j].get_procNum() << " is found at time : " << i <<endl;
                            }
                        }

                        if(ProcsQueue[0].get_burst() ==0 )   //Check when the process is finished
                        {
                            for (int j =0; j < AllProcs.size() ; j++)
                            {
                                if(ProcsQueue[0].get_procNum() == AllProcs[j].get_procNum())
                                {
                                    AllProcs[j].set_terminate(i);   //record terminate time
                                }
                            }
                            ProcsQueue.erase(ProcsQueue.begin());   //remove from queue
                        }

                        sort(ProcsQueue.begin(),ProcsQueue.end());  //Sort queue by burst time

                        ProcsQueue[0].deduct_burst();
                        currentProc = ProcsQueue[0].get_procNum();

                        if(lastProc != currentProc && lastProc!= 0)
                        {
                            for (int j =0; j < AllProcs.size() ; j++)
                            {
                                if(lastProc == AllProcs[j].get_procNum())
                                {
                                    AllProcs[j].set_interrupt(i);
                                }
                            }
                        }
                        if(lastProc != currentProc)
                        {
                            for (int j =0; j < AllProcs.size() ; j++)
                            {
                                if(currentProc == AllProcs[j].get_procNum())
                                {
                                    AllProcs[j].set_execute_time(i);
                                }
                            }
                        }
                        lastProc = currentProc;
                    }/*

                    for(int i=0;i<AllProcs.size();i++)
                    {
                        vector<int> temp = AllProcs[i].get_interrupts();
                        vector<int> temp2 = AllProcs[i].get_execute_time();

                        for(int j=0; j <temp.size(); j++)
                        {
                            cout << "Process "<< AllProcs[i].get_procNum()<< " interrupt time = " << temp[j] << endl;
                        }

                        for(int j=0; j <temp2.size(); j++)
                        {
                            cout << "Process "<< AllProcs[i].get_procNum()<< " Exceute time = " << temp2[j] << endl;
                        }

                        cout << "Process "<< AllProcs[i].get_procNum() << " Terminate time = "  <<AllProcs[i].get_terminate_time() <<"\n\n";

                    }*/
                    PrintGanttChart(AllProcs, AllProcs.size());
                    calcTT(AllProcs, SJF_TT);
                    calcWT(AllProcs, SJF_TT, SJF_WT);
                    for(int i=0; i < SJF_TT.size(); i++)
                    {
                        cout << "Process" << i+1 << " Turnaround Time = " << SJF_TT[i] << ", Waiting Time =" << SJF_WT[i] <<endl;
                    }
                    TotalTT=0;
                    TotalWT=0;
                    for (int i=0; i < SJF_TT.size(); i++)
                    {
                        TotalTT = TotalTT + SJF_TT[i];
                    }

                    avgTT = double(TotalTT)/double(SJF_TT.size());

                    cout << "The Average Turnaround Time for Pre-emptive SJF(Shortest Job First) scheduling:" << avgTT << endl;
                    for (int i=0; i < SJF_WT.size(); i++)
                    {
                        TotalWT = TotalWT + SJF_WT[i];
                    }
                    avgWT= double(TotalWT)/double(SJF_WT.size());
                    cout << "The Average Waiting Time for Pre-emptive SJF(Shortest Job First) scheduling:" << avgWT << endl;

                    break;
                }

            case 5:
                {
                    loop = false;
                    break;
                }
            default:
                {


                }

        }
    }while(loop==true);
    return 0;
}



