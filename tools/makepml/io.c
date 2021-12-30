/* io.c */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pml.h"
#include "common.h"

void read_msh (Mesh *msh, const char *filename) {
  FILE *fp;

  int ielem;
  int inode;
  int ivol;

  int n;
  double x, y, z;

  fp = fopen (filename, "r");
  if (fp == NULL) {
    fprintf (stderr, "ERROR: cannot open msh file [%s]\n", filename);
    exit (100);
  }
  
  // element
  fscanf (fp, "%d", &(msh->nelements));
  assert(msh->nelements > 0);

  msh->element = (int *)malloc (msh->nelements * NNODES_ELEM * sizeof (int));
  assert(msh->element != NULL);
  msh->volume = (int *)calloc (msh->nelements, sizeof (int));
  assert(msh->volume != NULL);

  for (ielem = 0; ielem < msh->nelements; ielem++) {
    for (inode = 0; inode < NNODES_ELEM; inode++) {
      fscanf (fp, "%d", &n);
      msh->element[ielem*NNODES_ELEM+inode] = n;
    }
  }

  // node
  fscanf (fp, "%d", &(msh->nnodes));
  assert(msh->nnodes > 0);

  msh->node = (double *)malloc (msh->nnodes * 3 * sizeof (double));
  assert(msh->node != NULL);

  for (inode = 0; inode < msh->nnodes; inode++) {
    fscanf (fp, "%lf %lf %lf\n", &x, &y, &z);
    msh->node[inode*3] = x;
    msh->node[inode*3+1] = y;
    msh->node[inode*3+2] = z;
  }

  // volume
  if (fscanf (fp, "%d", &(msh->nvolumes)) == 1) {
    int nelems;
    int elem_id;

    // volume definition found.
    for (ivol = 0; ivol < msh->nvolumes; ivol++) {
      fscanf (fp, "%d", &nelems);
      
      for (ielem = 0; ielem < nelems; ielem++) {
        fscanf (fp, "%d", &elem_id);
        msh->volume[elem_id] = ivol;
      }
    }
  } else {
    // no volume definition
    msh->nvolumes = 1;
  }
  
  fprintf (stdout, "original: nnodes=%d, nelements=%d, nvolumes=%d\n", msh->nnodes, msh->nelements, msh->nvolumes);

  fclose (fp);
}

static void update_volume_range (Mesh *msh, int *vmin, int *vmax) {
  int ielem;
  int v;

  for (ielem = 0; ielem < msh->nelements; ielem++) {
    v = msh->volume[ielem];
    *vmin = (*vmin < v) ? *vmin : v;
    *vmax = (*vmax > v) ? *vmax : v;
  }
}

static void update_volume_count (Mesh *msh, int vmin, int vmax, int *vol_count) {
  int ielem;
  int vol;

  for (ielem = 0; ielem < msh->nelements; ielem++) {
    vol = msh->volume[ielem];
    assert(vol >= vmin);
    assert(vol <= vmax);

    vol_count[vol - vmin]++;
  }
}

static int write_volume_elements (Mesh *msh, int offset_elem, int volume_id, FILE *fp) {
  int ielem;
  int vol;
  int count;

  count = 0;
  for (ielem = 0; ielem < msh->nelements; ielem++) {
    vol = msh->volume[ielem];

    if (vol == volume_id) {
      fprintf (fp, "  %d\n", offset_elem + ielem);
      count++;
    }
  }
  return count;
}

static void write_element (FILE *fp, Mesh *msh) {
  int ielem;

  for (ielem = 0; ielem < msh->nelements; ielem++) {
    fprintf
      (fp, " %d %d %d %d\n",
       msh->element[ielem*4],
       msh->element[ielem*4+1],
       msh->element[ielem*4+2],
       msh->element[ielem*4+3]);
  }
}

static void write_node (FILE *fp, Mesh *msh) {
  int inode;
  for (inode = 0; inode < msh->nnodes; inode++) {
    fprintf
      (fp, " % 23.16le % 23.16le % 23.16le\n",
       msh->node[inode*3],
       msh->node[inode*3+1],
       msh->node[inode*3+2]);
  }
}

static void summarize_msh (Mesh *submesh, int *nelements, int *nnodes, int *vmin, int *vmax) {
  *nelements += submesh->nelements;
  *nnodes += submesh->nnodes;
  update_volume_range (submesh, vmin, vmax);
}

