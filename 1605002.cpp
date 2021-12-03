#include <bits/stdc++.h>
using namespace std;

const double INF = 1e99;

enum event_type {ARRIVAL, DEPARTURE};
enum server_status_type {IDLE, BUSY};

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

/*
    We first generate a random real number u in [0, 1] from uniform distribution.
    Then we turn that into an exponential distribution sample by taking -mean * ln(u).
    "mean" is the mean of the desired exponential distribution.
*/
double get_exponential_sample(double mean)
{
    double u = uniform_real_distribution<double>(0, 1)(rng);
    return -mean * log(u);
}

int get_bt(server_status_type server_status)
{
    return server_status == BUSY? 1: 0;
}

class SSQS
{
public:
    const double mean_interval_time;
    const double mean_service_time;
    const int number_of_customers;

    // system states
    server_status_type server_status;
    int number_in_queue;
    queue<double> times_of_arrival;
    double time_of_last_event;
    double clock;
    double next_A;
    double next_D;

    // statistical counters
    int number_delayed;
    double total_delay;
    double area_under_qt;
    double area_under_bt;

    SSQS(double mean_interval_time,
        double mean_service_time,
        double number_of_customers)
    : mean_interval_time(mean_interval_time),
      mean_service_time(mean_service_time),
      number_of_customers(number_of_customers)
    {}

    void initialize()
    {
        server_status = IDLE;
        number_in_queue = 0;
        time_of_last_event = 0;
        clock = 0;
        next_A = get_exponential_sample(mean_interval_time); // first customer arrival time
        next_D = INF; // no customer is currently being served, hence next departure time is infinity

        number_delayed = 0;
        total_delay = 0;
        area_under_qt = 0;
        area_under_bt = 0;
    }

    event_type get_next_event_type()
    {
        if(next_A < next_D) return ARRIVAL; // next customer arrival is coming before current customer finishes being served
        else return DEPARTURE;
    }

    void simulate()
    {
        initialize();

        while(number_delayed < number_of_customers)
        {
            event_type next_event_type = get_next_event_type();

            if(next_event_type == ARRIVAL)
            {
                clock = next_A;
                next_A += get_exponential_sample(mean_interval_time); // scheduling next arrival event

                area_under_qt += (clock-time_of_last_event) * number_in_queue;
                area_under_bt += (clock-time_of_last_event) * get_bt(server_status);

                if(server_status == BUSY)
                {
                    // server is busy, so current customer will have to wait in the queue
                    number_in_queue++; 
                    times_of_arrival.push(clock);
                }
                else
                {
                    // this customer will face no delay
                    double delay = 0;

                    total_delay += delay;
                    number_delayed++;

                    // server is going to be busy serving this customer
                    server_status = BUSY;
                    next_D = clock + get_exponential_sample(mean_service_time);
                }
            }
            else
            {

                clock = next_D;

                area_under_qt += (clock-time_of_last_event) * number_in_queue;
                area_under_bt += (clock-time_of_last_event) * get_bt(server_status);

                // no customer is waiting in the queue, so servle will be idle and no next departure event
                if(times_of_arrival.empty())
                {
                    server_status = IDLE;
                    next_D = INF;
                }
                else
                {
                    number_in_queue--;

                    double current_arrival_time = times_of_arrival.front();
                    times_of_arrival.pop();

                    double delay = clock-current_arrival_time;

                    total_delay += delay;
                    number_delayed++;

                    next_D = clock+get_exponential_sample(mean_service_time);
                }
            }

            time_of_last_event = clock;
        }
    }

    double get_average_delay()
    {
        return total_delay/number_of_customers;
    }

    double get_average_number_in_queue()
    {
        return area_under_qt/clock;
    }

    double get_server_utilization()
    {
        return area_under_bt/clock;
    }

    double get_final_time()
    {
        return clock;
    }
};

void task_a()
{
    double mean_interval_time = 1;
    double mean_service_time = 0.5;
    int number_of_customers = 1000;

    SSQS ssqs(mean_interval_time, mean_service_time, number_of_customers);

    ssqs.simulate();

    ofstream fout("output_a.txt");

    fout << "Single-server queuing system" << endl;
    fout << "Mean interval time = " << mean_interval_time << endl;
    fout << "Mean service time = " << mean_service_time << endl;
    fout << "Number of customers = " << number_of_customers << endl << endl << endl;

    fout << "Average delay in queue = " << ssqs.get_average_delay() << endl;
    fout << "Average number in queue = " << ssqs.get_average_number_in_queue() << endl;
    fout << "Server utilization = " << ssqs.get_server_utilization() << endl;
    fout << "Time simulation ended = " << ssqs.get_final_time() << endl;
}

int main()
{
    task_a();

    return 0;
}