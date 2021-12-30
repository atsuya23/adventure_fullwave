/* pml.c */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pml.h"

static void get_vertex_index (int axis, int v, int w, int vindex[2][3]) {
  if (axis == 0) {
    // axis=x : v=y, w=z
    vindex[0][0] = 0;
    vindex[1][0] = 1;
    vindex[0][1] = vindex[1][1] = v;
    vindex[0][2] = vindex[1][2] = w;
  } else if (axis == 1) {
    // axis=y : v=z, w=x
    vindex[0][1] = 0;
    vindex[1][1] = 1;
    vindex[0][2] = vindex[1][2] = v;
    vindex[0][0] = vindex[1][0] = w;
  } else {
    // axis=z : v=x, w=y
    vindex[0][2] = 0;
    vindex[1][2] = 1;
    vindex[0][0] = vindex[1][0] = v;
    vindex[0][1] = vindex[1][1] = w;
  }
}

/**
 * generate layer elements & nodes.
 *
 * @param msh          source mesh data.
 * @param surface      surface data of source mesh.
 * @param mmesh        generated mesh data
 * @param nlayers      number of generated layers
 * @param layer_width  array of width of layers
 */
static void generate_layers (Mesh *msh, Surface *surface, MatchedMesh *mmesh, int nlayers, double *layer_width) {
  int ix, iy, iz;
  int iaxis;
  int iv, iw;
  int idir;
  int ilayer;
  int vertex_volume_id;
  int edge_volume_id;
  int *face_volume_id;

  int  vaxis, waxis;
  int vindex[2][3];
  
  mmesh->nelements = msh->nelements;
  mmesh->nnodes = msh->nnodes;
  mmesh->nvolumes = msh->nvolumes;

#ifdef ENABLE_GENERATE_VERTEX_ELEMENTS
  // all vertex elements join same volume.
  vertex_volume_id = mmesh->nvolumes;
  mmesh->nvolumes++;

  // generate vertex elements
  for (ix = 0; ix < 2; ix++) {
    for (iy = 0; iy < 2; iy++) {
      for (iz = 0; iz < 2; iz++) {
        generate_vertex_elements
          (msh, &(mmesh->nelements), &(mmesh->nnodes),
	   mmesh,
           surface->vertex[ix][iy][iz],
           &(mmesh->vmesh[ix][iy][iz]),
           ix*2-1, iy*2-1, iz*2-1,
           nlayers, layer_width,
	   vertex_volume_id);

#ifdef DEBUG
	{
	  char debug_msh[1024];
	  sprintf (debug_msh, "%s_v_%d_%d_%d.msh", "debug", ix, iy, iz);
	  write_msh (msh, mmesh, debug_msh);
	}
#endif

	fprintf (stdout, "vertex[%d][%d][%d] : nnodes+=%d, nelements+=%d\n", ix, iy, iz, mmesh->vmesh[ix][iy][iz].mesh.nnodes, mmesh->vmesh[ix][iy][iz].mesh.nelements);

      }
    }
  }
#endif

#ifdef ENABLE_GENERATE_EDGE_ELEMENTS
  // all edge elements join same volume.
  edge_volume_id = mmesh->nvolumes;
  mmesh->nvolumes++;

  // generate edge elements
  for (iaxis = 0; iaxis < 3; iaxis++) {
    vaxis = (iaxis+1)%3;
    waxis = (iaxis+2)%3;

    for (iv = 0; iv < 2; iv++) {
      for (iw = 0; iw < 2; iw++) {
	VertexMesh *vmesh[2];
	int ev[2];
	Edge *ee[2][2];
	FaceGroup *ef[2];
	BlockFaceGroup *eb[2];

	// get node ids of edge vertice.
	get_vertex_index (iaxis, iv, iw, vindex);

	// vertices of the edge
	ev[0] = surface->vertex[vindex[0][0]][vindex[0][1]][vindex[0][2]];
	ev[1] = surface->vertex[vindex[1][0]][vindex[1][1]][vindex[1][2]];

	// nodes on the edge
	vmesh[0] = &(mmesh->vmesh[vindex[0][0]][vindex[0][1]][vindex[0][2]]);
	vmesh[1] = &(mmesh->vmesh[vindex[1][0]][vindex[1][1]][vindex[1][2]]);

	ee[0][0] = vmesh[0]->axis_edge[vaxis];
	ee[0][1] = vmesh[0]->axis_edge[waxis];
	ee[1][0] = vmesh[1]->axis_edge[vaxis];
	ee[1][1] = vmesh[1]->axis_edge[waxis];

	ef[0] = vmesh[0]->ve_face + iaxis;
	ef[1] = vmesh[1]->ve_face + iaxis;

	eb[0] = vmesh[0]->ve_bface + iaxis;
	eb[1] = vmesh[1]->ve_bface + iaxis;
	
        generate_edge_elements
          (msh, &(mmesh->nelements), &(mmesh->nnodes),
	   mmesh,
           &(surface->egr[iaxis][iv][iw]),
	   ev, ee, ef, eb, 
	   &(mmesh->emesh[iaxis][iv][iw]),
           iaxis, iv*2-1, iw*2-1,
           nlayers, layer_width,
	   edge_volume_id);

#ifdef DEBUG
	{
	  char debug_msh[1024];
	  sprintf (debug_msh, "%s_e_%d_%d_%d.msh", "debug", iaxis, iv, iw);
	  write_msh (msh, mmesh, debug_msh);
	}
#endif

  	fprintf (stdout, "  edge[%d][%d][%d] : nnodes+=%d, nelements+=%d\n", iaxis, iv, iw, mmesh->emesh[iaxis][iv][iw].mesh.nnodes, mmesh->emesh[iaxis][iv][iw].mesh.nelements);
	
      }
    }
  }
#endif

#ifdef ENABLE_GENERATE_FACE_ELEMENTS
  // generate face elements
  face_volume_id = (int *)malloc (nlayers * sizeof (int));
  assert(face_volume_id != NULL);

  for (iaxis = 0; iaxis < 3; iaxis++) {
    vaxis = (iaxis+1)%3;
    waxis = (iaxis+2)%3;

    for (ilayer = 0; ilayer < nlayers; ilayer++) {
      // face elements of each faces in same layer
      // join same volumes.
      face_volume_id[ilayer] = mmesh->nvolumes;
      mmesh->nvolumes++;
    }
    
    for (idir = 0; idir < 2; idir++) {
      // generate_face_elements
      // bfg[0][0] : edge of vaxis-
      // bfg[0][1] : edge of vaxis+
      // bfg[1][0] : edge of waxis-
      // bfg[1][1] : edge of waxis+
      BlockFaceGroup *bfg[2][2];
      FaceGroup *target_fgr;
      FaceMesh *target_msh;
      
      //bfg[0][0] = mmesh->emesh[vaxis][idir][0].bfgr;
      //bfg[0][1] = mmesh->emesh[vaxis][idir][1].bfgr;
      bfg[0][0] = mmesh->emesh[vaxis][0][idir].bfgr + 1;
      bfg[0][1] = mmesh->emesh[vaxis][1][idir].bfgr + 1;
      bfg[1][0] = mmesh->emesh[waxis][idir][0].bfgr;
      bfg[1][1] = mmesh->emesh[waxis][idir][1].bfgr;
    
      target_fgr = &(surface->fgr[iaxis][idir]);
      target_msh = &(mmesh->smesh[iaxis][idir]);
      
      generate_face_elements
	(msh,
	 &(mmesh->nelements), &(mmesh->nnodes), mmesh,
	 target_msh, target_fgr, bfg,
	 iaxis, idir*2-1,
	 nlayers, layer_width,
	 face_volume_id);

#ifdef DEBUG
	{
	  char debug_msh[1024];
	  sprintf (debug_msh, "%s_s_%d_%d.msh", "debug", iaxis, idir);
	  write_msh (msh, mmesh, debug_msh);
	}
#endif

	fprintf (stdout, "  face[%d][%d]    : nnodes+=%d, nelements+=%d\n", iaxis, idir, mmesh->smesh[iaxis][idir].mesh.nnodes, mmesh->smesh[iaxis][idir].mesh.nelements);
    }
  }
#endif
}

