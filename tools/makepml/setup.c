/* setup.c */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pml.h"
#include "common.h"

static void detect_boundary_box (Mesh *msh) {
  BoundingBox bbox;

  int inode;
  double x, y, z;
  
  bbox.xmin = bbox.xmax = msh->node[0];
  bbox.ymin = bbox.ymax = msh->node[1];
  bbox.zmin = bbox.zmax = msh->node[2];

  for (inode = 1; inode < msh->nnodes; inode++) {
    x = msh->node[inode*3];
    y = msh->node[inode*3+1];
    z = msh->node[inode*3+2];

    bbox.xmin = (bbox.xmin < x) ? bbox.xmin : x;
    bbox.ymin = (bbox.ymin < y) ? bbox.ymin : y;
    bbox.zmin = (bbox.zmin < z) ? bbox.zmin : z;
    bbox.xmax = (bbox.xmax > x) ? bbox.xmax : x;
    bbox.ymax = (bbox.ymax > y) ? bbox.ymax : y;
    bbox.zmax = (bbox.zmax > z) ? bbox.zmax : z;
  }

  msh->bbox = bbox;

#ifdef DEBUG
  fprintf (stderr, "x=(%le %le), y=(%le %le), z=(%le %le)\n", bbox.xmin, bbox.xmax, bbox.ymin, bbox.ymax, bbox.zmin, bbox.zmax);
#endif
}

static int inside (const BoundingBox *bbox, double x, double y, double z) {
  if (bbox->xmin-EPS <= x && x <= bbox->xmax &&
      bbox->ymin-EPS <= y && y <= bbox->ymax &&
      bbox->zmin-EPS <= z && z <= bbox->zmax) {
    return 1;
  }
  return 0;
}

static void detect_fgrnodes (FaceGroup *fgr, Mesh *msh, BoundingBox *bbox) {
  int count;

  int inode;
  double x, y, z;
  
  count = 0;
  for (inode = 0; inode < msh->nnodes ;inode++) {
    x = msh->node[inode*3];
    y = msh->node[inode*3+1];
    z = msh->node[inode*3+2];

    if (inside (bbox, x, y, z)) {
      count++;
    }
  }

  fgr->nnodes = count;
  fgr->node_id = (int *)malloc (fgr->nnodes * sizeof (int));
  assert(fgr->node_id != NULL);

  count = 0;
  for (inode = 0; inode < msh->nnodes ;inode++) {
    x = msh->node[inode*3];
    y = msh->node[inode*3+1];
    z = msh->node[inode*3+2];

    if (inside (bbox, x, y, z)) {
      fgr->node_id[count] = inode;
      count++;
    }
  }
  assert(count == fgr->nnodes);

  qsort (fgr->node_id, fgr->nnodes, sizeof (int), int_cmp);
}

int face_index[4][3] = {
  {1,2,3},
  {2,3,0},
  {3,0,1},
  {0,1,2}
};

static void fill_triangle (Triangle *tri, int *element, int iface) {
  int n0, n1, n2;

  n0 = element[face_index[iface][0]];
  n1 = element[face_index[iface][1]];
  n2 = element[face_index[iface][2]];

  set_triangle (tri, n0, n1, n2);
}

