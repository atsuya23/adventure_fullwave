/* pml.h */
#ifndef __PML_H__
#define __PML_H__

#define NNODES_ELEM 4
#define EPS 1e-6

#define ENABLE_GENERATE_VERTEX_ELEMENTS
#define ENABLE_GENERATE_EDGE_ELEMENTS
#define ENABLE_GENERATE_FACE_ELEMENTS

#include "pml_types.h"
//#include "common.h"

extern const int nblockelements;
extern const int dindex[24][4];

// io.c
void read_msh (Mesh *msh, const char *filename);
void write_msh (Mesh *msh, MatchedMesh *mmesh, const char *filename);

// setup.c
void setup (Mesh *msh, Surface *surface, MatchedMesh *mmesh);

// elem_vertex.c
void generate_vertex_elements
(Mesh *msh, int *element_offset, int *node_offset,
 MatchedMesh *mmsh,
 int vertex_id, VertexMesh *vmesh,
 int xdir, int ydir, int zdir,
 int nlayers, double *layer_width,
 int vertex_volume_id);

// elem_edge.c
void generate_edge_elements
(Mesh *msh, int *element_offset, int *node_offset,
 MatchedMesh *mmsh,
 EdgeGroup *egr,
 int ev[2], Edge *ee[2][2], FaceGroup *ef[2], BlockFaceGroup *eb[2],
 EdgeMesh *emesh,
 int axis, int vdir, int wdir,
 int nlayers, double *layer_width,
 int edge_volume_id);
// elem_face.c
/**
 * generate elements for a selected face.
 *
 * @param msh            original mesh.
 * @param nelements      # of elements.
 * @param nnodes         # of nodes.
 * @param mmesh          generated mesh groups.
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
 int *nelements, int *nnodes, MatchedMesh *mmesh,
 FaceMesh *fmesh, FaceGroup *fgr, BlockFaceGroup *bfg[][2],
 int iaxis, int side,
 int nlayers, double *layer_width,
 int *face_volume_id);

#endif /* __PML_H__ */
