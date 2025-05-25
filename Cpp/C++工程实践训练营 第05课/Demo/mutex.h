#pragma once

class mutex {
public:
    void lock();
    void unlock();
};

class unique_lock {
public:
    unique_lock(mutex& m) : mm(m) {
        mm.lock();
    }

    ~unique_lock() {
        mm.unlock();
    }

private:
    mutex& mm;
};