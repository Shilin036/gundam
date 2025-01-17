//
// Created by Nadrino on 21/05/2021.
//

#include "sstream"

#include "Logger.h"

#include "Dial.h"
#include "DialSet.h"
#include "FitParameter.h"
#include "FitParameterSet.h"
#include "GlobalVariables.h"

LoggerInit([](){
  Logger::setUserHeaderStr("[Dial]");
  Logger::setMaxLogLevel(LogDebug);
} )


Dial::Dial(DialType::DialType dialType_)
: _dialType_{dialType_}, _isEditingCache_{std::make_shared<std::mutex>()} {}
Dial::~Dial() = default;

void Dial::reset() {
  _isInitialized_ = false;
  _dialResponseCache_ = std::nan("Unset");
  _dialParameterCache_ = std::nan("Unset");
  _applyConditionBin_ = DataBin();
  _associatedParameterReference_ = nullptr;
  _useMirrorDial_ = false;
  _mirrorLowEdge_ = std::nan("unset");
  _mirrorRange_   = std::nan("unset");
}

void Dial::setApplyConditionBin(const DataBin &applyConditionBin) {
  _applyConditionBin_ = applyConditionBin;
}
void Dial::setAssociatedParameterReference(void *associatedParameterReference) {
  _associatedParameterReference_ = associatedParameterReference;
}
void Dial::setIsReferenced(bool isReferenced) {
  _isReferenced_ = isReferenced;
}
void Dial::setUseMirrorDial(bool useMirrorDial) {
  _useMirrorDial_ = useMirrorDial;
}
void Dial::setMirrorLowEdge(double mirrorLowEdge) {
  _mirrorLowEdge_ = mirrorLowEdge;
}
void Dial::setMirrorRange(double mirrorRange) {
  _mirrorRange_ = mirrorRange;
}
void Dial::setMinDialResponse(double minDialResponse_) {
  _minDialResponse_ = minDialResponse_;
}
void Dial::setMaxDialResponse(double maxDialResponse_) {
  _maxDialResponse_ = maxDialResponse_;
}
void Dial::setOwner(const DialSet* dialSetPtr) {
    _ownerDialSetReference_ = dialSetPtr;
}
const DialSet* Dial::getOwner() const {
    LogThrowIf(!_ownerDialSetReference_,"Invalid owning DialSet");
    return _ownerDialSetReference_;
}
void Dial::initialize() {
  LogThrowIf( _dialType_ == DialType::Invalid, "_dialType_ is not set." );
  LogThrowIf( _associatedParameterReference_ == nullptr,  "Parameter not set");

  if( _useMirrorDial_ ){
    LogThrowIf(_mirrorLowEdge_!=_mirrorLowEdge_, "_mirrorLowEdge_ is not set.")
    LogThrowIf(_mirrorRange_!=_mirrorRange_, "_mirrorRange_ is not set.")
    LogThrowIf(_mirrorRange_<0, "_mirrorRange_ is not valid.")
  }
}

bool Dial::isInitialized() const {
  return _isInitialized_;
}
bool Dial::isReferenced() const {
  return _isReferenced_;
}
double Dial::getDialResponseCache() const{
  return _dialResponseCache_;
}
const DataBin &Dial::getApplyConditionBin() const {
  return _applyConditionBin_;
}
DataBin &Dial::getApplyConditionBin() {
  return _applyConditionBin_;
}
DialType::DialType Dial::getDialType() const {
  return _dialType_;
}
void *Dial::getAssociatedParameterReference() const {
  return _associatedParameterReference_;
}
double Dial::getAssociatedParameter() const {
    return static_cast<const FitParameter *>(_associatedParameterReference_)
        ->getParameterValue();
}
int Dial::getAssociatedParameterIndex() const {
    return static_cast<const FitParameter *>(_associatedParameterReference_)
        ->getParameterIndex();
}