static void detect_faces (FaceGroup *fgr, Mesh *msh) {
  int ielem;
  int iface;
  int inode;
  
  int *element;
  int node_id;

  int face_flag[NNODES_ELEM];
  int count;

  // count
  count = 0;
  for (ielem = 0; ielem < msh->nelements; ielem++) {
    element = msh->element + ielem * NNODES_ELEM;
    
    for (inode = 0; inode < NNODES_ELEM; inode++) {
      node_id = element[inode];
      
      if (bsearch (&node_id, fgr->node_id, fgr->nnodes, sizeof (int), int_cmp) == NULL) {
        face_flag[inode] = 0;
      } else {
        face_flag[inode] = 1;
      }
    }
    
    for (iface = 0; iface < 4; iface++) {
      if (face_flag[face_index[iface][0]] &&
          face_flag[face_index[iface][1]] &&
          face_flag[face_index[iface][2]]) {

#ifdef DEBUG
	fprintf (stderr, "detect_faces(): element[%d][%d] is on face [%d].\n", ielem, iface, count);
#endif

        count++;
      } else {

#ifdef DEBUG
	fprintf (stderr, "detect_faces(): element[%d][%d] is not on face [%d].\n", ielem, iface, count);
#endif

      }
    }
  }
  
  fgr->ntriangles = count;
  fgr->triangle = (Triangle *)malloc (fgr->ntriangles * sizeof (Triangle));
  assert(fgr->triangle != NULL);

  // fill
  count = 0;
  for (ielem = 0; ielem < msh->nelements; ielem++) {
    element = msh->element + ielem * NNODES_ELEM;
    
    for (inode = 0; inode < NNODES_ELEM; inode++) {
      node_id = element[inode];
      
      if (bsearch (&node_id, fgr->node_id, fgr->nnodes, sizeof (int), int_cmp) == NULL) {
        face_flag[inode] = 0;
      } else {
        face_flag[inode] = 1;
      }
    }
    
    for (iface = 0; iface < 4; iface++) {
      if (face_flag[face_index[iface][0]] &&
          face_flag[face_index[iface][1]] &&
          face_flag[face_index[iface][2]]) {
        fill_triangle (fgr->triangle + count, element, iface);
        count++;
      }
    }
  }
  assert(count == fgr->ntriangles);

  qsort (fgr->triangle, fgr->ntriangles, sizeof (Triangle), tri_cmp);
}

int edge_index[3][2] = {
  {0, 1},
  {1, 2},
  {2, 0},
};

