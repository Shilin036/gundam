//
// Created by Adrien BLANCHET on 30/07/2021.
//

#include "Logger.h"
#include "GenericToolbox.h"

#include "GlobalVariables.h"
#include "SampleElement.h"

LoggerInit([](){
  Logger::setUserHeaderStr("[SampleElement]");
})


SampleElement::SampleElement() = default;
SampleElement::~SampleElement() = default;

void SampleElement::reserveEventMemory(size_t dataSetIndex_, size_t nEvents, const PhysicsEvent &eventBuffer_) {
  LogThrowIf(isLocked, "Can't " << __METHOD_NAME__ << " while locked");
  if( nEvents == 0 ){ return; }
  dataSetIndexList.emplace_back(dataSetIndex_);
  eventOffSetList.emplace_back(eventList.size());
  eventNbList.emplace_back(nEvents);
  eventList.resize(eventOffSetList.back()+eventNbList.back(), eventBuffer_);
}
void SampleElement::shrinkEventList(size_t newTotalSize_){
  LogThrowIf(isLocked, "Can't " << __METHOD_NAME__ << " while locked");
  if( eventNbList.empty() and newTotalSize_ == 0 ) return;
  LogThrowIf(eventList.size() < newTotalSize_,
             "Can't shrink since eventList is too small: " << GET_VAR_NAME_VALUE(newTotalSize_)
             << " > " << GET_VAR_NAME_VALUE(eventList.size()));
  LogThrowIf(not eventNbList.empty() and eventNbList.back() < (eventList.size() - newTotalSize_), "Can't shrink since eventList of the last dataSet is too small.");
  LogDebug << "Shrinking " << eventList.size() << " to " << newTotalSize_ << "..." << std::endl;
  eventNbList.back() -= (eventList.size() - newTotalSize_);
  eventList.resize(newTotalSize_);
  eventList.shrink_to_fit();
}
void SampleElement::updateEventBinIndexes(int iThread_){
  if( isLocked ) return;
  int nBins = int(binning.getBinsList().size());
  if(iThread_ <= 0) LogInfo << "Finding bin indexes for \"" << name << "\"..." << std::endl;
  int toDelete = 0;
  for( size_t iEvent = 0 ; iEvent < eventList.size() ; iEvent++ ){
    if( iThread_ != -1 and iEvent % GlobalVariables::getNbThreads() != iThread_ ) continue;
    auto& event = eventList.at(iEvent);
    for( int iBin = 0 ; iBin < nBins ; iBin++ ){
      auto& bin = binning.getBinsList().at(iBin);
      bool isInBin = true;
      for( size_t iVar = 0 ; iVar < bin.getVariableNameList().size() ; iVar++ ){
        if( not bin.isBetweenEdges(iVar, event.getVarAsDouble(bin.getVariableNameList().at(iVar))) ){
          isInBin = false;
          break;
        }
      } // Var
      if( isInBin ){
        event.setSampleBinIndex(iBin);
        break;
      }
    } // Bin

    if( event.getSampleBinIndex() == -1 ){
      toDelete++;
    }

  } // Event

//  LogTrace << iThread_ << " -> unbinned events: " << toDelete << std::endl;
}
void SampleElement::updateBinEventList(int iThread_) {
  if( isLocked ) return;

  if(iThread_ <= 0) LogInfo << "Filling bin event cache for \"" << name << "\"..." << std::endl;
  int nBins = int(perBinEventPtrList.size());
  int nbThreads = GlobalVariables::getNbThreads();
  if( iThread_ == -1 ){
    nbThreads = 1;
    iThread_ = 0;
  }

  int iBin = iThread_;
  size_t count;
  while( iBin < nBins ){
    count = 0;
    for( auto& event : eventList ){ if( event.getSampleBinIndex() == iBin ){ count++; } }
    std::vector<PhysicsEvent*> thisBinEventList(count, nullptr);

    // Now filling the event indexes
    size_t index = 0;
    for( auto& event : eventList ){
      if( event.getSampleBinIndex() == iBin ){
        thisBinEventList.at(index++) = &event;
      }
    } // event

    GlobalVariables::getThreadMutex().lock();
    // BETTER TO MAKE SURE THE MEMORY IS NOT MOVED WHILE FILLING UP
    perBinEventPtrList.at(iBin) = thisBinEventList;
    GlobalVariables::getThreadMutex().unlock();

    iBin += nbThreads;
  }
}
void SampleElement::refillHistogram(int iThread_){
  if( isLocked ) return;

  int nbThreads = GlobalVariables::getNbThreads();
  if( iThread_ == -1 ){
    nbThreads = 1;
    iThread_ = 0;
  }

  // Faster that pointer shifter. -> would be slower if refillHistogram is handled by the propagator

  // Size = Nbins + 2 overflow (0 and last)
  auto* binContentArray = histogram->GetArray();

  int iBin = iThread_;
  int nBins = int(perBinEventPtrList.size());
  while( iBin < nBins ){
    binContentArray[iBin+1] = 0;
    for( auto* eventPtr : perBinEventPtrList.at(iBin)){
      binContentArray[iBin+1] += eventPtr->getEventWeight();
    }
    histogram->GetSumw2()->GetArray()[iBin+1] = TMath::Sqrt(binContentArray[iBin+1]);
    iBin += nbThreads;
  }
}
void SampleElement::rescaleHistogram() {
  if( isLocked ) return;
  if(histScale != 1) histogram->Scale(histScale);
}