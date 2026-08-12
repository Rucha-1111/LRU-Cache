# LRU Cache (C++)

A simple, from-scratch implementation of an **LRU (Least Recently Used) Cache**
in a single C++ file, with an interactive command-line demo.

An LRU cache is a fixed-size cache that automatically discards the item
that hasn't been used for the longest time when it runs out of room —
the same core idea used in things like browser caches, database query
caches, and CPU caches.

## Features

- **O(1) GET and PUT** using a hash map + doubly linked list
- **Automatic eviction** of the least recently used item when the cache is full
- **Live cache visualization** — see the full cache state (ordered from
  most to least recently used) printed after every command
- Small, single-file, dependency-free (just the C++ standard library)

## How it works

Two data structures work together:

| Structure | Purpose |
|---|---|
| `std::unordered_map<string, iterator>` | Instantly finds where a key lives — O(1) lookup |
| `std::list<CacheEntry>` | Keeps items ordered from most recently used (front) to least recently used (back) — O(1) reordering and eviction |

On every `GET` or `PUT`, the accessed item is moved to the front of the
list (most recently used). When the cache is full and a new key is
added, the item at the back of the list (least recently used) is evicted.

## Build & Run

Requires a C++17 compiler (g++, clang++, or MSVC).

```bash

./lru_cache
```

On Windows (MinGW):

```bash
g++ lru.cpp -o lru
.\lru
```

## Usage

```
Enter cache capacity: 4

Commands: PUT <key> <value>  |  GET <key>  |  EXIT

> PUT a 1
OK
  Cache [MRU -> LRU]: a:1
> PUT b 2
OK
  Cache [MRU -> LRU]: b:2  ->  a:1
> PUT c 3
OK
  Cache [MRU -> LRU]: c:3  ->  b:2  ->  a:1
> PUT d 4
OK
  Cache [MRU -> LRU]: d:4  ->  c:3  ->  b:2  ->  a:1
> PUT e 5
OK  (evicted: a)
  Cache [MRU -> LRU]: e:5  ->  d:4  ->  c:3  ->  b:2
> GET c
HIT: 3
  Cache [MRU -> LRU]: c:3  ->  e:5  ->  d:4  ->  b:2
> GET a
MISS
  Cache [MRU -> LRU]: c:3  ->  e:5  ->  d:4  ->  b:2
> EXIT

Goodbye!
```

Notice how `a` gets evicted once the cache (capacity 4) fills up and a
5th key is added — and afterward, `GET a` correctly returns `MISS`.

## Project structure

```
lru_cache.cpp   — the entire implementation + interactive demo
README.md       — this file
```

## Possible extensions

Ideas for taking this further:

- `DEL <key>` command to manually remove an entry
- TTL (Time To Live) — keys that automatically expire after N seconds
- Hit / miss / eviction statistics
- Thread-safety for concurrent access
- Wrapping it in a small TCP server so other programs can use it remotely

## License

Free to use for learning, portfolio, or personal projects.
