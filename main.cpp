#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <GLFW/glfw3.h>
#include "core/map.h"
#include "engine/engine.h"
#include "render/render.h"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

// OLM ADAM IMPORT KODUNU KONTROL EDIYOR OHA ^

// bismillah

class Window { // sigma classımız
public:
    Window(int width, int height, const char* title) // pencereyi tanımlıyoruz
        : m_engine(m_map)
    {
        if (!glfwInit()) { // eğer glfw hatası yaşanırsa
            std::cerr << "glfw ossurdu" << std::endl; // ossurma mesajı
            exit(EXIT_FAILURE); //  uygulamayı kapatıyoruz
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL sürümünü 3 olarak ayarlıyoruz
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL sürümünü 3.3 olarak ayarlıyoruz
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // Basit 2D çizim için compatibility profili kullanıyoruz çünkü malım

        // say wallahi bro

        #ifdef __APPLE__ // OHA COPILOT BENDEN ÖNCE YAZIYOR 
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr); // pencereyi oluşturuyoruz
        if (!m_handle) { // eğer pencere oluşturulamazsa
            std::cerr << "glfw pencereyi oluşturamadı" << std::endl; // bura biraz kötü geliştirilecek <-----
            glfwTerminate(); // glfw'i kapatıyoruz
            exit(EXIT_FAILURE); // uygulamayı kapatıyoruz
        }

        glfwMakeContextCurrent(m_handle); // pencereyi aktif hale getiriyoruz // aynen öyle
        if (!m_renderer.init("assets/osmun.png")) {
            std::cerr << "Warning: failed to initialize textures, fallback rendering will be used." << std::endl;
        }
        glfwSetWindowUserPointer(m_handle, this); // pencereyi işaret ediyoruz OHA OLM HALA BİLİYOR HER ŞEYİ
        glfwSetFramebufferSizeCallback(m_handle, framebuffer_size_callback); // pencere boyutu değiştiğinde çağrılacak fonksiyonu ayarlıyoruz
        glfwSetKeyCallback(m_handle, key_callback); // klavye olaylarını ayarlıyoruz // bu ne???

        m_width = width;
        m_height = height;
        glViewport(0, 0, width, height); // pencere boyutunu ayarlıyoruz
    }

    ~Window() { // pencereyi temizliyoruz
        if (m_handle) {
            glfwDestroyWindow(m_handle);
        }
        glfwTerminate();
    }

    void run() { // pencere döngüsünü başlatıyoruz
        double lastTime = glfwGetTime();

        while (!glfwWindowShouldClose(m_handle)) {
            const double currentTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentTime - lastTime);
            lastTime = currentTime;
            deltaTime = std::min(deltaTime, 0.1f); // donma/sekme sonrası fiziğin dev adım atmasını önler

            processInput();
            m_engine.update(deltaTime);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_renderer.drawMap(m_map, m_width, m_height);

            float tileSize, offsetX, offsetY;
            Renderer::computeLayout(m_width, m_height, tileSize, offsetX, offsetY);
            m_renderer.drawPlayer(m_engine.player(), tileSize, offsetX, offsetY);

            glfwSwapBuffers(m_handle);
            glfwPollEvents();
        }
    }

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { // pencere boyutu değiştiğinde çağrılacak fonksiyonu ayarlıyoruz
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (self) {
            self->m_width = width;
            self->m_height = height;
        }
        glViewport(0, 0, width, height);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { // klavye tuşlarını dinliyoruz
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void processInput() { // giriş işlemlerini yapıyoruz
        if (glfwGetKey(m_handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_handle, true);
        }

        const bool left = glfwGetKey(m_handle, GLFW_KEY_A) == GLFW_PRESS ||
                          glfwGetKey(m_handle, GLFW_KEY_LEFT) == GLFW_PRESS;
        const bool right = glfwGetKey(m_handle, GLFW_KEY_D) == GLFW_PRESS ||
                           glfwGetKey(m_handle, GLFW_KEY_RIGHT) == GLFW_PRESS;
        const bool jump = glfwGetKey(m_handle, GLFW_KEY_SPACE) == GLFW_PRESS ||
                          glfwGetKey(m_handle, GLFW_KEY_W) == GLFW_PRESS ||
                          glfwGetKey(m_handle, GLFW_KEY_UP) == GLFW_PRESS;
        const bool hook = glfwGetMouseButton(m_handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        m_engine.setKeyState(left, right, jump, hook);

        double mouseX, mouseY;
        glfwGetCursorPos(m_handle, &mouseX, &mouseY);

        float tileSize, offsetX, offsetY;
        Renderer::computeLayout(m_width, m_height, tileSize, offsetX, offsetY);

        const Vec2 playerPos = m_engine.player().position();
        const float playerScreenX = offsetX + playerPos.x * tileSize;
        const float playerScreenY = offsetY + playerPos.y * tileSize;

        const Vec2 aimDir(static_cast<float>(mouseX) - playerScreenX,
                          static_cast<float>(mouseY) - playerScreenY);
        m_engine.setAimDirection(aimDir);
    }

    GLFWwindow* m_handle{nullptr}; // pencere işaretçisi
    Map m_map;
    Engine m_engine;
    Renderer m_renderer;
    int m_width{800};
    int m_height{600};
};

int main() { // ana fonksiyon
    Window window(800, 600, "AeroHook"); // gtkmm boşa öğrenmişim dostlar ~malerdem
    window.run();
    return 0;
}

// OFFF OPENGL NEDEN BU KADAR ZOR ??


// kodun yarısı erdamn, yarısı copilot tarafından yazıldı kanıt var.