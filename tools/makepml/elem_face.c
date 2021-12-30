/* elem_face.c */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <string.h>
#include <math.h>

#include "pml.h"
#include "pml_types.h"
#include "common.h"

#define FORCE_CLOCKWISE


#define NDIV_PRISM 17
static const int div_pattern[2][NDIV_PRISM][4] = {
  { // reverse=0
    // MAT_F(A2) : 2
    {0, 5, 6, 4}, {0, 7, 9, 8},
    // MAT_F(A3) : 6
    {0, 1, 6, 5}, {0, 1, 8, 9},
    {0, 2, 4, 6}, {0, 2, 9, 7},
    {0, 3, 7, 8}, {0, 3, 5, 4},
    // MAT_F(A4) : 3
    {4, 7, 2, 3},
    {5, 8, 3, 1},
    {6, 9, 1, 2},
    // MAT_F(A5) : 6
    {0, 1, 2, 6}, {0, 1, 9, 2},
    {0, 2, 3, 4}, {0, 2, 7, 3},
    {0, 3, 1, 5}, {0, 3, 8, 1}
  }, { // reverse=1
    // MAT_F(A2) : 2
    {0, 5, 4, 6}, {0, 7, 8, 9},
    // MAT_F(A3) : 6
    {0, 1, 5, 6}, {0, 1, 9, 8},
    {0, 2, 6, 4}, {0, 2, 7, 9},
    {0, 3, 8, 7}, {0, 3, 4, 5},
    // MAT_F(A4) : 3
    {4, 7, 3, 2},
    {5, 8, 1, 3},
    {6, 9, 2, 1},
    // MAT_F(A5) : 6
    {0, 1, 6, 2}, {0, 1, 2, 9},
    {0, 2, 4, 3}, {0, 2, 3, 7},
    {0, 3, 5, 1}, {0, 3, 1, 8}
  }
};

#ifdef DEBUG
int div_count[2][NDIV_PRISM] = {
  {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0},
  {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0},
};
#endif

typedef struct {
  int id;
  int index;
} IndexTable;

int cmp_index_table_using_id (const void *v0, const void *v1) {
  const IndexTable *p0 = (const IndexTable *)v0;
  const IndexTable *p1 = (const IndexTable *)v1;

  return p0->id - p1->id;
}

IndexTable *create_index_table (int nitems, int *ids) {
  IndexTable *ret;
  int iitem;

  ret = (IndexTable *)malloc (nitems * sizeof (IndexTable));
  assert(ret != NULL);

  for (iitem = 0; iitem < nitems; iitem++) {
    ret[iitem].id = ids[iitem];
    ret[iitem].index = iitem;
  }

  qsort (ret, nitems, sizeof (IndexTable), cmp_index_table_using_id);

  return ret;
}

int *create_triangle_vertex_index (int nnodes, int *node_id, int ntriangles, Triangle *triangle) {
  IndexTable *nd_index_table;
  int *tri_vertex_index;

  int itri;
  int inode;

  tri_vertex_index = (int *)malloc (ntriangles * 3 * sizeof (int));
  assert(tri_vertex_index != NULL);

  nd_index_table = create_index_table (nnodes, node_id);

  for (itri = 0; itri < ntriangles; itri++) {
    for (inode = 0; inode < 3; inode++) {
      IndexTable key;
      IndexTable *result;
      key.id = triangle[itri].node_id[inode];
      
      result = (IndexTable *)bsearch (&key, nd_index_table, nnodes, sizeof (IndexTable), cmp_index_table_using_id);
      assert(result != NULL);

      tri_vertex_index[itri*3+inode] = result->index;

#ifdef DEBUG
      fprintf (stderr, "tri_node_index[%d][%d] : node_id=%d index=%d\n", itri, inode, triangle[itri].node_id[inode], result->index);
#endif

    }
  }
  free (nd_index_table);

  return tri_vertex_index;
}

