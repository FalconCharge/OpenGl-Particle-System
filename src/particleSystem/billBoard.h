#pragma once

#include "../wolf/wolf.h"

class BillBoard{

    public:
        void Init();
        void Render();
        void Update(float p_fDelta);


    private:
        wolf::Program* m_pProgram;
        


};