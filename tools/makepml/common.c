/* common.c */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pml_types.h"
#include "common.h"

const int nblockelements = 24;
const int dindex[24][4] = {
  // 2 cpoints and a block edge.
  // around xaxis
  {12, 10,  0,  4},
  {10, 13,  1,  5},
  {13, 11,  3,  7},
  {11, 12,  2,  6},
  // around yaxis
  { 9, 13,  5,  7},
  {13,  8,  1,  3},
  { 8, 12,  0,  2},
  {12,  9,  4,  6},
  // around zaxis
  { 8, 11,  2,  3},
  {11,  9,  6,  7},
  { 9, 10,  4,  5},
  {10,  8,  0,  1},
  // 1 block vertex and 3 cpoints
  { 0,  8, 10, 12},
  { 1,  8, 13, 10},
  { 2,  8, 12, 11},
  { 3,  8, 11, 13},
  { 4,  9, 12, 10},
  { 5,  9, 10, 13},
  { 6,  9, 11, 12},
  { 7,  9, 13, 11},
  // 4 cpoints
  { 8,  9, 10, 12},
  { 8,  9, 12, 11},
  { 8,  9, 11, 13},
  { 8,  9, 13, 10},
};

void set_bounding_box (BoundingBox *bbox, double x0, double y0, double z0, double x1, double y1, double z1) {
  bbox->xmin = x0<x1 ? x0 : x1;
  bbox->xmax = x0<x1 ? x1 : x0;
  bbox->ymin = y0<y1 ? y0 : y1;
  bbox->ymax = y0<y1 ? y1 : y0;
  bbox->zmin = z0<z1 ? z0 : z1;
  bbox->zmax = z0<z1 ? z1 : z0;
}

int check_inside (BoundingBox *bbox, double x, double y, double z) {
  static int eps = 1e-9;
  
  int ret;
  
  ret = 
    bbox->xmin-eps <= x && x <= bbox->xmax+eps &&
    bbox->ymin-eps <= y && y <= bbox->ymax+eps &&
    bbox->zmin-eps <= z && z <= bbox->zmax+eps;

  assert(ret != 0);
  return ret;
}

/**
 * comparation function of int for qsort() and bsearch().
 */
int int_cmp (const void *v0, const void *v1) {
  const int *i0 = (const int *)v0;
  const int *i1 = (const int *)v1;
  
  return *i0 - *i1;
}

/**
 * comparation function of Edge for qsort() and bsearch().
 */
int edge_cmp (const void *v0, const void *v1) {
  const Edge *e0 = (const Edge *)v0;
  const Edge *e1 = (const Edge *)v1;

  int min0, max0;
  int min1, max1;

  if (e0->node_id[0] < e0->node_id[1]) {
    min0 = e0->node_id[0];
    max0 = e0->node_id[1];
  } else {
    max0 = e0->node_id[0];
    min0 = e0->node_id[1];
  }

  if (e1->node_id[0] < e1->node_id[1]) {
    min1 = e1->node_id[0];
    max1 = e1->node_id[1];
  } else {
    max1 = e1->node_id[0];
    min1 = e1->node_id[1];
  }

  int delta = min0 - min1;
  if (delta == 0) {
    delta = max0 - max1;
  }
  return delta;
}

/**
 * comparation function of Triangle for qsort() and bsearch().
 */
int tri_cmp (const void *v0, const void *v1) {
  const Triangle *t0 = (const Triangle *)v0;
  const Triangle *t1 = (const Triangle *)v1;
  
  int delta = t0->node_id[0] - t1->node_id[0];
  if (delta == 0) {
    delta = t0->node_id[1] - t1->node_id[1];
  }
  if (delta == 0) {
    delta = t0->node_id[2] - t1->node_id[2];
  }
  return delta;
}

/**
 * check 2 coordinates are very near or not.
 */
int nearly (double mx, double my, double mz, double x, double y, double z) {
  if (-EPS < mx-x && mx-x < EPS &&
      -EPS < my-y && my-y < EPS &&
      -EPS < mz-z && mz-z < EPS) {
    return 1;
  }
  return 0;
}

/**
 * get value for flag.
 * @param flag    value type to get. =-1:min, =1:max, other=mean(=(min+max)/2)
 * @param min     minimum value
 * @param max     maximum value
 */
static double fill_coord_for (int flag, double min, double max) {
  switch (flag) {
  case -1:
    return min;
  case 1:
    return max;
  default:
    return (min+max)/2.0;
  }
}

void fill_bound (Mesh *msh, double *mx, double *my, double *mz, int xflag, int yflag, int zflag) {
  *mx = fill_coord_for (xflag, msh->bbox.xmin, msh->bbox.xmax);
  *my = fill_coord_for (yflag, msh->bbox.ymin, msh->bbox.ymax);
  *mz = fill_coord_for (zflag, msh->bbox.zmin, msh->bbox.zmax);
}

