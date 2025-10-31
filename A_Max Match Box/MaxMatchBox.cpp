#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;
struct Point {
    double x, y;
};

double get_area(const vector<Point>& poly) {
    double area = 0.0;
    int N = poly.size();
    if (N < 3) return 0.0; 

    for (int i = 0; i < N; ++i) {
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % N]; 
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    return abs(area) / 2.0;
}

bool check_constraints(const vector<Point>& poly) {
    int N = poly.size();
    if (N == 0) return false;
    const double EPSILON = 1e-9; 
    for (int i = 0; i < N; ++i) {
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % N];
        double len = abs(p1.x - p2.x) + abs(p1.y - p2.y);
        if (len < 0.1 - EPSILON) {
            return false; 
        }
    }
    return true;
}

vector<Point> get_new_polygon(const vector<Point>& orig_poly, double H) {
    int N = orig_poly.size();
    vector<Point> new_poly(N);

    for (int i = 0; i < N; ++i) {
        Point prev = orig_poly[(i - 1 + N) % N];
        Point curr = orig_poly[i];
        Point next = orig_poly[(i + 1) % N];

        if (prev.x == curr.x) { 
            if (curr.y > prev.y) new_poly[i].x = curr.x - H; 
            else new_poly[i].x = curr.x + H; 
        } else { 
            if (next.y > curr.y) new_poly[i].x = curr.x - H; 
            else new_poly[i].x = curr.x + H; 
        }

        if (prev.y == curr.y) { 
            if (curr.x > prev.x) new_poly[i].y = curr.y + H; 
            else new_poly[i].y = curr.y - H; 
        } else { 
            if (next.x > curr.x) new_poly[i].y = curr.y + H; 
            else new_poly[i].y = curr.y - H; 
        }
    }
    return new_poly;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    vector<Point> original_polygon(N);
    for (int i = 0; i < N; ++i) {
        cin >> original_polygon[i].x >> original_polygon[i].y;
    }

    double max_volume = 0.0;
    const double EPSILON = 1e-9;
    
    for (int h_int = 1; h_int <= 250; ++h_int) {
        double H = h_int * 0.1;

        vector<Point> new_poly = get_new_polygon(original_polygon, H);

        if (!check_constraints(new_poly)) {
            break; 
        }

        double area = get_area(new_poly);

        if (area < EPSILON) {
            break; 
        }

        double volume = area * H;
        max_volume = max(max_volume, volume);
    }

    cout << fixed << setprecision(2) << max_volume << endl;

    return 0;
}