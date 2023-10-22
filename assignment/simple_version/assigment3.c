#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// Define the struct for shared memory
typedef struct
{
    int x;     // x
    int y;     // y
    int z;     // z = x + y
    int ready; // ready status
} SharedData;

int main()
{
    key_t key = ftok("/tmp", 'S');                                 // Generate a unique key for shared memory
    int shmid = shmget(key, sizeof(SharedData), 0666 | IPC_CREAT); // Get shared memory segment

    SharedData *shared_data = shmat(shmid, NULL, 0); // Attach shared memory segment

    shared_data->ready = 0;

    /***************************************************/

    pid_t child_pid = fork(); // Create a child process

    if (child_pid < 0)
    {
        perror("fork error.");
        exit(1);
    }
    else if (child_pid == 0)
    { // Child process
        while (shared_data->ready != 1)
        {
            // Wait for the parent to set 'ready' to 1
            sleep(1);
        }

        shared_data->z = shared_data->x + shared_data->y;
        shared_data->ready = 0;

        shmdt(shared_data); // Detach shared memory segment
        exit(0);
    }
    else
    { // Parent process
        printf("x = ");
        scanf("%d", &shared_data->x);
        printf("y = ");
        scanf("%d", &shared_data->y);

        shared_data->ready = 1;

        while (shared_data->ready != 0)
        {
            // Wait for the child to set 'ready' back to 0
            sleep(1);
        }

        printf("z = %d\n", shared_data->z);

        shmdt(shared_data);
        shmctl(shmid, IPC_RMID, NULL); // Remove shared memory segment
    }

    return 0;
}