void set_bbox (BoundingBox *bbox, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {
  bbox->xmin = xmin;
  bbox->ymin = ymin;
  bbox->zmin = zmin;
  bbox->xmax = xmax;
  bbox->ymax = ymax;
  bbox->zmax = zmax;
}

void get_bounding_boxes (BoundingBox *bbox, int axis, int side, int nlayers, double *layer_width, BoundingBox *bbox_bs, BoundingBox bbox_bfg[][2]) {
  double sum_width;
  double oxmin, oxmax, oymin, oymax, ozmin, ozmax;
  double lxmin, lxmax, lymin, lymax, lzmin, lzmax;

  BoundingBox *bbox_v0;
  BoundingBox *bbox_v1;
  BoundingBox *bbox_w0;
  BoundingBox *bbox_w1;

  int ilayer;

  assert(axis >= 0);
  assert(axis < 3);

  bbox_v0 = &(bbox_bfg[0][0]);
  bbox_v1 = &(bbox_bfg[0][1]);
  bbox_w0 = &(bbox_bfg[1][0]);
  bbox_w1 = &(bbox_bfg[1][1]);

  sum_width = 0.0;
  for (ilayer = 0; ilayer < nlayers; ilayer++) {
    sum_width += layer_width[ilayer];
  }

  oxmin = bbox->xmin;
  oymin = bbox->ymin;
  ozmin = bbox->zmin;
  oxmax = bbox->xmax;
  oymax = bbox->ymax;
  ozmax = bbox->zmax;

  lxmin = bbox->xmin - sum_width;
  lymin = bbox->ymin - sum_width;
  lzmin = bbox->zmin - sum_width;
  lxmax = bbox->xmax + sum_width;
  lymax = bbox->ymax + sum_width;
  lzmax = bbox->zmax + sum_width;

  if (axis == 0) {
    if (side < 0) {
      set_bbox (bbox_bs, oxmin, oxmin, oymin, oymax, ozmin, ozmax);
      set_bbox (bbox_v0, lxmin, oxmin, oymin, oymax, ozmin, ozmin);
      set_bbox (bbox_v1, lxmin, oxmin, oymin, oymax, ozmax, ozmax);
      set_bbox (bbox_w0, lxmin, oxmin, oymin, oymin, ozmin, ozmax);
      set_bbox (bbox_w1, lxmin, oxmin, oymin, oymin, ozmin, ozmax);
    } else {
      set_bbox (bbox_bs, oxmax, oxmax, oymin, oymax, ozmin, ozmax);
      set_bbox (bbox_v0, oxmax, lxmax, oymin, oymax, ozmin, ozmin);
      set_bbox (bbox_v1, oxmax, lxmax, oymin, oymax, ozmax, ozmax);
      set_bbox (bbox_w0, oxmax, lxmax, oymin, oymin, ozmin, ozmax);
      set_bbox (bbox_w1, oxmax, lxmax, oymin, oymin, ozmin, ozmax);
    }
  } else if (axis == 1) {
    if (side < 0) {
      set_bbox (bbox_bs, oxmin, oxmax, oymin, oymin, ozmin, ozmax);
      set_bbox (bbox_v0, oxmin, oxmin, lymin, oymin, ozmin, ozmax);
      set_bbox (bbox_v1, oxmax, oxmax, lymin, oymin, ozmin, ozmax);
      set_bbox (bbox_w0, oxmin, oxmax, lymin, oymin, ozmin, ozmin);
      set_bbox (bbox_w1, oxmax, oxmax, lymin, oymin, ozmin, ozmin);
    } else {
      set_bbox (bbox_bs, oxmin, oxmax, oymax, oymax, ozmin, ozmax);
      set_bbox (bbox_v0, oxmin, oxmin, oymax, lymax, ozmin, ozmax);
      set_bbox (bbox_v1, oxmax, oxmax, oymax, lymax, ozmin, ozmax);
      set_bbox (bbox_w0, oxmin, oxmax, oymax, lymax, ozmin, ozmin);
      set_bbox (bbox_w1, oxmin, oxmax, oymax, lymax, ozmin, ozmin);
    }
  } else if (axis == 2) {
    if (side < 0) {
      set_bbox (bbox_bs, oxmin, oxmax, oymin, oymax, ozmin, ozmin);
      set_bbox (bbox_v0, oxmin, oxmax, oymin, oymin, lzmin, ozmin);
      set_bbox (bbox_v1, oxmin, oxmax, oymax, oymax, lzmin, ozmin);
      set_bbox (bbox_w0, oxmin, oxmin, oymin, oymax, lzmin, ozmin);
      set_bbox (bbox_w1, oxmin, oxmin, oymax, oymax, lzmin, ozmin);
    } else {
      set_bbox (bbox_bs, oxmin, oxmax, oymin, oymax, ozmax, ozmax);
      set_bbox (bbox_v0, oxmin, oxmax, oymin, oymin, ozmax, lzmax);
      set_bbox (bbox_v1, oxmin, oxmax, oymax, oymax, ozmax, lzmax);
      set_bbox (bbox_w0, oxmin, oxmin, oymin, oymax, ozmax, lzmax);
      set_bbox (bbox_w1, oxmin, oxmin, oymin, oymax, ozmax, lzmax);
    }
  }
}

static int is_same_edge (const int e0[2], const int e1[2]) {
  if (e0[0] == e1[0] && e0[1] == e1[1]) {
    return 1;
  }
  if (e0[1] == e1[0] && e0[0] == e1[1]) {
    return 2;
  }
  return 0;
}
/*
static int is_same_edge (const Edge *e0, const Edge *e1) {
  if (e0->node_id[0] == e1->node_id[0] &&
      e0->node_id[1] == e1->node_id[1]) {
    return 1;
  }
  if (e0->node_id[1] == e1->node_id[0] &&
      e0->node_id[0] == e1->node_id[1]) {
    return 2;
  }
  return 0;
}
*/

/**
 * generate elements for a selected face.
 *
 * @param msh            original mesh.
 * @param nelements      # of elements.
 * @param nnodes         # of nodes.
 * @param mmesh          generated mesh groups.
 * @param fmesh          face mesh for selected face.
 * @param fgr            face group for selected face of original element.
 * @param bfg            block face group around face element group
 * @param iaxis          direction of normal.
 * @param side           side of face.
 * @param nlayers        # of layers of generated elements
 * @param layer_width    widths of layers
 * @param face_volume_id volume id list for each layer.
 */
int generate_face_elements
(Mesh *msh,
 int *element_offset, int *node_offset, MatchedMesh *mmesh,
 FaceMesh *fmesh, FaceGroup *fgr,
 BlockFaceGroup *bfg[][2],
 int iaxis, int side,
 int nlayers, double *layer_width,
 int *face_volume_id) {
  
  int inode;
  int itri;
  int iedge;
  int ilayer;
  int iv, iw;

  int nbedges[5];

  int nnodes;
  int nnodes_exists;

  int *nindex;    // node id index table (face mesh -> global mesh)

  int *node_index;
  int *edge_index;

  int *tri_vertex_index;  // node id of triangle vertices on base face group.
  int *tri_edge_index;      // edge id of triangle edges on base face group.

  int inner_node_id;
  //  int inner_edge_id;
  
  int ni_count;
  int nd_count;
  int el_count;
  
  double sum_width;

  int layer_offset;
  int layer_delta;
  int idiv;


  fmesh->is_generated = 1;


#ifdef DEBUG
  {
    BoundingBox bbox_fgr;
    BoundingBox bbox_bfg[2][2];
    int inode;
    int node_id;
    double x, y, z;
    int ret;

    get_bounding_boxes (&(msh->bbox), iaxis, side, nlayers, layer_width, &bbox_fgr, bbox_bfg);

    for (inode = 0; inode < fgr->nnodes; inode++) {
      node_id = fgr->node_id[inode];
      x = msh->node[node_id*3];
      y = msh->node[node_id*3+1];
      z = msh->node[node_id*3+2];
      ret = check_inside (&bbox_fgr, x, y, z);
      assert(ret != 0);
    }

    // TODO : check node coord of bfg
  }
#endif

#ifdef DEBUG
  // dump face group
  {
    int ieaxis;
    int ieside;

    fprintf (stderr, "face[%d][%d] nnodes=%d\n", iaxis, side, fgr->nnodes);
    for (inode = 0; inode < fgr->nnodes; inode++) {
      int node_id = fgr->node_id[inode];
      fprintf (stderr, "    node[%d]=%d (%le %le %le)\n", inode, node_id, msh->node[node_id*3], msh->node[node_id*3+1], msh->node[node_id*3+2]);
    }

    fprintf (stderr, "face[%d][%d] ntriangles=%d\n", iaxis, side, fgr->ntriangles);
    for (itri = 0; itri < fgr->ntriangles; itri++) {
      Triangle *tri = fgr->triangle + itri;
      fprintf (stderr, "    triangle[%d]=(%d %d %d)\n", itri, tri->node_id[0], tri->node_id[1], tri->node_id[2]);
    }

    for (ieaxis = 0; ieaxis < 2; ieaxis++) {
      for (ieside = 0; ieside < 2; ieside++) {
	EdgeGroup *boundary = fgr->boundary[ieaxis][ieside];
	BlockFaceGroup *side_face_group = bfg[ieaxis][ieside];

	fprintf (stderr, "face[%d][%d].boundary[%d][%d].nedge=%d\n", iaxis, side, ieaxis, ieside, boundary->nedges);

	for (iedge = 0; iedge < boundary->nedges; iedge++) {
	  Edge *edge = boundary->edge + iedge;

	  fprintf (stderr, "    edge[%d]=(%d, %d)\n", iedge, edge->node_id[0],edge->node_id[1]);
	}

	fprintf (stderr, "face[%d][%d].bfg[%d][%d] axis=%d, nv=%d, nw=%d\n", iaxis, side, ieaxis, ieside, side_face_group->axis, side_face_group->nvblocks, side_face_group->nwblocks);

	for (iv = 0; iv < side_face_group->nvblocks; iv++) {
	  for (iw = 0; iw < side_face_group->nwblocks; iw++) {
	    BlockFace *bf = side_face_group->bface + (iv*side_face_group->nwblocks+iw);
	    
	    fprintf (stderr, "    bface[%d][%d]=(%d %d / %d %d) %d\n", iv, iw, bf->vertex[0][0], bf->vertex[0][1], bf->vertex[1][0], bf->vertex[1][1], bf->cpoint);
	    
	  }
	}
      }
    }
  }
#endif

  nbedges[0] = 0;
  nbedges[1] = nbedges[0] + fgr->boundary[0][0]->nedges;
  nbedges[2] = nbedges[1] + fgr->boundary[0][1]->nedges;
  nbedges[3] = nbedges[2] + fgr->boundary[1][0]->nedges;
  nbedges[4] = nbedges[3] + fgr->boundary[1][1]->nedges;

  // generate new nodes
  fmesh->node_offset = *node_offset;
  nnodes =
    fgr->nnodes * (nlayers+1) + // vertex
    fgr->nedges * nlayers +     // center of side squares
    nbedges[4] * nlayers +         // center of boundary squares
    fgr->ntriangles * nlayers;  // center of prisms
  nnodes_exists =
    fgr->nnodes +          // on base face group
    nbedges[4] * nlayers * 2; // on boundary face egroups (vertex and center)
    
  fmesh->mesh.nnodes = nnodes - nnodes_exists;
  fmesh->mesh.node = (double *)malloc (fmesh->mesh.nnodes * 3 * sizeof (double));
  assert(fmesh->mesh.node != NULL);

  fmesh->mesh.nelements = fgr->ntriangles * nlayers * NDIV_PRISM;
  fmesh->mesh.element = (int *)malloc (fmesh->mesh.nelements * 4 * sizeof (int));
  assert(fmesh->mesh.element != NULL);

  fmesh->mesh.volume = (int *)malloc (fmesh->mesh.nelements * sizeof (int));
  assert(fmesh->mesh.volume != NULL);

  // generate tri_edge_index
  // if edge belongs boundary:
  //   [(itri*3+iedge)*2  ] : indices of bfg (iv*2+iw)
  //   [(itri*3+iedge)*2+1] : bfg[iv][iw].edge index
  // else
  //   [(itri*3+iedge)*2  ] : < 0
  //   [(itri*3+iedge)*2+1] : fgr->edge index
  //
  tri_edge_index = (int *)malloc (fgr->ntriangles * 3 * 2 * sizeof (int));
  assert(edge_index != NULL);

  for (itri = 0; itri < fgr->ntriangles; itri++) {
    for (iedge = 0; iedge < 3; iedge++) {
      //Edge *tri_edge = fgr->triangle[itri].edge[iedge];
      int tri_edge_node[2];
      tri_edge_node[0] = fgr->triangle[itri].node_id[(iedge+1)%3];
      tri_edge_node[1] = fgr->triangle[itri].node_id[(iedge+2)%3];
      
      tri_edge_index[(itri*3+iedge)*2]   = -1;
      tri_edge_index[(itri*3+iedge)*2+1] = -1;

      // search boundary edges
      for (iv = 0; iv < 2; iv++) {
	for (iw = 0; iw < 2; iw++) {
	  BlockFaceGroup *bnd = bfg[iv][iw];
	  EdgeGroup *egr = bnd->egr;
	  int ibedge;
	  
	  for (ibedge = 0; ibedge < egr->nedges; ibedge++) {
	    //if (is_same_edge (tri_edge, egr->edge+ibedge)) {
	    if (is_same_edge (tri_edge_node, egr->edge[ibedge].node_id)) {
	      tri_edge_index[(itri*3+iedge)*2]   = iv*2+iw;
	      tri_edge_index[(itri*3+iedge)*2+1] = ibedge;

#ifdef DEBUG
	      {
	      double x0, y0, z0;
	      double x1, y1, z1;

	      get_coord (msh, mmesh, tri_edge_node[0], &x0, &y0, &z0);
	      get_coord (msh, mmesh, tri_edge_node[1], &x1, &y1, &z1);

	      fprintf (stderr, "tri_edge_index[%d][%d]=(%d, %d, %d) <- edge(%d(%lf %lf %lf) %d(%lf %lf %lf))\n", itri, iedge, iv, iw, ibedge, tri_edge_node[0], x0, y0, z0, tri_edge_node[1], x1, y1, z1);
	    }
#endif

	      break;
	    }
	  } // for (ibedge)
	  if (tri_edge_index[(itri*3+iedge)*2] >= 0) {
	    break;
	  }
	} // for (iw)
	  if (tri_edge_index[(itri*3+iedge)*2] >= 0) {
	    break;
	  }
      } // for (iv)

      // search inner edge
      if (tri_edge_index[(itri*3+iedge)*2] < 0) {
	int iiedge;
	for (iiedge = 0; iiedge < fgr->nedges; iiedge++) {
	  //if (is_same_edge (tri_edge, fgr->edge+iiedge)) {
	  if (is_same_edge (tri_edge_node, fgr->edge[iiedge].node_id)) {
	    tri_edge_index[(itri*3+iedge)*2+1] = iiedge;

#ifdef DEBUG
	      fprintf (stderr, "tri_edge_index[%d][%d]=(%d, %d, %d)\n", itri, iedge, -1, -1, iiedge);
#endif

	    break;
	  }
	} // for (iedge)
      }


      assert(tri_edge_index[(itri*3+iedge)*2+1] >= 0);
    } // for (iedge)
  } // for (itri)

  // boundary/inner node id
  node_index = (int *)malloc (fgr->nnodes * 2 * sizeof (int));
  assert(node_index != NULL);

  for (inode = 0; inode < fgr->nnodes; inode++) {
    node_index[inode*2] = -1;    // index of fgr->boundary if boundary edge
    node_index[inode*2+1] = -1;  // index
  }

  inner_node_id = 0;
  for (inode = 0; inode < fgr->nnodes; inode++) {
    for (iv = 0; iv < 2; iv++) {
      for (iw = 0; iw < 2; iw++) {
	BlockFaceGroup *bnd = bfg[iv][iw];
	EdgeGroup *egr = bnd->egr;
	int ibedge;
	
	for (ibedge = 0; ibedge < egr->nedges; ibedge++) {
	  int *node_id = egr->edge[ibedge].node_id;
	  //for (ibedge = 0; ibedge < bnd->nvblocks; ibedge++) {
	  //int *node_id = bnd->bface[ibedge].vertex[0];
	  
	  if (node_id[0] == fgr->node_id[inode]) {
	    node_index[inode*2]   = (iv*2+iw)*2;
	    node_index[inode*2+1] = ibedge;
	    break;
	  } else if (node_id[1] == fgr->node_id[inode]) {
	    node_index[inode*2]   = (iv*2+iw)*2+1;
	    node_index[inode*2+1] = ibedge;
	    break;
	  }
	} // for (ibedge)
	if (node_index[inode*2] >= 0) {
	  break;
	}
      } // for (iw)
	if (node_index[inode*2] >= 0) {
	  break;
	}
    } // for (iv)

    if (node_index[inode*2] < 0) {
      node_index[inode*2+1] = inner_node_id;
      inner_node_id++;

#ifdef DEBUG
      fprintf (stderr, "node_index[%d*2+1]=%d\n", inode, inner_node_id-1);
#endif
    }
  } // for (inode)
  assert(inner_node_id == fgr->nnodes - nbedges[4]);
  
  // calculate incircle center of triangles
  double *ccrd;
  ccrd = (double *)malloc (fgr->ntriangles * 3 * sizeof (double));
  assert(ccrd != NULL);

  for (itri = 0; itri < fgr->ntriangles; itri++) {
    int *n;
    double xa, ya, za;
    double xb, yb, zb;
    double xc, yc, zc;
    double a, b, c, abc;

    n = fgr->triangle[itri].node_id;
    xa = msh->node[n[0]*3];
    xb = msh->node[n[1]*3];
    xc = msh->node[n[2]*3];
    ya = msh->node[n[0]*3+1];
    yb = msh->node[n[1]*3+1];
    yc = msh->node[n[2]*3+1];
    za = msh->node[n[0]*3+2];
    zb = msh->node[n[1]*3+2];
    zc = msh->node[n[2]*3+2];

    a = sqrt ((xb-xc)*(xb-xc)+(yb-yc)*(yb-yc)+(zb-zc)*(zb-zc));
    b = sqrt ((xc-xa)*(xc-xa)+(yc-ya)*(yc-ya)+(zc-za)*(zc-za));
    c = sqrt ((xa-xb)*(xa-xb)+(ya-yb)*(ya-yb)+(za-zb)*(za-zb));
    abc = a+b+c;

    ccrd[itri*3]   = (a*xa+b*xb+c*xc)/abc;
    ccrd[itri*3+1] = (a*ya+b*yb+c*yc)/abc;
    ccrd[itri*3+2] = (a*za+b*zb+c*zc)/abc;
  }

  // generate inner nodes
  nindex = (int *)malloc (nnodes * sizeof (int));
  assert(nindex != NULL);

  ni_count = 0;
  nd_count = 0;
  for (inode = 0; inode < fgr->nnodes; inode++) {
    nindex[ni_count] = fgr->node_id[inode];

#ifdef DEBUG
    fprintf (stderr, "[%d][%d] nindex[%d]=%d <- fgr->node_id[%d]\n", iaxis, side, ni_count, nindex[ni_count], inode);
#endif

    ni_count++;
  }
  
  sum_width = 0.0;
  for (ilayer = 0; ilayer < nlayers; ilayer++) {
    // prism center nodes
    for (itri = 0; itri < fgr->ntriangles; itri++) {
      double crd[3];
      
      crd[0] = ccrd[itri*3];
      crd[1] = ccrd[itri*3+1];
      crd[2] = ccrd[itri*3+2];

      crd[iaxis] += ((double)(side)) * (layer_width[ilayer]/2.0 + sum_width);

      fmesh->mesh.node[nd_count*3  ] = crd[0];
      fmesh->mesh.node[nd_count*3+1] = crd[1];
      fmesh->mesh.node[nd_count*3+2] = crd[2];

      nindex[ni_count] = nd_count + *node_offset;


#ifdef DEBUG
      fprintf (stderr, "[%d][%d] nindex[%d]=%d <- center of prism[%d][%d]=%d (%lf %lf %lf)\n", iaxis, side, ni_count, nindex[ni_count], ilayer, itri, nd_count, crd[0], crd[1], crd[2]);
#endif

      nd_count++;
      ni_count++;
    } // for (itri)

    // side center nodes for inner edge
    for (iedge = 0; iedge < fgr->nedges; iedge++) {
      double crd[3];
      double ecrd[2][3];
      int *n = fgr->edge[iedge].node_id;
      
      ecrd[0][0] = msh->node[n[0]*3];
      ecrd[0][1] = msh->node[n[0]*3+1];
      ecrd[0][2] = msh->node[n[0]*3+2];
      ecrd[1][0] = msh->node[n[1]*3];
      ecrd[1][1] = msh->node[n[1]*3+1];
      ecrd[1][2] = msh->node[n[1]*3+2];

      crd[0] = (ecrd[0][0] + ecrd[1][0]) / 2.0;
      crd[1] = (ecrd[0][1] + ecrd[1][1]) / 2.0;
      crd[2] = (ecrd[0][2] + ecrd[1][2]) / 2.0;
      
      crd[iaxis] += ((double)(side)) * (layer_width[ilayer]/2.0 + sum_width);
      
      fmesh->mesh.node[nd_count*3  ] = crd[0];
      fmesh->mesh.node[nd_count*3+1] = crd[1];
      fmesh->mesh.node[nd_count*3+2] = crd[2];
      
      nindex[ni_count] = nd_count + *node_offset;

#ifdef DEBUG
      fprintf (stderr, "[%d][%d] nindex[%d]=%d <- center of edge[%d][%d]=%d (%lf %lf %lf) =%d(%lf %lf %lf) / %d(%lf %lf %lf)\n", iaxis, side, ni_count, nindex[ni_count], ilayer, iedge, nd_count, crd[0], crd[1], crd[2], n[0], ecrd[0][0], ecrd[0][1], ecrd[0][2], n[1], ecrd[1][0], ecrd[1][1], ecrd[1][2]);
#endif
      
      nd_count++;
      ni_count++;
    } // for (iedge)

    // side center nodes for boundary edges
    for (iv = 0; iv < 2; iv++) {
      for (iw = 0; iw < 2; iw++) {
	BlockFaceGroup *bnd = bfg[iv][iw];
	EdgeGroup *egr = bnd->egr;
	int ibv, ibw;

	ibw = ilayer;
	//for (ibv = 0; ibv < bnd->nvblocks; ibv++) {
        for (ibv = 0; ibv < egr->nedges; ibv++) {
	  // TODO : ??? 
	  //BlockFace *bface = bnd->bface + ibv*bnd->nwblocks+ibw;
	  BlockFace *bface = bnd->bface + ibw*egr->nedges + ibv;;

	  
	  nindex[ni_count] = bface->cpoint;

#ifdef DEBUG
	  fprintf (stderr, "[%d][%d] nindex[%d]=%d <- center of bnd cpoint[%d][%d][%d]=%d/%d\n", iaxis, side, ni_count, nindex[ni_count], ilayer, iv, iw, ibv, ibw);
#endif

	  ni_count++;
	}

      } // for (iw)
    } // for (iv)

    sum_width += layer_width[ilayer];

    // prism vertex
    for (inode = 0; inode < fgr->nnodes; inode++) {
      if (node_index[inode*2] < 0) {
	// inner node
	double crd[3];

	crd[0] = msh->node[fgr->node_id[inode]*3];
	crd[1] = msh->node[fgr->node_id[inode]*3+1];
	crd[2] = msh->node[fgr->node_id[inode]*3+2];

	crd[iaxis] += ((double)(side)) * sum_width;

	fmesh->mesh.node[nd_count*3]   = crd[0];
	fmesh->mesh.node[nd_count*3+1] = crd[1];
	fmesh->mesh.node[nd_count*3+2] = crd[2];
	
	nindex[ni_count] = nd_count + *node_offset;

#ifdef DEBUG
	fprintf (stderr, "[%d][%d] nindex[%d]=%d <- prism(inner)[%d][%d]=%d (%lf %lf %lf)\n", iaxis, side, ni_count, nindex[ni_count], ilayer, inode, nd_count, crd[0], crd[1], crd[2]);
#endif

	nd_count++;
	ni_count++;
      } else {
	// boundary node -> refer to boundary
	iv = node_index[inode*2]/4;
	iw = (node_index[inode*2]/2)&1;
	int node_side = node_index[inode*2]&1;
	iedge = node_index[inode*2+1];

	BlockFaceGroup *bnd = bfg[iv][iw];
	BlockFace *bface = bnd->bface + bnd->nvblocks*ilayer+iedge;
	nindex[ni_count] = bface->vertex[1][node_side];

#ifdef DEBUG
	fprintf (stderr, "[%d][%d] nindex[%d]=%d <- prism(bnd)[%d][%d][%d][%d]=(%d %d/%d %d/%d)\n", iaxis, side, ni_count, nindex[ni_count], ilayer, iv, iw, iedge, bface->vertex[0][0], bface->vertex[0][1], bface->vertex[1][0], bface->vertex[1][1], bface->cpoint);
#endif
	
	ni_count++;
      } // if (node_index[inode*2] < 0)
    } // for (inode)

    assert(ni_count == fgr->nnodes + (ilayer+1)*(fgr->nnodes + fgr->ntriangles + fgr->nedges + nbedges[4]));
  } // for (ilayer)

#ifdef DEBUG
  fprintf (stderr, "face [%d][%d]: ntriangles=%d, nedges=%d, nbedges=%d\n", iaxis, side, fgr->ntriangles, fgr->nedges, nbedges[4]);
  fprintf (stderr, "  -> ni_count=%d, nd_count=%d\n", ni_count, nd_count);

  for (inode = 0; inode < ni_count; inode++) {
    double x, y, z;

    get_coord (msh, mmesh, nindex[inode], &x, &y, &z);
    fprintf (stderr, "[%d][%d] nindex[%d]=%d(%lf %lf %lf)\n", iaxis, side, inode, nindex[inode], x, y, z);
  }

#endif

  assert(ni_count == nnodes);
  assert(nd_count == nnodes-nnodes_exists);

  *node_offset += (nnodes - nnodes_exists);

  tri_vertex_index = create_triangle_vertex_index (fgr->nnodes, fgr->node_id, fgr->ntriangles, fgr->triangle);

  // generate prisms
  el_count = 0;
  layer_delta = fgr->nnodes + fgr->ntriangles + fgr->nedges + nbedges[4];
  layer_offset = 0;
  for (ilayer = 0; ilayer < nlayers; ilayer++) {
    int prism[10];     // incircle center
    int gnode_id[10];

    for (itri = 0; itri < fgr->ntriangles; itri++) {
      int reverse = (fgr->triangle[itri].reverse_flag)^((side+1)/2);

      // [4-6] : vertices on ilayer
      prism[4] = tri_vertex_index[itri*3  ] + layer_offset;
      prism[5] = tri_vertex_index[itri*3+1] + layer_offset;
      prism[6] = tri_vertex_index[itri*3+2] + layer_offset;

      // [7-9] : vertices on (ilayer+1)
      prism[7] = prism[4] + layer_delta;
      prism[8] = prism[5] + layer_delta;
      prism[9] = prism[6] + layer_delta;
      
      // [1-3] : center nodes on side faces
      for (iedge = 0; iedge < 3; iedge++) {
	int *index = tri_edge_index + (itri*3+iedge)*2;
	if (index[0] >= 0) {
	  prism[iedge+1] = fgr->nnodes + fgr->ntriangles + fgr->nedges + nbedges[index[0]] + index[1] + layer_offset;
	} else {
	  // inner edge
	  prism[iedge+1] = fgr->nnodes + fgr->ntriangles + index[1] + layer_offset;
	}
      }
      // [0] : center of prism
      prism[0] = itri + fgr->nnodes + layer_offset;

      for (inode = 0; inode < 10; inode++) {
	gnode_id[inode] = nindex[prism[inode]];
      }

#ifdef DEBUG
      fprintf (stderr, "[%d][%d] prism[%d][%d] : reverse=%d\n", iaxis, side, ilayer, itri, fgr->triangle[itri].reverse_flag);

      for (inode = 0; inode < 10; inode++) {
	double x, y, z;
	get_coord (msh, mmesh, gnode_id[inode], &x, &y, &z);
	
	fprintf (stderr, "  [%d] prism=%d, gnode=%d (%lf %lf %lf)\n", inode, prism[inode], gnode_id[inode], x, y, z);
      }
#endif


      for (idiv = 0; idiv < NDIV_PRISM; idiv++) {
	for (inode = 0; inode < 4; inode++) {

#ifdef DEBUG
	  div_count[reverse][idiv]++;
#endif

	  fmesh->mesh.element[el_count*4+inode] = gnode_id[div_pattern[reverse][idiv][inode]];
	} // for (inode)

#ifdef FORCE_CLOCKWISE
	if (!is_clockwise(msh, mmesh, fmesh->mesh.element+el_count*4)) {
	  int tmp = fmesh->mesh.element[el_count*4+2];
	  fmesh->mesh.element[el_count*4+2] = fmesh->mesh.element[el_count*4+3];
	  fmesh->mesh.element[el_count*4+3] = tmp;

#ifdef DEBUG
	  fprintf (stderr, "swapped\n");
#endif
	}
#endif

#ifdef DEBUG
	for (inode = 0; inode < 4; inode++) {
	  double x, y, z;
	  get_coord (msh, mmesh, gnode_id[div_pattern[reverse][idiv][inode]], &x, &y, &z);
	  fprintf 
	    (stderr, 
	    "[%d][%d][%d][%d][%d][%d->%d] %d (%lf %lf %lf)\n",
	     iaxis, side,
	     ilayer, itri, idiv, inode,
	    div_pattern[reverse][idiv][inode],
	    gnode_id[div_pattern[reverse][idiv][inode]], 
	    x, y, z);
	}
#endif

	fmesh->mesh.volume[el_count] = face_volume_id[ilayer];

#ifdef DEBUG
	assert(is_clockwise(msh, mmesh, fmesh->mesh.element+el_count*4));
#endif
	
	el_count++;
      } // for (idiv)
    } // for (itri)

    layer_offset += layer_delta;
  } // for (ilayer)
  assert(el_count == fgr->ntriangles*nlayers*NDIV_PRISM);

  *element_offset += fgr->ntriangles*nlayers*NDIV_PRISM;

  free (tri_vertex_index);
  free (tri_edge_index);
  free (nindex);

  return 0;
}

