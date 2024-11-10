#include <iostream>
#include <pthread.h>
#include <unistd.h>

struct Conta {
    int saldo;
    pthread_mutex_t lock;
};

Conta conta1 = {500, PTHREAD_MUTEX_INITIALIZER};
Conta conta2 = {500, PTHREAD_MUTEX_INITIALIZER};

void* transferenciaAParaB(void* arg) {
    pthread_mutex_lock(&conta1.lock);
    sleep(1); // SIMULANDO ATRASO PRA AUMENTA A CHANCE DE DEADLOCK
    pthread_mutex_lock(&conta2.lock);

    if (conta1.saldo >= 100) {
        conta1.saldo -= 100;
        conta2.saldo += 100;
        std::cout << "Transferência de A para B concluída. Saldo A: " << conta1.saldo
                  << ", Saldo B: " << conta2.saldo << "\n";
    }

    pthread_mutex_unlock(&conta2.lock);
    pthread_mutex_unlock(&conta1.lock);
    return nullptr;
}

void* transferenciaBParaA(void* arg) {
    pthread_mutex_lock(&conta2.lock);
    sleep(1); //SIMULANDO ATRASO PRA AUMENTAR A CHANCE DE DEADLOCK
    pthread_mutex_lock(&conta1.lock);

    if (conta2.saldo >= 100) {
        conta2.saldo -= 100;
        conta1.saldo += 100;
        std::cout << "Transferência de B para A concluída. Saldo A: " << conta1.saldo
                  << ", Saldo B: " << conta2.saldo << "\n";
    }

    pthread_mutex_unlock(&conta1.lock);
    pthread_mutex_unlock(&conta2.lock);
    return nullptr;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, nullptr, transferenciaAParaB, nullptr);
    pthread_create(&t2, nullptr, transferenciaBParaA, nullptr);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    return 0;
}
