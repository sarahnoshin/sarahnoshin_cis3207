//importing all the required library
#include<iostream>
#include<fstream>
#include <cstdlib>
#include <bits/stdc++.h>
#include <string>
#include <sstream> 
#include<time.h> 

using namespace std;

string out="";

//function to append output log file string

void appendString(string data) 
{ 
    
//    out.append("\n"); 
//    out.append(data);
//cout << data<<endl;
out = out+data;

    //cout << "-------------->>>>>Size of string variable: " << sizeof(out) << endl;
    //cout << out;
    return;
} 

//function to append integer to log file 

void int2string(int data)
{
string send;
ostringstream str1; 
str1 << data; 
   	send = str1.str(); 
   	appendString(send);
   	return;
}

// Structure to store all system configuration
struct Configuration
{
int init_time, fin_time, arrive_min, arrive_max;
int quit_prob, cpu_min, cpu_max;
int disk1_min, disk1_max, disk2_min, disk2_max;
} configuration;

//probability function for exiting a programme
bool exit_decision(){

bool TrueFalse = (rand() % 100) < configuration.quit_prob;

return(TrueFalse);
}

//probability function for choosing a disk if both que are equal 
bool disk_decision(){

bool TrueFalse = (rand() % 100) < 50;

return(TrueFalse);
}

//probability function for choosing if a new job arrives or not after every clock 
bool job_decision(){

bool TrueFalse = (rand() % 100) < 1;

return(TrueFalse);
}

//genrating a random number in a range

int random_within_range(int minimum_number, int maximum_number) {

const int new_number = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
return(new_number);

}




//variables for QUE size and clock
int cpu_size = 0, disk1_size = 0, disk2_size = 0;
int max_cpu =0, max_disk1 = 0, max_disk2 = 0;

// counters
int count_cpu =0, count_disk1 = 0, count_disk2 = 0;

//utilization counter
int utilize_cpu =0, utilize_disk1 = 0, utilize_disk2 = 0;

//variables for total job
int total =0;

// QUE for cpu processing

struct CPU 
{
    int arrival, execution;    //Data of the node
    struct CPU *nextptr;                  //Address of the next node
};


// QUE for DISK1 processing

struct disk1 
{
    int execution,job;    //Data of the node
    struct disk1 *nextptr;                  //Address of the next node
};


// QUE for DISK1 processing

struct disk2 
{
    int execution, job;    //Data of the node
    struct disk2 *nextptr;                  //Address of the next node
};

//defining starting point of each QUE as NULL
struct CPU *start_cpu = NULL; 
struct disk1 *start_disk1 = NULL; 
struct disk2 *start_disk2 = NULL; 


//creating PUSH functions for the three QUE

void create_CPU_QUE(int clk){
struct CPU *current, *temp;

if(start_cpu == NULL) //check whether the fnnode is NULL and if so no memory allocation
    {
    start_cpu = (struct CPU *)malloc(sizeof(struct CPU));
        start_cpu->arrival = clk +  random_within_range(configuration.arrive_min, configuration.arrive_max);   
start_cpu->execution = random_within_range(configuration.cpu_min,configuration.cpu_max);  
utilize_cpu = utilize_cpu + start_cpu->execution ; 
        start_cpu->nextptr = NULL;
        cpu_size++;
    }else{
    current = (struct CPU *)malloc(sizeof(struct CPU));
    current->execution = random_within_range(configuration.cpu_min,configuration.cpu_max);   
    utilize_cpu = utilize_cpu + start_cpu->execution ; 
    current->arrival = clk +  random_within_range(configuration.arrive_min, configuration.arrive_max);
        current->nextptr = NULL;
    temp = start_cpu;
    while( temp->nextptr != NULL ){
//    	cout<<"============>>>"<<temp->arrival<<endl;
    temp = temp->nextptr; 
}
temp->nextptr = current;
cpu_size++;
}
if (cpu_size>max_cpu)
max_cpu=cpu_size;

count_cpu++;
}

