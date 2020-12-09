#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;
#define SEPARATOR "#<ab@17943918#@>#"
class JobNode
{
public:
    JobNode(int birth_time, int remained_time, string name)
    {
        this->birth_time = birth_time;
        this->remained_time = remained_time;
        prev = next = NULL;
        this->name = name;
    };
    int birth_time; // the time the job appears
    int remained_time; // the remaining time to finish the job
    string name; // the name of the job
    JobNode* prev; // pointer to previous node
    JobNode* next; // pointer to next node
};

enum Order   // two sorting orders of jobs in the waiting list
{
    By_Birth_Time,
    By_Remaining_Time
};

enum Policy
{
    Least_Waiting_Time_First,
    Shortest_List_First
};

template <class T>
class DList
{
public:
    DList(); // constructor
    void insert(T& node, int order); // insert the node T into the list according to the specified order (either By_Birth_Time or By_Remaining_Time)
    void insert(T& node, T* at); // insert the node T before the specified node (at)
    bool del(T* nodep); // delete the specified node from the list
    void clear(); // clear the whole list
    int length(); // return the number of jobs in the list
    T* first; // pointer to the 1st node of the list
    T* last; // pointer to the last node of the list
};

template <class T>
DList<T> :: DList()
{
    first = NULL;
    last = NULL;
}

template <class T>
void DList<T> :: insert(T& node, T* at)
{
    cout<<"into insert at" << endl;
    if(at == first)
    {

        node.next = at;
        node.next -> prev = &node;
        first = &node;
        cout << "hi" << endl;
    }
    else
    {
        node.prev = at -> prev;
        node.next = at;
        node.prev -> next = &node;
        node.next -> prev = &node;
        cout << "hello" << endl;
    }
}

template <class T>
void DList<T> :: insert(T& node, int order)
{
    cout << "into insert order" << endl;
    if(length() == 0)
    {
        first = &node;
        last = &node;
        cout << "length = 0" << endl;
    }
    else
    {
        T *att = first;
        while (att != NULL)
        {
            if (order == 1)
            {
                if (att->remained_time > node.remained_time)
                {
                    insert(node, att);
                    cout << "length != 0" << endl;
                    break;
                }
            }
            else if (order == 0)
            {
                if (att->birth_time > node.birth_time)
                {
                    insert(node, att);
                    cout << "length != 0" << endl;
                    break;
                }
            }
        }
        att = att -> next;

        if(att == NULL)
        {
            last -> next = &node;
            node.prev = last;
            last = last -> next;
            cout << "att == NULL" << endl;
        }
    }
}

template <class T>
bool DList<T> :: del(T* nodep)
{
    if(first == NULL)
        return false;
    else
    {
        if(nodep == first)
            first = first -> next;
        if(nodep == last)
            last = last -> prev;
        if((nodep != first) &&(nodep != last))
        {
            nodep -> prev -> next = nodep -> next;
            nodep -> next -> prev = nodep -> prev;
        }
        return true;
    }
}

template <class T>
void DList<T> :: clear()
{
    while(length()!=0)
        del(first);
}

template <class T>
int DList<T> :: length()
{
    if((first == NULL) && (last == NULL))
    {
        cout << "length = 0" << endl;
        return 0;
    }
    int cnt = 0;
    if((first == last) && (first != NULL))
        return 1;
    else if(first != last)
    {
        T* counter = last;

        while (counter -> prev != NULL)
        {
            cnt ++;
            counter = counter -> prev;
        }
        cnt ++;
    }
    return cnt;
}

class Manager
{
public:
    Manager(int n_serv); // constructor to prepare the specified number (n_serv) of waiting lists for the CPUs
    int dispatch(JobNode* j, int order); // dispatch the given job (j) according to the specified sorting order of waiting jobs
    void process(int cpu_id, int time_slice); // emulate the serving of jobs on the waiting list of the specified CPU (cpu_id) with the given length for a time_slice
    void showLists(); // display the jobs in all lists
    int n_servers; // the number of waiting lists
    DList<JobNode> *servers; // an array of waiting lists for all CPUs
};

Manager :: Manager(int n_serv)
{
    n_servers = n_serv;
    servers = new DList<JobNode> [n_servers];
}

/*int Manager :: dispatch(JobNode *j, int order)
{
    if(order == 1)
    {
        int mintime = 9999;
        int rematotal[n_servers];
        for(int i = 0; i < n_servers; i ++)
        {
            rematotal[i] = 0;
            JobNode* ffirst = servers[i].first;
            while(ffirst != NULL)
            {
                rematotal[i] += ffirst -> remained_time;
                ffirst = ffirst -> next;
            }
            if(rematotal[i] < mintime)
            {
                mintime = rematotal[i];
            }
        }
        for(int i = 0; i < n_servers; i ++)
        {
            if(rematotal[i] == mintime)
            {
                JobNode* first = servers[i].first;
                if(j->remained_time < first->remained_time)
                {
                    servers[i].insert(j, first);
                }
                else if(first->next != NULL)
                {

                }
            }
        }

    }
    else if(order == 0)
    {

    }
}*/

template <class T>
ostream& operator <<(ostream& os, DList<T> dl)
{
    JobNode* p = dl.first;
    if (dl.length() == 0)
        cout << "Empty list.";
    while (p)
    {
        cout << "-[" << p->name << ","<< p->birth_time  << "," << p->remained_time << "]";
        p = p->next;
    }
    cout << endl;
    return os;
}

JobNode* generateJob(int birth_time, string name)   // generate job with specified birth time and name
{
    int time_slice_period = 50;
    int remained_time = rand() % (2*time_slice_period); // randomly generating the job's complete time
    JobNode *jn = new JobNode(birth_time, remained_time, name); // create a job node
    return jn;
}

int main()
{
    /* JobNode* jn; //
     Manager* mgr = new Manager(10); // prepare 3 waiting lists for 3 CPUs
     ostringstream ss;
     // testing the operations on waiting lists
     for (int i = 0; i < 15; i++)   // generate randomly 15 jobs
     {
         ss.str("");
         ss << "t" << i;
         jn = new JobNode(rand()%20, rand()%10, ss.str());
         mgr->servers[0].insert(*jn, By_Remaining_Time); // send the job into the waiting list of CPU 0
     }
     cout << mgr->servers[0]; // display all waithing jobs for the waiting list
     mgr->servers[0].clear(); // clear the waiting list of CPU 0

     cout << mgr->servers[0];

     // testing the emulation of job generation, dispatching, and CPU serving
    */
    DList <JobNode> *dlist;
    int order = By_Remaining_Time;
    dlist -> insert(*(new JobNode(10, 10, "t1")), order);
    cout << dlist -> length() << endl;
    dlist -> insert(*(new JobNode(20, 20, "t2")), order);
    dlist -> insert(*(new JobNode(30, 30, "t3")), order);
    cout << dlist -> length() << endl;
    return 0;
}
