/* elem_vertex.c */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pml.h"
#include "common.h"

static int ve_index[6][2] = {
  {0, 4},
  {1, 4},
  {2, 4},
  {3, 4},
  {1, 3},
  {2, 3},
};

static int vf_index[4][3] = {
  {0,1,4},
  {1,3,4},
  {3,2,4},
  {2,0,4},
};

double calc_layer_coord (double origin, int dir, double *layer_width, int layer_num) {
  int ilayer;
  double ret;
  double width;

  width = 0.0;
  if (layer_num >= 0) {
    for (ilayer = 0; ilayer <= layer_num; ilayer++) {
      width += layer_width[ilayer];
    }
  }
  ret = origin + ((double)dir) * width;

  return ret;
}

/**
 * generate vertex elements.
 * 
 * @param msh              (IN)     original mesh
 * @param element_offset   (IN/OUT) offset number of element.
 * @param node_offset      (IN/OUT) offset number of node. 
 * @param mmesh            ???
 * @param vertex_id        (IN)     origin vertex id.
 * @param vmesh            (OUT)    generated vertex mesh with edge/face data.
 * @param xdir             (IN)     direction of x (+/-).
 * @param ydir             (IN)     direction of y (+/-).
 * @param zdir             (IN)     direction of z (+/-).
 * @param nlayers          (IN)     number of generated layers
 * @param layer_width      (IN)     width of each layer
 * @param vertex_volume_id (IN)     volume id for generated elements
 */
