#include<bits/stdc++.h>
using namespace std;
#define faster ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
typedef long long ll;

struct Point{
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
    ll dist(const Point& other) const {
        return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
    }
};

struct Vector{
    int x, y;
    Vector(int x, int y) : x(x), y(y) {}
    ll crossProduct(const Vector& other) const {
        return x * other.y - y * other.x;
    }
};


Vector operator-(const Point& a, const Point& b){
    return Vector(a.x - b.x, a.y - b.y);
}

int orientation(const Point& a, const Point& b, const Point& c){
    Vector ab = b - a;
    Vector ac = c - a;
    ll orient = ab.crossProduct(ac);
    if(orient == 0) return 0; // Thẳng hàng
    return (orient > 0) ? 1 : 2; // 1: quay trái, 2: quay phải
}

bool collinear(const Point& a, const Point& b, const Point& c){
    return orientation(a, b, c) == 0;
}

bool cw(const Point& a, const Point& b, const Point& c, bool include_collinear = false){
    int orient = orientation(a, b, c);
    return (orient == 2 || (include_collinear && orient == 0));
}

vector <Point> convexHull(const ll& n, vector<Point>& points, bool include_collinear = false){
    if(n < 3) return points; // Không đủ điểm để tạo thành đa giác
    
    Point P = points[0];
    int index = 0, l, r;
    for(int i = 1; i < n; i++){
        if(P.x > points[i].x){
            P = points[i];
            index = i;
        }
    }
    l = index;
    vector <Point> hull;
    do{
        hull.push_back(points[l]);
        r = (l + 1) % n;

        for(int i = 0; i < n; i++){
            if(i == l) continue;
            if(!cw(points[l], points[i], points[r], include_collinear)){
                r = i;
            }
        }
        l = r;
    }
    while(l != index);
    return hull;
}

vector <Point> pointsOnHull(const vector<Point>& points, const vector<Point>& hull){
    vector<Point> others;
    for(auto& p : points){
        bool found = false;
        for(auto& h : hull){
            if(p.x == h.x && p.y == h.y){
                found = true;
                break;
            }
        }
        if(!found)
            others.push_back(p);
    }
    return others;
}

double distanceBetweenPoints(const vector<Point>& points, bool mode){
    double Distance;
    if(points.size() < 2) return 0.0;
    if(mode){
        Distance = 0;
        for(int i = 0; i < points.size(); i++){
            for(int j = i + 1; j < points.size(); j++){
                Distance = max(Distance, sqrt((double)points[i].dist(points[j])));
            }
        }
    }
    else{
        Distance = DBL_MAX;
        for(int i = 0; i < points.size(); i++){
            for(int j = i + 1; j < points.size(); j++){
                Distance = min(Distance, sqrt((double)points[i].dist(points[j])));
            }
        }
    }
    return Distance;
}

double convexHullArea(const vector<Point>& hull){
    double area = 0;
    for(int i = 0; i < hull.size(); i++){
        int j = (i + 1) % hull.size();
        area += (hull[i].x * hull[j].y) - (hull[j].x * hull[i].y);
    }
    return abs(area) / 2.0;
}

double distanceOnHull(const vector<Point>& hull, bool mode){
    double Distance = sqrt(hull[0].dist(hull[hull.size() - 1]));
    if(mode){
        for(int i = 0; i < hull.size() - 1; i++){
            Distance = max(Distance, sqrt((double)hull[i].dist(hull[i + 1])));
            // cout << i << " " << i + 1 << ": " << sqrt((double)hull[i].dist(hull[i + 1]))  << " " << Distance << endl;
        }
    }
    else{
        for(int i = 0; i < hull.size() - 1; i++){
            Distance = min(Distance, sqrt((double)hull[i].dist(hull[i + 1])));
        }
    }
    return Distance;
}

void Output(const vector<Point>& points, const vector<Point>& hull, const vector<Point>& others){
    cout << "Cac diem thuoc bao loi: " << endl;
    for (auto p: hull)
        cout << p.x << ' ' << p.y << endl;
    
    cout << fixed << setprecision(2) << fixed;
    cout << "Khoang cach lon nhat va nho nhat cua cac diem tren bao loi: " << endl;
    cout << distanceBetweenPoints(hull, true) << ' ' << distanceBetweenPoints(hull, false) << endl;

    cout << "Canh dai nhat va nho nhat cua bao loi: " << endl;
    cout << distanceOnHull(hull, true) << ' ' << distanceOnHull(hull, false) << endl;

    cout << "khoang cach lon nhat va nho nhat cua cac diem trong bao loi: " << endl;
    cout << distanceBetweenPoints(others, true) << ' ' << distanceBetweenPoints(others, false) << endl;

    cout << "Khoang cach lon nhat va nho nhat cua cac diem thuoc bao loi: " << endl;
    cout << distanceBetweenPoints(points, true) << ' ' << distanceBetweenPoints(points, false) << endl;

    cout << "Dien tich bao loi: " << endl;
    cout << convexHullArea(hull) << endl;
}


int main(){
    faster
    #ifndef ONLINE_JUDGE
    freopen("JarvisMarch.in", "r", stdin);
    freopen("JarvisMarch.out", "w", stdout);
    #endif

    int n;
    cin >> n;

    vector<Point> points;
    for(int i = 0; i < n; i++){
        ll x, y;
        cin >> x >> y;
        points.push_back(Point(x, y));
    }

    vector <Point> hull = convexHull(n, points, true);
    vector <Point> others = pointsOnHull(points, hull);

    Output(points, hull, others);
    return 0;
}