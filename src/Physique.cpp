#include "Physique.h"



Physique::Physique()
{
}


Physique::~Physique()
{
}

void Physique::updateObjets(std::vector<Balle>& balles, std::vector<Player>& players)
{
    int i,j;
    // On met à jour chaque balle
    
    for (i = 0; i < balles.size(); i++)
    {
        balles[i].updatePos();
    }
    // Methode safe pour la collision des balles //
    
    /*
     int i;
     int a = 0;
     
     // On met à jour chaque balle
     for (i = 0; i < balles.size(); i++)
     {
     cv::Point2f p = balles[i].Centre;
     balles[i].updatePos();
     
     // collision entre deux balles
     int a = Physique::collisionBalles(balles, i);
     if (a != -1)
					balles[i].Centre = p;
     while (a != -1) {
     
					float norme = ((balles[a].Centre.x - balles[i].Centre.x)*(balles[a].Centre.x - balles[i].Centre.x) + (balles[a].Centre.y - balles[i].Centre.y)*(balles[a].Centre.y - balles[i].Centre.y));
					float nx = (balles[a].Centre.x - balles[i].Centre.x) / norme;
					float ny = (balles[a].Centre.y - balles[i].Centre.y) / norme;
					balles[a].dirx = balles[a].dirx - 2 * (nx*balles[a].dirx + ny * balles[a].diry)*nx;
					balles[a].diry = balles[a].diry - 2 * (nx*balles[a].dirx + ny * balles[a].diry)*ny;
     
     
					nx = -nx;
					ny = -ny;
					balles[i].dirx = balles[i].dirx - 2 * (nx*balles[i].dirx + ny * balles[i].diry)*nx;
					balles[i].diry = balles[i].diry - 2 * (nx*balles[i].dirx + ny * balles[i].diry)*ny;
     
					balles[a].updatePos();
					balles[i].updatePos();
					a = Physique::collisionBalles(balles, i);
     }
     }
     */
    
    // Methode experimentale pour la collision des balles //
    
    // on va calculer le plus petit temps o˘ deux balles se sont collisionnÈes et traiter cette collision prioritairement,
    //puis rÈitÈrer jusqu'‡ ce qu'il n'y ait plus de collision entre balles
    float time = 2.f;
    do{
        time = 2.f;
        int col1, col2;
        for (i = 0; i < balles.size(); i++)
        {
            Balle b = balles[i];
            for (j = 0; j < i; j++) {
                Balle B = balles[j];
                if (collisionCercle(b, B)) {
                    float colTime = getColTime(b, B);
                    if (colTime < time) {
                        col1 = i;
                        col2 = j;
                        time = colTime;
                    }
                }
                
            }
        }
        if (time != 2) collide(balles[col1], balles[col2], time);
        
    }while (time != 2.f);
    // collision balle-rectangle
    for (int j = 0; j < balles.size(); j++) {
        bool collide;
        int compteur = 0;
        do {
            for (int p = 0; p < players.size(); p++) {
                // on parcourt chaque joueur (groupe de rectangles )
                for (int q = 0; q < players[p].shape.size(); q++) {
                    // On parcourt chaque rectangle
                    // Centre et angle pour la rotation du rectangle que l'on va remettre droit pour calculer la collision (le centre est la position actuelle)
                    cv::Point2f centre = balles[j].Centre;
                    float angle = players[p].shape[q].angle;
                    //on fait faire à la balle la même rotation
                    balles[j].lastPos = rotate(balles[j].Centre, balles[j].lastPos, angle);
                    // on récupère les 4 Point2f du rectangle
                    cv::Point2f pts[4];
                    players[p].shape[q].points(pts);
                    // on en fait tourner 3 ce qui suffit a construire son rotaté
                    //cv::Point2f p1 = rotate(balles[j].Centre, pts[0], angle);
                    //cv::Point2f p2 = rotate(balles[j].Centre, pts[1], angle);
                    //cv::Point2f p3 = rotate(balles[j].Centre, pts[2], angle);
                    // construction du rotaté
                    //cv::RotatedRect r = cv::RotatedRect(p1,p2,p3);
                    // methode alternative
                    cv::Point2f rotatedCenter = rotate(balles[j].Centre, players[p].shape[q].center, angle);
                    cv::RotatedRect r = cv::RotatedRect(rotatedCenter, players[p].shape[q].size, 0.0);
                    // rotation de la vitesse
                    cv::Point2f v(balles[j].dirx, balles[j].diry);
                    cv::Point2f vr = rotate(cv::Point2f(0, 0), v, angle);
                    balles[j].dirx = vr.x;
                    balles[j].diry = vr.y;
                    // collision
                    collide = collisionRect(balles[j], r);
                    // on retourne
                    vr = cv::Point2f(balles[j].dirx, balles[j].diry);
                    v = rotate(cv::Point2f(0, 0), vr, -angle);
                    balles[j].dirx = v.x;
                    balles[j].diry = v.y;
                    balles[j].lastPos = rotate(centre, balles[j].lastPos, -angle);
                    balles[j].Centre = rotate(centre, balles[j].Centre, -angle);
                }
                
            }
            
            compteur += 1;
        } while (collide && compteur < 5);
    }
}



