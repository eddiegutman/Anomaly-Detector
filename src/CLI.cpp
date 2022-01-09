#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    HybridAnomalyDetector* ad = new HybridAnomalyDetector();
    TimeSeries* t1 = new TimeSeries();
    TimeSeries* t2 = new TimeSeries();
    vector<AnomalyReport>* ar = new vector<AnomalyReport>();
    //auto c = new Command*[4];
    Command* c1 = new CommandUploadTimeSeries(dio, ad, t1, t2, ar);
    *commands = c1;
    Command* c2 = new CommandSettings(dio, ad, t1, t2, ar);
    commands[1] = c2;
    Command* c3 = new CommandDetectAnomalies(dio, ad, t1, t2, ar);
    commands[2] = c3;
    Command* c4 = new CommandDisplayResults(dio, ad, t1, t2, ar);
    commands[3] = c4;
    Command* c5 = new CommandUploadAnomaliesAnalyzeResults(dio, ad, t1, t2, ar);
    commands[4] = c5;
   // commands = c;
}

void CLI::start(){
    float option;

    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");
    for (int i = 0; i < 5; i++) {
        dio->write(i+1);
        dio->write(".");
        commands[i]->print();
    }
    dio->write("6.exit\n");

    dio->read(&option);

    while(option != 6) {
        commands[(int)option - 1]->execute();

        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i < 5; i++) {
            dio->write(i+1);
            dio->write(".");
            commands[i]->print();
        }
        dio->write("6.exit\n");

        dio->read(&option);
    }
}

CLI::~CLI() = default;

