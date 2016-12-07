#include "util.h"

float calcNormVect(float vx, float vy)
{
    return vx*vx+vy*vy;
}

float distanceCarre(float x1, float y1, float x2, float y2)
{
    return (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
}
