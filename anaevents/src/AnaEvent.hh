#ifndef __AnaEvent_hh__
#define __AnaEvent_hh__

#include <iostream>

#include <TMath.h>

#include <FitStructs.hh>

class AnaEvent
{
    public:
        AnaEvent(long int evid)
        {
            m_evid     = evid;
            m_flavor   = -1;
            m_beammode = -1;
            m_topology = -1;
            m_reaction = -1;
            m_target   = -1;
            m_sample   = -1;
            m_signal   = false;
            m_sig_type = -1;
            m_true_evt = false;
            m_enu_true = -999.0;
            m_trueD1   = -999.0;
            m_trueD2   = -999.0;
            m_trueD3   = -999.0;
            m_trueD4   = -999.0;
            m_recoD1   = -999.0;
            m_recoD2   = -999.0;
            m_recoD3   = -999.0;
            m_recoD4   = -999.0;
            m_wght     = 1.0;
            m_wghtMC   = 1.0;
        }

        //Set/Get methods
        void SetTopology(int val){ m_topology = val; }
        int GetTopology(){ return m_topology; }

        void SetReaction(int val){ m_reaction = val; }
        int GetReaction(){ return m_reaction; }

        void SetTarget(int val){ m_target = val; }
        int GetTarget(){ return m_target; }

        void SetSampleType(int val){ m_sample = val; }
        int GetSampleType(){ return m_sample; }

        void SetSignalEvent(const bool flag = true){ m_signal = flag; }
        bool isSignalEvent(){ return m_signal; }

        void SetSignalType(int val){ m_sig_type = val; }
        int GetSignalType(){ return m_sig_type; }

        void SetTrueEvent(const bool flag = true){ m_true_evt = flag; }
        bool isTrueEvent(){ return m_true_evt; }

        void SetFlavor(const int flavor){ m_flavor = flavor; }
        int GetFlavor(){ return m_flavor; }

        void SetBeamMode(int val){ m_beammode = val; }
        int GetBeamMode(){ return m_beammode; }

        long int GetEvId(){ return m_evid; }

        void SetTrueEnu(double val) {m_enu_true = val;}
        double GetTrueEnu(){ return m_enu_true; }

        void SetTrueD1(double val){ m_trueD1 = val; }
        double GetTrueD1(){ return m_trueD1; }

        void SetRecoD1(double val){ m_recoD1 = val; }
        double GetRecoD1(){ return m_recoD1; }

        void SetTrueD2(double val){ m_trueD2 = val; }
        double GetTrueD2(){ return m_trueD2; }

        void SetRecoD2(double val){ m_recoD2 = val; }
        double GetRecoD2(){ return m_recoD2; }

        void SetTrueD3(double val){ m_trueD3 = val; }
        double GetTrueD3(){ return m_trueD3; }

        void SetRecoD3(double val){ m_recoD3 = val; }
        double GetRecoD3(){ return m_recoD3; }

        void SetTrueD4(double val){ m_trueD4 = val; }
        double GetTrueD4(){ return m_trueD4; }

        void SetRecoD4(double val){ m_recoD4 = val; }
        double GetRecoD4(){ return m_recoD4; }

        void SetEvWght(double val){ m_wght  = val; }
        void SetEvWghtMC(double val){ m_wghtMC  = val; }

        // Multiplies the current event weight with the input argument:
        void AddEvWght(double val){ m_wght *= val; }
        
        double GetEvWght(){ return m_wght; }
        double GetEvWghtMC(){ return m_wghtMC; }

        void ResetEvWght(){ m_wght = m_wghtMC; }

        void Print()
        {
            std::cout << "Event ID    " << m_evid << std::endl
                      << "Topology    " << GetTopology() << std::endl
                      << "Reaction    " << GetReaction() << std::endl
                      << "Target      " << GetTarget() << std::endl
                      << "Flavor      " << GetFlavor() << std::endl
                      << "Beam mode   " << GetBeamMode() << std::endl
                      << "Sample      " << GetSampleType() << std::endl
                      << "Signal      " << GetSignalType() << std::endl
                      << "True energy " << GetTrueEnu() << std::endl
                      << "True D1     " << GetTrueD1() << std::endl
                      << "Reco D1     " << GetRecoD1() << std::endl
                      << "True D2     " << GetTrueD2() << std::endl
                      << "Reco D2     " << GetRecoD2() << std::endl
                      << "True D3     " << GetTrueD3() << std::endl
                      << "Reco D3     " << GetRecoD3() << std::endl
                      << "True D4     " << GetTrueD4() << std::endl
                      << "Reco D4     " << GetRecoD4() << std::endl
                      << "Weight      " << GetEvWght() << std::endl
                      << "Weight MC   " << GetEvWghtMC() << std::endl;
        }

        int GetEventVar(const std::string& var) const
        {
            if(var == "topology")
                return m_topology;
            else if(var == "reaction")
                return m_reaction;
            else if(var == "target")
                return m_target;
            else if(var == "beammode")
                return m_beammode;
            else if(var == "flavor")
                return m_flavor;
            else if(var == "sample")
                return m_sample;
            else if(var == "signal")
                return m_sig_type;
            else
                return -1;
        }

    private:
        long int m_evid;   //unique event id
        int m_flavor;      //flavor of neutrino (numu, etc.)
        int m_beammode;    //Forward horn current (+1) or reverse horn current (-1)
        int m_topology;    //final state topology type
        int m_reaction;    //event interaction mode
        int m_target;      //target nuclei
        int m_sample;      //sample type (aka cutBranch)
        int m_sig_type;
        bool m_signal;     //flag if signal event
        bool m_true_evt;   //flag if true event
        double m_enu_true; //true nu energy
        double m_trueD1;   //true D1
        double m_trueD2;   //true D2
        double m_trueD3;   //true D3
        double m_trueD4;   //true D4
        double m_recoD1;   //reco D1
        double m_recoD2;   //reco D2
        double m_recoD3;   //reco D3
        double m_recoD4;   //reco D4
        double m_wght;     //event weight
        double m_wghtMC;   //event weight from original MC
};

#endif
