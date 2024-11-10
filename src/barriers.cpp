#include <iostream>
#include <pthread.h>
#include <unistd.h>

const int NUM_THREADS = 5;
pthread_barrier_t barreira;

void* processar_dados(void* id) {
    int thread_id = *(int*)id;

    for (int etapa = 1; etapa <= 3; ++etapa) {
        std::cout << "Thread " << thread_id << " processando etapa " << etapa << "\n";
        sleep(1); 

        pthread_barrier_wait(&barreira);
        std::cout << "Thread " << thread_id << " completou a etapa " << etapa << "\n";
    }
    return nullptr;
}

int main() {
    pthread_barrier_init(&barreira, nullptr, NUM_THREADS);

    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        ids[i] = i + 1;
        pthread_create(&threads[i], nullptr, processar_dados, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    pthread_barrier_destroy(&barreira);
    return 0;
}
