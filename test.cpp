#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <mutex>
#include <list>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <future>
#include <fstream>
#include <string>
#include <stdexcept>
#include <typeindex>

using namespace std;

int main()
{
    condition_variable cv;
    mutex m;
    thread customer = thread([&]()
                             {
        while(true)
        {
            unique_lock lk(m);
            cv.wait(lk);
            //get_value
        } });

    auto producer = thread([&]()
                           {
        while(true)
        {
            //set value
            cv.notify_all();
        } });
}