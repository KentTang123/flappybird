#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <exception>
#include <vector>
#include <fstream>
#include <optional>

namespace Kent {
    class Application {
    public:
        Application();
        ~Application();
        void mainLoop();
    private:
        // glfw window function
        void initWindow();
        // vulkan function
        void initVulkan();
        void createInstance();
        void setupDebugMessenger();
        void pickPhysicaclDevice();
        // helper function
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
        VkResult createDebugUtilsMessengerEXT(
            VkInstance& instance,
            const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
            const VkAllocationCallbacks* callback,
            VkDebugUtilsMessengerEXT* debugMessenger
        );
        void destroyDebugUtilsMessengerEXT(
            VkInstance& instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* callback);
        // cleanup
        void cleanUp();
    private:
        // glfw objects
        
        // vulkan objects
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        // configuration
        bool enableDebug;
        const std::vector<const char*>enableLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        std::vector<const char*>enableExtension;
    };
}