static void detect_edges (FaceGroup *fgr) {
  Edge *tmp_edge;
  Edge *found;
  int itri;
  int iedge;
  int n0, n1;

  int count;
  int index;
  int bnd_count;
  int nbedges;
  int ix, iy;
  int ibnd;

  Edge *bedge;
  
  fgr->nedges = -1;

  tmp_edge = (Edge *)malloc (fgr->ntriangles * 3 * sizeof (Edge));
  assert(tmp_edge != NULL);

  for (itri = 0; itri < fgr->ntriangles; itri++) {
    for (iedge = 0; iedge < 3; iedge++) {
      n0 = fgr->triangle[itri].node_id[edge_index[iedge][0]];
      n1 = fgr->triangle[itri].node_id[edge_index[iedge][1]];

      if (n0 < n1) {
        tmp_edge[itri*3+iedge].node_id[0] = n0;
        tmp_edge[itri*3+iedge].node_id[1] = n1;
      } else {
        tmp_edge[itri*3+iedge].node_id[0] = n1;
        tmp_edge[itri*3+iedge].node_id[1] = n0;
      }
      tmp_edge[itri*3+iedge].triangle[0] = fgr->triangle + itri;
      tmp_edge[itri*3+iedge].triangle[1] = NULL;
    }
  }
  
  qsort (tmp_edge, fgr->ntriangles * 3, sizeof (Edge), edge_cmp);

  // count
  count = 0;

#ifdef DEBUG
      fprintf (stderr, "node=(%d,%d) : count=%d(%s/%s)\n",
               tmp_edge[count].node_id[0], tmp_edge[count].node_id[1],
               count,
               (tmp_edge[count].triangle[0]!=NULL)?"*":"-",
               (tmp_edge[count].triangle[1]!=NULL)?"*":"-");
#endif

  for (iedge = 1; iedge < fgr->ntriangles * 3; iedge++) {
    if (tmp_edge[count].node_id[0] == tmp_edge[iedge].node_id[0] &&
        tmp_edge[count].node_id[1] == tmp_edge[iedge].node_id[1]) {

#ifdef DEBUG
      fprintf (stderr, "node=(%d,%d) : count=%d(%s/%s), iedge=%d(%s/%s)\n",
               tmp_edge[count].node_id[0], tmp_edge[count].node_id[1],
               count,
               (tmp_edge[count].triangle[0]!=NULL)?"*":"-",
               (tmp_edge[count].triangle[1]!=NULL)?"*":"-",
               iedge,
               (tmp_edge[iedge].triangle[0]!=NULL)?"*":"-",
               (tmp_edge[iedge].triangle[1]!=NULL)?"*":"-");
#endif

      
      assert(tmp_edge[count].triangle[1] == NULL);

      tmp_edge[count].triangle[1] = tmp_edge[iedge].triangle[0];
    } else {

#ifdef DEBUG
      fprintf (stderr, "node=(%d,%d) : count=%d <- iedge=%d(%s/%s)\n",
               tmp_edge[iedge].node_id[0], tmp_edge[iedge].node_id[1],
               count+1,
               iedge,
               (tmp_edge[iedge].triangle[0]!=NULL)?"*":"-",
               (tmp_edge[iedge].triangle[1]!=NULL)?"*":"-");
#endif
      
      count++;
      tmp_edge[count].node_id[0] = tmp_edge[iedge].node_id[0];
      tmp_edge[count].node_id[1] = tmp_edge[iedge].node_id[1];
      tmp_edge[count].triangle[0] = tmp_edge[iedge].triangle[0];
      tmp_edge[count].triangle[1] = NULL;
    }
  }
  count++;

  nbedges = fgr->boundary[0][0]->nedges +
    fgr->boundary[0][1]->nedges +
      fgr->boundary[1][0]->nedges +
        fgr->boundary[1][1]->nedges; 

#ifdef DEBUG
  fprintf (stderr, "count=%d -> ntriangles=%d, nbedges=%d\n", count, fgr->ntriangles, nbedges);
#endif

  assert(count*2 == fgr->ntriangles*3+nbedges);

  bnd_count = 0;
  for (ix = 0; ix < 2; ix++) {
    for (iy = 0; iy < 2; iy++) {
      for (ibnd = 0; ibnd < fgr->boundary[ix][iy]->nedges; ibnd++) {
        bedge = fgr->boundary[ix][iy]->edge + ibnd;
        found = bsearch (bedge, tmp_edge, count, sizeof (Edge), edge_cmp);

        assert(found != NULL);
        assert(found->triangle[1] == NULL);
        assert(bedge->triangle[1] == NULL);
        
        if (bedge->triangle[0] == NULL) {
          bedge->triangle[0] = found->triangle[0];
          found->triangle[0] = NULL;
          bnd_count++;
        } else if (bedge->triangle[1] == NULL) {
          bedge->triangle[1] = found->triangle[0];
          found->triangle[0] = NULL;
          bnd_count++;
        } else {
          fprintf (stderr, "ERROR: boundary edge already has 2 triangles and added 1 more triangle...\n");
          exit (1);
        }
      }
    }
  }
  assert(bnd_count == nbedges);

  fgr->nedges = count - bnd_count;
  fgr->edge = (Edge *)malloc (fgr->nedges * sizeof (Edge));
  assert(fgr->edge != NULL);

  index = 0;
  for (iedge = 0; iedge < count; iedge++) {
    if (tmp_edge[iedge].triangle[0] != NULL) {
      fgr->edge[index] = tmp_edge[iedge];
      index++;
    }
  }
  assert(index == fgr->nedges);

  free (tmp_edge);
  /*
  // reorder edge->node_id
  if (fgr->nedges > 1) {
    int start_node;

    if (fgr->edge[0].node_id[0] == fgr->edge[1].node_id[0] ||
	fgr->edge[0].node_id[0] == fgr->edge[1].node_id[1]) {
      start_node = fgr->edge[0].node_id[0];
      fgr->edge[0].node_id[0] = fgr->edge[0].node_id[1];
      fgr->edge[0].node_id[1] = start_node;
    } else {
      start_node = fgr->edge[0].node_id[1];
    }

    for (iedge = 1; iedge < fgr->nedges; iedge++) {
      if (start_node == fgr->edge[iedge].node_id[1]) {
	// prev -> node[1]/node[0] -> next : swap
	start_node = fgr->edge[iedge].node_id[0];
	fgr->edge[iedge].node_id[0] = fgr->edge[iedge].node_id[1];
	fgr->edge[iedge].node_id[1] = start_node;
      } else {
	// prev -> node[0]/node[1] -> next : no swap
	start_node = fgr->edge[0].node_id[1];
      }
    }
  }
  */
}

