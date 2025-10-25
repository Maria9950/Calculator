#include <cmath>
#include <stdexcept>

extern "C" __declspec(dllexport) const char* plugin_name() {
    return "sin";
}

extern "C" __declspec(dllexport) int plugin_arity() {
    return 1;
}

static double to_rad(double deg) {
    return deg * 3.14159265358979323846 / 180.0; // ������� -> �������
}

extern "C" __declspec(dllexport) double plugin_eval(const double* args, int argc) {
    if (!args || argc != 1) {
        throw std::runtime_error("sin: arity mismatch");
    }
    double x = args[0];
    return std::sin(to_rad(x)); // ����� � ��������
}
