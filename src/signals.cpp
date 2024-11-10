#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

std::queue<int> fila;
pthread_mutex_t mutex;
pthread_cond_t cond_var;

void* produtor(void* arg) {
    int item = 1;
    while (true) {
        pthread_mutex_lock(&mutex);
        fila.push(item);
        std::cout << "Produtor adicionou o item " << item << "\n";
        item++;
        pthread_cond_signal(&cond_var); 
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return nullptr;
}

void* consumidor(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        while (fila.empty()) {
            pthread_cond_wait(&cond_var, &mutex); 
        }
        int item = fila.front();
        fila.pop();
        std::cout << "Consumidor processou o item " << item << "\n";
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    return nullptr;
}

int main() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond_var, nullptr);

    pthread_t prod, cons;
    pthread_create(&prod, nullptr, produtor, nullptr);
    pthread_create(&cons, nullptr, consumidor, nullptr);

    pthread_join(prod, nullptr);
    pthread_join(cons, nullptr);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
    return 0;
}
