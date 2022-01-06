//
// Created by Nadrino on 22/07/2021.
//

#ifndef XSLLHFITTER_FITSAMPLESET_H
#define XSLLHFITTER_FITSAMPLESET_H

#include "json.hpp"

#include "GenericToolbox.h"

#include "FitSample.h"
#include "FitParameterSet.h"
#include "Likelihoods.hh"

ENUM_EXPANDER(
  DataEventType, -1,
  Unset,
  Asimov,
  DataFiles
)


class FitSampleSet {

public:
  FitSampleSet();
  virtual ~FitSampleSet();

  void reset();

  void setConfig(const nlohmann::json &config);

  void addEventByEventDialLeafName(const std::string& leafName_);

  void initialize();

  // Post init
  void loadAsimovData();
  void applyStatFluctOnData();

  // Getters
  DataEventType getDataEventType() const;
  const std::vector<FitSample> &getFitSampleList() const;
  std::vector<FitSample> &getFitSampleList();

  const nlohmann::json &getConfig() const;

  //Core
  bool empty() const;
  double evalLikelihood() const;

  // Parallel
  void updateSampleEventBinIndexes() const;
  void updateSampleBinEventList() const;
  void updateSampleHistograms() const;

private:
  bool _isInitialized_{false};
  bool _showTimeStats_{false};
  bool _statFluctuation_{false};
  nlohmann::json _config_;
  DataEventType _dataEventType_{DataEventType::Unset};

  std::vector<FitSample> _fitSampleList_;

  std::shared_ptr<CalcLLHFunc> _likelihoodFunctionPtr_{nullptr};

  std::vector<std::string> _eventByEventDialLeafList_;

};


#endif //XSLLHFITTER_FITSAMPLESET_H
