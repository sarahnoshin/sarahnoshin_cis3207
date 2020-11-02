In the beginning of this project I first decided to use three queues for the CPU, disk 1 and disk 2 components of the system. The fourth queue that is the priority queue that keeps tracks of events will be part of the POP method of the program.  I also chose to make a struct that contains all the variables from the text file because those variables will be called multiple times throughout the program.Each queue will have similar variables such as the arrival, the execution and the ability to get the address of the next node. Also each queue must be initialed to start with NULL. Additionally there must be a Push method which will push any job entering any queue from the back of the queue while the pop method will pop out the job from the front of the queue.
These were the top things I knew must be included in my program. Starting from the main function I chose to use srand in order to get a new time for every run so that in my testing phase I can check for errors easily. I chose to program for the right amount of input files and the program will exit if the text file is not compiled. The first job will be created in the CPU as well as a random time for its arrival and then the next time jobs arrival will also be calculated. I decided in order to keep track of the time, I will create a for loop that will start at time 1 and run until the finish time with an incrementation of one. While the for loop is running the system will do various amounts of tasks using if- else statements. First it will execute the job in the CPU and then check if the job will exit the program or it will go one of the disks. Then both disk 1 and disk 2 will be compared to see which queue is smaller and the job will go to the queue that's smaller. If both of the queues are the same size then one is chosen. As soon as the job is done being processed in the system, the job will be using the POP method described previously to pop off the job from the top of the queue and the next job will be incremented. I also implemented an if else statement for any time the CPU may be idle.
Because the main method called for so many events to be randomized,  I decided to create several other methods that may be helpful to call throughout the program. For example, if the job needs to exit after being in the CPU, a randomized method was created. Rand was also used if disk 1 and disk 2 has the same length for the queue. Also since there were so many variables between a certain min and max range, a method called random within range was also created to help simplify the code in the program.
I also created a method called configure to read the text file. First it will open the file as well as check if the file exists. Then it read each value in the file until all the values are read. There are three PUSH methods for each of the three components as well. The queue will allocate memory, calculate the arrival of the job and the arrival of the next job and increase the size of the queue. Just like the three PUSH methods, there are three POP methods for each component. Each method is similar because I decided it will be easy to check if there is anything in the component and then pop it off and decrement by one since the job is now out of the queue.