static void build_face_group (FaceGroup *fgr, Mesh *msh, BoundingBox *bbox) {
  detect_fgrnodes (fgr, msh, bbox);
  detect_faces (fgr, msh);
  detect_edges (fgr);
}

static void setup_face_group (Mesh *msh, Surface *surface) {
  BoundingBox bbox;
  FaceGroup *fgr;
  
  // -x
  bbox = msh->bbox;
  bbox.xmax = msh->bbox.xmin;
  fgr = &(surface->fgr[0][0]);
  fgr->boundary[0][0] = &(surface->egr[1][0][0]);
  fgr->boundary[0][1] = &(surface->egr[1][1][0]);
  fgr->boundary[1][0] = &(surface->egr[2][0][0]);
  fgr->boundary[1][1] = &(surface->egr[2][0][1]);
  build_face_group (fgr, msh, &bbox);

#ifdef DEBUG
  fprintf (stdout, "setup : fgr[%d][%d].ntriangles=%d, nedges=%d\n", 0, 0, fgr->ntriangles, fgr->nedges);
#endif

  // +x
  bbox = msh->bbox;
  bbox.xmin = msh->bbox.xmax;
  fgr = &(surface->fgr[0][1]);
  fgr->boundary[0][0] = &(surface->egr[1][0][1]);
  fgr->boundary[0][1] = &(surface->egr[1][1][1]);
  fgr->boundary[1][0] = &(surface->egr[2][1][0]);
  fgr->boundary[1][1] = &(surface->egr[2][1][1]);
  build_face_group (fgr, msh, &bbox);

#ifdef DEBUG
    fprintf (stdout, "setup : fgr[%d][%d].ntriangles=%d, nedges=%d\n", 0, 1, fgr->ntriangles, fgr->nedges);
#endif

  // -y
  bbox = msh->bbox;
  bbox.ymax = msh->bbox.ymin;
  fgr = &(surface->fgr[1][0]);
  fgr->boundary[0][0] = &(surface->egr[2][0][0]);
  fgr->boundary[0][1] = &(surface->egr[2][1][0]);
  fgr->boundary[1][0] = &(surface->egr[0][0][0]);
  fgr->boundary[1][1] = &(surface->egr[0][0][1]);
  build_face_group (fgr, msh, &bbox);

#ifdef DEBUG
  fprintf (stdout, "setup : fgr[%d][%d].ntriangles=%d, nedges=%d\n", 1, 0, fgr->ntriangles, fgr->nedges);
#endif

  // +y
  bbox = msh->bbox;
  bbox.ymin = msh->bbox.ymax;
  fgr = &(surface->fgr[1][1]);
  fgr->boundary[0][0] = &(surface->egr[2][0][1]);
  fgr->boundary[0][1] = &(surface->egr[2][1][1]);
  fgr->boundary[1][0] = &(surface->egr[0][1][0]);
  fgr->boundary[1][1] = &(surface->egr[0][1][1]);
  build_face_group (fgr, msh, &bbox);

#ifdef DEBUG
  fprintf (stdout, "setup : fgr[%d][%d].ntriangles=%d, nedges=%d\n", 1, 1, fgr->ntriangles, fgr->nedges);
#endif

  // -z
  bbox = msh->bbox;
  bbox.zmax = msh->bbox.zmin;
  fgr = &(surface->fgr[2][0]);
  fgr->boundary[0][0] = &(surface->egr[0][0][0]);
  fgr->boundary[0][1] = &(surface->egr[0][1][0]);
  fgr->boundary[1][0] = &(surface->egr[1][0][0]);
  fgr->boundary[1][1] = &(surface->egr[1][0][1]);
  build_face_group (fgr, msh, &bbox);

#ifdef DEBUG
  fprintf (stdout, "setup : fgr[%d][%d].ntriangles=%d, nedges=%d\n", 2, 0, fgr->ntriangles, fgr->nedges);
#endif

  // +z
  bbox = msh->bbox;
  bbox.zmin = msh->bbox.zmax;
  fgr = &(surface->fgr[2][1]);
  fgr->boundary[0][0] = &(surface->egr[0][0][1]);
  fgr->boundary[0][1] = &(surface->egr[0][1][1]);
  fgr->boundary[1][0] = &(surface->egr[1][1][0]);
  fgr->boundary[1][1] = &(surface->egr[1][1][1]);
  build_face_group (fgr, msh, &bbox);

#ifdef DEBUG
  fprintf (stdout, "setup : fgr[%d][%d].ntriangles=%d, nedges=%d\n", 2, 1, fgr->ntriangles, fgr->nedges);
#endif
}

