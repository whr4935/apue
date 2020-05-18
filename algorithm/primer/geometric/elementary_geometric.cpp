#include <stdio.h>
#include "elementary_geometric.h"


typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Line {
    Point p1;
    Point p2;
} Line;

int ccw(Point p0, Point p1, Point p2)
{
    int dx1, dy1, dx2, dy2;

    dx1 = p1.x - p0.x;
    dy1 = p1.y - p0.y;
    dx2 = p2.x - p0.x;
    dy2 = p2.y - p0.y;

    if (dx1*dy2 > dx2*dy1) return 1;
    if (dx1*dy2 < dx2*dy1) return -1;
    if (dx1*dx2 < 0 || dy1*dy2 < 0) return -1;          //p0在中间
  //  if (dx1*dx1+dy1*dy1 < dx2*dx2+dy2*dy2) return -1;   //p1在中间
    if (dx1*dx2 > 0 || dy1*dy2 > 0) return 1;

    return 0;       //p2在中间(与p0或p1重叠也认为是在中间)
}

int line_intersect(Line l1, Line l2)
{
    return ccw(l1.p1, l1.p2, l2.p1) *
             ccw(l1.p1, l1.p2, l2.p2) <= 0 &&
            ccw(l2.p1, l2.p2, l1.p1) *
             ccw(l2.p1, l2.p2, l1.p2) <= 0;
}

void test_line_intersect()
{
    Line l1, l2;

    l1.p1.x = 1;
    l1.p1.y = 1;
    l1.p2.x = 3;
    l1.p2.y = 3;

    l2.p1.x = 6;
    l2.p1.y = 6;
    l2.p2.x = 5;
    l2.p2.y = 1;

    int ret = -1;
    ret = line_intersect(l1, l2);
    printf("lin_intersect, ret = %d\n", ret);

    ret = ccw(l1.p1, l1.p2, l2.p1);
    printf("test ccw, ret = %d\n", ret);
}


/////////////////////////////////////
void test_elementary_geometric()
{
    test_line_intersect();
}
