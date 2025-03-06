#include "nodeClass/driver.h"
#include "particleSystem/particleDriver.h"
#include "projectRunner.h"
#include "textRendering/src/textRenderingDriver.h"

int main(){
    // Start the demo of the Node class assignment
    //return RunDemoNodeClass();

    // Start the demo of the Particle System assignment 
    return RunParticleSystemDemo();

    // Start the demo of the Textrendering assignment
    //return RunTextRenderingDemo();

    // Start the main Runner of this System
    //return RunMainProject();


}

// Note that I could probably move the helper methods in each class into here;
// It would just make it more clean but that's for another day