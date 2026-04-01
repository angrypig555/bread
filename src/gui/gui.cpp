#include<iostream>

#include "compiler.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "imguifd/ImGuiFileDialog.h"
#include "imgui/imgui_stdlib.h"



bool open_about;
bool open_credits;
bool open_load;
bool open_save;
bool show_error = false;

std::string file_to_load;
std::string file_to_save;

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
    glfwSetWindowAttrib(window, GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    bool first_time = true;
    static std::string editor_text = "Bread Developer Suite";
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
        if (open_credits == true) {
            ImGui::OpenPopup("credits_popup");
            open_credits = false;
        }
        if (open_load == true) {
            IGFD::FileDialogConfig config;
	        config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseBreadFile", "Choose File", ".bread", config);
            if (ImGuiFileDialog::Instance()->Display("ChooseBreadFile")) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    file_to_load = ImGuiFileDialog::Instance()->GetFilePathName();
                }
                ImGuiFileDialog::Instance()->Close();
            }
            open_load = false;
        }
        if (open_save == true) {
            IGFD::FileDialogConfig config;
	        config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseBreadFileToSave", "Choose File", ".bread", config);
            if (ImGuiFileDialog::Instance()->Display("ChooseBreadFileToSave")) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    file_to_save = ImGuiFileDialog::Instance()->GetFilePathName();
                }
                ImGuiFileDialog::Instance()->Close();
            }
            open_save = false;
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
        if (ImGui::BeginPopupModal("credits_popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Third Party Licenses\n Dear ImGui - MIT License Copyright (c) 2014-2024 Omar Cornut\n ImGui File Dialog - MIT License Copyright (c) 2018-2025 Stephane Cuillerdier (aka Aiekick)");
            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::Text("Welcome to the Bread Developer Suite");
        ImGui::Text("Make sure to start this from the terminal to see the output of the compiler");
        if (compiler == 2) {
            ImGui::Text("Using compiler clang");
        } else if (compiler == 1) {
            ImGui::Text("Using compiler GCC");
        }
        if (ImGui::Button("Compile")) { // todo, implement this
            std::ofstream breadfile("/tmp/bread/editor_compile.bread");
            breadfile << editor_text;
            breadfile.close();
            filename = "editor_compile.bread";
            try {
                compile_to_cpp("/tmp/bread/editor_compile.bread");
                show_error = false;
            }   catch (int e) {
                show_error = true;
            }
            std::cout << "Compiled executable copied to running directory" << std::endl;
        }
        ImGui::Text("Compilation failed, please see terminal for info.");
        ImGui::Text("Enter your program here:");
        ImVec2 box_size = ImVec2(-1.0f, -1.0f);
        ImGui::InputTextMultiline("##Editor", &editor_text, box_size);
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