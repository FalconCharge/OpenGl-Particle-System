#include "pointBB.h"

#include <iostream>
#include <string>

PointBB::PointBB() :    m_position(glm::vec3(0)),
                        m_scale(glm::vec3(1)),
                        m_rotation(glm::vec3(0)),
                        m_velocity(glm::vec3(0))

{

}

PointBB::~PointBB(){

}

void PointBB::Init(){
    return;
}

void PointBB::Update(float p_fDelta){
    if(!IsExpired()){
        if(m_timeAlive >= m_maxTimeAlive){
            SetIsExpired(true);
        }else{
            m_timeAlive += p_fDelta;
        }
    }
}
void PointBB::ResetPoint(){
    SetIsExpired(false);
    m_timeAlive = 0.0f;
    m_velocity = glm::vec3(0);
    //std::cout << "reset values" << std::endl;
}


