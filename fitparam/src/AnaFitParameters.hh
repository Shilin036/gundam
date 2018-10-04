#ifndef __AnaFitParameters_hh__
#define __AnaFitParameters_hh__

#include <iostream>
#include <string>
#include <vector>

#include <TMatrixTSym.h>
using TMatrixDSym = TMatrixTSym<double>;

#include "AnaSample.hh"
#include "EigenDecomp.hh"

// some error codes
const int PASSEVENT = -1;
const int BADBIN    = -2;

class AnaFitParameters
{
public:
    AnaFitParameters();
    virtual ~AnaFitParameters();

    // pure virtual functions
    // InitEventMap -- defines a mapping between events
    // and some unique id (e.g., bin number in True Enu)
    virtual void InitParameters()                                        = 0;
    virtual void InitEventMap(std::vector<AnaSample*>& sample, int mode) = 0;
    // ReWeights a single event based on m_evmap obtained in InitEventMap
    virtual void ReWeight(AnaEvent* event, const std::string& det, int nsample, int nevent,
                          std::vector<double>& params)
        = 0;

    double GetChi2(const std::vector<double>& params) const;
    void SetCovarianceMatrix(const TMatrixDSym& covmat, bool decompose = false);
    std::string GetName() const { return m_name; }
    TMatrixDSym* GetCovMat() const { return covariance; }
    TMatrixDSym GetOriginalCovMat(const TMatrixDSym& cov, unsigned int start_idx) const
    {
        return eigen_decomp->GetOriginalCovMat(cov, start_idx);
    }
    std::vector<double> GetOriginalParameters(const std::vector<double>& param) const
    {
        return eigen_decomp->GetOriginalParameters(param);
    }
    std::vector<double> GetOriginalParameters(const std::vector<double>& param,
                                              unsigned int start_idx) const
    {
        return eigen_decomp->GetOriginalParameters(param, start_idx);
    }

    void GetParNames(std::vector<std::string>& vec) const { vec = pars_name; }
    void GetParPriors(std::vector<double>& vec) const { vec = pars_prior; }
    double GetParPrior(int i) const { return pars_prior.at(i); }
    void GetParSteps(std::vector<double>& vec) const { vec = pars_step; }
    void GetParLimits(std::vector<double>& vec1, std::vector<double>& vec2) const
    {
        vec1 = pars_limlow;
        vec2 = pars_limhigh;
    }

    void SetParNames(std::vector<std::string>& vec) { pars_name = vec; }
    void SetParPriors(std::vector<double>& vec) { pars_prior = vec; }
    void SetParSteps(std::vector<double>& vec) { pars_step = vec; }
    void SetParLimits(std::vector<double>& vec1, std::vector<double>& vec2)
    {
        pars_limlow  = vec1;
        pars_limhigh = vec2;
    }

    int GetNpar() const { return Npar; }
    bool HasCovMat() const { return covariance != nullptr; }
    bool IsDecomposed() const { return m_decompose; }

protected:
    bool CheckDims(const std::vector<double>& params) const;

    std::size_t Npar;
    std::string m_name;
    std::vector<std::string> pars_name;
    std::vector<double> pars_original;
    std::vector<double> pars_prior; // prior values of param
    std::vector<double> pars_step;
    std::vector<double> pars_limlow;
    std::vector<double> pars_limhigh;

    // map for events in each sample
    std::vector<std::vector<int>> m_evmap;
    bool m_rng_priors;
    bool m_decompose;

    EigenDecomp* eigen_decomp;
    TMatrixDSym* covariance;
    TMatrixDSym* covarianceI;
    TMatrixDSym* original_cov;
};

#endif
