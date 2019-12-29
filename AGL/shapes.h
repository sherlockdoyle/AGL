#ifndef BASIC_SHAPE_H
#define BASIC_SHAPE_H

#include "entity.h"

namespace agl {
/*!
 * \defgroup shapes Shapes
 * These functions creates some of the common shapes, including the regular polyhedrons. All these functions returns an
 * Entity with the appropriate vertices. Optionally, for some of the shapes, you can also get the normals and texture
 * coordinates calculated.
 * @{
 */
/*!
 * \brief Create a [tetrahedron](https://en.wikipedia.org/wiki/Tetrahedron).
 *
 * The tetrahedron is as big such that it touches the faces or vertices of a #cube.
 */
Entity tetrahedron();
/*!
 * \brief Create a [cube](https://en.wikipedia.org/wiki/Cube).
 * \param calcNorm If \c true, also calculates the normals.
 * \param calcUV If \c true, also calculates the texture coordinates.
 *
 * The cube ranges from -1 to +1 on all the axes, ie. centered at the origin and going to unit length on all the axes.
 */
Entity cube(bool calcNorm=false, bool calcUV=false);
/*!
 * \brief Create a [cuboid](https://en.wikipedia.org/wiki/Cuboid).
 * \param x Length along the x axis.
 * \param y Length along the y axis.
 * \param z Length along the z axis.
 * \param calcNorm If \c true, also calculates the normals.
 * \param calcUV If \c true, also calculates the texture coordinates.
 *
 * The cuboid is centered at the origin and ranges from \f$-\frac l2\f$ to \f$+\frac l2\f$ on each axis (\f$l\f$ is the
 * length for that axis).
 */
Entity cuboid(float x=2, float y=2, float z=2, bool calcNorm=false, bool calcUV=false);
/*!
 * \brief Create an [octahedron](https://en.wikipedia.org/wiki/Octahedron).
 *
 * The vertices of the octahedron is at the center of the faces of the #cube.
 */
Entity octahedron();
/*!
 * \brief Create a [dodecahedron](https://en.wikipedia.org/wiki/Regular_dodecahedron).
 *
 * Each vertex of the dodecahedron is at a distance of \f$\sqrt 3\f$ from the origin.
 */
Entity dodecahedron();
/*!
 * \brief Create an [icosahedron](https://en.wikipedia.org/wiki/Icosahedron).
 *
 * Each of the vertices are at a coordinate with values of 0, \f$\pm1\f$ and \f$\pm\href{https://en.wikipedia.org/wiki/Golden_ratio}\phi\f$
 * in some order.
 */
Entity icosahedron();
/*!
 * \brief Create a unit sphere.
 * \param lat Number of latitudes.
 * \param lng Number of longitudes.
 * \param calcNorm If \c true, also calculates the normals.
 * \param calcUV If \c true, also calculates the texture coordinates.
 *
 * Create a unit sphere in uv coordinates. A uv-sphere has quadrilateral faces formed with rings and segments, like the
 * Earth's latitudes and longitudes. The sphere has \a lat rings and \a lng segments. Read more [here](https://blender.stackexchange.com/questions/72/what-is-the-difference-between-a-uv-sphere-and-an-icosphere).
 *
 * \sa icosphere
 */
Entity sphere(int lat=20, int lng=20, bool calcNorm=false, bool calcUV=false);
/*!
 * \brief Create a unit sphere.
 * \param subdivision Number of times the #icosahedron is subdivided.
 * \param calcNorm If \c true, also calculates the normals.
 *
 * Create a unit icosphere, ie. sphere created by subdividing a icosahedron. It has triangular faces. Read more [here](https://blender.stackexchange.com/questions/72/what-is-the-difference-between-a-uv-sphere-and-an-icosphere).
 *
 * \sa sphere icosahedron subdivideVertices
 */
Entity icosphere(int subdivision=3, bool calcNorm=false);
/*!
 * \brief Create a cylinder.
 * \param r Radius of the cylinder.
 * \param h Height of the cylinder.
 * \param strips Number of stripes to use for the curved surface.
 */
Entity cylinder(float r=1, float h=2, int strips=20);
/*!
 * \brief Create a plane on the xz-plane.
 * \param x Length along x-axis.
 * \param z Length along z-axis.
 * \param calcNorm If \c true, also calculates the normals.
 * \param calcUV If \c true, also calculates the texture coordinates.
 *
 * The plane consists of just the four corner vertices at y=0.
 */
Entity plane(float x=2, float z=2, bool calcNorm=false, bool calcUV=false);
/*!
 * \brief Create a plane on the xz-plane.
 * \param l Number of segments along the x-axis.
 * \param h Number of segments along the z-axis.
 *
 * This creates a tessellated plane, ie. the plane is divided into smaller square unit segments, each at y=0.
 */
Entity tessellatedPlane(int l=10, int h=10);
//! @}

/*!
 * \defgroup shape_utility Shape utility
 * These are utility functions that can be used on Entity or the [shapes](\ref shapes). These functions will modify the
 * Entity they take in as parameter instead of returning a new Entity.
 * @{
 */
/*!
 * \brief Normalize all vertices of \a e.
 * \param e Entity to normalize vertices.
 * \param t Amount of normalization.
 *
 * This function normalizes all the vertices of an Entity, it projects all the vertices on the surface of a unit sphere
 * centered at the origin. The value of \a t defines the amount of normalization. 0 means no normalization, ie. the
 * vertices will remain at their original position. 1 means complete normalization, ie. the vertices will be completely
 * mapped on the surface of a unit sphere. For example, a value of 0.5 will bring all the vertices halfway to the
 * surface of a unit sphere.
 */
void normalizeVertices(Entity &e, float t=1);
/*!
 * \brief Subdivide all faces of \a e.
 * \param e The entity whose faces will be normalized.
 *
 * This function will insert a new vertex at the center of each of the edges of the entity. It'll then join the three
 * vertices at the center of each of the edges of each of the triangular faces, thereby dividing each face into four
 * face. This way more vertex and faces are introduced, which can later be modified for a smoother surface.
 */
void subdivideFaces(Entity &e);
/*!
 * \brief Calculates the normals for the Entity algorithmically.
 * \param e Entity to calculate the normals for.
 * \param perFace If \c true, normals are calculated for each face, instead of each vertex. **Currently unused.**
 *
 * This function calculates the normals for each vertex of \a e. The normals are pointed radially outwards from the
 * geometric center of the entity. Geometric center is calculated simply by averaging all the vertices.
 */
void calcNormals(Entity &e, bool perFace=false);
/*!
 * \brief Calculates texture coordinates for the Entity algorithmically.
 * \param e Entity to calculate the Entity#uvs for.
 * \param px Parameter for x coordinate of texture.
 * \param py Parameter for y coordinate of texture.
 * \param normalize If \c true, the coordinates are scaled to range from 0 to 1.
 *
 * This function calculates the texture coordinates for \a e automatically. The coordinates may not be what you expect,
 * but are promised to be continuous. The coordinate for each vertex is generated by calculating the dot product of the
 * vertex with the respective parameter. The default parameters works almost well to generate non-repetitive and smooth
 * coordinates for vertices ranging between 0 to 1.
 */
void calcTextureCoords(Entity &e, glm::vec3 px=glm::vec3(.5,0,.5), glm::vec3 py=glm::vec3(.5,.5,0), bool normalize=false);
/*!
 * \brief Triangulates polygons with 3+ vertices.
 * \param polygon Indices to triangulate.
 * \return The new set of indices such that all triplets represents a valid triangle.
 *
 * This function is used to triangulate non-triangular faces. This is different from a normal [triangulation algorithm](https://en.wikipedia.org/wiki/Polygon_triangulation).
 * This function do not require the actual vertices as it does not introduce new vertices. It just rearranges and copies
 * the indices forming the faces (polygons) such that each triplet represents a non-overlapping triangle. This also
 * means that this algorithm will fail for non-convex polygons. The algorithm tries to equally distribute the vertices
 * among different faces to reduce crowding of edges. It also preserves the orientation of the face vertices in the new
 * triangular faces.
 */
std::vector<GLuint> triangulatePolygon(const std::vector<GLuint> &polygon);
//! @}
}

#endif // BASIC_SHAPE_H