void Dial::updateEffectiveDialParameter(){
  _effectiveDialParameterValue_ = _dialParameterCache_;
  if( _useMirrorDial_ ){
    _effectiveDialParameterValue_ = std::abs(std::fmod(
        _dialParameterCache_ - _mirrorLowEdge_,
        2 * _mirrorRange_
    ));

    if( _effectiveDialParameterValue_ > _mirrorRange_ ){
      // odd pattern  -> mirrored -> decreasing effective X while increasing parameter
      _effectiveDialParameterValue_ -= 2 * _mirrorRange_;
      _effectiveDialParameterValue_ = -_effectiveDialParameterValue_;
    }

    // re-apply the offset
    _effectiveDialParameterValue_ += _mirrorLowEdge_;
  }
}
double Dial::evalResponse(){
  LogThrowIf(_associatedParameterReference_==nullptr, "_associatedParameterReference_ is not set.")
  return this->evalResponse( static_cast<FitParameter *>(_associatedParameterReference_)->getParameterValue() );
}
void Dial::copySplineCache(TSpline3& splineBuffer_){
  if( _responseSplineCache_ == nullptr ) this->buildResponseSplineCache();
  splineBuffer_ = *_responseSplineCache_;
}

// Virtual
double Dial::evalResponse(double parameterValue_) {
  while( _dialParameterCache_ != parameterValue_) {
    std::lock_guard<std::mutex> g(*_isEditingCache_); // There can be only one.
    if( _dialParameterCache_ == parameterValue_ ) break;        // stop if already updated

    // Edit the cache
    _dialParameterCache_ = parameterValue_;
    this->updateEffectiveDialParameter();
    this->fillResponseCache(); // specified in the corresponding dial class
    if     ( _minDialResponse_ == _minDialResponse_ and _dialResponseCache_<_minDialResponse_ ){ _dialResponseCache_=_minDialResponse_; }
    else if( _maxDialResponse_ == _maxDialResponse_ and _dialResponseCache_>_maxDialResponse_ ){ _dialResponseCache_=_maxDialResponse_; }

    break; // All done, lock will be released
  }

  return _dialResponseCache_;
}
std::string Dial::getSummary(){
  std::stringstream ss;
  if( _associatedParameterReference_ != nullptr ){
    ss << ((FitParameterSet*)((FitParameter*) _associatedParameterReference_)->getParSetRef())->getName();
    ss << "/" << ((FitParameter*) _associatedParameterReference_)->getTitle();
    ss << "(" << ((FitParameter*) _associatedParameterReference_)->getParameterValue() << ")";
    ss << "/";
  }
  ss << DialType::DialTypeEnumNamespace::toString(_dialType_, true);
  if( not _applyConditionBin_.getEdgesList().empty() ) ss << ":b{" << _applyConditionBin_.getSummary() << "}";
  return ss.str();
}
void Dial::buildResponseSplineCache(){
  // overridable
  double xmin = static_cast<FitParameter *>(_associatedParameterReference_)->getMinValue();
  double xmax = static_cast<FitParameter *>(_associatedParameterReference_)->getMaxValue();
  double prior = static_cast<FitParameter *>(_associatedParameterReference_)->getPriorValue();
  double sigma = static_cast<FitParameter *>(_associatedParameterReference_)->getStdDevValue();

  std::vector<double> xSigmaSteps = {-5, -3, -2, -1, -0.5,  0,  0.5,  1,  2,  3,  5};
  for( size_t iStep = xSigmaSteps.size() ; iStep > 0 ; iStep-- ){
    if( xmin == xmin and (prior + xSigmaSteps[iStep-1]*sigma) < xmin ){
      xSigmaSteps.erase(xSigmaSteps.begin() + int(iStep)-1);
    }
    if( xmax == xmax and (prior + xSigmaSteps[iStep-1]*sigma) > xmax ){
      xSigmaSteps.erase(xSigmaSteps.begin() + int(iStep)-1);
    }
  }

  std::vector<double> yResponse(xSigmaSteps.size(), 0);

  for( size_t iStep = 0 ; iStep < xSigmaSteps.size() ; iStep++ ){
    yResponse[iStep] = this->evalResponse(prior + xSigmaSteps[iStep]*sigma);
  }
  _responseSplineCache_ = std::shared_ptr<TSpline3>(
      new TSpline3(Form("%p", this), &xSigmaSteps[0], &yResponse[0], int(xSigmaSteps.size()))
  );
}
