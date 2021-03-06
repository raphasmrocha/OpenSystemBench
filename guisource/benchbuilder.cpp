#include "headers/benchbuilder.h"
#include <string>
#include <sstream>
#include <QThread>
#include <QStringList>
#include <chrono>
#include <thread>

/* empty constructor does nothing */
BenchBuilder::BenchBuilder(){}

/* parameterized constructor takes a config array an instance of a running window to perform operations on
 * and a thread to run algorithms in
 */
BenchBuilder::BenchBuilder(bool *config_, RunningWindow *running_,Thread *pass_) {
    pass = pass_;
    running = running_;
    if(config_[0]) {
        createStandardBench();
    }
    else {
        if(config_[2]) {
            if(config_[3]){
                algList.push_back(new NBody());
            }
            if(config_[4]) {
                algList.push_back(new PiDigits());
            }
            if(config_[5]) {
                algList.push_back(new Mandelbrot());
            }
            if(config_[6]) {
                algList.push_back(new SpectralNorm());
            }
        }
        if(config_[7]) {
            if(config_[8]) {
                algList.push_back(new BinaryTrees());
            }
        }
    }
}
/* this method runs all algorithms in the vector algList */
void BenchBuilder::runBench() {
    QStringList names;

    for (Algorithm &alg : algList ) {
        string name = alg.getName();
        QString qname = QString::fromStdString(name);
        names<<qname;
        emit pass->signalText(qname);

        alg.runAlgorithm();

        double time_ = alg.getTime();

        string strtime = to_string(time_);
        emit pass->signalText(QString::fromStdString(strtime));

        emit pass->signalResult(time_);
    }
    //the next few lines are only for user experience
    emit pass->signalText(QString::fromStdString("Preparing Results..."));
    //give a status update and wait 4 seconds so a user can read it before jumping to results window
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    emit pass->finished(names);
}
/* if standard option in config just create a vector with all available algorithms */
void BenchBuilder::createStandardBench() {
    algList.push_back(new NBody());
    algList.push_back(new PiDigits());
    algList.push_back(new Mandelbrot());
    algList.push_back(new SpectralNorm());
    algList.push_back(new BinaryTrees());
}