void generate_vertex_elements
(Mesh *msh, int *element_offset, int *node_offset,
 MatchedMesh *mmesh,
 int vertex_id, VertexMesh *vmesh,
 int xdir, int ydir, int zdir,
 int nlayers, double *layer_width,
 int vertex_volume_id) {
  int iaxis;
  int ix, iy ,iz;
  double x, y, z;
  int node_id;

  int ilayer;

  int d_vertex[3];
  int d_cpoint[3][3];

  int dir[3];
  double *vertex;
  int *nindex;
  int vertex_origin;
  int cpoint_origin[3];
  int reverse;

  int uaxis, vaxis, waxis;
  int iu, iv, iw;
  int iedge;
  int iface;
  Edge *e;
  Triangle *f;
  double u, v, w;
  int dv_vertex, dw_vertex;
  int dv_cpoint, dw_cpoint;

  int ve_count;
  int vf_count;

  int fnode[5];


  int ielem;
  int element_id;

  int bindex[14];
  int n0, n1, n2, n3;

  dir[0] = xdir;
  dir[1] = ydir;
  dir[2] = zdir;


  vmesh->is_generated = 1;

  vmesh->node_offset = *node_offset;
  vmesh->element_offset = *element_offset;

#ifdef DEBUG
  fprintf (stderr, "generate_vertex_elements(): dir=[%d][%d][%d]\n", xdir, ydir, zdir);
  fprintf (stderr, "  offset=elem(%d), node(%d), vertec_volume_id(%d)\n", *element_offset, *node_offset, vertex_volume_id);
  fprintf (stderr, "  vertex_id=%d\n", vertex_id);
  fprintf (stderr, "  nlayers=%d\n", nlayers);
#endif
  
  // 1*3 or 1*1+(-1)*2 -> reverse=0
  // 1*2+(-1)*1 or (-1)*3 -> reverse=1
  reverse = (((xdir+ydir+zdir+3)/2)%2 == 0) ? 1 : 0;

  d_vertex[0] = (nlayers+1)*(nlayers+1);
  d_vertex[1] = (nlayers+1);
  d_vertex[2] = 1;
  
  d_cpoint[0][0] = 1;
  d_cpoint[0][1] = nlayers+1;
  d_cpoint[0][2] = nlayers*(nlayers+1);

  d_cpoint[1][0] = 1;
  d_cpoint[1][1] = nlayers+1;
  d_cpoint[1][2] = nlayers*(nlayers+1);

  d_cpoint[2][0] = 1;
  d_cpoint[2][1] = nlayers+1;
  d_cpoint[2][2] = nlayers*(nlayers+1);

  /*
  d_cpoint[1][0] = nlayers*(nlayers+1);
  d_cpoint[1][1] = 1;
  d_cpoint[1][2] = nlayers+1;

  d_cpoint[2][0] = nlayers+1;
  d_cpoint[2][1] = nlayers*(nlayers+1);
  d_cpoint[2][2] = 1;
  */
  vertex = msh->node + vertex_id*3;
  
  vmesh->mesh.nelements = nlayers * nlayers * nlayers * 24;
  vmesh->mesh.element = (int *)malloc (vmesh->mesh.nelements * 4 * sizeof (int));
  assert(vmesh->mesh.element != NULL);
  vmesh->mesh.volume = (int *)malloc (vmesh->mesh.nelements * 4 * sizeof (int));
  assert(vmesh->mesh.volume != NULL);
  
  vmesh->mesh.nnodes = (nlayers+1) * (nlayers+1) * (nlayers+1) - 1; // block square
  vmesh->mesh.nnodes += nlayers* nlayers*(nlayers+1)*3; // cross points
  vmesh->mesh.node = (double *)malloc (vmesh->mesh.nnodes * 3 * sizeof (double));
  assert(vmesh->mesh.node != NULL);

  nindex = (int *)malloc ((vmesh->mesh.nnodes+1) * sizeof (int));
  assert(nindex != NULL);
  
  // build square node coord.
  vertex_origin= 0;
  node_id = 0;
  x = vertex[0];
  for (ix = 0; ix <= nlayers; ix++) {
    y = vertex[1];
    for (iy = 0; iy <= nlayers; iy++) {
      z = vertex[2];
      for (iz = 0; iz <= nlayers; iz++) {
	
        if (ix == 0 && iy == 0 && iz == 0) {
          // origin vertex
	  
          nindex[node_id] = vertex_id;
        } else {

          vmesh->mesh.node[(node_id-1)*3] = x;
          vmesh->mesh.node[(node_id-1)*3+1] = y;
          vmesh->mesh.node[(node_id-1)*3+2] = z;
          nindex[node_id] = *node_offset;

	  (*node_offset)++;
	  
#ifdef DEBUG
	  fprintf (stderr, "[%d][%d][%d] vertex: nindex[%d] is (%le %le %le)\n", ix, iy, iz, node_id, x, y, z);
#endif

        }

#ifdef DEBUG
	fprintf (stderr, "[%d][%d][%d] nindex[%d]=%d\n", ix, iy, iz, node_id, nindex[node_id]);
#endif
	  

	node_id++;

	z += ((double)zdir)*layer_width[iz];
      }
      y += ((double)ydir)*layer_width[iy];
    }
    x += ((double)xdir)*layer_width[ix];
  }
  
#ifdef DEBUG
  fprintf (stderr, "generate_vertex_elements(): node_id=%d, expected=%d(nlayers=%d)\n", node_id, (nlayers+1) * (nlayers+1) * (nlayers+1), nlayers);
#endif
  
  assert(node_id == (nlayers+1) * (nlayers+1) * (nlayers+1));

  // build cross point coord.
  for (iaxis = 0; iaxis < 3; iaxis++) {
    uaxis = iaxis;
    vaxis = (iaxis+1)%3;
    waxis = (iaxis+2)%3;

    cpoint_origin[iaxis] = node_id;

#ifdef DEBUG
    fprintf (stderr, "iaxis=%d: dir=(%d %d %d), vertex=(%le %le %le)\n", iaxis, dir[uaxis], dir[vaxis], dir[waxis], vertex[uaxis], vertex[vaxis], vertex[waxis]);
#endif

    for (iw = 0; iw < nlayers; iw++) {
      w = calc_layer_coord (vertex[waxis], dir[waxis], layer_width, iw-1) + layer_width[iw]/2.0*dir[waxis];

      for (iv = 0; iv < nlayers; iv++) {
	v = calc_layer_coord (vertex[vaxis], dir[vaxis], layer_width, iv-1) + layer_width[iv]/2.0*dir[vaxis];
	
	for (iu = 0; iu <= nlayers; iu++) {
	  u = calc_layer_coord (vertex[uaxis], dir[uaxis], layer_width, iu-1);

          vmesh->mesh.node[(node_id-1)*3+uaxis] = u;
          vmesh->mesh.node[(node_id-1)*3+vaxis] = v;
	  vmesh->mesh.node[(node_id-1)*3+waxis] = w;
          nindex[node_id] = *node_offset;

	  (*node_offset)++;

#ifdef DEBUG
	  fprintf (stderr, "[%d][%d][%d] cpoint: nindex[%d](=%d) is (%le %le %le) (width=%le)\n", iu, iv, iw, node_id, nindex[node_id], vmesh->mesh.node[(node_id-1)*3], vmesh->mesh.node[(node_id-1)*3+1], vmesh->mesh.node[(node_id-1)*3+2], layer_width[iw]);
#endif

          node_id++;
        }
      }
    }

#ifdef DEBUG
    fprintf (stderr, "generate_vertex_elements(): iaxis=%d, expected=%d(nlayers=%d)\n", iaxis, (iaxis+1)*nlayers*nlayers*(nlayers+1) + (nlayers+1) * (nlayers+1) * (nlayers+1), nlayers);
#endif
  
    assert(node_id ==(iaxis+1)*nlayers*nlayers*(nlayers+1) + (nlayers+1) * (nlayers+1) * (nlayers+1));

  }

  assert(node_id == (nlayers+1)*(nlayers+1)*(nlayers+1) + 3*nlayers*nlayers*(nlayers+1));
  assert(cpoint_origin[0] == vertex_origin    + (nlayers+1)*(nlayers+1)*(nlayers+1));
  assert(cpoint_origin[1] == cpoint_origin[0] + nlayers*nlayers*(nlayers+1));
  assert(cpoint_origin[2] == cpoint_origin[1] + nlayers*nlayers*(nlayers+1));
  
#ifdef DEBUG
  fprintf (stderr, "generate_vertex_elements(): node_id=%d, expected=%d(nlayers=%d)\n", node_id, vmesh->mesh.nnodes+1, nlayers);
#endif
  
  assert(node_id == vmesh->mesh.nnodes+1);
  
  // axis edges
  vmesh->nlayers = nlayers;
  for (iaxis = 0; iaxis < 3; iaxis++) {
    // ve_edge & ve_face
    uaxis = iaxis;
    vaxis = (iaxis+1)%3;
    waxis = (iaxis+2)%3;
    
    dv_vertex = d_vertex[vaxis];
    dw_vertex = d_vertex[waxis];
    
    dv_cpoint = d_cpoint[iaxis][1];
    dw_cpoint = d_cpoint[iaxis][2];
    
    // axis edges
    vmesh->axis_edge[iaxis] = (Edge *)malloc (nlayers * sizeof (Edge));
    assert(vmesh->axis_edge[iaxis] != NULL);
    
    vmesh->ve_edge[iaxis].nedges = nlayers*nlayers*6;
    vmesh->ve_edge[iaxis].edge = (Edge *)malloc (vmesh->ve_edge[iaxis].nedges * sizeof (Edge));
    assert(vmesh->ve_edge[iaxis].edge != NULL);

    vmesh->ve_face[iaxis].ntriangles = nlayers*nlayers*4;
    vmesh->ve_face[iaxis].triangle = (Triangle *)malloc (vmesh->ve_face[iaxis].ntriangles * sizeof (Triangle));
    assert(vmesh->ve_face[iaxis].triangle != NULL);

    vmesh->ve_bface[iaxis].axis = iaxis;
    vmesh->ve_bface[iaxis].nvblocks = nlayers;
    vmesh->ve_bface[iaxis].nwblocks = nlayers;
    vmesh->ve_bface[iaxis].bface = (BlockFace *)malloc (nlayers*nlayers*sizeof (BlockFace));
    assert(vmesh->ve_bface[iaxis].bface != NULL);

    // axis_edge
    //set_edge (vmesh->axis_edge[iaxis], nindex[vertex_id], nindex[d_vertex[iaxis]]);
    set_edge (vmesh->axis_edge[iaxis], vertex_id, nindex[d_vertex[iaxis]]);

    for (ilayer = 1; ilayer < nlayers; ilayer++) {
      set_edge
        (vmesh->axis_edge[iaxis] + ilayer,
         nindex[ilayer * d_vertex[iaxis]],
         nindex[(ilayer+1) * d_vertex[iaxis]]);
    }

    // edge/face/bface
    ve_count = 0;
    vf_count = 0;
    for (iv = 0; iv < nlayers; iv++) {
      for (iw = 0; iw < nlayers; iw++) {
	BlockFace *bface = vmesh->ve_bface[iaxis].bface + iv*nlayers+iw;

        fnode[0] =  iv    * dv_vertex +  iw    * dw_vertex;
        fnode[1] =  iv    * dv_vertex + (iw+1) * dw_vertex;
        fnode[2] = (iv+1) * dv_vertex +  iw    * dw_vertex;
        fnode[3] = (iv+1) * dv_vertex + (iw+1) * dw_vertex;
        fnode[4] =  iv    * dv_cpoint +  iw    * dw_cpoint + cpoint_origin[iaxis];
        
#ifdef DEBUG
	fprintf (stderr, "[axis %d][%d][%d] fnode=(%d %d %d %d %d)\n", iaxis, iv, iw, fnode[0], fnode[1], fnode[2], fnode[3], fnode[4]);
#endif
	
	bface->vertex[0][0] = nindex[fnode[0]]; 
	bface->vertex[0][1] = nindex[fnode[1]]; 
	bface->vertex[1][0] = nindex[fnode[2]]; 
	bface->vertex[1][1] = nindex[fnode[3]]; 
	bface->cpoint = nindex[fnode[4]]; 

#ifdef DEBUG
	fprintf (stderr, "[axis %d][%d][%d] bface=((%d %d) (%d %d) %d)\n", iaxis, iv, iw, bface->vertex[0][0], bface->vertex[0][1], bface->vertex[1][0], bface->vertex[1][1], bface->cpoint);
#endif
	

        // edge
        for (iedge = 0; iedge < 6; iedge++) {
          e = vmesh->ve_edge[iaxis].edge + ve_count;
          set_edge (e, nindex[fnode[ve_index[iedge][0]]], nindex[fnode[ve_index[iedge][1]]]);
          ve_count++;
        }

        // face
        for (iface = 0; iface < 4; iface++) {
          f = vmesh->ve_face[iaxis].triangle + vf_count;
          set_triangle (f, nindex[fnode[vf_index[iface][0]]], nindex[fnode[vf_index[iface][1]]], nindex[fnode[vf_index[iface][2]]]);
          vf_count++;
        }
      }
    }
  }

  // divide block
  element_id = 0;
  for (ix = 0; ix < nlayers; ix++) {
    for (iy = 0; iy < nlayers; iy++) {
      for (iz = 0; iz < nlayers; iz++) {
	// vertex node index in nindex[]
	bindex[0] = vertex_origin + ( ix   *(nlayers+1)+ iy   )*(nlayers+1)+ iz;
	bindex[1] = vertex_origin + ( ix   *(nlayers+1)+ iy   )*(nlayers+1)+(iz+1);
	bindex[2] = vertex_origin + ( ix   *(nlayers+1)+(iy+1))*(nlayers+1)+ iz;
	bindex[3] = vertex_origin + ( ix   *(nlayers+1)+(iy+1))*(nlayers+1)+(iz+1);
	bindex[4] = vertex_origin + ((ix+1)*(nlayers+1)+ iy   )*(nlayers+1)+ iz;
	bindex[5] = vertex_origin + ((ix+1)*(nlayers+1)+ iy   )*(nlayers+1)+(iz+1);
	bindex[6] = vertex_origin + ((ix+1)*(nlayers+1)+(iy+1))*(nlayers+1)+ iz;
	bindex[7] = vertex_origin + ((ix+1)*(nlayers+1)+(iy+1))*(nlayers+1)+(iz+1);

	// cross point node index in nindex[]
	bindex[8]  = cpoint_origin[0] + (iz*nlayers+iy)*(nlayers+1)+ ix;
	bindex[9]  = cpoint_origin[0] + (iz*nlayers+iy)*(nlayers+1)+(ix+1);
	bindex[10] = cpoint_origin[1] + (ix*nlayers+iz)*(nlayers+1)+ iy;
	bindex[11] = cpoint_origin[1] + (ix*nlayers+iz)*(nlayers+1)+(iy+1);
	bindex[12] = cpoint_origin[2] + (iy*nlayers+ix)*(nlayers+1)+ iz;
	bindex[13] = cpoint_origin[2] + (iy*nlayers+ix)*(nlayers+1)+(iz+1);

#ifdef DEBUG
	fprintf (stderr, "   [%d][%d][%d] bindex(v)=(%d %d %d %d %d %d %d %d)\n", ix, iy, iz, bindex[0], bindex[1], bindex[2], bindex[3], bindex[4], bindex[5], bindex[6], bindex[7]);
	fprintf (stderr, "   [%d][%d][%d] bindex(x)=(%d %d), (%d %d), (%d %d)\n", ix, iy, iz, bindex[8], bindex[9], bindex[10], bindex[11], bindex[12], bindex[13]);
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
	  fprintf (stderr, "    [%d][%d][%d] dindex[%d]=(%d %d %d %d)\n", ix, iy, iz, ielem, n0, n1, n2, n3);
	  fprintf (stderr, "               ->    bindex=(%d %d %d %d)\n", bindex[n0], bindex[n1], bindex[n2], bindex[n3]);
	  fprintf (stderr, "               ->    nindex=(%d %d %d %d)\n",nindex[bindex[n0]], nindex[bindex[n1]], nindex[bindex[n2]], nindex[bindex[n3]]);
#endif

	  vmesh->mesh.element[element_id*4] = nindex[bindex[n0]];
	  vmesh->mesh.element[element_id*4+1] = nindex[bindex[n1]];
	  vmesh->mesh.element[element_id*4+2] = nindex[bindex[n2]];
	  vmesh->mesh.element[element_id*4+3] = nindex[bindex[n3]];

	  vmesh->mesh.volume[element_id] = vertex_volume_id;

	  element_id++;
	}
      }
    }
  }



#ifdef DEBUG
  fprintf (stderr, "[%d][%d][%d] : axis_edge\n", xdir, ydir, zdir);
#endif
}