void write_msh (Mesh *omesh, MatchedMesh *mmesh, const char *filename) {
  FILE *fp;

  int nelements;
  int nnodes;
  int vmin, vmax;

  int ix, iy, iz;
  int iaxis, iv, iw;
  int iside;

  int *vol_count;

  Mesh *msh;

  fp = fopen (filename, "w");
  assert(fp != NULL);

  // sum
  // original
  nelements = omesh->nelements;
  nnodes = omesh->nnodes;
  vmin = vmax = 0;
  update_volume_range (omesh, &vmin, &vmax);
  
#ifdef ENABLE_GENERATE_VERTEX_ELEMENTS
  // vertex
  for (ix = 0; ix < 2; ix++) {
    for (iy = 0; iy < 2; iy++) {
      for (iz = 0; iz < 2; iz++) {
	if (mmesh->vmesh[ix][iy][iz].is_generated) {
	  msh = &(mmesh->vmesh[ix][iy][iz].mesh);
	  summarize_msh (msh, &nelements, &nnodes, &vmin, &vmax);
	}
      }
    }
  }
#endif

#ifdef ENABLE_GENERATE_EDGE_ELEMENTS
  // edge
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iv = 0; iv < 2; iv++) {
      for (iw = 0; iw < 2; iw++) {
	if (mmesh->emesh[iaxis][iv][iw].is_generated) {
	  msh = &(mmesh->emesh[iaxis][iv][iw].mesh);
	  summarize_msh (msh, &nelements, &nnodes, &vmin, &vmax);
	}
      }
    }
  }
#endif

#ifdef ENABLE_GENERATE_FACE_ELEMENTS
  // face
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iside = 0; iside < 2; iside++) {
      if (mmesh->smesh[iaxis][iside].is_generated) {
	msh = &(mmesh->smesh[iaxis][iside].mesh);
	summarize_msh (msh, &nelements, &nnodes, &vmin, &vmax);
      }
    }
  }
#endif

  fprintf (stdout, "total: nnodes=%d, nelements=%d, nvolumes=%d\n", nnodes, nelements, vmax-vmin+1);


  fprintf (fp, "%d\n", nelements);

  // nelements
  // original
  msh = omesh;
  write_element (fp, msh);
  
#ifdef ENABLE_GENERATE_VERTEX_ELEMENTS
  // vertex
  for (ix = 0; ix < 2; ix++) {
    for (iy = 0; iy < 2; iy++) {
      for (iz = 0; iz < 2; iz++) {
	if (mmesh->vmesh[ix][iy][iz].is_generated) {
	  msh = &(mmesh->vmesh[ix][iy][iz].mesh);
	  write_element (fp, msh);
	}
      }
    }
  }
#endif

#ifdef ENABLE_GENERATE_EDGE_ELEMENTS
  // edge
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iv = 0; iv < 2; iv++) {
      for (iw = 0; iw < 2; iw++) {
	if (mmesh->emesh[iaxis][iv][iw].is_generated) {
	  msh = &(mmesh->emesh[iaxis][iv][iw].mesh);
	  write_element (fp, msh);
	}
      }
    }
  }
#endif

#ifdef ENABLE_GENERATE_FACE_ELEMENTS
  // face
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iside = 0; iside < 2; iside++) {
      if (mmesh->smesh[iaxis][iside].is_generated) {
	msh = &(mmesh->smesh[iaxis][iside].mesh);
	write_element (fp, msh);
      }
    }
  }
#endif

  // node
  fprintf (fp, "%d\n", nnodes);

  msh = omesh;
  write_node (fp, msh);
  
#ifdef ENABLE_GENERATE_VERTEX_ELEMENTS
  // vertex
  for (ix = 0; ix < 2; ix++) {
    for (iy = 0; iy < 2; iy++) {
      for (iz = 0; iz < 2; iz++) {
	if (mmesh->vmesh[ix][iy][iz].is_generated) {
	  msh = &(mmesh->vmesh[ix][iy][iz].mesh);
	  write_node (fp, msh);
	}
      }
    }
  }
#endif
  
#ifdef ENABLE_GENERATE_EDGE_ELEMENTS
  // edge
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iv = 0; iv < 2; iv++) {
      for (iw = 0; iw < 2; iw++) {
	if (mmesh->emesh[iaxis][iv][iw].is_generated) {
	  msh = &(mmesh->emesh[iaxis][iv][iw].mesh);
	  write_node (fp, msh);
	}
      }
    }
  }
#endif

