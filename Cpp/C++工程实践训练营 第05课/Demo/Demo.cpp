// Demo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include "HttpServer.h"

void myHandler(const HttpRequest& req, HttpResponse& resp) {

}

void threadfunc() {

}



int main()
{
    HttpServer httpServer;
    httpSever.registerPath("/hello", [](const HttpRequest& req, HttpResponse& resp) -> void {
        //Âß¼­´¦Àí
    });

    std::thread t1(threadfunc);
    //pthread_create
    //CreateThread

    //t1.join();
    t1.detach();

    mutex m;
    unique_lock ul(m);
    

    if (a) {
        
        return;
    }
    else {
        if (xx) {
            
            return;
        }
    }

    
}

explicit ThreadPool(size_t n) : shutdown_(false) {
    while (n) {
        threads_.emplace_back(worker(*this));
        n--;
    }
}