/**
 * usage: pml input_msh output_msh nlayers width_L1 width_L2 ...
 */
int main (int argc, char *argv[]) {
  char *input_msh;
  char *output_msh;

  int nlayers;
  double *layer_width;

  Mesh msh;
  Surface surface;

  MatchedMesh mmesh;

  int iarg;
  double width;

  if (argc < 5) {
    fprintf (stderr, "usage: pml input_msh output_msh nlayers width_L1 width_L2 ...\n");
    exit (1);
  }

#ifdef DEBUG
  for (iarg = 0; iarg < argc; iarg++) {
    fprintf (stderr, "param[%d/%d]=[%s]\n", iarg, argc, argv[iarg]);
  }
#endif

  input_msh = argv[1];
  output_msh = argv[2];

  nlayers = atoi (argv[3]);
  if (nlayers <= 0) {
    fprintf (stderr, "ERROR: invalid nlayers : [%s]\n", argv[2]);
    exit (2);
  }

  layer_width = (double *)calloc (nlayers, sizeof (double));
  assert(layer_width != NULL);

  for (iarg = 4; iarg < argc; iarg++) {
    width = atof (argv[iarg]);
    if (width <= 0.0) {
      fprintf (stderr, "ERROR: invalid layer width : [%s]\n", argv[iarg]);
      exit (3);
    }
     
    layer_width[iarg-4] = width;
    
#ifdef DEBUG
    fprintf (stderr, "layer_width[%d]=%le\n", iarg-4, layer_width[iarg-4]);
#endif
  }

  read_msh (&msh, input_msh);
  setup (&msh, &surface, &mmesh);
  generate_layers (&msh, &surface, &mmesh, nlayers, layer_width);
  write_msh (&msh, &mmesh, output_msh);
  
  return 0;

}