int Physique::collisionBalles(Balle a, Balle b )
{
    
    float d = (a.Centre.x - b.Centre.x)*(a.Centre.x - b.Centre.x) + (a.Centre.y - b.Centre.y)*(a.Centre.y - b.Centre.y);
    d = sqrt(d);
    return (d < 2 * a.R) ? 1 : -1;
    
}
bool Physique::collisionRect(Balle &b, cv::RotatedRect r){
    cv::Point2f pts[4];
    r.points(pts);
    cv::Point2f a = pts[0];
    cv::Point2f start = b.lastPos, end = b.Centre;
    float radius = b.R;
    // chargement des grandeurs utiles : deplacement dx et dy, abscisse Left, Right ordonnee Bottom, Top
    float L = a.x;
    float T = a.y;
    float R = a.x;
    float B = a.y;
    for (int i = 0; i < 4; i++) {
        float x = pts[i].x;
        float y = pts[i].y;
        if (L > x) L = x;
        if (R < x) R = x;
        if (T > y)T = y;
        if (B < y)B = y;
    }
    float dx = b.Centre.x - b.lastPos.x;
    float dy = b.Centre.y - b.lastPos.y;
    
    float ltime = std::numeric_limits<float>::max();
    float rtime = std::numeric_limits<float>::max();
    float ttime = std::numeric_limits<float>::max();
    float btime = std::numeric_limits<float>::max();
    // computing colliding time for each side
    if (start.x + radius < L && end.x + radius > L ) {
        ltime = ((L - radius) - start.x) / dx;
    }
    if (start.x - radius > R && end.x - radius < R ) {
        rtime = ( (R + radius) - start.x) / dx;
    }
    if (start.y + radius < T && end.y + radius > T ) {
        ttime = ((T - radius) - start.y) / dy;
    }
    if (start.y - radius > B && end.y - radius < B ) {
        btime = ( (B + radius) - start.y) / dy;
    }
    
    // cases with a simple side-intersection
    // We check if t belongs to (0,1) and then if the center is between the bounds of the rectangle when colliding the side
    if (ltime >= 0.0f && ltime <= 1.0f) {
        float ly = dy * ltime + start.y;
        if (ly >= T && ly <= B) {
            float time = ltime;
            float ix = time * dx + start.x;
            float iy = time * dy + start.y;
            float nx = -1;
            float ny = 0;
            float remainingTime = 1.0f - time;
            // Point intermédiaire de la trajectoire (moment exact de collision)
            b.lastPos = cv::Point2f(ix, iy);
            float dot = dx * nx + dy * ny;
            float ndx = dx - 2 * dot * nx;
            float ndy = dy - 2 * dot * ny;
            float newx = ix + ndx * remainingTime;
            float newy = iy + ndy * remainingTime;
            b.lastPos = cv::Point2f(ix, iy);
            b.dirx = -b.dirx;
            b.Centre = cv::Point2f(newx, newy);
            return true;
        }
    }
    else if (rtime >= 0.0f && rtime <= 1.0f) {
        float ry = dy * rtime + start.y;
        if (ry >= T && ry <= B) {
            float time = rtime;
            float ix = time * dx + start.x;
            float iy = time * dy + start.y;
            float nx = 1;
            float ny = 0;
            float remainingTime = 1.0f - time;
            // Point2f intermédiaire de la trajectoire (moment exact de collision)
            b.lastPos = cv::Point2f(ix, iy);
            float dot = dx * nx + dy * ny;
            float ndx = dx - 2 * dot * nx;
            float ndy = dy - 2 * dot * ny;
            float newx = ix + ndx * remainingTime;
            float newy = iy + ndy * remainingTime;
            b.lastPos = cv::Point2f(ix, iy);
            b.dirx = -b.dirx;
            b.Centre = cv::Point2f(newx, newy);
            return true;
        }
    }
    
    if (ttime >= 0.0f && ttime <= 1.0f) {
        float tx = dx * ttime + start.x;
        if (tx >= L && tx <= R) {
            float time = ttime;
            float ix = time * dx + start.x;
            float iy = time * dy + start.y;
            float nx = 0;
            float ny = -1;
            float remainingTime = 1.0f - time;
            // Point2f intermédiaire de la trajectoire (moment exact de collision)
            b.lastPos = cv::Point2f(ix, iy);
            float dot = dx * nx + dy * ny;
            float ndx = dx - 2 * dot * nx;
            float ndy = dy - 2 * dot * ny;
            float newx = ix + ndx * remainingTime;
            float newy = iy + ndy * remainingTime;
            b.lastPos = cv::Point2f(ix, iy);
            b.diry = -b.diry;
            b.Centre = cv::Point2f(newx, newy);
            return true;
        }
    }
    else if (btime >= 0.0f && btime <= 1.0f) {
        float bx = dx * btime + start.x;
        if (bx >= L && bx <= R) {
            // Solve the intersection and normal
            float time = btime;
            float ix = time * dx + start.x;
            float iy = time * dy + start.y;
            float nx = 0;
            float ny = 1;
            float remainingTime = 1.0f - time;
            // Point2f intermédiaire de la trajectoire (moment exact de collision)
            float dot = dx * nx + dy * ny;
            float ndx = dx - 2 * dot * nx;
            float ndy = dy - 2 * dot * ny;
            float newx = ix + ndx * remainingTime;
            float newy = iy + ndy * remainingTime;
            b.lastPos = cv::Point2f(ix, iy);
            b.diry = -b.diry;
            b.Centre = cv::Point2f(newx, newy);
            return true;
        }
    }
    // if we are not in one of the previous cases, then either there's no collision or with a corner.
    // this code determines the corner collided.
    float cornerX = std::numeric_limits<float>::max();
    float cornerY = std::numeric_limits<float>::max();
    
    bool ledge = (end.x + radius >= L && end.x - radius <= L);
    bool redge = (end.x + radius >= R && end.x - radius <= R);
    bool tedge = (end.y + radius >= T && end.y - radius <= T);
    bool bedge = (end.y + radius >= B && end.y - radius <= B);
    // au cas o˘ la balle est plus grosse que le cÙtÈ du rectangle, on Èlimine un des deux angles du rectangle (ils seront tous les deux marques vrais)
    if (redge && ledge)
        b.dirx > 0 ? redge = false : ledge = false;
    if (tedge && bedge)
        b.diry > 0 ? bedge = false : tedge = false;
    if ((bedge || tedge) && (redge || ledge)) {
        cv::Point2f cRect = r.center;
        cornerX = redge ? R : L;
        cornerY = tedge ? T : B;
        // Vecteur normal ‡ l'angle (reprÈsentÈ comme un quart de cercle)
        float nx = cornerX - cRect.x;
        float ny = cornerY - cRect.y;
        float N = sqrt(nx*nx + ny * ny);
        // On normalise
        nx = nx / N;
        ny = ny / N;
        cv::Point2f c1;
        c1.x = cRect.x + nx * (N - radius);
        c1.y = cRect.y + ny * (N - radius);
        if (sqrt((c1.x - end.x)*(c1.x - end.x) + (c1.y - end.y)*(c1.y-end.y)) <= 2 * radius) {
            
            if (ledge) cornerX = L;
            if (redge) cornerX = R;
            if (tedge) cornerY = T;
            if (bedge) cornerY = B;
            
            
            
            float x = start.x;
            float y = start.y;
            float epsilon = 0.01;
            float dif = (sqrt((x - c1.x)*(x - c1.x) + (y - c1.y)*(y - c1.y)) - 2 * radius);
            while (dif > epsilon) {
                x = x + 0.01*b.dirx;
                y = y + 0.01*b.diry;
                dif = (sqrt((x - c1.x)*(x - c1.x) + (y - c1.y)*(y - c1.y)) - 2 * radius);
            }
            float lineLength = sqrt(dx * dx + dy * dy);
            float id = sqrt((x - start.x)*(x - start.x) + (y - start.y)*(y - start.y));
            b.lastPos = cv::Point2f(x, y);
            nx = x - c1.x;
            ny = y - c1.y;
            N = sqrt(nx*nx + ny*ny);
            nx = nx / N;
            ny = ny / N;
            float dot = nx * b.dirx + ny * b.diry;
            b.dirx = b.dirx - 2 * dot * nx;
            b.diry = b.diry - 2 * dot * ny;
            b.Centre.x = b.lastPos.x + (lineLength - id) / lineLength*b.dirx;
            b.Centre.y = b.lastPos.y + (lineLength - id) / lineLength*b.diry;
            return true;
            
        }
        
    }
    
    
    if (ltime != std::numeric_limits<float>::max() ) {
        cornerX = L;
    }
    else if (rtime != std::numeric_limits<float>::max()) {
        cornerX = R;
    }
    
    if (ttime != std::numeric_limits<float>::max()) {
        cornerY = T;
    }
    else if (btime != std::numeric_limits<float>::max()) {
        cornerY = B;
    }
    
    // Account for the times where we don't pass over a side but we do hit its corner
    if (cornerX != std::numeric_limits<float>::max() && cornerY == std::numeric_limits<float>::max()) {
        cornerY = (dy > 0.0f ? T : B);
    }
    
    if (cornerY != std::numeric_limits<float>::max() && cornerX == std::numeric_limits<float>::max()) {
        cornerX = (dx > 0.0f ? L : R);    }
    
    // calculus...
    double inverseRadius = 1.0 / radius;
    double lineLength = sqrt(dx * dx + dy * dy);
    double cornerdx = cornerX - start.x;
    double cornerdy = cornerY - start.y;
    double cornerdist = sqrt(cornerdx * cornerdx + cornerdy * cornerdy);
    double cosinnerAngle = (cornerdx * dx + cornerdy * dy) / (lineLength * cornerdist);
    double innerAngle = acos(cosinnerAngle);
    double innerAngleSin = sin(innerAngle);
    double angle1Sin = innerAngleSin * cornerdist * inverseRadius;
    
    //double angle1Sin = innerAngleSin * sqrt((end.x-cornerX)*(end.x-cornerX)+(end.y-cornerY)*(end.y-cornerY)) * inverseRadius;
    // The angle is too large, there cannot be an intersection
    if (angle1Sin > 1.0f || angle1Sin < -1.0f) {
        return false;
    }
    
    double angle1 = 3.1415 - asin(angle1Sin);
    double angle2 = 3.1415 - innerAngle - angle1;
    double intersectionDistance = radius * sin(angle2) / innerAngleSin;
    if(intersectionDistance <0)
        intersectionDistance = -intersectionDistance;
    
    // Solve for time
    float time = (float)(intersectionDistance / lineLength);
    
    // If time is outside the boundaries, return null. This algorithm can
    // return a negative time which indicates the previous intersection.
    if (time > 1.0f || time < 0.0f) {
        return false;
    }
    
    // Solve the intersection and normal
    float ix = time * dx + start.x;
    float iy = time * dy + start.y;
    float nx = (float)((ix - cornerX) * inverseRadius);
    float ny = (float)((iy - cornerY) * inverseRadius);
    float remainingTime = 1.0f - time;
    // Point2f intermÈdiaire de la trajectoire (moment exact de collision)
    b.lastPos = cv::Point2f(ix, iy);
    
    float scalarn = b.dirx * nx + b.diry * ny;
    float tx = -ny;
    float ty = nx;
    
    float scalart = b.dirx * tx + b.diry * ty;
    
    b.dirx = -scalarn * nx + scalart * tx;
    b.diry = -scalarn * ny + scalart * ty;
    
    float dot = dx * nx + dy * ny;
    float ndx = dx - 2 * dot * nx;
    float ndy = dy - 2 * dot * ny;
    float newx = ix + ndx * remainingTime;
    float newy = iy + ndy * remainingTime;
    b.Centre = cv::Point2f(newx, newy);
    return true;
    
    
}