static void setup_vertex (Mesh *msh, Surface *surface) {
  int ix, iy, iz;
  double mx, my, mz;
  double x, y, z;
  int inode;
  int xdir, ydir, zdir;
  
  for (ix = 0; ix < 2; ix++) {
    for (iy = 0; iy < 2; iy++) {
      for (iz = 0; iz < 2; iz++) {
        surface->vertex[ix][iy][iz] = -1;
      }
    }
  }
  
  for (inode = 0; inode < msh->nnodes; inode++) {
    x = msh->node[inode*3];
    y = msh->node[inode*3+1];
    z = msh->node[inode*3+2];

    for (ix = 0; ix < 2; ix++) {
      xdir = ix*2-1;
      for (iy = 0; iy < 2; iy++) {
        ydir = iy*2-1;
        for (iz = 0; iz < 2; iz++) {
          zdir = iz*2-1;
          fill_bound (msh, &mx, &my, &mz, xdir, ydir, zdir);
          if (nearly (mx, my, mz, x, y, z)) {
            assert(surface->vertex[ix][iy][iz] < 0);
            surface->vertex[ix][iy][iz] = inode;
          }
        }
      }
    }
  }

#ifdef DEBUG
  for (ix = 0; ix < 2; ix++) {
    for (iy = 0; iy < 2; iy++) {
      for (iz = 0; iz < 2; iz++) {
        fprintf (stdout, "setup : vertex[%d][%d][%d] = %d\n", ix, iy, iz, surface->vertex[ix][iy][iz]);
      }
    }
  }
#endif
}

typedef struct {
  int id;
  double crd[3];
} NodeCoord;

static int node_crd_cmp (const void *v0, const void *v1, int axis) {
  const NodeCoord *n0 = (const NodeCoord *)v0;
  const NodeCoord *n1 = (const NodeCoord *)v1;

  return (n0->crd[axis] < n1->crd[axis]) ? -1 : 1;
}

static int node_crd_cmp0 (const void *v0, const void *v1) {
  return node_crd_cmp (v0, v1, 0);
}
static int node_crd_cmp1 (const void *v0, const void *v1) {
  return node_crd_cmp (v0, v1, 1);
}
static int node_crd_cmp2 (const void *v0, const void *v1) {
  return node_crd_cmp (v0, v1, 2);
}

