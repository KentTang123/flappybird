#include "Application.h"

Kent::Application::Application() {
#ifdef _DEBUG
    enableDebug = true;
#elif
    enableDebug = false;
#endif
    initWindow();
    initVulkan();
}

Kent::Application::~Application() { cleanUp(); }

void Kent::Application::mainLoop() {
}

void Kent::Application::initWindow() {
}

void Kent::Application::initVulkan() {
    createInstance();
    setupDebugMessenger();
    pickPhysicaclDevice();
}

void Kent::Application::createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "flappybird";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "no engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t extensionCount = 0;
    const char** requiredExtension = glfwGetRequiredInstanceExtensions(&extensionCount);
    enableExtension.resize(extensionCount);

    for (int i = 0;i < extensionCount;i++) {
        enableExtension.at(i) = requiredExtension[i];
    }

    if (enableDebug) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(enableLayers.size());
        createInfo.ppEnabledLayerNames = enableLayers.data();

        enableExtension.push_back("VK_EXT_debug_utils");
        extensionCount++;
    } else {
        createInfo.enabledLayerCount = 0;
    }
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = enableExtension.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("fail to create instance");
}

void Kent::Application::setupDebugMessenger() {
    if (!enableDebug)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.pNext = nullptr;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    createInfo.pUserData = 0;
    createInfo.flags = 0;
    createInfo.pUserData = nullptr;
    createInfo.pfnUserCallback = debugCallback;

    if (createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
        throw std::runtime_error("fail to create debug messenager");
}

void Kent::Application::pickPhysicaclDevice() {
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0)
        throw std::runtime_error("no device support vulkan");

    std::vector<VkPhysicalDevice>physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

    for (const auto& device : physicalDevices) {
        VkPhysicalDeviceProperties property;
        vkGetPhysicalDeviceProperties(device, &property);

        if (property.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            physicalDevice = device;
            return;
        }
    }
    physicalDevice = physicalDevices.at(0);
}

VKAPI_ATTR VkBool32 VKAPI_CALL Kent::Application::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

VkResult Kent::Application::createDebugUtilsMessengerEXT(VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* callback, VkDebugUtilsMessengerEXT* debugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func == nullptr)
        return VK_ERROR_EXTENSION_NOT_PRESENT;

    func(instance, createInfo, nullptr, debugMessenger);
    return VK_SUCCESS;
}

void Kent::Application::destroyDebugUtilsMessengerEXT(VkInstance& instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* callback) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func == nullptr)
        return;
    func(instance, debugMessenger, nullptr);
}

void Kent::Application::cleanUp() {
    if (enableDebug)
        destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    vkDestroyInstance(instance, nullptr);
}