cv::Point2f Physique::rotate(cv::Point2f c, cv::Point2f p, float a) {
    
    float angle = -a*(3.1415f / 180.f);
    float s = sin(angle);
    float d = cos(angle);
    
    // translate Point2f back to origin:
    p.x -= c.x;
    p.y -= c.y;
    
    // rotate Point2f
    float xnew = p.x * d - p.y * s;
    float ynew = p.x * s + p.y * d;
    
    // translate Point2f back:
    p.x = xnew + c.x;
    p.y = ynew + c.y;
    return cv::Point2f(p.x,p.y);
}






bool Physique::collisionCercle(Balle a, Balle b)
{
    float d = (a.Centre.x - b.Centre.x)*(a.Centre.x - b.Centre.x) + (a.Centre.y - b.Centre.y)*(a.Centre.y - b.Centre.y);
    d = sqrt(d);
    return (d < 2*a.R);
}


float Physique:: getColTime(Balle a, Balle b) {
    
    float ax = a.Centre.x;
    float ay = a.Centre.y;
    float bx = b.Centre.x;
    float by = b.Centre.y;
    float epsilon = 0.01;
    int t = 0.01;
    float dif = (sqrt((ax - bx)*(ax - bx) + (ay - by)*(ay - by)) - 2 *a.R);
    while (dif > epsilon) {
        ax = ax + 0.01*a.dirx;
        ay = ay + 0.01*a.diry;
        bx = bx + 0.01*b.dirx;
        by = by + 0.01*b.diry;
        dif = (sqrt((ax - bx)*(ax - bx) + (ay - by)*(ay - by)) - 2 * a.R);
    }
    return t;
}

void Physique::collide(Balle a, Balle b, float t) {
    
    a.lastPos = cv::Point2f(a.lastPos.x + t*a.dirx, a.lastPos.y + t*a.diry);
    b.lastPos = cv::Point2f(b.lastPos.x + t*b.dirx, b.lastPos.y + t*b.diry);
    float nx = a.lastPos.x - b.lastPos.x;
    float ny = a.lastPos.y - b.lastPos.y;
    float adot = a.dirx * nx + a.diry * ny;
    float bdot = a.diry * ny + b.dirx * nx;
    
    b.dirx = b.dirx - 2 * nx * bdot;
    b.diry = b.diry - 2 * ny * bdot;
    
    a.dirx = a.dirx + 2 * nx * adot;
    a.diry = a.diry + 2 * ny * adot;
    
    a.Centre = cv::Point2f(a.lastPos.x + a.dirx * (1 - t), a.lastPos.y + a.diry * (1 - t));
    b.Centre = cv::Point2f(b.lastPos.x + b.dirx * (1 - t), b.lastPos.y + b.diry * (1 - t));
    
    
}
