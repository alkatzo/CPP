#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <map>
#include <unordered_set>

struct Customer {
    int id;
    std::string name;
};

struct Shop {
    int id;
    std::string name;
    std::string category;
};

struct Visit {
    int customerId;
    int shopId;
    std::string date;
    std::vector<double> purchases;
};

class MallDatabase {
public:
    std::vector<Customer> customers;
    std::vector<Shop> shops;
    std::vector<Visit> visits;

    // Query: Total money spent by a given customer
    double totalSpentByCustomer(int customerId) const {
        return std::accumulate(visits.begin(), visits.end(), 0, [customerId](double acc, const Visit& visit){
            if(visit.customerId == customerId) {
                return acc + std::accumulate(visit.purchases.begin(), visit.purchases.end(), 0, [](double acc, double purches){
                           return acc + purches;
                       });
            }
            else {
                return 0.0;
            }
        });
    }

    // Query: Total revenue per shop
    std::map<int, double> totalRevenuePerShop() const {
        std::map<int, double> revenue;
        for(const auto& visit : visits) {
            revenue[visit.shopId] += std::accumulate(visit.purchases.begin(), visit.purchases.end(), 0);
        }
        return revenue;
    }

    // Query: Shop with the highest total revenue
    const Shop* highestRevenueShop() const {
        std::map<int, double> revByShop = totalRevenuePerShop();
        double max_rev = -1.0;
        int max_shop_id = -1;
        for(const auto& p : revByShop) {
            if(p.second > max_rev) {
                max_rev = p.second;
                max_shop_id = p.first;
            }
        }
        if(max_shop_id == -1) {
            return nullptr;
        }
        auto sit = std::find_if(shops.begin(), shops.end(), [max_shop_id](const Shop& s){
            return s.id == max_shop_id;
        });

        return &(*sit);
    }

    // Query: Total amount spent in shops of a given category
    double totalSpendingByCategory(const std::string& category) const {
        std::unordered_set<int> shop_set;

        for(const Shop& shop : shops) {
            if(shop.category == category) {
                shop_set.emplace(shop.id);
            }
        }

        double total_spent_by_category = 0.0;
        for(const Visit& visit : visits) {
            if(shop_set.find(visit.shopId) != shop_set.end())
                continue;
            total_spent_by_category += std::accumulate(visit.purchases.begin(), visit.purchases.end(), 0.0);
        }

        return total_spent_by_category;
    }

};

int main() {
    MallDatabase db;

    // Add customers
    db.customers.push_back({1, "Alice"});
    db.customers.push_back({2, "Bob"});
    db.customers.push_back({3, "Carol"});

    // Add shops
    db.shops.push_back({101, "Tech World", "Electronics"});
    db.shops.push_back({102, "Fashion Hub", "Clothing"});
    db.shops.push_back({103, "Tasty Treats", "Food"});

    // Add visits
    db.visits.push_back({1, 101, "2025-04-10", {299.99, 89.50}});
    db.visits.push_back({1, 102, "2025-04-11", {49.99}});
    db.visits.push_back({2, 103, "2025-04-10", {12.99, 5.50}});
    db.visits.push_back({2, 101, "2025-04-12", {199.99}});
    db.visits.push_back({3, 103, "2025-04-10", {3.50, 7.25, 2.25}});

    // Run a sample query
    std::cout << "Total spent by Alice: $"
              << db.totalSpentByCustomer(1) << "\n";

    auto shopRevenue = db.totalRevenuePerShop();
    std::cout << "\nTotal revenue per shop:\n";
    for (const auto& [shopId, revenue] : shopRevenue) {
        auto it = std::find_if(db.shops.begin(), db.shops.end(), [shopId](const Shop& s) {
            return s.id == shopId;
        });
        if (it != db.shops.end()) {
            std::cout << it->name << ": $" << revenue << "\n";
        }
    }

    return 0;
}
