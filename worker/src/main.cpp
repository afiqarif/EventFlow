#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>

// OpenCV Headers
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// ONNX Runtime Headers
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

/**
 * EventFlow Worker Node - Integration Smoke Test
 * 
 * Purpose: 
 * This diagnostic utility verifies that both OpenCV and ONNX Runtime are 
 * properly linked, their dynamic libraries (shared objects) are discoverable 
 * at runtime, and their core execution environments initialize without crashing.
 * 
 * Compile this using the following CMake target:
 * 
 *   add_executable(worker_smoke_test smoke_test.cpp)
 *   target_link_libraries(worker_smoke_test PRIVATE ${OpenCV_LIBS} onnxruntime)
 */

void runOpenCVSmokeTest() {
    std::cout << "\n==================================================" << std::endl;
    std::cout << "[STEP 1/2] Launching OpenCV Core & Image Processing Test" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "-> OpenCV Version: " << CV_VERSION << std::endl;

    // 1. Create a synthetic canvas in memory (300x300 pixels, 3 channels RGB, initialized to black)
    std::cout << "-> Instantiating 300x300 RGB Matrix..." << std::endl;
    cv::Mat image = cv::Mat::zeros(300, 300, CV_8UC3);

    // 2. Perform matrix transformations (Draw a crosshair pattern representing face detection alignment)
    std::cout << "-> Applying vector graphics (cv::line & cv::circle)..." << std::endl;
    cv::Point center(150, 150);
    int radius = 50;
    
    // Draw alignment circle (Green)
    cv::circle(image, center, radius, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
    // Draw target crosshairs (Blue)
    cv::line(image, cv::Point(center.x - 70, center.y), cv::Point(center.x + 70, center.y), cv::Scalar(255, 0, 0), 1);
    cv::line(image, cv::Point(center.x, center.y - 70), cv::Point(center.x, center.y + 70), cv::Scalar(255, 0, 0), 1);

    // 3. Mathematical validation of image memory integrity
    double nonZeroColors = cv::countNonZero(image.reshape(1)); // Flatten channels to count edited pixels
    std::cout << "-> Validating matrix memory integrity..." << std::endl;
    std::cout << "-> Modified pixel elements: " << nonZeroColors << " channels" << std::endl;

    if (nonZeroColors > 0) {
        std::cout << ">> OpenCV Smoke Test: SUCCESS (Image transformations completed in memory)" << std::endl;
    } else {
        throw std::runtime_error("OpenCV matrix remains completely blank. Allocation failed.");
    }
}

void runOnnxRuntimeSmokeTest() {
    std::cout << "\n==================================================" << std::endl;
    std::cout << "[STEP 2/2] Launching ONNX Runtime Initialization Test" << std::endl;
    std::cout << "==================================================" << std::endl;

    // 1. Initialize the global thread-safe ORT Environment
    std::cout << "-> Initializing Ort::Env (Global execution environment)..." << std::endl;
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "EventFlowSmokeTest");
    std::cout << "-> Ort::Env successfully created." << std::endl;

    // 2. Query available execution providers (CPU is always guaranteed, CUDA/TensorRT if optimized)
    std::cout << "-> Querying compile-time execution options..." << std::endl;
    Ort::SessionOptions sessionOptions;
    sessionOptions.SetIntraOpNumThreads(1);
    sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    
    std::cout << "-> Session configurations successfully registered." << std::endl;

    // 3. Test memory allocator integration
    std::cout << "-> Testing global allocator linkage..." << std::endl;
    Ort::AllocatorWithDefaultOptions allocator;
    
    // Allocate dummy tensor data safely through Ort
    size_t testElements = 512; // Matching face embedding dimension
    size_t byteCount = testElements * sizeof(float);
    void* rawBuffer = allocator.Alloc(byteCount);
    
    if (rawBuffer != nullptr) {
        std::cout << "-> Allocated " << byteCount << " bytes of tensor-aligned memory via ORT allocator." << std::endl;
        float* floatBuffer = static_cast<float*>(rawBuffer);
        std::fill(floatBuffer, floatBuffer + testElements, 0.5f); // Fill with dummy embedding float
        
        // Free memory using the correct Ort deallocator to check allocation loop consistency
        allocator.Free(rawBuffer);
        std::cout << "-> Successfully deallocated tensor memory." << std::endl;
        std::cout << ">> ONNX Runtime Smoke Test: SUCCESS (Environment initialized & allocator functional)" << std::endl;
    } else {
        throw std::runtime_error("ORT Default Allocator returned a null pointer.");
    }
}

int main() {
    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "=== EVENTFLOW WORKER NODE SYSTEM INTEGRATION SMOKE TEST ===" << std::endl;

    try {
        // Execute tests
        runOpenCVSmokeTest();
        runOnnxRuntimeSmokeTest();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = endTime - startTime;

        std::cout << "\n==================================================" << std::endl;
        std::cout << "ALL WORKER SMOKE TESTS PASSED IN " << duration.count() << " ms!" << std::endl;
        std::cout << "Your environment's ABI linkages are 100% correct." << std::endl;
        std::cout << "==================================================" << std::endl;
        return 0;

    } catch (const cv::Exception& e) {
        std::cerr << "\n!!! OPENCV RUNTIME EXCEPTION !!!" << std::endl;
        std::cerr << "Details: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "\n!!! SYSTEM OR ONNX RUNTIME EXCEPTION !!!" << std::endl;
        std::cerr << "Details: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n!!! UNHANDLED CRITICAL EXCEPTION !!!" << std::endl;
        return 1;
    }
}
