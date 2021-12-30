/* elem_edge.c */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pml.h"
#include "common.h"

/**
 * generate elements for a edge of original mesh.
 *
 * @param msh            original mesh.
 * @param element_offset offset value for element id.
 * @param node_offset    offset value for node id.
 * @param egr            source edge group picked up from original mesh.
 * @param ev             vertex node id of source edge.
 * @param ee             edges for each vertex node picked up from vertex mesh.
 * @param ef             face groups for each vertex node picked up from vertex mesh.
 * @param eb             block face groups for eavh vertex node picked up from vertex mesh.
 * @param emesh          generated elements & nodes.
 * @param axis           axis number paralleled for edge. 0=x, 1=y, 2=z.<br>
 *                       In this method, edge-parallel axis is called as "uaxis".
 * @param vdir           position of edge in vaxis.
 * @param wdir           position of edge in waxis.
 * @param nlayers        number of layers
 * @param layer_width    array of layer width.
 * @param edge_volume_id volume id for generated elements.
 */
void generate_edge_elements
(Mesh *msh, int *element_offset, int *node_offset,
 MatchedMesh *mmesh,
 EdgeGroup *egr,
 int ev[2], Edge *ee[2][2], FaceGroup *ef[2], BlockFaceGroup *eb[2],
 EdgeMesh *emesh,
 int axis, int vdir, int wdir,
 int nlayers, double *layer_width,
 int edge_volume_id) {

  int iaxis;
  int iu, iv, iw;

  double dir[3];

  int uaxis, vaxis, waxis;

  int nnodes;
  int nnodes_exists;
  
  int *nindex;
  int index;
  int count;

  int vertex_origin;
  int cpoint_origin[3];

  int ref_node;
  Edge *ref_edge;
  int ref_node0, ref_node1;
  int edge_node;

  double u0, u1;
  double u, v, w;
  double base_v, base_w;

  int du_v, dv_v, dw_v;
  int du_cp[3], dv_cp[3], dw_cp[3];
  int ielem;
  int element_id;
  int bindex[14];

  int n0, n1, n2, n3;

  int reverse;
  
  int ilayer;
  //double bbox[3][2];
  BoundingBox bbox;

  BlockFaceGroup *bf0;
  BlockFaceGroup *bf1;

  emesh->is_generated = 1;

  reverse = (vdir != wdir);

  uaxis = axis;
  vaxis = (uaxis+1)%3;
  waxis = (uaxis+2)%3;
  
  dir[uaxis] = 1.0;
  dir[vaxis] = vdir;
  dir[waxis] = wdir;

  emesh->node_offset = *node_offset;
  emesh->element_offset = *element_offset;

  // bounding box for coordinate check
  {
    double bb[3][2];
    bb[uaxis][0] = msh->node[egr->edge[0].node_id[0]*3+uaxis];
    bb[vaxis][0] = msh->node[egr->edge[0].node_id[0]*3+vaxis];
    bb[waxis][0] = msh->node[egr->edge[0].node_id[0]*3+waxis];
    bb[uaxis][1] = msh->node[egr->edge[(egr->nedges)-1].node_id[1]*3+uaxis];
    bb[vaxis][1] = msh->node[egr->edge[0].node_id[0]*3+vaxis];
    bb[waxis][1] = msh->node[egr->edge[0].node_id[0]*3+waxis];
    for (ilayer = 0; ilayer < nlayers; ilayer++) {
      bb[vaxis][1] += dir[vaxis] * layer_width[ilayer];
      bb[waxis][1] += dir[waxis] * layer_width[ilayer];
    }
    set_bounding_box (&bbox, bb[0][0], bb[1][0], bb[2][0], bb[0][1], bb[1][1], bb[2][1]);
  }

#ifdef DEBUG
  fprintf (stderr, "start generate_edge_element(%d,%d,%d):\n", axis, vdir, wdir);  
  
  fprintf (stderr, "  bounding box:\n");
  fprintf (stderr, "    min (% le % le % le)\n", bbox.xmin, bbox.ymin, bbox.zmin);
  fprintf (stderr, "    max (% le % le % le)\n", bbox.xmax, bbox.ymax, bbox.zmax);

  for (ivert = 0; ivert < 2; ivert++) {
    fprintf (stderr, "   vertex[%d]: id=%d\n", ivert, ev[ivert]);
    for (idir = 0; idir < 2; idir++) {
      fprintf (stderr, "     ee[%d][%d]=%d\n", ivert, idir, nlayers);
      for (iedge = 0; iedge < nlayers; iedge++) {
	fprintf (stderr, "      edge[%d]=(%d, %d)\n", iedge, ee[ivert][idir][iedge].node_id[0], ee[ivert][idir][iedge].node_id[1]);
      }
    }
  
    fprintf (stderr, "    eb=%d*%d\n", eb[ivert]->nvblocks, eb[ivert]->nwblocks);
    for (iv = 0; iv < eb[ivert]->nvblocks; iv++) {
      for (iw = 0; iw < eb[ivert]->nwblocks; iw++) {
	fprintf (stderr, "      eb[%d][%d]=(%d %d %d %d : %d)\n", 
		 iv, iw,
		 eb[ivert]->bface[iv*eb[ivert]->nvblocks+iw].vertex[0][0],
		 eb[ivert]->bface[iv*eb[ivert]->nvblocks+iw].vertex[0][1],
		 eb[ivert]->bface[iv*eb[ivert]->nvblocks+iw].vertex[1][0],
		 eb[ivert]->bface[iv*eb[ivert]->nvblocks+iw].vertex[1][1],
		 eb[ivert]->bface[iv*eb[ivert]->nvblocks+iw].cpoint);
	
      }
    }
  }
  
  fprintf (stderr, "   edge: nedges=%d\n", egr->nedges);
  for (iedge = 0; iedge < egr->nedges; iedge++) {
    fprintf (stderr, "    edge[%d]=(%d, %d)\n", iedge, egr->edge[iedge].node_id[0], egr->edge[iedge].node_id[1]);
  }
  
#endif

  // generate nodes
  nnodes = (nlayers+1)*(nlayers+1)*(egr->nedges+1);
  nnodes += nlayers*nlayers*(egr->nedges+1);
  nnodes += egr->nedges*nlayers*(nlayers+1)*2;

  nnodes_exists = (nlayers+1)*(nlayers+1)*2 + nlayers*nlayers*2 + egr->nedges-1;

  emesh->mesh.nnodes = nnodes - nnodes_exists;
  emesh->mesh.node = (double *)malloc (emesh->mesh.nnodes * 3 * sizeof (double));
  assert(emesh->mesh.node != NULL);

  emesh->mesh.nelements = nlayers*nlayers*egr->nedges*24;
  emesh->mesh.element = (int *)malloc (emesh->mesh.nelements * 4 * sizeof(int));
  assert(emesh->mesh.element != NULL);

  emesh->mesh.volume = (int *)malloc (emesh->mesh.nelements * sizeof(int));

  nindex = (int *)malloc (nnodes * sizeof (int));
  assert(nindex != NULL);
  
  // vertex nodes
  vertex_origin = 0;
  count = 0;
  index = 0;

  v = msh->node[egr->edge[0].node_id[0]*3+vaxis];
  for (iv = 0; iv <= nlayers; iv++) {
    w = msh->node[egr->edge[0].node_id[0]*3+waxis];
    for (iw = 0; iw <= nlayers; iw++) {
      for (iaxis = 0; iaxis <= egr->nedges; iaxis++) {
	if (iaxis == 0) {
	  if (iv == 0 && iw == 0) {
	    nindex[index] = ev[0];
	  } else if (iv == 0) {
	    // iw != 0
	    nindex[index] = ee[0][1][iw-1].node_id[1];
	  } else if (iw == 0) {
	    // iv != 0
	    nindex[index] = ee[0][0][iv-1].node_id[1];
	  } else {
	    // iv>0 && iw>0
	    nindex[index] = eb[0]->bface[(iv-1)*nlayers+(iw-1)].vertex[1][1];
	  }

#ifdef DEBUG
	  {
	    double x, y, z;
	    int ret;
	    ret = get_coord (msh, mmesh, nindex[index], &x, &y, &z);
	    assert(ret == 0);

	    ret = check_inside (&bbox, x, y, z);
	    assert(ret != 0);
	  }
#endif

	} else if (iaxis == egr->nedges) {
	  if (iv == 0 && iw == 0) {
	    nindex[index] = ev[1];
	  } else if (iv == 0) {
	    // iw > 0
	    nindex[index] = ee[1][1][iw-1].node_id[1];
	  } else if (iw == 0) {
	    // iv > 0
	    nindex[index] = ee[1][0][iv-1].node_id[1];
	  } else {
	    // iv>0 && iw>0
	    nindex[index] = eb[1]->bface[(iv-1)*nlayers+(iw-1)].vertex[1][1];
	  }


#ifdef DEBUG
	  {
	    double x, y, z;
	    int ret;
	    ret = get_coord (msh, mmesh, nindex[index], &x, &y, &z);
	    assert(ret == 0);

	    ret = check_inside (&bbox, x, y, z);
	    assert(ret != 0);
	  }
#endif

	} else if (iv == 0 && iw == 0) {
	  // iaxis > 0
	  nindex[index] = egr->edge[iaxis-1].node_id[1];


#ifdef DEBUG
	  {
	    double x, y, z;
	    int ret;
	    ret = get_coord (msh, mmesh, nindex[index], &x, &y, &z);
	    assert(ret == 0);

	    ret = check_inside (&bbox, x, y, z);
	    assert(ret != 0);
	  }
#endif

	} else {
	  ref_node = egr->edge[iaxis-1].node_id[1];
	  u = msh->node[ref_node*3+uaxis];

	  nindex[index] = count + *node_offset;
	  emesh->mesh.node[count*3+uaxis] = u;
	  emesh->mesh.node[count*3+vaxis] = v;
	  emesh->mesh.node[count*3+waxis] = w;

#ifdef DEBUG
	  {
	    double x = emesh->mesh.node[count*3];
	    double y = emesh->mesh.node[count*3+1];
	    double z = emesh->mesh.node[count*3+2];
	    assert(check_inside (&bbox, x, y, z) != 0);

	    fprintf (stderr, "(g)emesh->mesh.node[%d]=(%le %le %le)\n", count, emesh->mesh.node[count*3], emesh->mesh.node[count*3+1], emesh->mesh.node[count*3+2]);
	  }
#endif

	    count++;
	}

#ifdef DEBUG
	fprintf (stderr, "  nindex[%d] = %d\n", index, nindex[index]);
#endif

 	index++;
      }  // iaxis
      
      if (iw != nlayers) {
	w += dir[waxis] * layer_width[iw];
      }
    }  // iw

    if (iv != nlayers) {
      v += dir[vaxis] * layer_width[iv];
    }
  }  // iv

#ifdef DEBUG
  fprintf (stderr, "  finish vertex nodes: nlayers=%d, egr->nedges=%d\n", nlayers, egr->nedges);
  fprintf (stderr, "    -> index=%d <=> %d\n", index, (nlayers+1)*(nlayers+1)*(egr->nedges+1));
  fprintf (stderr, "       count=%d <=> %d\n", count, ((nlayers+1)*(nlayers+1)-1)*(egr->nedges-1));
#endif

  assert(index == (nlayers+1)*(nlayers+1)*(egr->nedges+1));
  assert(count == ((nlayers+1)*(nlayers+1)-1)*(egr->nedges-1));

  cpoint_origin[0] = index;

  base_v = msh->node[ev[0]*3+vaxis];
  for (iv = 0; iv < nlayers; iv++) {
    v = base_v + dir[vaxis]*layer_width[iv]/2.0;

    base_w = msh->node[ev[0]*3+waxis];
    for (iw = 0; iw < nlayers; iw++) {
      w = base_w + dir[waxis]*layer_width[iw]/2.0;

      ref_node0 = eb[0]->bface[iv*nlayers+iw].cpoint;
      ref_node1 = eb[1]->bface[iv*nlayers+iw].cpoint;
      
#ifdef DEBUG
      fprintf (stderr, "(u)  base_v=%le base_w=%le -> [%d][%d] ref_node=(%d %d)\n\n", base_v, base_w, iv, iw, ref_node0, ref_node1);
#endif      

      // iaxis = 0: use ref_node
      nindex[index] = ref_node0;

#ifdef DEBUG
      {
	int ret;
	double x0, y0, z0;
	double x1, y1, z1;
	ret = get_coord (msh, mmesh, ref_node0, &x0, &y0, &z0);
	assert(ret == 0);
	ret = get_coord (msh, mmesh, ref_node1, &x1, &y1, &z1);
	assert(ret == 0);

	fprintf (stderr, "   0(%le %le %le), 1(%le %le %le)\n", x0, y0, z0, x1, y1, z1);

	ret = check_inside (&bbox, x0, y0, z0);
	assert(ret != 0);
	ret = check_inside (&bbox, x1, y1, z1);
	assert(ret != 0);

	fprintf (stderr, "  nindex[%d] = %d\n", index, nindex[index]);
      }
#endif

      index++;
      
      // iaxis = [1..egr->nedges) : generate node from ref_node & edge_node
      for (iaxis = 1; iaxis < egr->nedges; iaxis++) {
	edge_node = egr->edge[iaxis].node_id[0];
	u = msh->node[edge_node*3+uaxis];
	
	nindex[index] = count + *node_offset;
	emesh->mesh.node[count*3+uaxis] = u;
	emesh->mesh.node[count*3+vaxis] = v;
	emesh->mesh.node[count*3+waxis] = w;

#ifdef DEBUG
	{
	  double x, y, z;
	  x = emesh->mesh.node[count*3];
	  y = emesh->mesh.node[count*3+1];
	  z = emesh->mesh.node[count*3+2];
	  assert(check_inside (&bbox, x, y, z) != 0);

	  fprintf (stderr, "(u)emesh->mesh.node[%d]=(%le %le %le) <= ref_node=(%d, %d), edge_node=%d\n", count, emesh->mesh.node[count*3], emesh->mesh.node[count*3+1], emesh->mesh.node[count*3+2], ref_node0, ref_node1, edge_node);
	}
#endif
#ifdef DEBUG
	fprintf (stderr, "  nindex[%d] = %d\n", index, nindex[index]);
#endif

	count++;
	index++;
      }  // for (iaxis)

      // iaxis = egr->nedges: use ref_node
      nindex[index] = ref_node1;

      u = msh->node[egr->edge[(egr->nedges)-1].node_id[1]*3+uaxis];

#ifdef DEBUG
      {
	double crd[3];
	crd[uaxis] = u;
	crd[vaxis] = v;
	crd[waxis] = w;
	assert(check_inside (&bbox, crd[0], crd[1], crd[2]) != 0);

	fprintf (stderr, "  nindex[%d] = %d\n", index, nindex[index]);
      }
#endif
      index++;

      base_w += dir[waxis] * layer_width[iw];
    }  // for (iw)
    base_v += dir[vaxis] * layer_width[iv];
  }  // for (iv)
  assert(index == (nlayers+1)*(nlayers+1)*(egr->nedges+1) + nlayers*nlayers*(egr->nedges+1));
  assert(count == ((nlayers+1)*(nlayers+1)-1)*(egr->nedges-1) + nlayers*nlayers*(egr->nedges-1));

#ifdef DEBUG
  fprintf (stderr, "  [%d][%d][%d] ucrd=[%d, %d)\n", axis, vdir, wdir, cpoint_origin[0], index);
#endif

  cpoint_origin[1] = index;
  for (iu = 0; iu < egr->nedges; iu++) {
    ref_edge = egr->edge + iu;
    ref_node0 = ref_edge->node_id[0];
    ref_node1 = ref_edge->node_id[1];
    u0 = msh->node[ref_node0*3+uaxis];
    u1 = msh->node[ref_node1*3+uaxis];
    u = (u0+u1)/2.0;
    
    v = msh->node[ev[0]*3+vaxis];
    for (iv = 0; iv <= nlayers; iv++) {

      base_w = msh->node[ev[0]*3+waxis];
      for (iw = 0; iw < nlayers; iw++) {
	//ref_node = eb[0]->bface[iv*nlayers+iw].cpoint;
	//w = msh->node[ref_node*3+waxis];
	w = base_w + dir[waxis]*layer_width[iw]/2.0;

	nindex[index] = count + *node_offset;
	emesh->mesh.node[count*3+uaxis] = u;
	emesh->mesh.node[count*3+vaxis] = v;
	emesh->mesh.node[count*3+waxis] = w;

#ifdef DEBUG
	    fprintf (stderr, "(v)emesh->mesh.node[%d]=(%le %le %le)\n", count, emesh->mesh.node[count*3], emesh->mesh.node[count*3+1], emesh->mesh.node[count*3+2]);
#endif
#ifdef DEBUG
	fprintf (stderr, "  nindex[%d] = %d\n", index, nindex[index]);
#endif

	count++;
	index++;

	base_w += dir[waxis]*layer_width[iw];
      }

      if (iv < nlayers) {
	v += dir[vaxis] * layer_width[iv];
      }
    }
  }
  assert(index == (nlayers+1)*(nlayers+1)*(egr->nedges+1) + nlayers*nlayers*(egr->nedges+1) + egr->nedges*nlayers*(nlayers+1));
  assert(count == ((nlayers+1)*(nlayers+1)-1)*(egr->nedges-1) + nlayers*nlayers*(egr->nedges-1) + egr->nedges*nlayers*(nlayers+1));

#ifdef DEBUG
  fprintf (stderr, "  [%d][%d][%d] vcrd=[%d, %d)\n", axis, vdir, wdir, cpoint_origin[1], index);
#endif  

  cpoint_origin[2] = index;
  for (iu = 0; iu < egr->nedges; iu++) {
    ref_edge = egr->edge + iu;
    ref_node0 = ref_edge->node_id[0];
    ref_node1 = ref_edge->node_id[1];
    u0 = msh->node[ref_node0*3+uaxis];
    u1 = msh->node[ref_node1*3+uaxis];
    u = (u0+u1)/2.0;

    base_v = msh->node[ev[0]*3+vaxis];
    for (iv = 0; iv < nlayers; iv++) {
      //ref_node = eb[0]->bface[iv*nlayers+iw].cpoint;
      //v = msh->node[ref_node*3+vaxis];
      v = base_v + dir[vaxis]*layer_width[iv]/2.0;
      
      w = msh->node[ev[0]*3+waxis];
      for (iw = 0; iw <= nlayers; iw++) {

	nindex[index] = count + *node_offset;
	emesh->mesh.node[count*3+uaxis] = u;
	emesh->mesh.node[count*3+vaxis] = v;
	emesh->mesh.node[count*3+waxis] = w;

#ifdef DEBUG
	    fprintf (stderr, "(w)emesh->mesh.node[%d]=(%le %le %le)\n", count, emesh->mesh.node[count*3], emesh->mesh.node[count*3+1], emesh->mesh.node[count*3+2]);
#endif
#ifdef DEBUG
	fprintf (stderr, "  nindex[%d] = %d\n", index, nindex[index]);
#endif

	count++;
	index++;
	
	if (iw < nlayers) {
	  w += dir[waxis] * layer_width[iw];
	}
      }  // for (iw)

      base_v += dir[vaxis]*layer_width[iv];
    }  // for (iv)
  }  // for (iu)
  assert(index == (nlayers+1)*(nlayers+1)*(egr->nedges+1) + nlayers*nlayers*(egr->nedges+1) + egr->nedges*nlayers*(nlayers+1)*2);
  assert(count == ((nlayers+1)*(nlayers+1)-1)*(egr->nedges-1) + nlayers*nlayers*(egr->nedges-1) + egr->nedges*nlayers*(nlayers+1)*2);

#ifdef DEBUG
  fprintf (stderr, "  [%d][%d][%d] wcrd=[%d, %d)\n", axis, vdir, wdir, cpoint_origin[2], index);
#endif

  *node_offset += emesh->mesh.nnodes;

  // generate elements
  du_v = 1;
  dv_v = (nlayers+1)*(egr->nedges+1);
  dw_v = (egr->nedges+1);

  du_cp[0] = 1;
  dv_cp[0] = (egr->nedges+1) * nlayers;
  dw_cp[0] = (egr->nedges+1);

  du_cp[1] = nlayers * (nlayers+1);
  dv_cp[1] = nlayers;
  dw_cp[1] = 1;

  du_cp[2] = nlayers * (nlayers+1);
  dv_cp[2] = nlayers + 1;
  dw_cp[2] = 1;

  element_id = 0;
  for (iu = 0; iu < egr->nedges; iu++) {
    for (iv = 0; iv < nlayers; iv++) {
      for (iw = 0; iw < nlayers; iw++) {
	// vertex node index in nindex[]
	bindex[0] = vertex_origin + iu   *du_v+ iv   *dv_v+ iw   *dw_v;
	bindex[1] = vertex_origin + iu   *du_v+ iv   *dv_v+(iw+1)*dw_v;
	bindex[2] = vertex_origin + iu   *du_v+(iv+1)*dv_v+ iw   *dw_v;
	bindex[3] = vertex_origin + iu   *du_v+(iv+1)*dv_v+(iw+1)*dw_v;
	bindex[4] = vertex_origin +(iu+1)*du_v+ iv   *dv_v+ iw   *dw_v;
	bindex[5] = vertex_origin +(iu+1)*du_v+ iv   *dv_v+(iw+1)*dw_v;
	bindex[6] = vertex_origin +(iu+1)*du_v+(iv+1)*dv_v+ iw   *dw_v;
	bindex[7] = vertex_origin +(iu+1)*du_v+(iv+1)*dv_v+(iw+1)*dw_v;

	// cross point node index in nindex[]
	bindex[8]  = cpoint_origin[0] + iu   *du_cp[0]+ iv   *dv_cp[0]+ iw   *dw_cp[0];
	bindex[9]  = cpoint_origin[0] +(iu+1)*du_cp[0]+ iv   *dv_cp[0]+ iw   *dw_cp[0];
	bindex[10] = cpoint_origin[1] + iu   *du_cp[1]+ iv   *dv_cp[1]+ iw   *dw_cp[1];
	bindex[11] = cpoint_origin[1] + iu   *du_cp[1]+(iv+1)*dv_cp[1]+ iw   *dw_cp[1];
	bindex[12] = cpoint_origin[2] + iu   *du_cp[2]+ iv   *dv_cp[2]+ iw   *dw_cp[2];
	bindex[13] = cpoint_origin[2] + iu   *du_cp[2]+ iv   *dv_cp[2]+(iw+1)*dw_cp[2];

#ifdef DEBUG
	fprintf (stderr, "   [%d][%d][%d] bindex(v)=(%d %d %d %d %d %d %d %d)\n", iu, iv, iw, bindex[0], bindex[1], bindex[2], bindex[3], bindex[4], bindex[5], bindex[6], bindex[7]);
	fprintf (stderr, "   [%d][%d][%d] bindex(x)=(%d %d), (%d %d), (%d %d)\n", iu, iv, iw, bindex[8], bindex[9], bindex[10], bindex[11], bindex[12], bindex[13]);
	fprintf (stderr, "   [%d][%d][%d] nindex(v)=(%d %d %d %d %d %d %d %d)\n", iu, iv, iw, nindex[bindex[0]], nindex[bindex[1]], nindex[bindex[2]], nindex[bindex[3]], nindex[bindex[4]], nindex[bindex[5]], nindex[bindex[6]], nindex[bindex[7]]);
	fprintf (stderr, "   [%d][%d][%d] nindex(x)=(%d %d), (%d %d), (%d %d)\n", iu, iv, iw, nindex[bindex[8]], nindex[bindex[9]], nindex[bindex[10]], nindex[bindex[11]], nindex[bindex[12]], nindex[bindex[13]]);
#endif

#ifdef DEBUG
	{
	  int inode;
	  for (inode = 0; inode < 14; inode++) {
	    double x, y, z;
	    int ret = get_coord (msh, mmesh, nindex[bindex[inode]], &x, &y, &z);
	    assert(ret == 0);
	    fprintf (stderr, "  [%d][%d][%d] node[%d]=(%d->%d: %le %le %le)\n", iu, iv, iw, inode, bindex[inode], nindex[bindex[inode]], x, y, z);

	    ret = check_inside (&bbox, x, y, z);
	    assert(ret != 0);
	  }
	}
#endif
	
	for (ielem = 0; ielem < nblockelements; ielem++) {
	  // element node id in bindex[]
	  n0 = dindex[ielem][0];
	  n1 = dindex[ielem][1];
	  n2 = dindex[ielem][2];
	  n3 = dindex[ielem][3];
	  
	  if (reverse) {
	    n2 = dindex[ielem][3];
	    n3 = dindex[ielem][2];
	  }
	  
#ifdef DEBUG
	  fprintf (stderr, "    (%d %d %d)[%d][%d][%d] dindex[%d]=(%d %d %d %d)\n", axis, vdir, wdir, iu, iv, iw, ielem, n0, n1, n2, n3);
	  fprintf (stderr, "               ->    bindex=(%d %d %d %d)\n", bindex[n0], bindex[n1], bindex[n2], bindex[n3]);
	  fprintf (stderr, "               ->    nindex=(%d %d %d %d)\n",nindex[bindex[n0]], nindex[bindex[n1]], nindex[bindex[n2]], nindex[bindex[n3]]);
#endif
	  
	  emesh->mesh.element[element_id*4] = nindex[bindex[n0]];
	  emesh->mesh.element[element_id*4+1] = nindex[bindex[n1]];
	  emesh->mesh.element[element_id*4+2] = nindex[bindex[n2]];
	  emesh->mesh.element[element_id*4+3] = nindex[bindex[n3]];
	  
	  emesh->mesh.volume[element_id] = edge_volume_id;

#ifdef DEBUG
	  fprintf (stderr, "              -> clockwise=%s\n",
		   is_clockwise(msh, mmesh, emesh->mesh.element+element_id*4) ? "OK" : "NG");
#endif
	  
	  element_id++;
	}
      }
    }
  }
  assert(emesh->mesh.nelements = element_id);

  *element_offset += emesh->mesh.nelements;

  // [0]:uv plane
  // generate block face group
  bf0 = emesh->bfgr;
  bf0->egr = egr;
  bf0->axis = waxis;            // base axis=w
  bf0->nvblocks = egr->nedges;  // base axis=w -> v=u
  bf0->nwblocks = nlayers;      // base axis=w -> w=v
  bf0->bface = (BlockFace *)malloc (egr->nedges*nlayers*sizeof (BlockFace));
  assert(bf0->bface != NULL);

  // [1]: uw plane
  bf1 = emesh->bfgr+1;
  bf1->egr = egr;
  bf1->axis = vaxis;            // reverse....
  bf1->nvblocks = egr->nedges;  // base axis=-w -> v=u
  bf1->nwblocks = nlayers;      // base axis=-w -> w=v
  bf1->bface = (BlockFace *)malloc (egr->nedges*nlayers*sizeof (BlockFace));
  assert(bf1->bface != NULL);
  

  for (iu = 0; iu < egr->nedges; iu++) {
    // [0] uv plane
    iw = 0;
    for (iv = 0; iv < nlayers; iv++) {
      //BlockFace *bf = bf0->bface + iu*nlayers + iv;
      BlockFace *bf = bf0->bface + iv*egr->nedges + iu;
 
      // vertex node index in nindex[]
      bindex[0] = vertex_origin + iu   *du_v+ iv   *dv_v+ iw   *dw_v;
      bindex[1] = vertex_origin + iu   *du_v+ iv   *dv_v+(iw+1)*dw_v;
      bindex[2] = vertex_origin + iu   *du_v+(iv+1)*dv_v+ iw   *dw_v;
      bindex[3] = vertex_origin + iu   *du_v+(iv+1)*dv_v+(iw+1)*dw_v;
      bindex[4] = vertex_origin +(iu+1)*du_v+ iv   *dv_v+ iw   *dw_v;
      bindex[5] = vertex_origin +(iu+1)*du_v+ iv   *dv_v+(iw+1)*dw_v;
      bindex[6] = vertex_origin +(iu+1)*du_v+(iv+1)*dv_v+ iw   *dw_v;
      bindex[7] = vertex_origin +(iu+1)*du_v+(iv+1)*dv_v+(iw+1)*dw_v;
      
      // cross point node index in nindex[]
      bindex[8]  = cpoint_origin[0] + iu   *du_cp[0]+ iv   *dv_cp[0]+ iw   *dw_cp[0];
      bindex[9]  = cpoint_origin[0] +(iu+1)*du_cp[0]+ iv   *dv_cp[0]+ iw   *dw_cp[0];
      bindex[10] = cpoint_origin[1] + iu   *du_cp[1]+ iv   *dv_cp[1]+ iw   *dw_cp[1];
      bindex[11] = cpoint_origin[1] + iu   *du_cp[1]+(iv+1)*dv_cp[1]+ iw   *dw_cp[1];
      bindex[12] = cpoint_origin[2] + iu   *du_cp[2]+ iv   *dv_cp[2]+ iw   *dw_cp[2];
      bindex[13] = cpoint_origin[2] + iu   *du_cp[2]+ iv   *dv_cp[2]+(iw+1)*dw_cp[2];
      
      bf->vertex[0][0] = nindex[bindex[0]];
      bf->vertex[0][1] = nindex[bindex[4]];
      bf->vertex[1][0] = nindex[bindex[2]];
      bf->vertex[1][1] = nindex[bindex[6]];
      bf->cpoint = nindex[bindex[12]];

#ifdef DEBUG
      fprintf (stderr, "bf0[%d][%d][%d] [%d][%d][%d]=(%d %d %d %d/%d %d %d %d)->(%d %d %d %d)\n",
	       axis, vdir, wdir, 
	       iu, iv, iw, 
	       bindex[0], bindex[1], bindex[2], bindex[3],
	       bindex[4], bindex[5], bindex[6], bindex[7],
	       bf->vertex[0][0], bf->vertex[0][1], bf->vertex[1][0], bf->vertex[1][1]);
#endif
    }
    
    // [1] wu plane
    iv = 0;
    for (iw = 0; iw < nlayers; iw++) {
      //BlockFace *bf = bf1->bface + iu*nlayers + iw;
      BlockFace *bf = bf1->bface + iw*egr->nedges + iu;

      // vertex node index in nindex[]
      bindex[0] = vertex_origin + iu   *du_v+ iv   *dv_v+ iw   *dw_v;
      bindex[1] = vertex_origin + iu   *du_v+ iv   *dv_v+(iw+1)*dw_v;
      bindex[2] = vertex_origin + iu   *du_v+(iv+1)*dv_v+ iw   *dw_v;
      bindex[3] = vertex_origin + iu   *du_v+(iv+1)*dv_v+(iw+1)*dw_v;
      bindex[4] = vertex_origin +(iu+1)*du_v+ iv   *dv_v+ iw   *dw_v;
      bindex[5] = vertex_origin +(iu+1)*du_v+ iv   *dv_v+(iw+1)*dw_v;
      bindex[6] = vertex_origin +(iu+1)*du_v+(iv+1)*dv_v+ iw   *dw_v;
      bindex[7] = vertex_origin +(iu+1)*du_v+(iv+1)*dv_v+(iw+1)*dw_v;
      
      // cross point node index in nindex[]
      bindex[8]  = cpoint_origin[0] + iu   *du_cp[0]+ iv   *dv_cp[0]+ iw   *dw_cp[0];
      bindex[9]  = cpoint_origin[0] +(iu+1)*du_cp[0]+ iv   *dv_cp[0]+ iw   *dw_cp[0];
      bindex[10] = cpoint_origin[1] + iu   *du_cp[1]+ iv   *dv_cp[1]+ iw   *dw_cp[1];
      bindex[11] = cpoint_origin[1] + iu   *du_cp[1]+(iv+1)*dv_cp[1]+ iw   *dw_cp[1];
      bindex[12] = cpoint_origin[2] + iu   *du_cp[2]+ iv   *dv_cp[2]+ iw   *dw_cp[2];
      bindex[13] = cpoint_origin[2] + iu   *du_cp[2]+ iv   *dv_cp[2]+(iw+1)*dw_cp[2];
      
      bf->vertex[0][0] = nindex[bindex[0]];
      bf->vertex[0][1] = nindex[bindex[4]];
      bf->vertex[1][0] = nindex[bindex[1]];
      bf->vertex[1][1] = nindex[bindex[5]];
      bf->cpoint = nindex[bindex[10]];

#ifdef DEBUG
      fprintf (stderr, "bf1[%d][%d][%d] [%d][%d][%d]=(%d %d %d %d/%d %d %d %d)->(%d %d %d %d)\n",
	       axis, vdir, wdir, 
	       iu, iv, iw, 
	       bindex[0], bindex[1], bindex[2], bindex[3],
	       bindex[4], bindex[5], bindex[6], bindex[7],
	       bf->vertex[0][0], bf->vertex[0][1], bf->vertex[1][0], bf->vertex[1][1]);
#endif
    }
  }
}
