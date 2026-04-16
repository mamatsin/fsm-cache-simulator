#include <iostream>
#include <vector>
#include <queue>
using namespace std;

enum State {
    IDLE,
    COMPARE_TAG,
    WRITE_BACK,
    ALLOCATE
};

struct CacheLine {
    bool valid = false;
    bool dirty = false;
    int tag = -1;
    int data = 0;
};

struct Request {
    char type; // 'R' or 'W'
    int address;
    int data;  // only used for write
};

class Memory {
public:
    int latency = 2;
    int counter = 0;
    bool busy = false;

    void start() {
        busy = true;
        counter = latency;
    }

    bool ready() {
        if (busy && --counter <= 0) {
            busy = false;
            return true;
        }
        return false;
    }
};

class CacheController {
private:
    vector<CacheLine> cache;
    queue<Request> cpuQueue;
    Memory memory;

    State state = IDLE;
    Request currentRequest;
    int index, tag;

public:
    CacheController(int size) {
        cache.resize(size);
    }

    void addRequest(Request r) {
        cpuQueue.push(r);
    }

    void run() {
        int cycle = 0;

        while (!cpuQueue.empty() || state != IDLE) {
            cout << "\nCycle: " << cycle++ << endl;

            switch (state) {
                case IDLE:
                    if (!cpuQueue.empty()) {
                        currentRequest = cpuQueue.front();
                        cpuQueue.pop();

                        index = currentRequest.address % cache.size();
                        tag = currentRequest.address / cache.size();

                        cout << "IDLE -> COMPARE_TAG\n";
                        state = COMPARE_TAG;
                    }
                    break;

                case COMPARE_TAG: {
                    CacheLine &line = cache[index];

                    if (line.valid && line.tag == tag) {
                        cout << "Cache HIT\n";

                        if (currentRequest.type == 'R') {
                            cout << "Read Data: " << line.data << endl;
                        } else {
                            line.data = currentRequest.data;
                            line.dirty = true;
                            cout << "Write Done, Dirty Bit Set\n";
                        }

                        cout << "Return to IDLE\n";
                        state = IDLE;
                    } else {
                        cout << "Cache MISS\n";

                        if (line.valid && line.dirty) {
                            cout << "Dirty block -> WRITE_BACK\n";
                            memory.start();
                            state = WRITE_BACK;
                        } else {
                            cout << "Clean block -> ALLOCATE\n";
                            memory.start();
                            state = ALLOCATE;
                        }
                    }
                    break;
                }

                case WRITE_BACK:
                    if (memory.ready()) {
                        cout << "Write-back completed\n";
                        memory.start();
                        state = ALLOCATE;
                    } else {
                        cout << "Writing back...\n";
                    }
                    break;

                case ALLOCATE:
                    if (memory.ready()) {
                        cout << "Block loaded from memory\n";

                        CacheLine &line = cache[index];
                        line.valid = true;
                        line.tag = tag;
                        line.dirty = false;

                        // After allocation, go back to compare (important!)
                        state = COMPARE_TAG;
                    } else {
                        cout << "Waiting for memory...\n";
                    }
                    break;
            }
        }
    }
};

int main() {
    CacheController cache(4);

    // Test sequence
    cache.addRequest({'R', 0, 0});
    cache.addRequest({'W', 4, 10});
    cache.addRequest({'R', 0, 0});
    cache.addRequest({'W', 8, 20});
    cache.addRequest({'R', 4, 0});

    cache.run();
}
