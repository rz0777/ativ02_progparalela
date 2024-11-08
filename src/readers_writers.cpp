#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>

pthread_rwlock_t rwlock; 
int saldo = 1000;       

void* leitor(void* id) {
    int thread_id = *(int*)id;
    while (true) {
        pthread_rwlock_rdlock(&rwlock);
        std::cout << "Leitor " << thread_id << " leu o saldo: " << saldo << "\n";
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return nullptr;
}

void* escritor(void* id) {
    int thread_id = *(int*)id;
    while (true) {
        pthread_rwlock_wrlock(&rwlock);
        int ajuste = (rand() % 200) - 100;
        saldo += ajuste;
        std::cout << "Escritor " << thread_id << (ajuste >= 0 ? " depositou " : " sacou ")
                  << abs(ajuste) << ". Novo saldo: " << saldo << "\n";
        pthread_rwlock_unlock(&rwlock);
        sleep(2);
    }
    return nullptr;
}

int main() {
    srand(time(0));
    pthread_rwlock_init(&rwlock, nullptr);

    int numLeitores = 3;
    int numEscritores = 2;
    int totalThreads = numLeitores + numEscritores;

    std::vector<pthread_t> threads(totalThreads);
    std::vector<int> ids(totalThreads);

    int leitoresCriados = 0;
    int escritoresCriados = 0;

    for (int i = 0; i < totalThreads; ++i) {
        ids[i] = i + 1;
        int tipo = rand() % 2;

        if ((tipo == 0 && leitoresCriados < numLeitores) || escritoresCriados >= numEscritores) {
            pthread_create(&threads[i], nullptr, leitor, &ids[i]);
            leitoresCriados++;
        } else {
            pthread_create(&threads[i], nullptr, escritor, &ids[i]);
            escritoresCriados++;
        }
    }

    for (auto& t : threads) {
        pthread_join(t, nullptr);
    }

    pthread_rwlock_destroy(&rwlock);
    return 0;
}
