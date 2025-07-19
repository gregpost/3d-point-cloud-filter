#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

// Простая структура для 3D-точки
struct Point {
    double x, y, z;
};

// Векторное произведение двух векторов u x v
Point crossProduct(const Point& u, const Point& v) {
    return {
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    };
}

// По трём точкам строим уравнение плоскости A x + B y + C z + D = 0
void computePlaneFromPoints(const Point& p0, const Point& p1, const Point& p2,
                            double& A, double& B, double& C, double& D) {
    // два вектора в плоскости
    Point v1 { p1.x - p0.x, p1.y - p0.y, p1.z - p0.z };
    Point v2 { p2.x - p0.x, p2.y - p0.y, p2.z - p0.z };
    // нормаль
    Point n = crossProduct(v1, v2);
    A = n.x;  B = n.y;  C = n.z;
    // расчет D
    D = - (A * p0.x + B * p0.y + C * p0.z);
}

// Чтение точек из файла (каждая строка: x y z)
bool readPoints(const std::string& filename, std::vector<Point>& pts) {
    std::ifstream infile(filename);
    if (!infile) return false;
    pts.clear();
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        Point p;
        if (!(iss >> p.x >> p.y >> p.z)) continue;
        pts.push_back(p);
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " points.txt plains.txt\n";
        return 1;
    }
    std::string pointsFile = argv[1];
    std::string planesFile = argv[2];

    // 1) читаем облако точек
    std::vector<Point> allPoints;
    if (!readPoints(pointsFile, allPoints)) {
        std::cerr << "Error: Cannot read points from " << pointsFile << "\n";
        return 1;
    }

    // 2) читаем 6 точек плоскостей
    std::vector<Point> planePts;
    if (!readPoints(planesFile, planePts) || planePts.size() < 6) {
        std::cerr << "Error: Cannot read 6 plane points from " << planesFile << "\n";
        return 1;
    }
    Point P0 = planePts[0], P1 = planePts[1], P2 = planePts[2];
    Point Q0 = planePts[3], Q1 = planePts[4], Q2 = planePts[5];

    // 3) вычисляем коэффициенты уравнений
    double A1,B1,C1,D1, A2,B2,C2,D2;
    computePlaneFromPoints(P0,P1,P2, A1,B1,C1,D1);
    computePlaneFromPoints(Q0,Q1,Q2, A2,B2,C2,D2);

    // 4) знак в уравнении для ориентира (0,0,0)
    double sign0_1 = D1;   // A1*0+B1*0+C1*0 + D1
    double sign0_2 = D2;

    std::vector<Point> goodPoints, wrongPoints;
    for (auto& pt : allPoints) {
        double v1 = A1*pt.x + B1*pt.y + C1*pt.z + D1;
        double v2 = A2*pt.x + B2*pt.y + C2*pt.z + D2;
        bool wrong = false;
        // теперь “ложная” точка, если знак отличается от ориентира
        if (v1 * sign0_1 < 0) wrong = true;
        if (v2 * sign0_2 < 0) wrong = true;
        (wrong ? wrongPoints : goodPoints).push_back(pt);
    }

    // 5) готовим имена выходных файлов
    auto pos = pointsFile.find_last_of('.');
    std::string base = (pos==std::string::npos ? pointsFile : pointsFile.substr(0,pos));
    std::string goodFile = base + "_good.txt";
    std::string badFile  = base + "_wrong.txt";

    // 6) записываем
    std::ofstream foutGood(goodFile);
    for (auto& p : goodPoints) foutGood << p.x<<" "<<p.y<<" "<<p.z<<"\n";
    std::ofstream foutBad(badFile);
    for (auto& p : wrongPoints) foutBad << p.x<<" "<<p.y<<" "<<p.z<<"\n";

    // 7) лог
    std::cout<<"Total: "<<allPoints.size()
             <<", Good: "<<goodPoints.size()
             <<", Wrong: "<<wrongPoints.size()<<"\n";
    return 0;
}
