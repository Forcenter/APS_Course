#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

class Application {
private:
  std::string carBrandName;
  std::string brokenPart;
  std::string carNumber;
  int sourceId;

public:
  Application(const std::string& carName, const std::string& brokenPart, const std::string& carNumber, int sourceId);
  Application(const Application& other);

  const std::string& getCarBrandName() const;
  const std::string& getBrokenPart() const;
  const std::string& getCarNumber() const;
  int getSourceId() const;

  bool operator==(const Application& other);
  const Application& operator=(const Application& other);
  const Application& operator=(Application&& other);
};

#endif