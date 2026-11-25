#pragma once
#include "STILO-Q/Structs/AnalyzerStructs/ACOConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/GAConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/SAConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/QEAConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/Job.h"

namespace QEVO
{
    class JobCreator
    {
    public:

        std::vector<std::unique_ptr<Job>> createACOJobs(const ACOConfigurationSet& params);

        std::vector<std::unique_ptr<Job>> createGAJobs(const GAConfigurationSet& params);

        std::vector<std::unique_ptr<Job>> createSAJobs(const SAConfigurationSet& params);

        std::vector<std::unique_ptr<Job>> createQEAJobs(const QEAConfigurationSet& params);

    private:

        std::vector<std::unique_ptr<Job>>* jobs_;
        SolverAnalysisInput inp_;

        void addACOJob();

        void addGAJob();

        void addSAJob();

        void addQEAJob();

        void createACOSub1(const ACOConfigurationSet& params);

        void createACOSub2(const ACOConfigurationSet& params);

        void createGASub1(const GAConfigurationSet& params);

        void createGASub2(const GAConfigurationSet& params);

        void createGASub3(const GAConfigurationSet& params);

        void createSASub1(const SAConfigurationSet& params);

        void createSASub2(const SAConfigurationSet& params);

        void createQEASub1(const QEAConfigurationSet& params);

        void createQEASub2(const QEAConfigurationSet& params);

        void createQEASub3(const QEAConfigurationSet& params);
    };
}