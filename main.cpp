#include <cstdlib>
#include <iostream>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

// OLM ADAM IMPORT KODUNU KONTROL EDIYOR OHA ^

class Window { // sigma classımız
public:
    Window(int width, int height, const char* title) { // pencereyi tanımlıyoruz
        if (!glfwInit()) { // eğer glfw hatası yaşanırsa
            std::cerr << "glfw ossurdu" << std::endl; // ossurma mesajı
            exit(EXIT_FAILURE); //  uygulamayı kapatıyoruz
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL sürümünü 3 olarak ayarlıyoruz
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL sürümünü 3.3 olarak ayarlıyoruz
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL profilini core olarak ayarlıyoruz

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
        glfwSetWindowUserPointer(m_handle, this); // pencereyi işaret ediyoruz OHA OLM HALA BİLİYOR HER ŞEYİ
        glfwSetFramebufferSizeCallback(m_handle, framebuffer_size_callback); // pencere boyutu değiştiğinde çağrılacak fonksiyonu ayarlıyoruz
        glfwSetKeyCallback(m_handle, key_callback); // klavye olaylarını ayarlıyoruz // bu ne???

        glViewport(0, 0, width, height); // pencere boyutunu ayarlıyoruz
    }

    ~Window() { // pencereyi temizliyoruz
        if (m_handle) {
            glfwDestroyWindow(m_handle);
        }
        glfwTerminate();
    }

    void run() { // pencere döngüsünü başlatıyoruz
        while (!glfwWindowShouldClose(m_handle)) {
            processInput();
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(m_handle);
            glfwPollEvents();
        }
    }

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { // pencere boyutu değiştiğinde çağrılacak fonksiyonu ayarlıyoruz
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
    }

    GLFWwindow* m_handle{nullptr}; // pencere işaretçisi
};

int main() { // ana fonksiyon
    Window window(800, 600, "AeroHook"); // gtkmm boşa öğrenmişim dostlar ~malerdem
    window.run();
    return 0;
}

// OFFF OPENGL NEDEN BU KADAR ZOR ??


// kodun yarısı erdamn, yarısı copilot tarafından yazıldı kanıt var. 