static void fill_boundary_edge (Mesh *msh, BoundingBox *bbox,  EdgeGroup *egr, int axis) {
  int inode;
  double x, y, z;

  int iedge;
  
  int nnodes;
  NodeCoord *node;

  int count;
  
  // count
  count = 0;
  for (inode = 0; inode < msh->nnodes; inode++) {
    x = msh->node[inode*3];
    y = msh->node[inode*3+1];
    z = msh->node[inode*3+2];

    if (inside (bbox, x, y, z)) {
      count++;
    }
  }

  nnodes = count;
  node = (NodeCoord *)malloc (nnodes * sizeof (NodeCoord));
  assert(node != NULL);
  
  // fill
  count = 0;
  for (inode = 0; inode < msh->nnodes; inode++) {
    x = msh->node[inode*3];
    y = msh->node[inode*3+1];
    z = msh->node[inode*3+2];

    if (inside (bbox, x, y, z)) {
      node[count].id = inode;
      node[count].crd[0] = x;
      node[count].crd[1] = y;
      node[count].crd[2] = z;
      count++;
    }
  }
  assert(count == nnodes);

  switch (axis) {
  case 0:
    qsort (node, nnodes, sizeof (NodeCoord), node_crd_cmp0);
    break;
  case 1:
    qsort (node, nnodes, sizeof (NodeCoord), node_crd_cmp1);
    break;
  case 2:
    qsort (node, nnodes, sizeof (NodeCoord), node_crd_cmp2);
    break;
  default:
    fprintf (stderr, "ERROR : irregal axis[%d] is set!\n", axis);
    exit (1);
  }

  // build EdgeGroup
  egr->nedges = nnodes - 1;
  egr->edge = (Edge *)malloc (egr->nedges * sizeof (Edge));

  for (iedge = 0; iedge < egr->nedges; iedge++) {
    if (node[iedge].id < node[iedge+1].id) {
      egr->edge[iedge].node_id[0] = node[iedge].id;
      egr->edge[iedge].node_id[1] = node[iedge+1].id;
    } else {
      egr->edge[iedge].node_id[0] = node[iedge+1].id;
      egr->edge[iedge].node_id[1] = node[iedge].id;
    }
    egr->edge[iedge].triangle[0] = NULL;
    egr->edge[iedge].triangle[1] = NULL;
  }
  
  free (node);

  // reorder edge->node_id
  if (egr->nedges > 1) {
    int start_node;
    
    if (egr->edge[0].node_id[0] == egr->edge[1].node_id[0] ||
	egr->edge[0].node_id[0] == egr->edge[1].node_id[1]) {
      start_node = egr->edge[0].node_id[0];
      egr->edge[0].node_id[0] = egr->edge[0].node_id[1];
      egr->edge[0].node_id[1] = start_node;
    } else {
      start_node = egr->edge[0].node_id[1];
    }

    for (iedge = 1; iedge < egr->nedges; iedge++) {
      if (start_node == egr->edge[iedge].node_id[1]) {
	// prev -> node[1]/node[0] -> next : swap
	start_node = egr->edge[iedge].node_id[0];
	egr->edge[iedge].node_id[0] = egr->edge[iedge].node_id[1];
	egr->edge[iedge].node_id[1] = start_node;
      } else {
	// prev -> node[0]/node[1] -> next : no swap
	start_node = egr->edge[iedge].node_id[1];
      }
    }
  }

#ifdef DEBUG
  fprintf (stderr, "fill_boundary_edge(%d) :\n", axis);
  fprintf (stderr, "  bbox=(%le %le %le)-(%le %le %le)\n", bbox->xmin, bbox->ymin, bbox->zmin, bbox->xmax, bbox->ymax, bbox->zmax);
  for (iedge = 0; iedge < egr->nedges; iedge++) {
    fprintf (stderr, "  [%d] (%d - %d)\n", iedge, egr->edge[iedge].node_id[0], egr->edge[iedge].node_id[1]);
  }
#endif
}

