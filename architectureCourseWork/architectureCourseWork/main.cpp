#include "BufferSetter.h"
#include "Buffer.h"
#include "BufferGetter.h"
#include "StatisticsGatherer.h"
#include "ChartBuilder.h"
#include "Synchro.h"

#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <functional>

int main(int argc, char* argv[]) {
  if (argc < 8) {
    std::cout << "Not enough arguments!";
    return -1;
  }

  unsigned int bufferSize     = std::atoi(argv[1]);
  unsigned int workTime       = std::atoi(argv[2]);//In ticks
  unsigned int engineers      = std::atoi(argv[3]);//Their amount
  unsigned int engWorkTime    = std::atoi(argv[4]);//In ticks
  unsigned int sendersAmount  = std::atoi(argv[5]);
  unsigned int mode           = std::atoi(argv[6]);//1 - step, 2 - auto
  unsigned int chosenSender   = std::atoi(argv[7]);
  std::string  reportFileName = argv[8];

  //Main working classes
  Buffer buffer(bufferSize); //gets its size

  BufferSetter setter(1024, buffer); //works on port 1024

  //gets engineer amount and their work speed
  BufferGetter getter(engineers, engWorkTime, buffer);

  StatisticsGatherer stat;

  //Simulation helper. Coordinates senders and main program to work step by step
  //works on port 1025, gets sender amount to wait for them
  Synchro synchro(sendersAmount, 1025);

  Sleep(100);

  std::cout << "Starting main loop\n";

  //Main loop
  for (int i = 0; i < workTime; ++i) { //works set num of iterations (argv[2])
    stat.append(setter.update());
    stat.append(getter.update());
      
    stat.append(buffer.getStateSummary());

    synchro.letThemWork();//Gives senders a go
    stat.nextIteration();

    if (i % 20 == 0 || mode == 1) {
      unsigned int appRecieved    = *stat.getDeltaAppReceived().rbegin();
      unsigned int appDenied      = *stat.getDeltaAppDenied().rbegin();
      unsigned int bufferFullness = *stat.getBufferAmountFilled().rbegin();
      unsigned int engWorking     = stat.getWorksStarted() - stat.getWorksFinished();

      if (!appRecieved && !appDenied && !bufferFullness && !engWorking)
        continue;

      //Percentage of progress
      system("cls");
      std::cout << std::ceil(static_cast<float>(i) / workTime * 100) << '\n';

      if (mode != 1)
        continue;

      std::cout << " N |Applications received|Applications denied|Buffer fullness|Engineers working|\n";
      std::cout << "---|---------------------|-------------------|---------------|-----------------|\n";
      std::cout <<                                  " - |";
      std::cout << std::setw(21) << appRecieved     << '|';
      std::cout << std::setw(19) << appDenied       << '|';
      std::cout << std::setw(15) << bufferFullness  << '|';
      std::cout << std::setw(17) << engWorking      << "|\n";

      std::cout << "---|---------------------|-------------------|---------------|-----------------|\n";
      for (auto& it: stat.getSenderSummary()) {
        std::cout << std::setw(3)  << it.first                            << '|';
        std::cout << std::setw(21) << it.second.applicationsReceived      << '|';
        std::cout << std::setw(19) << it.second.applicationsDenied        << '|';
        std::cout << std::setw(15) << *it.second.bufferFullness.rbegin()  << '|';
        std::cout << std::setw(17) << '-'                                 << "|\n";
      }
      std::cout << "---|---------------------|-------------------|---------------|-----------------|\n";
      std::cin.get();
    }
  }

  system("cls");

  //Summary statistics
  std::cout << "Applications received: " << stat.getAppReceived() << '\n';
  std::cout << "Applications denied: " << stat.getAppDenied() << '\n';
  std::cout << "Connections with sources: " << stat.getConnections() << '\n';
  std::cout << "Works started: " << stat.getWorksStarted() << '\n';
  std::cout << "Works finished: " << stat.getWorksFinished() << '\n';

  std::cout << "Max package size in buffer getter: ";
  std::cout << *std::max_element(stat.getPackageSizes().begin(), stat.getPackageSizes().end()) << '\n';

  //Generating report into a file
  std::ofstream fout;
  fout.open(reportFileName);

  fout << " ¹ |Applications received|Applications denied| % |Max package size|Buffer Usage|\n";
  fout << "---|---------------------|-------------------|---|----------------|------------|\n";

  for (auto& it: stat.getSenderSummary()) {
    float percent = std::ceil(static_cast< float >(it.second.applicationsDenied) / it.second.applicationsReceived * 100);
    unsigned int maxPackage = 0;
    if (it.second.generatedPackageSizes.size() > 0)
      maxPackage = *std::max_element(it.second.generatedPackageSizes.begin(), it.second.generatedPackageSizes.end());
    

    //Calculates percentage of buffer taken whenever senders application is in it
    float bufferUsage = 0;
    int inBufferCounter = 0;
    
    auto& buffer = stat.getBufferAmountFilled();
    size_t j = 0;
    for (size_t i = it.second.firstIterationInBuffer; i < buffer.size(); ++i) {
      if (it.second.bufferFullness[j] > 0) {
        if (buffer[i] == 0)
          continue;
        bufferUsage += static_cast< float >(it.second.bufferFullness[j]) / buffer[i];
        ++inBufferCounter;
      }
      ++j;
    }

    if (inBufferCounter == 0)
      inBufferCounter = 1;
    bufferUsage /= inBufferCounter;
    bufferUsage *= 100;
    
    //Pretty output
    fout << std::fixed << std::setprecision(0);
    fout << std::setw(3)  << it.first                             << '|';
    fout << std::setw(21) << it.second.applicationsReceived       << '|';
    fout << std::setw(19) << it.second.applicationsDenied         << '|';
    fout << std::setw(3)  << percent                              << '|';
    fout << std::fixed << std::setprecision(4);
    fout << std::setw(16) << maxPackage                           << '|';
    fout << std::setw(12) << bufferUsage                          << "|\n";
  }
  fout << "---|---------------------|-------------------|---|----------------|------------|\n";
  fout << '\n';
  fout << '\n';
  fout << '\n';

  auto& engWorkTimeStat = stat.getWorkTime();

  fout << " ¹ |Engineer work time|   %   |\n";
  fout << "---|------------------|-------|\n";
  for (auto& it: engWorkTimeStat) {
    double perc = static_cast<double>(it.second) / workTime * 100;

    fout << std::setw(3)  << it.first  << '|'
         << std::setw(18) << it.second << '|'
         << std::setw(7)  << perc      << "|\n";
  }
  fout << "---|------------------|-------|\n";

  fout.close();

  //Chart generating class made with SFML Graphics.
  ChartBuilder graph1(stat.getBufferAmountFilled(), 0, "Buffer fullness graph");
  ChartBuilder graph2(stat.getDeltaAppReceived(), 1, "Amount of applications received graph");
  ChartBuilder graph3(stat.getDeltaAppDenied(), 2, "Amount of applications denied graph");
  bool skip4 = false;
  if (stat.getSenderSummary().find(chosenSender) == stat.getSenderSummary().end())
    skip4 = true;
  ChartBuilder* graph4 = nullptr;
  if (!skip4)
    graph4 = new ChartBuilder(stat.getSenderSummary().find(chosenSender)->second.bufferFullness, 3, "First sender buffer usage graph");

  //To move the windows of graphs freely
  while (true) {
    graph1.update();
    graph2.update();
    graph3.update();
    if (!skip4)
      graph4->update();
  }

  delete graph4;
}