void set_edge (Edge *e, int n0, int n1) {
  e->node_id[0] = n0;
  e->node_id[1] = n1;

  e->triangle[0] = NULL;
  e->triangle[1] = NULL;
}

void set_triangle (Triangle *tri, int n0, int n1, int n2) {
  if (n0 < n1) {
    if (n1 < n2) {
      // n0 < n1 < n2
      tri->node_id[0] = n0;
      tri->node_id[1] = n1;
      tri->node_id[2] = n2;
      tri->reverse_flag = 0;
    } else {
      if (n0 < n2) {
        // n0 < n2 < n1
        tri->node_id[0] = n0;
        tri->node_id[1] = n2;
        tri->node_id[2] = n1;
        tri->reverse_flag = 1;
      } else {
        // n2 < n0 < n1
        tri->node_id[0] = n2;
        tri->node_id[1] = n0;
        tri->node_id[2] = n1;
        tri->reverse_flag = 0;
      }
    }
  } else {
    // n1 < n0
    if (n2 < n1) {
      // n2 < n1 < n0
      tri->node_id[0] = n2;
      tri->node_id[1] = n1;
      tri->node_id[2] = n0;
      tri->reverse_flag = 1;
    } else {
      // n1<n0 & n1<n2
      if (n0 < n2) {
        // n1 < n0 < n2
        tri->node_id[0] = n1;
        tri->node_id[1] = n0;
        tri->node_id[2] = n2;
        tri->reverse_flag = 1;
      } else {
        // n1 < n2 < n0
        tri->node_id[0] = n1;
        tri->node_id[1] = n2;
        tri->node_id[2] = n0;
        tri->reverse_flag = 0;
      }
    }
  }

  tri->edge[0] = NULL;
  tri->edge[1] = NULL;
  tri->edge[2] = NULL;
}

int get_coord (Mesh *omsh, MatchedMesh *mmesh, int nodeId, double *x, double *y, double *z) {
  Mesh *msh;
  VertexMesh *vmesh;
  EdgeMesh *emesh;
  FaceMesh *smesh;

  int ix, iy, iz;
  int iaxis, iv, iw;
  int iside;
  int id;

  if (0 <= nodeId && nodeId < omsh->nnodes) {
    *x = omsh->node[nodeId*3];
    *y = omsh->node[nodeId*3+1];
    *z = omsh->node[nodeId*3+2];
    return 0;
  }

  // vertex
  for (ix = 0; ix < 2; ix++) {
    for (iy = 0; iy < 2; iy++) {
      for (iz = 0; iz < 2; iz++) {
	if (mmesh->vmesh[ix][iy][iz].is_generated) {
	  vmesh = &(mmesh->vmesh[ix][iy][iz]);
	  msh = &(vmesh->mesh);
	  id = nodeId - vmesh->node_offset;
	  if (0 <= id && id < msh->nnodes) {
	    *x = msh->node[id*3];
	    *y = msh->node[id*3+1];
	    *z = msh->node[id*3+2];
	    return 0;
	  }
	}
      }
    }
  }

  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iv = 0; iv < 2; iv++) {
      for (iw = 0; iw < 2; iw++) {
	if (mmesh->emesh[iaxis][iv][iw].is_generated) {
	  emesh = &(mmesh->emesh[iaxis][iv][iw]);
	  msh = &(emesh->mesh);
	  id = nodeId - emesh->node_offset;
	  if (0 <= id && id < msh->nnodes) {
	    *x = msh->node[id*3];
	    *y = msh->node[id*3+1];
	    *z = msh->node[id*3+2];
	    return 0;
	  }
	}
      }
    }
  }

  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iside = 0; iside < 2; iside++) {
      if (mmesh->smesh[iaxis][iside].is_generated) {
	smesh = &(mmesh->smesh[iaxis][iside]);
	msh = &(smesh->mesh);
	id = nodeId - smesh->node_offset;
	if (0 <= id && id < msh->nnodes) {
	  *x = msh->node[id*3];
	  *y = msh->node[id*3+1];
	  *z = msh->node[id*3+2];
	  return 0;
	}
      }
    }
  }
  
  assert(0);

  *x = *y = *z = 0.0;
  return -1;
}

int is_clockwise (Mesh *omsh, MatchedMesh *mmesh, int *element) {
  double x[4], y[4], z[4];

  double x12, y12, z12;
  double x13, y13, z13;
  double a, b, c, d;

  int inode;

  for (inode = 0; inode < 4; inode++) {
    get_coord (omsh, mmesh, element[inode], x+inode, y+inode, z+inode);
  }


  x12 = x[2] - x[1];
  y12 = y[2] - y[1];
  z12 = z[2] - z[1];
  x13 = x[3] - x[1];
  y13 = y[3] - y[1];
  z13 = z[3] - z[1];
  a = y12*z13 - y13*z12;
  b = z12*x13 - z13*x12;
  c = x12*y13 - x13*y12;
  d = -(a*x[1]+b*y[1]+c*z[1]);
  
  return (a*x[0]+b*y[0]+c*z[0]+d <= 0.0);
}
