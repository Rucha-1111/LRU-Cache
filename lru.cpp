// LRU Cache — see README.md for full details.
// Build: g++ lru.cpp -o lru
// Run:   ./lru

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <unordered_map>
#include <optional>
#include <vector>

// One entry stored in the cache.
struct CacheEntry {
    std::string key;
    std::string value;
};

class LRUCache {
public:
    explicit LRUCache(size_t capacity) : capacity_(capacity) {}

    // Returns value on hit, nullopt on miss.
    std::optional<std::string> get(const std::string& key) {
        auto it = lookup_.find(key);
        if (it == lookup_.end()) {
            return std::nullopt;
        }

        auto listIt = it->second;

        // Move to front = mark as most recently used.
        entries_.splice(entries_.begin(), entries_, listIt);
        return listIt->value;
    }

    // Insert or update a key. Returns the evicted key, if any.
    std::optional<std::string> put(const std::string& key, const std::string& value) {
        std::optional<std::string> evictedKey = std::nullopt;

        auto it = lookup_.find(key);
        if (it != lookup_.end()) {
            // Key exists: update in place, no eviction.
            auto listIt = it->second;
            listIt->value = value;
            entries_.splice(entries_.begin(), entries_, listIt);
            return evictedKey;
        }

        if (entries_.size() >= capacity_) {
            evictedKey = evictLeastRecentlyUsed();
        }

        CacheEntry entry;
        entry.key = key;
        entry.value = value;

        entries_.push_front(entry);
        lookup_[key] = entries_.begin();
        return evictedKey;
    }

    // Prints the cache contents from most-recently-used to least-recently-used.
    void printState() const {
        if (entries_.empty()) {
            std::cout << "  Cache: (empty)\n";
            return;
        }

        std::cout << "  Cache [MRU -> LRU]: ";
        bool first = true;
        for (const auto& entry : entries_) {
            if (!first) std::cout << "  ->  ";
            std::cout << entry.key << ":" << entry.value;
            first = false;
        }
        std::cout << "\n";
    }

private:
    size_t capacity_;
    std::list<CacheEntry> entries_; // front = MRU, back = LRU
    std::unordered_map<std::string, std::list<CacheEntry>::iterator> lookup_;

    // Removes the back of the list (least recently used).
    std::string evictLeastRecentlyUsed() {
        auto& oldest = entries_.back();
        std::string evictedKey = oldest.key;
        lookup_.erase(oldest.key);
        entries_.pop_back();
        return evictedKey;
    }
};

// Splits input into whitespace-separated tokens.
static std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string tok;
    while (iss >> tok) tokens.push_back(tok);
    return tokens;
}

int main() {
    std::cout << "=====================================\n";
    std::cout << "           LRU Cache Demo\n";
    std::cout << "=====================================\n\n";

    size_t capacity;
    std::cout << "Enter cache capacity: ";
    if (!(std::cin >> capacity) || capacity == 0) {
        capacity = 3;
    }
    std::cin.ignore(); // consume leftover newline

    LRUCache cache(capacity);

    std::cout << "\nCommands: PUT <key> <value>  |  GET <key>  |  EXIT\n\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break; // EOF
        if (line.empty()) continue;

        auto tokens = tokenize(line);
        std::string cmd = tokens[0];
        for (auto& c : cmd) c = static_cast<char>(toupper(c));

        if (cmd == "EXIT") {
            break;

        } else if (cmd == "PUT") {
            if (tokens.size() < 3) {
                std::cout << "Usage: PUT <key> <value>\n";
                continue;
            }
            auto evicted = cache.put(tokens[1], tokens[2]);
            if (evicted.has_value()) {
                std::cout << "OK  (evicted: " << *evicted << ")\n";
            } else {
                std::cout << "OK\n";
            }
            cache.printState();

        } else if (cmd == "GET") {
            if (tokens.size() < 2) {
                std::cout << "Usage: GET <key>\n";
                continue;
            }
            auto result = cache.get(tokens[1]);
            if (result.has_value()) {
                std::cout << "HIT: " << *result << "\n";
            } else {
                std::cout << "MISS\n";
            }
            cache.printState();

        } else {
            std::cout << "Unknown command. Use PUT, GET, or EXIT.\n";
        }
    }

    std::cout << "\nGoodbye!\n";
    return 0;
}