#pragma once
#define GLEW_NO_GLU
#include "W_Types.h"
#include <string>

namespace wolf
{
    class App
    {
    public:
        App(const std::string &name);
        virtual ~App();

        virtual void run();
        virtual void update(float dt) = 0;
        virtual void render() = 0;

        bool isKeyDown(int key) const;
        bool isKeyJustDown(int key) const;
        bool isLMBDown() const;
        bool isRMBDown() const;
        bool isMMBDown() const;
        glm::vec2 getMousePos() const;
        void setMousePos(double x, double y) const; //Added Ben Nov/12
        void hideCursor() const;                    //Added Ben Nov.12
        float getFPS() const;
        glm::vec2 getScreenSize() const;
        glm::vec2 getMouseScroll() const { return m_mouseScroll; }
        GLFWwindow* getWindow() const { return m_pWindow; }

        // Used internally but needs to be public
        void _setMouseScroll(const glm::vec2 &scroll);

    protected:
        int m_width = 0;
        int m_height = 0;

    private:
        void _init();
        void _internalUpdate(float dt);

        GLFWwindow *m_pWindow = nullptr;
        std::string m_name;
        glm::vec2 m_mouseScroll;
        float m_fps = 0.0f;
        double m_lastFpsUpdateTime = 0.0;
        int m_frameCount = 0;
        float m_smoothedFps = 0.0f;

        static const int NUM_KEYS = GLFW_KEY_LAST - GLFW_KEY_SPACE;
        bool m_lastKeysDown[NUM_KEYS];
    };
}