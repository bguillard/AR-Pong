#include "Player.h"



Player::~Player()
{
}

Player::Player(std::vector<cv::Point2f> Point2fs){
    
    this->points = Point2fs;
    
}

Player::Player()
{
    std::cout << "hello";
}


void Player::affiche(cv::Mat frame)
{
    /*
    clear(frame);
    prevShape = shape;
    for (size_t i = 0; i < this->shape.size(); i++)
    {
        cv::Point2f vtx[4];
        this->shape[i].points(vtx);
        for (int j = 0; j < 4; j++)
            line(frame, vtx[j], vtx[(j + 1) % 4], cvScalar(0, 255, 0), 1, cv::LINE_AA);
    }
    */
}


int Player::updatePos()
{
    return 0;
}

void Player::clear(cv::Mat frame)
{
    for (size_t i = 0; i < prevShape.size(); i++)
    {
        cv::Point2f vtx[4];
        prevShape[i].points(vtx);
        for (int j = 0; j < 4; j++)
            line(frame, vtx[j], vtx[(j + 1) % 4], cvScalar(255, 255, 255), 2, cv::LINE_AA);
    }
}

void Player::setShape(std::vector<cv::RotatedRect> s)
{
    this->shape = s;
}

// trace un polygone  qui suit les Point2fs du tableau en entrée
void Player::afficheConv(cv::Mat frame) {
    for (int i = 0; i < this->points.size(); i++) {
        int j = (i + 1) % points.size();
        line(frame, points[i] , points[j], cv::Scalar((double)0,(double) 0,(double) 255));
    }
}
// reconstitue le vecteur Point2f qui sera trace par affiche a partir de la sortie de convexHull (enveloppe convexe d'un nuage de Point2fs)
int Player::updatePosConv() {
    std::vector<int> next = convexHull(this->points);
    std::vector<cv::Point2f> temp(next.size());
    unsigned int i = 0, c=0;
    do {
        temp[c]=points[next[c]];
        c = next[c];
        i++;
    } while (i < points.size());
    this->points = temp;
    return 0;
}
// intermediaire pour convexHull
int Player::orientation(cv::Point2f const p, cv::Point2f const q, cv::Point2f const r)
{
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    
    if (val == 0)
        return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// Prints convex hull of a set of n Point2fs.
std::vector<int> Player::convexHull(const std::vector<cv::Point2f> pts)
{
    long n = pts.size();
    // There must be at least 3 Point2fs
    
    
    
    // Initialize Result
    std::vector<int> next(n);
    if (n < 3)
        return next;
    for (int i = 0; i < n; i++)
        next[i] = -1;
    
    // Find the leftmost Point2f
    int l = 0;
    for (int i = 1; i < n; i++) {
        if (pts[i].x < pts[l].x) {
            l = i;
        }
    }
    // Start from leftmost Point2f, keep moving counterclockwise
    // until reach the start Point2f again
    int p = l, q;
    do
    {
        // Search for a Point2f 'q' such that orientation(p, i, q) is
        // counterclockwise for all Point2fs 'i'
        q = (p + 1) % n;
        for (int i = 0; i < n; i++)
            if (Player::orientation(pts[p], pts[i], pts[q]) == 2) {
                q = i;
            }
        
        next[p] = q; // Add q to result as a next Point2f of p
        p = q; // Set p as q for next iteration
    } while (p != l);
    
    return next;
}




