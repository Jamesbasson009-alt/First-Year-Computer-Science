//26080975
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define MAX_COUNT  5

void parentProcess(void)
{
    for(int i = 0; i < MAX_COUNT; i++){
        std::cout << "Parent process executing" << std::endl;
        sleep(1);
    }
}

void childProcess(void)
{
     for(int i = 0; i < MAX_COUNT; i++){
        std::cout << "Child process executing" << std::endl;
        sleep(1);
    }
}

int main(int argc, char* argv[])
{
    pid_t pid = fork();

    if (pid == 0) {
        childProcess();
        exit(0);

    } else {
        parentProcess();
        wait(NULL);
    }
}