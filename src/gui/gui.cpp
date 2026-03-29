#include<iostream>

#include "compiler.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

std::string editor_text = "Bread Developer Suite";

bool open_about;
bool open_credits;
bool open_load;
bool open_save;

static int string_resize(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        std::string* str = (std::string*)data->UserData;
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    return 0;
}

int main() {
    try {
        create_tmp_dir_if_not_exist();
    } catch (int e) {
        return 1;
    }
    try {
        find_compiler();
    } catch (int e) {
        return 1;
    }
    if (!glfwInit()) return 1;
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Bread Developer Suite", NULL, NULL);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    bool first_time = true;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
        ImGui::Begin("Bread Developer Suite", NULL, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("About")) {
                if (ImGui::MenuItem("About")) {
                    open_about = true;
                }
                if (ImGui::MenuItem("Credits")) {
                    open_credits = true;
                }
                if (ImGui::MenuItem("Exit")) {
                    break;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Load")) {
                    open_load = true;
                }
                if (ImGui::MenuItem("Save")) {
                    open_save = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (open_about == true) {
            ImGui::OpenPopup("about_popup");
            open_about = false;
        }
        if (ImGui::BeginPopupModal("about_popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Bread Developer Suite");
            ImGui::Text("Part of the bread compiler");
            ImGui::Text("Licensed under the MIT License, see license for more information");
            ImGui::Text("The developer suite was made possible with external libraries, see credits for more info");
            ImGui::Text("By angrypig555");
            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::Text("Welcome to the Bread Developer Suite");
        if (compiler == 2) {
            ImGui::Text("Using compiler clang");
        } else if (compiler == 1) {
            ImGui::Text("Using compiler GCC");
        }
        ImGui::Text("Enter your program here:");
        ImVec2 box_size = ImVec2(-1.0f, -1.0f);
        ImGui::InputTextMultiline("##Editor", (char*)editor_text.c_str(), editor_text.capacity() + 1, box_size, ImGuiInputTextFlags_CallbackResize, string_resize, (void*)&editor_text);
        ImGui::End();
        ImGui::Render();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(window);
    }
}