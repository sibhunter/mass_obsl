#include <iostream>
#include <vector>
#include <random>

class Product {
public:
    std::string name;
    double popularity;  // Уровень популярности продукта
    double price;       // Цена продукта

    // Конструктор класса Product
    Product(const std::string& _name, double _popularity, double _price) : name(_name), popularity(_popularity), price(_price) {}

    // Метод определения покупки продукта
    bool willBuy() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution buyDistribution(popularity);
        return buyDistribution(gen);
    }
};

class Shelf {
public:
    int capacity;
    int currentStock;
    double productPrice;  // Цена продукта на полке
    int optimalRestockAmount;

    // Конструктор класса Shelf
    Shelf(int _capacity, double _productPrice) : capacity(_capacity), currentStock(0), productPrice(_productPrice), optimalRestockAmount(0) {}

    // Метод пополнения полки
    void restockShelf() {
        currentStock += optimalRestockAmount;
        std::cout << "Товарная полка пополнена на " << optimalRestockAmount << " единиц." << std::endl;
    }

    // Метод продажи продукта с указанием имени
    void sellProduct(const std::string& productName) {
        if (currentStock > 0) {
            std::cout << "Продукт " << productName << " продан!" << std::endl;
            currentStock--;
        } else {
            std::cout << "Нет товара на полке!" << std::endl;
        }
    }

    // Метод оптимизации стратегии пополнения
    void optimizeRestockStrategy() {
        if (currentStock < capacity / 2) {
            int additionalRestock = static_cast<int>(0.3 * capacity);
            optimalRestockAmount += additionalRestock;
        }
    }
};

class Store {
public:
    std::vector<Product> products;
    std::vector<Shelf> shelves;
    int restockInterval;

    // Конструктор класса Store
    Store(int _restockInterval) : restockInterval(_restockInterval) {}

    // Метод симуляции работы магазина
    void simulate(int simulationTime) {
        for (int currentTime = 0; currentTime < simulationTime; ++currentTime) {
            // Обработка пополнения товарных полок
            if (currentTime % restockInterval == 0) {
                for (auto& shelf : shelves) {
                    shelf.optimizeRestockStrategy();
                    shelf.restockShelf();
                }
            }

            // Обработка продаж
            for (auto& shelf : shelves) {
                if (shelf.currentStock > 0) {
                    // Продажа товара с учетом частоты покупок для каждого продукта
                    if (products[shelf.currentStock - 1].willBuy()) {
                        shelf.sellProduct(products[shelf.currentStock - 1].name);
                    }
                }
            }
        }
    }

    // Метод расчета общей прибыли магазина
    double calculateTotalProfit() const {
        double totalProfit = 0.0;
        for (const auto& shelf : shelves) {
            // Пример простого расчета прибыли: умножаем количество проданного товара на его цену
            totalProfit += shelf.currentStock * shelf.productPrice;
        }
        return totalProfit;
    }
};

int main() {
    int restockInterval = 10;  // Интервал времени между пополнениями товарных полок
    int simulationTime = 100;

    Store store(restockInterval);
    // Инициализация продуктов и полок
    store.products.emplace_back(Product("Bread", 0.8, 10.0));
    store.products.emplace_back(Product("Milk", 0.4, 5.0));
    store.products.emplace_back(Product("Eggs", 0.5, 11.0));
    store.products.emplace_back(Product("Steak", 0.1, 80.0));
    store.shelves.emplace_back(Shelf(20, 8.0));  // Полка с вместимостью 20 и ценой продукта 8.0
    store.shelves.emplace_back(Shelf(5, 100.0));
    store.shelves.emplace_back(Shelf(10, 5.0));
    store.shelves.emplace_back(Shelf(10, 11.0));
    // Симуляция
    store.simulate(simulationTime);

    // Оптимизация стратегии пополнения
    for (auto& shelf : store.shelves) {
        shelf.optimizeRestockStrategy();
    }

    // Анализ общей прибыли
    double totalProfit = store.calculateTotalProfit();
    std::cout << "Общая прибыль магазина: " << totalProfit << std::endl;

    return 0;
}