#ifdef ENABLE_GENERATE_FACE_ELEMENTS
  // face
  for (iaxis = 0; iaxis < 3; iaxis++) {
    for (iside = 0; iside < 2; iside++) {
      if (mmesh->smesh[iaxis][iside].is_generated) {
	msh = &(mmesh->smesh[iaxis][iside].mesh);
	write_node (fp, msh);
      }
    }
  }
#endif

  // volume
  if (vmin < vmax) {
    int ivol;
    int count;
    int offset_elem;

    vol_count = (int *)calloc (vmax - vmin + 1, sizeof (int));
    assert(vol_count != NULL);

    msh = omesh;
    update_volume_count (msh, vmin, vmax, vol_count);

#ifdef ENABLE_GENERATE_VERTEX_ELEMENTS
    // vertex
    for (ix = 0; ix < 2; ix++) {
      for (iy = 0; iy < 2; iy++) {
	for (iz = 0; iz < 2; iz++) {
	  if (mmesh->vmesh[ix][iy][iz].is_generated) {
	    msh = &(mmesh->vmesh[ix][iy][iz].mesh);
	    update_volume_count (msh, vmin, vmax, vol_count);
	  }
	}
      }
    }
#endif

#ifdef ENABLE_GENERATE_EDGE_ELEMENTS
    // edge
    for (iaxis = 0; iaxis < 3; iaxis++) {
      for (iv = 0; iv < 2; iv++) {
	for (iw = 0; iw < 2; iw++) {
	  if (mmesh->emesh[iaxis][iv][iw].is_generated) {
	    msh = &(mmesh->emesh[iaxis][iv][iw].mesh);
	    update_volume_count (msh, vmin, vmax, vol_count);
	  }
	}
      }
    }
#endif
    
#ifdef ENABLE_GENERATE_FACE_ELEMENTS
    // face
    for (iaxis = 0; iaxis < 3; iaxis++) {
      for (iside = 0; iside < 2; iside++) {
	if (mmesh->smesh[iaxis][iside].is_generated) {
	  msh = &(mmesh->smesh[iaxis][iside].mesh);
	  update_volume_count (msh, vmin, vmax, vol_count);
	}
      }
    }
#endif
    
    fprintf (fp, "%d\n", vmax+1);
    
    for (ivol = 0; ivol < vmin; ivol++) {
      fprintf (fp, "%d\n", 0);
    }
    for (ivol = vmin; ivol <= vmax; ivol++) {
      fprintf (fp, " %d\n", vol_count[ivol-vmin]);
      
      // original
      offset_elem = 0;
      
      msh = omesh;
      count = write_volume_elements (msh, offset_elem, ivol, fp);
      offset_elem += msh->nelements;
      
#ifdef ENABLE_GENERATE_VERTEX_ELEMENTS
      // vertex
      for (ix = 0; ix < 2; ix++) {
	for (iy = 0; iy < 2; iy++) {
	  for (iz = 0; iz < 2; iz++) {
	    if (mmesh->vmesh[ix][iy][iz].is_generated) {
	      msh = &(mmesh->vmesh[ix][iy][iz].mesh);
	      count += write_volume_elements (msh, offset_elem, ivol, fp);
	      offset_elem += msh->nelements;
	    }
	  }
	}
      }
#endif

#ifdef ENABLE_GENERATE_EDGE_ELEMENTS
      // edge
      for (iaxis = 0; iaxis < 3; iaxis++) {
	for (iv = 0; iv < 2; iv++) {
	  for (iw = 0; iw < 2; iw++) {
	    if (mmesh->emesh[iaxis][iv][iw].is_generated) {
	      msh = &(mmesh->emesh[iaxis][iv][iw].mesh);
	      count += write_volume_elements (msh, offset_elem, ivol, fp);
	      offset_elem += msh->nelements;
	    }
	  }
	}
      } 
#endif

#ifdef ENABLE_GENERATE_FACE_ELEMENTS
      // face
      for (iaxis = 0; iaxis < 3; iaxis++) {
	for (iside = 0; iside < 2; iside++) {
	  if (mmesh->smesh[iaxis][iside].is_generated) {
	    msh = &(mmesh->smesh[iaxis][iside].mesh);
	    count += write_volume_elements (msh, offset_elem, ivol, fp);
	    offset_elem += msh->nelements;
	  }
	}
      }
#endif

      assert(count == vol_count[ivol-vmin]);

    } // for (ivol)
    
    free (vol_count);
  }
  
  fclose (fp);
}

