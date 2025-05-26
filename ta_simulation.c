#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_CHAIRS 5
#define NUM_STUDENTS 10

sem_t students_sem;
sem_t ta_sem;
pthread_mutex_t chairs_mutex;

int waiting_students = 0;

// Function for student thread
void* student_thread(void* arg) {
    int id = *(int*)arg;
    free(arg);
    while (1) {
        sleep(rand() % 5 + 1);  // student arrives after random time

        pthread_mutex_lock(&chairs_mutex);
        if (waiting_students < NUM_CHAIRS) {
            waiting_students++;
            printf("🪑 Student %d sits in waiting chair. (Waiting: %d)\n", id, waiting_students);
            sem_post(&students_sem); // notify TA that a student is waiting
            pthread_mutex_unlock(&chairs_mutex);

            sem_wait(&ta_sem); // wait for TA to help
            printf("✅ Student %d is getting help from TA.\n", id);
        } else {
            pthread_mutex_unlock(&chairs_mutex);
            printf("❌ Student %d found no empty chair and left.\n", id);
        }
    }
    return NULL;
}

// Function for TA thread
void* ta_thread(void* arg) {
    while (1) {
        sem_wait(&students_sem); // wait for a student to arrive

        pthread_mutex_lock(&chairs_mutex);
        waiting_students--;
        pthread_mutex_unlock(&chairs_mutex);

        printf("👨‍🏫 TA is helping a student...\n");
        sleep(3); // time spent helping
        sem_post(&ta_sem); // signal that help is done
    }
    return NULL;
}

// Main function
int main() {
    pthread_t ta;
    pthread_t students[NUM_STUDENTS];

    srand(time(NULL)); // initialize random seed

    // Initialize synchronization primitives
    sem_init(&students_sem, 0, 0);
    sem_init(&ta_sem, 0, 0);
    pthread_mutex_init(&chairs_mutex, NULL);

    // Create TA thread
    pthread_create(&ta, NULL, ta_thread, NULL);

    // Create student threads
    for (int i = 0; i < NUM_STUDENTS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&students[i], NULL, student_thread, id);
    }

    // Wait for all threads (not strictly necessary since this simulation runs indefinitely)
    pthread_join(ta, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }

    // Cleanup (not reached in this simulation)
    sem_destroy(&students_sem);
    sem_destroy(&ta_sem);
    pthread_mutex_destroy(&chairs_mutex);

    return 0;
}
