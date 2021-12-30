/* common.h */
#ifndef __COMMON_H__
#define __COMMON_H__

#define NNODES_ELEM 4
#define EPS 1e-6

// common.c
int int_cmp (const void *v0, const void *v1);
int edge_cmp (const void *v0, const void *v1);
int tri_cmp (const void *v0, const void *v1);
int nearly (double mx, double my, double mz, double x, double y, double z);
void fill_bound (Mesh *msh, double *mx, double *my, double *mz, int xflag, int yflag, int zflag);

void set_edge (Edge *e, int n0, int n1);
void set_triangle (Triangle *tri, int n0, int n1, int n2);

int get_coord (Mesh *omsh, MatchedMesh *mmesh, int nodeId, double *x, double *y, double *z);

int is_clockwise(Mesh *omsh, MatchedMesh *mmesh, int *element);

// bounding box
void set_bounding_box (BoundingBox *bbox, double x0, double y0, double z0, double x1, double y1, double z1);
int check_inside (BoundingBox *bbox, double x, double y, double z);

#endif /* __COMMON_H__ */
