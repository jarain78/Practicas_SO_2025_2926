#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct Point {
    float x, y, z;
    Point(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

std::string pointToJson(const Point& p) {
    std::ostringstream ss;
    ss << "{ \"x\": " << p.x << ", \"y\": " << p.y << ", \"z\": " << p.z << " }";
    return ss.str();
}

int main() {
    std::vector<Point> points;
    points.emplace_back(0, 0, 0);
    points.emplace_back(2, 0, 2);
    points.emplace_back(4, 0, 0);
    points.emplace_back(6, 0, -2);

    std::ofstream file("points.json");
    file << "{\n  \"points\": [\n";

    for (size_t i = 0; i < points.size(); ++i) {
        file << "    " << pointToJson(points[i]);
        if (i < points.size() - 1) file << ",";
        file << "\n";
    }

    file << "  ]\n}";
    file.close();

    std::cout << "JSON written!\n";
    return 0;
}
