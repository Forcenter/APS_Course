#include "Application.h"

Application::Application(const std::string& carBrandName, const std::string& brokenPart, const std::string& carNumber, int sourceId):
  carBrandName(carBrandName),
  brokenPart(brokenPart),
  carNumber(carNumber),
  sourceId(sourceId)
{}

Application::Application(const Application& other):
  carBrandName(other.carBrandName),
  brokenPart(other.brokenPart),
  carNumber(other.carNumber),
  sourceId(other.sourceId)
{}

const std::string& Application::getCarBrandName() const {
  return carBrandName;
}
const std::string& Application::getBrokenPart() const {
  return brokenPart;
}
const std::string& Application::getCarNumber() const {
  return carNumber;
}
int Application::getSourceId() const {
  return sourceId;
}

bool Application::operator==(const Application& other) {
  bool result = carBrandName == other.carBrandName && brokenPart == other.brokenPart;
  result = result && carNumber == other.carNumber && sourceId == other.sourceId;
  return result;
}

const Application& Application::operator=(const Application& other) {
  carNumber = other.carNumber;
  brokenPart = other.brokenPart;
  carBrandName = other.carBrandName;
  sourceId = other.sourceId;
  return *this;
}

const Application& Application::operator=(Application&& other) {
  carNumber = other.carNumber;
  brokenPart = other.brokenPart;
  carBrandName = other.carBrandName;
  sourceId = other.sourceId;
  return *this;
}
