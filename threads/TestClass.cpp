#include <iostream>
#include <thread>
#include <chrono>

// Definição da classe TestClass
class TestClass {
    int i;
    public:
    // Construtor da classe
    TestClass(int n) { i = n; }
    void greeting(std::string const& msg) const {
        std::cout << msg << std::endl;
        std::cout << "My number: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(300));
        std::cout << "..." << std::endl;
    }
};

int main() {
    // cria uma instância da classe para ser utilizada na thread
    TestClass obj(10);
    // cria a thread
    std::thread t(&TestClass::greeting, &obj, "Hello!");
    // retorna a thread para o contexto da main
    t.join();
    return 0;
}
