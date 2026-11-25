#pragma oncethrow
#include "STILOQ/Structs/SolverStructs/SolverOutput.h"

namespace STILOQ
{
    class QEAIteratorWrapper
    {
    public:
    
        class QEASolverKey
        {
        private:

            QEASolverKey() { }

            friend class QEASolver;
            friend class BQEASolver;
            friend class MQEASolver;
        };

        virtual void iterate(QEASolverKey) = 0;

        virtual SolverOutput getOutput(QEASolverKey) = 0;
        
        virtual ~QEAIteratorWrapper() = default;
    };
}