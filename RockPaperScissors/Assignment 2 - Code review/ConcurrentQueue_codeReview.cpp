#include <cmath>
#include <functional>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>

template<typename T, uint64_t SIZE = 4096, uint64_t MAX_SPIN_ON_BUSY = 40000000>
class ConcurrentQueue {
private:
    static constexpr unsigned Log2(unsigned n, unsigned p = 0) {
        return (n <= 1) ? p : Log2(n / 2, p + 1);
    }

    // Write 1ULL << ... to fix the warning
    // Use static_cast<uint64_t> instead of (uint64_t) C style cast
    static constexpr uint64_t closestExponentOf2(uint64_t x) {
        return (1UL << ((uint64_t) (Log2(SIZE - 1)) + 1));
    }

    static constexpr uint64_t mRingModMask = closestExponentOf2(SIZE) - 1;
    static constexpr uint64_t mSize = closestExponentOf2(SIZE);

    // With C++17 mEmpty can directly be initialized here.
    // static inline const T mEmpty = T{};
    static const T mEmpty;

    // I would use std::array.
    T mMem[mSize];
    std::mutex mLock;
    uint64_t mReadPtr = 0;
    uint64_t mWritePtr = 0;

public:
    // pop() shall not return a dangling reference on mMem[mReadPtr & mRingModMask] as mMem might be modified by other threads.
    // A copy should be returned instead: T pop() {...
    const T& pop() {

        // Here mReadPtr and mWritePtr are shared by multiple threads and might be stored in a CPU cache. The consumer could miss the update made by producer threads,
        // peek() should not be called without aquiring a lock before.
        if (!peek()) {
            return mEmpty;
        }

        // Detail: Since C++17 prefer std::scoped_lock
        std::lock_guard<std::mutex> lock(mLock);

        if (!peek()) {
            return mEmpty;
        }

        T& ret = mMem[mReadPtr & mRingModMask];

        mReadPtr++;
        return ret;
    }

    bool peek() const {
        return (mWritePtr != mReadPtr);
    }

    // Same than above, read operations are made on mReadPtr and mWritePtr without aquiring a lock.
    // Even if the queue is full, getCount() could return a wrong value and push() could overwrite data which has not been pop yet.
    uint64_t getCount() const {
        return mWritePtr > mReadPtr ? mWritePtr - mReadPtr : mReadPtr - mWritePtr;
    }

    bool busyWaitForPush() {
        uint64_t start = 0;
        // If getCount get finally a lock, It's a bad idea to make pulling on a mutex as it may drastically decrease performances in the way that other 
        // producers and the consumer will struggle to aquire the lock. To keep a locked-base style concurrent queue, I would use a condition_variable:
        // if(getCount() == mSize)
        //      std::condition_variable data_cond.wait(lk,[this]{return getCount() < mSize;});
        //
        // pop() could make the same test: bool shouldNotify = (getCount() == mSize) and notify once an element is poped out from the queue if shouldNotify is true
        while (getCount() == mSize) {
            if (start++ > MAX_SPIN_ON_BUSY) {
                return false;
            }
        }
        return true;
    }
    
    // After busyWaitForPush() has been called and checked the queue is not full, another thread could call push() and fill the queue.
    // We may overwrite data which has not been pop yet. We have to lock, check that the queue is not full, write the data and unlock. If we keep the spin mechanism, I would
    // write something like:
    //  void push(const T& pItem) 
    //  {
    //      bool success = false;
    //      uint64_t tryCnt = 0;
    //      do
    //      {
    //          {
    //              std::lock_guard<std::mutex> lock(mLock);
    //              if(success = getCount() < mSize)
    //              {
    //                  mMem[mWritePtr & mRingModMask] = pItem;
    //                  mWritePtr++;
    //              }
    //          }
    //          if(!success)
    //              sleep();
    //      } while (!success || tryCnt++ < MAX_SPIN_ON_BUSY);
    //
    //     if(!success)
    //          throw std::runtime_error("Concurrent queue full cannot write to it!");
    //  }
    void push(const T& pItem) {

        if (!busyWaitForPush()) {
            throw std::runtime_error("Concurrent queue full cannot write to it!");
        }

        std::lock_guard<std::mutex> lock(mLock);
        mMem[mWritePtr & mRingModMask] = pItem;
        mWritePtr++;
    }

    void push(T&& pItem) {
        if (!busyWaitForPush()) {
            throw std::runtime_error("Concurrent queue full cannot write to it!");
        }

        std::lock_guard<std::mutex> lock(mLock);
        mMem[mWritePtr & mRingModMask] = std::move(pItem);
        mWritePtr++;
    }

    // I would add a "template<class ... Types> emplace(Types&& ... args)" function to construct in place with the arguments.
    // push(const T& pItem) and push(T&& pItem) could just call emplace() to avoid duplicate code.

};

// useless with C++17, see comment above.
template<typename T, uint64_t SIZE, uint64_t MAX_SPIN_ON_BUSY>
const T ConcurrentQueue<T, SIZE, MAX_SPIN_ON_BUSY>::mEmpty = T{ };

int main(int, char**) {
    using Functor = std::function<void()>;

    ConcurrentQueue<Functor*> queue;

    std::thread consumer([ & ] {
        while (true) {
            if (queue.peek()) {
                auto task = queue.pop();
                // task is a ref on pointer and may be null (pop can return mEmpty which is null in this case)
                // Before to call task and delete it, it shall be tested.
                (*task)();
                // This line causes an abort. As task is a dangling reference (on a pointer), the program might not delete the functor previously allocated by one of the 
                // producers.
                delete task;
            }
        }
    });

    std::thread producer([ & ] {
        uint64_t counter = 0;
        while (true) {
            auto taskId = counter++;
            // Size of Functor is quite small. I would just copy it instead of making time costly memory allocations (ConcurrentQueue<Functor> queue).
            // For bigger objects, I would use an unique_ptr in order to be clear on who take the ownership on the object pushed in the queue.   
            auto newTask = new Functor([ = ] {
                std::cout << "Running task " << taskId << std::endl << std::flush;
            });
            queue.push(newTask);
        }
    });

    consumer.join();
    producer.join();
    return 0;
}
