//
// Created by Adrien BLANCHET on 30/07/2021.
//

#ifndef GUNDAM_SAMPLEELEMENT_H
#define GUNDAM_SAMPLEELEMENT_H

#include "vector"
#include "memory"
#include "string"

#include "TH1D.h"

#include "DataBinSet.h"
#include "PhysicsEvent.h"


class SampleElement{

public:

  SampleElement();
  virtual ~SampleElement();

  std::string name;

  // Events
  std::vector<PhysicsEvent> eventList;

  // Datasets
  std::vector<size_t> dataSetIndexList;
  std::vector<size_t> eventOffSetList;
  std::vector<size_t> eventNbList;

  // Histograms
  DataBinSet binning;
  std::shared_ptr<TH1D> histogram{nullptr};
  std::vector<std::vector<PhysicsEvent*>> perBinEventPtrList;
  double histScale{1};
  bool isLocked{false};
  int cacheManagerIndex{-1};

  // Methods
  void reserveEventMemory(size_t dataSetIndex_, size_t nEvents, const PhysicsEvent &eventBuffer_);
  void shrinkEventList(size_t newTotalSize_);
  void updateEventBinIndexes(int iThread_ = -1);
  void updateBinEventList(int iThread_ = -1);
  void refillHistogram(int iThread_ = -1);
  void rescaleHistogram();

  double getSumWeights() const;

  int getCacheManagerIndex() const;
  void setCacheManagerIndex(int i);
  
  // debug
  void print() const;

  bool debugTrigger{false};

};


#endif //GUNDAM_SAMPLEELEMENT_H
