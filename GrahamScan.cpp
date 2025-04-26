#include<bits/stdc++.h>
using namespace std;
#define faster ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
typedef long long ll;

struct Point{
    ll x, y;
    Point() : x(0), y(0) {}
    Point(ll x, ll y) : x(x), y(y) {}
    
    ll dist(const Point& other)const {
        return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
    }
};

struct Vector{
    ll x, y;
    Vector() : x(0), y(0) {}
    Vector(ll x, ll y) : x(x), y(y) {}

    ll crossProduct(const Vector& other){
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

bool cmd(const Point& a, const Point& b, const Point& P){
    int orient = orientation(P, a, b);
    if(orient == 0) return a.dist(P) < b.dist(P); // Nếu thẳng hàng, chọn điểm gần hơn
    return (orient == 1); // Quay trái
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

vector <Point> convexHull(const ll& n, vector<Point>& points, bool include_collinear = false){
    if(n < 3) return points; // Không đủ điểm để tạo thành đa giác
    Point P(INT_MAX, INT_MAX);
    for(auto& p : points){
        if(p.y < P.y || (p.y == P.y && p.x < P.x))
            P = p;
    }
    
    sort(points.begin(), points.end(), [&](const Point& a, const Point& b){
        return cmd(a, b, P);
    });

    if(include_collinear){
        int i = n - 1;
        while(i >= 0 && collinear(points[0], points[i], P)) i--;
        reverse(points.begin() + i + 1, points.end());
    }

    vector<Point> hull;
    for(int i = 0; i < n; i++){
        while(hull.size() >= 2 && cw(hull[hull.size() - 2], hull[hull.size() - 1], points[i], include_collinear)){
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    return hull;
}

double convexHullArea(const vector<Point>& hull){
    double area = 0;
    for(int i = 0; i < hull.size(); i++){
        int j = (i + 1) % hull.size();
        area += (hull[i].x * hull[j].y) - (hull[j].x * hull[i].y);
    }
    return abs(area) / 2.0;
}
void Output(const vector<Point>& others, const vector<Point>& hull, const vector<Point>& points){
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
        freopen("GrahamScan.in", "r", stdin);
        freopen("GrahamScan.out", "w", stdout);
    #endif

    int n; // 1 <= n <= 100000
    cin >> n;
    
    if(n < 3){
        cout << "Khong du diem de tao thanh da giac" << endl;
        return 0; // Không đủ điểm để tạo thành đa giác
    }
    vector<Point> points(n);
    for(int i = 0; i < n; i++){
        cin >> points[i].x >> points[i].y;
    }

    vector<Point> hull = convexHull(n, points);

    // Tìm các điểm không thuộc bao lồi
    vector<Point> others = pointsOnHull(points, hull);  

    Output(others, hull, points);
    return 0;
}