/*
 * This is a sample main.cpp to test OpenCV and TensorRT.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <NvInfer.h>
#include <cuda_runtime_api.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>

using namespace nvinfer1;

// 1. TensorRT requires a logger instance
class Logger : public ILogger {
    void log(Severity severity, const char* msg) noexcept override {
        if (severity <= Severity::kWARNING) {
            std::cout << "[TRT] " << msg << std::endl;
        }
    }
} gLogger;

// Target the pre-compiled .engine file instead of .onnx
const std::string engine_path = "model/yolov8n.engine"; 
// const std::string image_path = "assets/Chamois.jpg";

int main()
{
    std::string image_path;
    std::cout << "Enter file name: ";
    std::cin >> image_path;
    std::cout << "\n";

    // 2. Load and Deserialize the TensorRT Engine
    std::ifstream file(engine_path, std::ios::binary | std::ios::ate);
    if (!file.good()) {
        std::cerr << "Engine file not found. Run trtexec first!" << std::endl;
        return -1;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    IRuntime* runtime = createInferRuntime(gLogger);
    ICudaEngine* engine = runtime->deserializeCudaEngine(buffer.data(), size);
    IExecutionContext* context = engine->createExecutionContext();

    // 3. Allocate GPU Memory & Stream
    void* d_input;
    void* d_output;
    size_t input_bytes = 1 * 3 * 640 * 640 * sizeof(float);
    size_t output_bytes = 1 * 84 * 8400 * sizeof(float);

    cudaMalloc(&d_input, input_bytes);
    cudaMalloc(&d_output, output_bytes);
    
    cudaStream_t stream;
    cudaStreamCreate(&stream);

    // Bind addresses to default YOLOv8 ONNX tensor names
    context->setTensorAddress("images", d_input);
    context->setTensorAddress("output0", d_output);

    // 4. Preprocess Image (Reusing OpenCV functionality)
    cv::Mat image = cv::imread("assets/" + image_path);
    if (image.empty()) return -1;

    float x_factor = image.cols / 640.0f;
    float y_factor = image.rows / 640.0f; // Fixed from image.cols

    // cv::dnn::blobFromImage handles resize, RGB conversion, CHW layout, and normalization
    cv::Mat blob = cv::dnn::blobFromImage(
        image, 1/255.0, cv::Size(640, 640), cv::Scalar(0, 0, 0), true, false
    );

    // 5. Run TensorRT Inference
    // Copy the contiguous blob data to the GPU
    cudaMemcpyAsync(d_input, blob.ptr<float>(), input_bytes, cudaMemcpyHostToDevice, stream);
    
    // Execute asynchronous inference
    context->enqueueV3(stream);

    // Copy output back to CPU
    std::vector<float> h_output(84 * 8400);
    cudaMemcpyAsync(h_output.data(), d_output, output_bytes, cudaMemcpyDeviceToHost, stream);
    
    // Block CPU until inference and transfers are complete
    cudaStreamSynchronize(stream); 

    // 6. Post-processing (Identical to original implementation)
    // Wrap the raw float array in a cv::Mat for parsing
    cv::Mat prediction(84, 8400, CV_32F, h_output.data());
    cv::Mat prediction_t;
    cv::transpose(prediction, prediction_t);

    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (int i = 0; i < prediction_t.rows; i++)
    {
        float* row = prediction_t.ptr<float>(i);
        float* classes_scores = row + 4;

        cv::Mat scores(1, 80, CV_32F, classes_scores);
        cv::Point class_id;
        double max_class_score;
        cv::minMaxLoc(scores, nullptr, &max_class_score, nullptr, &class_id);

        if (max_class_score > 0.45)
        {
            float cx = row[0];
            float cy = row[1];
            float w = row[2];
            float h = row[3];

            int left = int((cx - 0.5 * w) * x_factor);
            int top = int((cy - 0.5 * h) * y_factor);
            int width = int (w * x_factor);
            int height = int (h * y_factor);

            boxes.push_back(cv::Rect(left, top, width, height));
            confidences.push_back((float)max_class_score);
            class_ids.push_back(class_id.x);
        }
    }

    // 7. NMS and Drawing (Identical to original implementation)
    std::vector<int> nms_indices;
    cv::dnn::NMSBoxes(boxes, confidences, 0.45f, 0.50f, nms_indices);

    for (int i = 0; i < nms_indices.size(); i++)
    {
        int idx = nms_indices[i];
        cv::Rect box = boxes[idx];

        cv::rectangle(image, box, cv::Scalar(0, 255, 0), 2);

        std::string label = "Class " + std::to_string(class_ids[idx]) + " (" +
                            std::to_string(int(confidences[idx] * 100)) + "%)";
        
        cv::putText(image, label, cv::Point(box.x, box.y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
    }

    std::string processed_image_path = "processed_" + image_path;
    cv::imwrite("assets/" + processed_image_path, image);

    // 8. Cleanup GPU Resources
    cudaFree(d_input);
    cudaFree(d_output);
    cudaStreamDestroy(stream);
    delete context;
    delete engine;
    delete runtime;

    return 0;
}