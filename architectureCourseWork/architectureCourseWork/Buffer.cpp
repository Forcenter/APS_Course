#include "Buffer.h"

#include <numeric>
#include <iostream>

Buffer::Buffer(unsigned int size) {
  for (unsigned int i = 0; i < size; ++i)
    data.push_back(Application("", "", "", 0));
}

std::deque< Application >::iterator Buffer::getIterBegin() {
  return data.begin();
}

std::deque< Application >::iterator Buffer::getFirstEmptyFrom(std::deque< Application >::iterator pos) {
  if (pos->getSourceId() == 0)
    return pos;
  std::deque< Application >::iterator start = pos;
  ++pos;
  if (pos == data.end())
    pos = data.begin();
  while (pos != start) {
    if (pos->getSourceId() == 0)
      return pos;
    ++pos;
    if (pos == data.end())
      pos = data.begin();
  }
  return data.end();
}

std::deque< Application >::iterator Buffer::getFirstAppFromSource(unsigned int id, std::deque< Application >::iterator from) {
  std::deque< Application >::iterator pos = from;
  while (pos != data.end()) {
    if (pos->getSourceId() == id)
      return pos;
    ++pos;
  }
  return data.end();
}

std::deque< Application >::iterator Buffer::getIterEnd() {
  return data.end();
}

Application Buffer::pullApplicationAt(std::deque< Application >::iterator pos) {
  Application result = *pos;

  unsigned int id = pos->getSourceId();
  *pos = Application("", "", "", 0);

  --sourceIds.find(id)->second;

  --amountFilled;

  return result;
}

void Buffer::pushApplicationTo(std::deque< Application >::iterator where, Application app) {
  if (where->getSourceId() == 0)
    ++amountFilled;
  else
    --sourceIds.find(where->getSourceId())->second;
  *where = app;
  if (sourceIds.find(app.getSourceId()) == sourceIds.end())
    sourceIds.insert(std::pair< unsigned int, unsigned int >(app.getSourceId(), 1));
  else
    ++sourceIds.find(app.getSourceId())->second;
}

const unsigned int Buffer::getMinSourceId() {
  int min = std::numeric_limits< int >::max();
  for (auto& it : sourceIds) {
    if (it.second > 0) {
      min = it.first;
      break;
    }
  }

  if (min == std::numeric_limits< int >::max())
    min = 0;

  return min;
}

BufferStateSummary Buffer::getStateSummary() {
  return BufferStateSummary{ amountFilled, sourceIds };
}
