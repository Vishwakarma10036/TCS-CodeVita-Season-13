#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

using ll = long long;

struct Point {
    ll x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

struct Line {
    Point p1, p2;
};

struct Star {
    Point center;
    vector<Line> segments; 
};

ll chebyshev(Point p1, Point p2) {
    return max(abs(p1.x - p2.x), abs(p1.y - p2.y));
}

bool collinear(Point p, Point q, Point r) {
    return (q.y - p.y) * (r.x - q.x) == (r.y - q.y) * (q.x - p.x);
}

bool is_on_segment(Point q, Line l) {
    Point p = l.p1, r = l.p2;
    return (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
            q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y) &&
            collinear(p, q, r));
}

int orientation(Point p, Point q, Point r) {
    ll val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  
    return (val > 0) ? 1 : 2; 
}

bool lines_intersect(Line l1, Line l2) {
    int o1 = orientation(l1.p1, l1.p2, l2.p1);
    int o2 = orientation(l1.p1, l1.p2, l2.p2);
    int o3 = orientation(l2.p1, l2.p2, l1.p1);
    int o4 = orientation(l2.p1, l2.p2, l1.p2);

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == 0 && is_on_segment(l2.p1, l1)) return true;
    if (o2 == 0 && is_on_segment(l2.p2, l1)) return true;
    if (o3 == 0 && is_on_segment(l1.p1, l2)) return true;
    if (o4 == 0 && is_on_segment(l1.p2, l2)) return true;

    return false;
}

pair<bool, Point> find_intersection_point(Line l1, Line l2) {
    ll a1 = l1.p2.y - l1.p1.y;
    ll b1 = l1.p1.x - l1.p2.x;
    ll c1 = a1 * l1.p1.x + b1 * l1.p1.y;

    ll a2 = l2.p2.y - l2.p1.y;
    ll b2 = l2.p1.x - l2.p2.x;
    ll c2 = a2 * l2.p1.x + b2 * l2.p1.y;

    ll det = a1 * b2 - a2 * b1;
    if (det == 0) {
        return {false, {0, 0}}; 
    }

    ll x_num = b2 * c1 - b1 * c2;
    ll y_num = a1 * c2 - a2 * c1;

    if (x_num % det != 0 || y_num % det != 0) {
        return {false, {0, 0}};
    }

    Point p = {x_num / det, y_num / det};
    
    if (is_on_segment(p, l1) && is_on_segment(p, l2)) {
        return {true, p};
    }
    return {false, {0, 0}};
}

Point rotate_point(Point p, Point center, int rot_count) {
    ll dx = p.x - center.x;
    ll dy = p.y - center.y;
    for (int i = 0; i < rot_count; ++i) {
        ll ndx = dy;
        ll ndy = -dx;
        dx = ndx;
        dy = ndy;
    }
    return {center.x + dx, center.y + dy};
}

Line rotate_line(Line l, Point center, int rot_count) {
    return {rotate_point(l.p1, center, rot_count), rotate_point(l.p2, center, rot_count)};
}

Star rotate_star(const Star& s, int rot_count) {
    Star rotated_s;
    rotated_s.center = s.center;
    for (const auto& l : s.segments) {
        rotated_s.segments.push_back(rotate_line(l, s.center, rot_count));
    }
    return rotated_s;
}

bool do_stars_touch(const Star& sA, const Star& sB) {
    for (const auto& lA : sA.segments) {
        for (const auto& lB : sB.segments) {
            if (lines_intersect(lA, lB)) {
                return true;
            }
        }
    }
    return false;
}

bool can_connect(const Star& sU, const Star& sV) {
    for (int rot = 0; rot < 4; ++rot) {
        if (do_stars_touch(rotate_star(sU, rot), sV)) return true;
        if (do_stars_touch(sU, rotate_star(sV, rot))) return true;
    }
    return false;
}

ll min_chebyshev_to_segment(Point p, Line l) {
    ll min_d = 1e18;
    ll dx = (l.p2.x > l.p1.x) ? 1 : (l.p2.x < l.p1.x) ? -1 : 0;
    ll dy = (l.p2.y > l.p1.y) ? 1 : (l.p2.y < l.p1.y) ? -1 : 0;

    Point curr = l.p1;
    while (true) {
        min_d = min(min_d, chebyshev(p, curr));
        if (curr == l.p2) break;
        curr.x += dx;
        curr.y += dy;
    }
    return min_d;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    vector<Line> all_lines(N);
    for (int i = 0; i < N; ++i) {
        cin >> all_lines[i].p1.x >> all_lines[i].p1.y >> all_lines[i].p2.x >> all_lines[i].p2.y;
    }

    Point src, dest;
    cin >> src.x >> src.y;
    cin >> dest.x >> dest.y;

    map<Point, set<int>> center_map;
    map<Point, vector<int>> endpoint_map;

    for (int i = 0; i < N; ++i) {
        endpoint_map[all_lines[i].p1].push_back(i);
        endpoint_map[all_lines[i].p2].push_back(i);
    }
    for (auto const& [p, indices] : endpoint_map) {
        if (indices.size() > 1) {
            for (int idx : indices) center_map[p].insert(idx);
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            auto [intersects, p] = find_intersection_point(all_lines[i], all_lines[j]);
            if (intersects) {
                center_map[p].insert(i);
                center_map[p].insert(j);
            }
        }
    }

    vector<Star> stars;
    for (auto const& [center, indices] : center_map) {
        Star s;
        s.center = center;
        for (int idx : indices) {
            s.segments.push_back(all_lines[idx]);
        }
        stars.push_back(s);
    }

    int src_star_index = -1;
    for (int i = 0; i < stars.size(); ++i) {
        for (const auto& l : stars[i].segments) {
            if (is_on_segment(src, l)) {
                src_star_index = i;
                break;
            }
        }
        if (src_star_index != -1) break;
    }

    map<int, int> dist;
    queue<int> q;

    if (src_star_index != -1) {
        q.push(src_star_index);
        dist[src_star_index] = 1;
    }

    while (!q.empty()) {
        int u_idx = q.front();
        q.pop();

        for (int v_idx = 0; v_idx < stars.size(); ++v_idx) {
            if (u_idx == v_idx || dist.count(v_idx)) continue;

            if (can_connect(stars[u_idx], stars[v_idx])) {
                dist[v_idx] = dist[u_idx] + 1;
                q.push(v_idx);
            }
        }
    }

    bool reachable_directly = false;
    ll min_stars_to_reach = 1e18;
    ll min_shift = 1e18;

    for (auto const& [star_idx, path_len] : dist) {
        for (int rot = 0; rot < 4; ++rot) {
            Star s_rot = rotate_star(stars[star_idx], rot);
            for (const auto& l : s_rot.segments) {
                if (is_on_segment(dest, l)) {
                    reachable_directly = true;
                    min_stars_to_reach = min(min_stars_to_reach, (ll)path_len);
                }
                min_shift = min(min_shift, min_chebyshev_to_segment(dest, l));
            }
        }
    }

    if (reachable_directly) {
        cout << min_stars_to_reach << endl;
    } else {
        cout << min_shift << endl;
    }

    return 0;
}