void create_disk1_QUE(int clk, int jb){
struct disk1 *current, *temp;

if(start_disk1 == NULL) //check whether the fnnode is NULL and if so no memory allocation
    {
        start_disk1 = (struct disk1 *)malloc(sizeof(struct disk1));
start_disk1->execution = clk + random_within_range(configuration.disk1_min,configuration.disk1_max);
utilize_disk1 = utilize_disk1 + start_disk1->execution - clk ; 
start_disk1->job = jb;   
        start_disk1->nextptr = NULL;
        disk1_size++;
    }else{
    current = (struct disk1 *)malloc(sizeof(struct disk1));
    current->execution = clk + random_within_range(configuration.disk1_min,configuration.disk1_max);    
utilize_disk1 = utilize_disk1 + start_disk1->execution - clk ;  
    start_disk1->job = jb;  
        current->nextptr = NULL;
    temp = start_disk1;
    while( temp->nextptr != NULL ){
    temp = temp->nextptr; 
}
temp->nextptr = current;
disk1_size++;
}
if (disk1_size>max_disk1)
max_disk1=disk1_size;

count_disk1++;
}

void create_disk2_QUE(int clk, int jb){
struct disk2 *current, *temp;

if(start_disk2 == NULL) //check whether the fnnode is NULL and if so no memory allocation
    {
        start_disk2 = (struct disk2 *)malloc(sizeof(struct disk2));
start_disk2->execution = clk + random_within_range(configuration.disk2_min,configuration.disk2_max); 
utilize_disk2 = utilize_disk2 + start_disk2->execution - clk ;   
        start_disk2->job = jb;
start_disk2->nextptr = NULL;
        disk2_size++;
    }else{
    current = (struct disk2 *)malloc(sizeof(struct disk2));
    current->execution = clk + random_within_range(configuration.disk2_min,configuration.disk2_max);     
        utilize_disk2 = utilize_disk2 + start_disk2->execution - clk ;
start_disk2->job = jb;
current->nextptr = NULL;
    temp = start_disk2;
    while( temp->nextptr != NULL ){
    temp = temp->nextptr; 
}
temp->nextptr = current;
disk2_size++;
}
if (disk2_size>max_disk2)
max_disk2=disk2_size;

count_disk2++;
}


//creating POP functions for the three QUE

void POP_CPU_QUE(){


if(cpu_size>1){
start_cpu = start_cpu->nextptr;
cpu_size--;
}else if( cpu_size == 1){
start_cpu = NULL;
cpu_size--;
}else{
cout<<"Nothing in the QUE to POP"<<endl;
}
return;
}


void POP_disk1_QUE(){

if(disk1_size>1){
start_disk1 = start_disk1->nextptr;
disk1_size--;
}else if( disk1_size == 1){

start_disk1 = NULL;
disk1_size--;
}else{
cout<<"Nothing in the QUE to POP"<<endl;
}
return;
}

void POP_disk2_QUE(){

if(disk2_size>1){
start_disk2 = start_disk2->nextptr;
disk2_size--;
}else if( disk2_size == 1){

start_disk2 = NULL;
disk2_size--;
}else{
cout<<"Nothing in the QUE to POP"<<endl;
}
return;
}



