/* pml_types.h */
#ifndef __PML_TYPES_H__
#define __PML_TYPES_H__

#define NNODES_ELEM 4
#define EPS 1e-6

typedef struct _Triangle Triangle;

/**
 * edge definition.
 */
typedef struct {
  /**
   * edge vertex ids
   */
  int node_id[2];

  /**
   * (not used) triangles sharing the edge.
   */
  Triangle *triangle[2];
} Edge;

/**
 * triangle definition.
 */
struct _Triangle {
  /**
   * triangle vertex ids.
   */
  int node_id[3];
  /**
   * flag that means the triangle is clockwise or not.<br>
   * clockwise=1, counter-clockwise=-1
   */
  char reverse_flag;

  /**
   * edges of triangle
   */
  Edge *edge[3];
};

typedef struct {
  double xmin, xmax;
  double ymin, ymax;
  double zmin, zmax;
} BoundingBox;

typedef struct {
  int nedges;
  Edge *edge;
} EdgeGroup;

/**
 * face group
 */
typedef struct {
  /**
   * number of nodes on the face group.
   */
  int nnodes;
  /**
   * node ids
   */
  int *node_id;

  /**
   * boundary edges for up/down/left/right
   */
  EdgeGroup *boundary[2][2];
  
  // inner edges of the face group.
  int nedges;
  Edge *edge;

  // triangles of the face group.
  // TODO : need reference of source element?
  int ntriangles;
  Triangle *triangle;
} FaceGroup;

typedef struct {
  int vertex[2][2][2];
  FaceGroup fgr[3][2];
  EdgeGroup egr[3][2][2];
} Surface;

typedef struct {
  int nnodes;
  double *node;
  int nelements;
  int *element;
  int *volume;
  int nvolumes;

  BoundingBox bbox;
} Mesh;

typedef struct {
  int vertex[2][2];
  int cpoint;
} BlockFace;

typedef struct {
  int axis;
  EdgeGroup *egr;
  int nvblocks;
  int nwblocks;
  BlockFace *bface;  // iv*nwblocks+iw
} BlockFaceGroup;

typedef struct {
  int vertex[3];
} PrismFace;

typedef struct {
  int axis;
  int ntriangles;
  PrismFace *pface;
} PrismFaceGroup;

typedef struct {
  int node_offset;
  int element_offset;
  Mesh mesh;

  int nlayers;
  Edge *axis_edge[3];
  
  EdgeGroup ve_edge[3];
  FaceGroup ve_face[3];
  BlockFaceGroup ve_bface[3];

  int is_generated;

} VertexMesh;

typedef struct {
  int node_offset;
  int element_offset;
  Mesh mesh;

  int nlayers;
  Edge *uedge[4];
  Edge *vedge[4];
  Edge *wedge[4];
 
  // [0]=uv plane, [1]=uw plane
  BlockFaceGroup bfgr[2];
  //FaceGroup fgr[2];

  int is_generated;

} EdgeMesh;

typedef struct {
  int node_offset;
  int element_offset;
  Mesh mesh;

  int nlayers;
  FaceGroup *layer_fgr;

  int is_generated;
} FaceMesh;

typedef struct {
  int nnodes;
  int nelements;
  int nvolumes;

  /// vmesh[xaxis_side][yaxis_side][zaxis_side]
  VertexMesh vmesh[2][2][2];

  /// emesh[uaxis_dir][vaxis_side][waxis_side]
  EdgeMesh emesh[3][2][2];

  /// smesh[uaxis_normal_dir][uaxis_side]
  FaceMesh smesh[3][2];
} MatchedMesh;

#endif /* __COMMON_H__ */