static void setup_edge_group (Mesh *msh, Surface *surface) {
  BoundingBox edge_box;
  double dx, dy, dz;
  int ix, iy, iz;
int xdir, ydir, zdir;
  
  // -x .. +x : [0][*][*]
  edge_box.xmin = msh->bbox.xmin;
  edge_box.xmax = msh->bbox.xmax;
  xdir = 0;
  for (iy = 0; iy < 2; iy++) {
    ydir = iy*2-1;
    for (iz = 0; iz < 2; iz++) {
      zdir = iz*2-1;
      fill_bound (msh, &dx, &dy, &dz, xdir, ydir, zdir);
      edge_box.ymin = edge_box.ymax = dy;
      edge_box.zmin = edge_box.zmax = dz;
      fill_boundary_edge (msh, &edge_box, &(surface->egr[0][iy][iz]), 0);

#ifdef DEBUG
      fprintf (stdout, "setup: egr[%d][%d][%d].nedges=%d ((%d/%d) . (%d/%d))\n",
               0, iy, iz,
               surface->egr[0][iy][iz].nedges,
               surface->egr[0][iy][iz].edge[0].node_id[0],
               surface->egr[0][iy][iz].edge[0].node_id[1],
               surface->egr[0][iy][iz].edge[
                 surface->egr[0][iy][iz].nedges-1
                 ].node_id[0],
               surface->egr[0][iy][iz].edge[
                 surface->egr[0][iy][iz].nedges-1
                 ].node_id[1]);
#endif
    }
  }

  // -y .. +y : [1][*][*]
  edge_box.ymin = msh->bbox.ymin;
  edge_box.ymax = msh->bbox.ymax;
  ydir = 0;
  for (iz = 0; iz < 2; iz++) {
    zdir = iz*2-1;
    for (ix = 0; ix < 2; ix++) {
      xdir = ix*2-1;
      fill_bound (msh, &dx, &dy, &dz, xdir, ydir, zdir);
      edge_box.zmin = edge_box.zmax = dz;
      edge_box.xmin = edge_box.xmax = dx;
      fill_boundary_edge (msh, &edge_box, &(surface->egr[1][iz][ix]), 1);

#ifdef DEBUG
      fprintf (stdout, "setup: egr[%d][%d][%d].nedges=%d ((%d/%d) . (%d/%d))\n",
               1, iz, ix,
               surface->egr[1][iz][ix].nedges,
               surface->egr[1][iz][ix].edge[0].node_id[0],
               surface->egr[1][iz][ix].edge[0].node_id[1],
               surface->egr[1][iz][ix].edge[
                 surface->egr[1][iz][ix].nedges-1
                 ].node_id[0],
               surface->egr[1][iz][ix].edge[
                 surface->egr[1][iz][ix].nedges-1
                 ].node_id[1]);
#endif

    }
  }

  // -z .. +z : [2][*][*]
  edge_box.zmin = msh->bbox.zmin;
  edge_box.zmax = msh->bbox.zmax;
  iz = 0;
  for (ix = 0; ix < 2; ix++) {
    xdir = ix*2-1;
    for (iy = 0; iy < 2; iy++) {
      ydir = iy*2-1;
      fill_bound (msh, &dx, &dy, &dz, xdir, ydir, zdir);
      edge_box.xmin = edge_box.xmax = dx;
      edge_box.ymin = edge_box.ymax = dy;
      fill_boundary_edge (msh, &edge_box, &(surface->egr[2][ix][iy]), 2);

#ifdef DEBUG
      fprintf (stdout, "setup: egr[%d][%d][%d].nedges=%d ((%d/%d) . (%d/%d))\n",
               2, ix, iy,
               surface->egr[2][ix][iy].nedges,
               surface->egr[2][ix][iy].edge[0].node_id[0],
               surface->egr[2][ix][iy].edge[0].node_id[1],
               surface->egr[2][ix][iy].edge[
                 surface->egr[2][ix][iy].nedges-1
                 ].node_id[0],
               surface->egr[2][ix][iy].edge[
                 surface->egr[2][ix][iy].nedges-1
                 ].node_id[1]);
#endif
    }
  }
}

static void init_matched_meshes (MatchedMesh *mmesh) {
  int ix, iy, iz;
  int iaxis;
  int iv, iw;
  int iside;

  // VertexMesh
  for (ix = 0; ix < 2; ix++) {
    for (iy = 0; iy < 2; iy++) {
      for (iz = 0; iz < 2; iz++) {
	mmesh->vmesh[ix][iy][iz].is_generated = 0;
      }
    }
  }

  // EdgeMesh
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iv = 0; iv < 2; iv++) {
      for (iw = 0; iw < 2; iw++) {
	mmesh->emesh[iaxis][iv][iw].is_generated = 0;
      }
    }
  }

  // FaceMesh
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iside = 0; iside < 2; iside++) {
      mmesh->smesh[iaxis][iside].is_generated = 0;
    }
  }
}

void setup (Mesh *msh, Surface *surface, MatchedMesh *mmesh) {
  detect_boundary_box (msh);

  fprintf (stdout, "bounding box=(%lf %lf %lf) - (%lf %lf %lf)\n",
	   msh->bbox.xmin, msh->bbox.ymin, msh->bbox.zmin,
	   msh->bbox.xmax, msh->bbox.ymax, msh->bbox.zmax);

  setup_vertex (msh, surface);
  setup_edge_group (msh, surface);
  setup_face_group (msh, surface);

  init_matched_meshes (mmesh);
}