// function to read 
void configure(string fileName){

string line, temp;
ifstream inputFile;
inputFile.open(fileName.c_str());

int line_count = 0, tempInt;

//checking if the input file is present
if(!inputFile.good())
{
cout<<"File Not Found!!! EXITING programme ..."<<endl;
                exit(1);

}
cout<<endl<<"\n[Info] Opening "<<fileName<<" ..."<<endl<<endl;

while(getline(inputFile,line,'\n'))
{
//change the check value acccording to the line where the configurations are stored

if(line_count==2)
{	
if(line.length()>5)
{
cout<<"======================== SYSTEM CONFIGURATION ========================\n"<<endl;
appendString("======================== SYSTEM CONFIGURATION ========================\n");
stringstream stream(line);

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.init_time = tempInt;
cout<<"INITIALIZING time:\t"<<temp<<endl;

appendString("INITIALIZING time:\t\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.fin_time = tempInt;
cout<<"FINISHING time:\t\t"<<temp<<endl;

appendString("FINISHING time:\t\t\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.arrive_min = tempInt;
cout<<"Minimum ARRIVING Time:\t"<<temp<<endl;

appendString("Minimum ARRIVING Time:\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.arrive_max = tempInt;
cout<<"Maximum ARRIVING Time:\t"<<temp<<endl;

appendString("Maximum ARRIVING Time:\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.quit_prob = tempInt;
cout<<"Quiting Probability:\t"<<temp<<endl;

appendString("Quiting Probability:\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.cpu_min = tempInt;
cout<<"Min processing time:\t"<<temp<<endl;

appendString("Min processing time:\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.cpu_max = tempInt;
cout<<"Max processing time:\t"<<temp<<endl;

appendString("Max processing time:\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.disk1_min = tempInt;
cout<<"Minimum DISK 1 time:\t"<<temp<<endl;

appendString("Minimum DISK 1 time:\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.disk1_max = tempInt;
cout<<"Maximum DISK 1 time:\t"<<temp<<endl;

appendString("Maximum DISK 1 time:\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.disk2_min = tempInt;
cout<<"Minimum DISK 2 time:\t"<<temp<<endl;

appendString("Minimum DISK 2 time:\t");
appendString(temp);
appendString("\n");

getline(stream,temp,',');
tempInt = atoi(temp.c_str());

configuration.disk2_max = tempInt;
cout<<"Maximum DISK 2 time:\t"<<temp<<endl;

appendString("Maximum DISK 2 time:\t");
appendString(temp);
appendString("\n");


return;
}else
{
cout<<"\n Configuration file cannot be empty EXITING programme ...\n";	
exit(1);
}
}

line_count++;
}
}

//function to write log
void write_log(string data){
std::ofstream outfile;
outfile.open("log.txt", std::ios_base::app);
//	outfile << "Data\n"; 
outfile << data+"\n"; 
return ;
}

//function to clean previos log
void clean_log(){
ofstream myfile;
  myfile.open ("log.txt");
  myfile << "";
  myfile.close();
  return;
}


   

int main(int argc, char** argv){

// Use current time as seed for random generator 
srand(time(0)); 
clean_log();
ostringstream str1; 
//variable declration
int i, tempInt, start, job_count=0, currentJob, clock = 0, next_arrival_time;
int idle_flag=0;

// status of the components and false means they are idle
bool cpu_status=0, disk1_status=0, disk2_status=0;

// checking the number of arguments if it is not equal to 4 terminate the program
if(argc!=2)
{
cout<<"Wrong number of input files given"<<endl;
cout<<"Number of input files given is "<<argc-1<<". Number of input files reuired is 1"<<endl;
exit(1);
}

cout<<"[Info] Reading of configuration file initialized...."<<endl;

// setting the system configuration
configure(argv[1]);

//	random_within_range(configuration.cpu_min,configuration.cpu_max);
    
//	for(i=0;i<10;i++)
//	cout<<exit_decision()<<endl;

//next_job = random_within_range(configuration.arrive_min, configuration.arrive_max);

//creating the first job in cpu
create_CPU_QUE(0);
total++;
cpu_status = 1;
appendString("\n\n================= SYSTEM INITIALIZED =======================\n\n");
cout<<"JOB 1 started"<<endl;
appendString("JOB 1 started\n");
currentJob = random_within_range(configuration.arrive_min, configuration.arrive_max); 
next_arrival_time =  random_within_range(configuration.arrive_min, configuration.arrive_max);   
//program loop OR execution loop
for(clock=1;clock<configuration.fin_time;clock++){

//	cout<<start_cpu->arrival;


if(currentJob==0){
if(cpu_size>0){
//	cout<<"JOB: "<<total<<", completed at time => "<<clock<<endl;
//	cout<<"cpu_size: "<<cpu_size<<endl;

currentJob = start_cpu->execution;
next_arrival_time = start_cpu->arrival;
if(exit_decision()){
cout<<"JOB: "<<total<<", completed at time => "<<clock<<endl;
appendString("JOB: ");
int2string(total);
appendString(", completed at time => ");
int2string(clock);
appendString("\n");
}else{
//Saving to disk
cout<<"JOB: "<<total<<", completed at time => "<<clock<<". Saving the output ..."<<endl;

appendString("JOB: ");
int2string(total);
appendString(", completed at time => ");
int2string(clock);
appendString(".Saving the output ...");
appendString("\n");


//choosing the disk
if(disk1_size>disk2_size){
create_disk2_QUE(clock, job_count);
}else if(disk2_size>disk1_size){
create_disk1_QUE(clock, job_count);
}else{
//if both que are equal or zero then choose randomly
if(disk_decision()){
create_disk1_QUE(clock, job_count);
}else{
create_disk2_QUE(clock, job_count);
}
}
}
job_count++;
POP_CPU_QUE();
cpu_status = 1;
total++;
}else{
cpu_status = 0;
//cout<<"CPU is idle"<<endl;
}
}

if(cpu_status && clock>next_arrival_time){
currentJob--;
idle_flag=0;
}else{
if(idle_flag==0){
cout<<"CPU is idle"<<endl;
idle_flag = 1;
appendString("\nCPU is idle\n");
appendString("\n");
}

}


if(job_decision()){
create_CPU_QUE(clock);
}

if(disk1_size>0){
if(clock>start_disk1->execution){
cout<<"=> Output of JOB: "<<start_disk1->job<<" saved to DISK 1"<<endl;
appendString("=> Output of JOB: ");
int2string(start_disk1->job);
appendString(" saved to DISK 1");
appendString("\n");
POP_disk1_QUE();
}
}

if(disk2_size>0){
if(clock>start_disk2->execution){
cout<<"=> Output of JOB: "<<start_disk2->job<<" saved to DISK 2"<<endl;
appendString("=> Output of JOB: ");
int2string(start_disk2->job);
appendString(" saved to DISK 2");
appendString("\n");
POP_disk2_QUE();
}
}

}

appendString("\n\n======================== SYSTEM Statistics ========================\n");
appendString("# Total number of job done : ");
int2string(job_count);
appendString("\n");

appendString("# Average time taken per job  : ");
int2string((configuration.fin_time-configuration.init_time)/job_count );
appendString("\n");

appendString("# Maximum size of CPU que : ");
int2string(max_cpu);
appendString("\n");

appendString("# Maximum size of DISK1 que : ");
int2string(max_disk1);
appendString("\n");

appendString("# Maximum size of DISK2 que : ");
int2string(max_disk2);
appendString("\n");


appendString("# Average size of CPU que : ");
int2string(job_count/count_cpu);
appendString("\n");

appendString("# Average size of DISK1 que : ");
int2string(job_count/count_disk1);
appendString("\n");

appendString("# Average size of DISK2 que : ");
int2string(job_count/count_disk2);
appendString("\n");

//----------------------------

appendString("# Utilization of CPU que in cycles : ");
int2string(utilize_cpu);
appendString("\n");

appendString("# Utilization of DISK1 que in cycles : ");
int2string(utilize_disk1);
appendString("\n");

appendString("# Utilization of DISK2 que in cycles : ");
int2string(utilize_disk2);
appendString("\n");



    write_log(out);
    cout<<"\n\nTotal number of job done :"<<job_count<<endl;
    cout<<"Average time taken per job :"<< (configuration.fin_time-configuration.init_time)/job_count <<endl;
    return 0;
}