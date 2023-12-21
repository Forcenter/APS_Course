#ifndef BUFFER_H
#define BUFFER_H

#include "Application.h"
#include "SummaryStructs.h"
#include <queue>
#include <map>

class Buffer {
private:
  std::deque< Application > data;
  std::map< unsigned int, unsigned int > sourceIds;

  unsigned int amountFilled = 0;

public:
  Buffer(unsigned int size);

  std::deque< Application >::iterator getIterBegin();
  std::deque< Application >::iterator getFirstEmptyFrom(std::deque< Application >::iterator);
  std::deque< Application >::iterator getFirstAppFromSource(unsigned int id, std::deque< Application >::iterator from);
  std::deque< Application >::iterator getIterEnd();

  Application pullApplicationAt(std::deque< Application >::iterator where);
  void pushApplicationTo(std::deque< Application >::iterator where, Application app);
  const unsigned int getMinSourceId();
  BufferStateSummary getStateSummary();
};

#endif