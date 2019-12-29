#ifndef ENTITY_H
#define ENTITY_H

#include<vector>
#include<GL/gl.h>
#include<GLES3/gl32.h>
#include "glm/glm.hpp"
#include "util.h"

namespace agl {
class Entity;
class Light;
class Scene;

/*!
 * \brief Material class for entities.
 *
 * This class defines the kind of material used by the shader to render the Entity. It defines several different
 * components (emission, ambient, diffuse and specular) for the materials or a single texture. Each of the components
 * can be a valid color or a special color.
 *
 * ## Special Colors
 * AGL supports several special colors for the materials. This allows for procedurally generated textures for the
 * rendering. These colors are defined with a negative alpha value for the components. Each of these color can take
 * several parameters, defined by values in the rgb part of the colors. These colors are hard-coded in the generated
 * shaders, ie. they can't be animated without recreating the shaders. Available special colors are:
 *
 * ### Position based color
 * This is enabled by #AGL_COLOR_POS2RGB. The position of the fragment is mapped to a color for the same. All the
 * fragments are scaled between 0 to 1 and the value is used as RGB. The R, G, and B part of the color must be -1 for
 * the corresponding coordinates (x, y, and z) to contribute to the final color. If not -1, the value is used as-is in
 * the final color.
 *
 * ### Normal based color
 * This is enabled by #AGL_COLOR_NORM2RGB. The normal of the fragment is mapped to a color for the same. This mode
 * requires that the normal is included with the buffers. The x, y, and z components of the normal is directly used for
 * the R, G, and B components of the color.
 *
 * ### Checkerboard pattern
 * This is enabled by #AGL_COLOR_CHECKER. The position of the fragment is used to generate either black or white color to
 * give a checkerboard pattern finally. The R, G, or B component must be set to -1 to use the same for the color
 * calculation. If set to anything else, they're ignored. The equation used is \f$\left(\sum_{i\text{th component of
 * color is -1}} \left\lfloor\text{normal}_i\right\rfloor\right) \mod 2\f$.
 *
 * ### Example
 * \code{.cpp}
 * material.emission = glm::vec4(-1, -1, -1, AGL_COLOR_POS2RGB);  // This will use position based colors for the material. All the color components will be used.
 * material.emission = glm::vec4(-1, 0, -1, AGL_COLOR_CHECKER);  // This will create a checker board pattern with the x and z components. Equation used: mod(floor(pos.x) + floor(pos.z), 2)
 * \endcode
 */
class Material
{
public:
    glm::vec4 emission,    //!< Emission, self color of the material.
              ambient,     //!< Ambient, color due to ambient light.
              diffuse,     //!< Diffuse, color when light hits a surface.
              specular,    //!< Specular, color due to the reflection of light.
              ratios;  // (color, texture, reflection, refraction) [0, 1] -1: disabled
    float shininess = 32;   //!< Shininess, amount of light reflected by the material.
    bool lightsEnabled = false,  //!< If true, no lighting calculations are done.
         customShader = false;  //!< If true, Scene#prepare do not create shaders.
    int shadingType = AGL_SHADING_PHONG,  //!< Type of shading, currently unused. \warning Might be removed.
        tex_width = -1,  //!< Width of texture,if used.
        tex_height = -1,  //!< Height of texture, if used.
        tex_channel = -1;  //!< Number of channels in texture, if used.
    GLubyte *texture = nullptr;  //!< Pointer to texture data, if present.
    GLuint progID = 0,  //!< program ID
           tID = 0,  //!< texture ID
           mvpID,  //!< MVP matrix ID
           mID,  //!< model matrix ID
           nID,  //!< normal matrix ID
           eID,  //!< emission color ID
           aID,  //!< ambient color ID
           dID,  //!< diffuse color ID
           sID,  //!< specular color ID
           gID,  //!< shininess color ID
           vID;  //!< camera position ID

    /*!
     * \brief Creates a material.
     */
    Material();
    ~Material();

    /*!
     * \brief Mix two material.
     * \param other The material to mix with.
     * \return The mixed material.
     *
     * This mixes two materials by averaging the colors. The shininess is geometrically meaned.
     */
    Material operator+(const Material &other) const;
    Material &operator+=(const Material &other);  //!< Same as #operator+ but modifies the current material.
    /*!
     * \brief Mix two material.
     * \param other The material to mix with.
     * \return The mixed material.
     *
     * This mixes two materials by multiplying the colors. The shininess is geometrically meaned.
     */
    Material operator*(const Material &other) const;
    Material &operator*=(const Material &other);  //!< Same as #operator* but modifies the current material.
    /*!
     * \brief Set color for all the components.
     * \param color Color for all the components.
     */
    void setColor(glm::vec4 color=glm::vec4(1));
    /*!
     * \brief Same as #setColor.
     * \param r Red.
     * \param g Green.
     * \param b Blue.
     * \param a Alpha.
     */
    void setColor(float r=1, float g=1, float b=1, float a=1);
    /*!
     * \brief Sets each color components from another Material.
     * \param other The Material to copy the colors from.
     */
    void setColor(const Material &other);
    /*!
     * \brief Creates a texture.
     * \param path Path to the texture.
     *
     * If no \a path is provided, assumes that the #texture is already loaded, else loads the texture, and loads and
     * creates the ID.
     * \note Textures loaded with this method, or the ones loaded manually, are not freed when the Material is
     * destroyed. Make sure to do it manually with \c delete or \c stbi_image_free to avoid memory leak. This might
     * change in future.
     */
    void createTexture(const char *path=nullptr);
//    void setTexture(const Material &other);  // TODO: Why destructor does not work? I know why, need to deal with that.
    /*!
     * \brief Create a shader algorithmically.
     * \param e Entity, needed for which to create the position based coloring.
     * \param lights All the lights in the scene.
     * \return The generated vertex and fragment shader.
     */\
    virtual std::pair<std::string, std::string> createShader(Entity *e=nullptr, std::vector<Light*> lights=std::vector<Light*>());
    /*!
     * \brief Compile and set the shader for the material.
     * \param vertexShader Vertex shader.
     * \param fragmentShader Fragment shader.
     *
     * This can load shaders that were created by #createShader or those loaded manually. This will also set all the
     * required IDs automatically.
     */
    virtual void setShader(std::string vertexShader, std::string fragmentShader);

    /*!
     * \defgroup predefined_materials Predefined materials
     * These are predefined materials that can be used to get the corresponding material effects.
     * @{
     */
    static const Material emerald, jade, obsidian, pearl, ruby, turquoise, brass, bronze, chrome, copper, gold, silver,
                          black_plastic, cyan_plastic, green_plastic, red_plastic, white_plastic, yellow_plastic,
                          black_rubber,  cyan_rubber,  green_rubber,  red_rubber,  white_rubber,  yellow_rubber;
    //! @}
private:
    Material(float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, float sn);  //!< Used to create the color for the predefined materials quickly.
};

/*!
 * \brief The base for all Entity and Light.
 */
class BaseEntity
{
public:
    Entity *parent = nullptr;  //!< Parent of the entity, whose child this is. All transformations of the parent is also applied to the child.
    virtual ~BaseEntity();
};

/*!
 * \brief The Entity; basically all visible objects in AGL.
 *
 * Entity consists of anything that is visible on the scene. The entities consists of
 * - #vertices: Each vertex is a point on the 3D coordinate. A shape consists of multiple vertices connected by edges.
 *   Three such adjacent edges (ie. three vertices) combines to form a triangular face of the entity.
 * - #normals: Each of the #vertices can consist of a normal, a 3D vector pointing perpendicularly away from the surface
 *   that defines the surface and possibly it's orientation. Normals are generally used for lighting calculations.
 * - #uvs (texture coordinates): Each of the vertices can also contain a 2D texture coordinate. This is used to map a
 *   texture on the surface of the entity.
 * - #indices: Indices are a set of triplets of index, each defining a face of the entity. Since an entity can only have
 *   triangular faces with three vertices, each triplet of indices defines a single face. The index is formed with the
 *   0-indexed position of the vertices. For example, a triplet of (0, 1, 2) defines the face formed by the vertices at
 *   the first, second and third position in the #vertices array.
 */
class Entity: virtual public BaseEntity
{
public:
    GLuint VAO = 0,  //!< Vertex array
           VBO = 0,  //!< Vertex buffer
           EBO = 0;  //!< Index buffer
//           polyMode = GL_FILL;
    bool dynamic = false;  //!< If true, the material is dynamic, ie. vertices might change during runtime.
    std::vector<GLfloat> vertices,  //!< Vertices
                         normals,  //!< Normals
                         uvs,  //!< Texture coordinates
                         merged;  //!< The final combination of vertices, normals, and uvs.
    std::vector<GLuint> indices;  //!< Indices
    glm::vec3 position;  //!< Position of the entity, the entity is centered here.
    glm::mat4 model;  //!< The model matrix for the entity. This is the M part of the MVP matrix. This is responsible for all the transformations of this entity.
    Material material;  //!< Material for shading this entity.
    std::vector<BaseEntity*> children;  //!< Children of this entity.

    /*!
     * \brief Create an entity centered at \a pos.
     * \param pos Position of the entity.
     */
    Entity(const glm::vec3 &pos=glm::vec3(0));
    /*!
     * \brief Create an entity from data copied from \a other.
     * \param other Another entity for the copy constructor.
     */
    Entity(const Entity &other);
    ~Entity();

    /*!
     * \brief Translate (move,shift) the entity.
     * \param d Vector to translate by.
     */
    void translate(const glm::vec3 &d=glm::vec3(0));
    /*!
     * \brief Translate (move, shift) the entity.
     * \param dx Amount to translate along the x-axis.
     * \param dy Amount to translate along the y-axis.
     * \param dz Amount to translate along the z-axis.
     */
    void translate(float dx=0, float dy=0, float dz=0);
    /*!
     * \brief Rotate the entity.
     * \param rad Angle to rotate by, in radians.
     * \param axis Axis of rotation.
     */
    void rotate(float rad=0, glm::vec3 axis=glm::vec3(0));
    /*!
     * \brief Scale the entity.
     * \param s Vector to scale by.
     */
    void scale(const glm::vec3 &s=glm::vec3(1));
    /*!
     * \brief Scale the entity.
     * \param sx Amount to scale along the x-axis.
     * \param sy Amount to scale along the y-axis.
     * \param sz Amount to scale along the z-axis.
     *
     * If \a sy and \a sz is unspecified, they default to \a sx. If only \a sz is unspecified, it defaults to 1.
     */
    void scale(float sx=1, float sy=std::nanf(""), float sz=std::nanf(""));
    /*!
     * \brief Transform the entity.
     * \param m Matrix to transform by.
     */
    void transform(const glm::mat4 &m);
    /*!
     * \brief Transform the vertices permanently.
     *
     * After this operation, all the transformations will be applied to all the vertices and normals. The #model matrix
     * will reset to an identity matrix.
     */
    void applyTransform();
    /*!
     * \brief Add an Entity or Light as a child of the entity.
     * \param e Entity or Light to add.
     */
    void add(BaseEntity &e);
    /*!
     * \brief Merge the #vertices, #normals and #uvs into a single #merged array.
     */
    virtual void mergeData();
    /*!
     * \brief Create all the buffers for rendering.
     */
    virtual void createBuffers();
    /*!
     * \brief Get the model matrix.
     * \return The model matrix after accounting for the shift due to #position and parent transformations.
     */
    glm::mat4 getMatM();
};

/*!
 * \defgroup lights Lights
 * @{
 */
/*!
 * \brief Light, adds reality to the scene.
 *
 * These are different kinds of lights that can be added to the scene. There are mainly three kinds of light,
 * - **Point light:** Point lights are positioned and cast light in all directions. For a point light, the fourth
 *   component of the #position vector should be 1.
 * - **Directional light:** Directional lights originates at infinity and propagates straight in a certain direction.
 *   For a directional light, the fourth component of the #position vector should be 0.
 * - **Spotlight:** Spotlights are positioned and cast light in a cone. For a spotlight, the #spotCosCutoff should be
 *   between 0 and 1.
 */
class Light: virtual public BaseEntity
{
public:
    glm::vec4 ambient,         //!< Ambient, illuminates everything.
              diffuse,         //!< Diffuse, illuminates surfaces facing the light.
              specular,        //!< Specular, reflects from surface.
              position;        //!< Position (direction) of the light.
    glm::vec3 spotDirection;   //!< Direction of spotlight.
    float spotExponent  =  0,  //!< Effect of spotlight.
          spotCosCutoff = -1,  //!< Cosine of angle of spotlight. (range: [1, 0], -1 means disabled)
          constantAttenuation  = 1, //!< Constant attenuation.
          linearAttenuation    = 0, //!< Linear attenuation.
          quadraticAttenuation = 0; //!< Quadratic attenuation.

    /*!
     * \brief Creates a light.
     * \param pos Position of the light.
     * \param color Color of the light.
     *
     * Creates a point light. To create other kinds of light, use the constructor-like functions.
     *
     * \sa DirectionalLight
     */
    Light(const glm::vec3 &pos=glm::vec3(-2, 3, 2), const glm::vec4 &color=glm::vec4(1));
    /*!
     * \brief See #Light.
     * \param pos Position of the light.
     * \param r Red.
     * \param g Green.
     * \param b Blue.
     * \param a Alpha.
     */
    Light(const glm::vec3 &pos=glm::vec3(-2, 3, 2), float r=1, float g=1, float b=1, float a=1);
    /*!
     * \brief Set color of the light.
     * \param color Color to set.
     */
    void setColor(glm::vec4 color=glm::vec4(1));
    /*!
     * \brief Set light color. See #setColor and #Light(const glm::vec3&, float, float, float, float).
     */
    void setColor(float r=1, float g=1, float b=1, float a=1);
    /*!
     * \brief Get the position of the light.
     * \return The position after accounting for the parent transformations.
     */
    inline glm::vec4 getPos()
    {
        return parent == nullptr ? position : parent->getMatM() * position;
    }
};
/*!
 * \brief Create a directional Light.
 * \param dir Direction of light.
 * \param color Color of light.
 * \return A directional light.
 *
 * In a directional light, the Light#position is actually the direction of the light (positioned at infinity).
 */
inline Light DirectionalLight(const glm::vec3 &dir=glm::vec3(0), const glm::vec4 &color=glm::vec4(1))
{
    Light l(dir, color);
    l.position.w = 0;
    return l;
}
/*!
 * \brief Create a directional Light. See #DirectionalLight.
 * \param dir Direction of light.
 * \param r Red.
 * \param g Green.
 * \param b Blue.
 * \param a Alpha.
 * \return A directional light.
 */
inline Light DirectionalLight(const glm::vec3 &dir=glm::vec3(0), float r=1, float g=1, float b=1, float a=1)
{
    Light l(dir, r, g, b, a);
    l.position.w = 0;
    return l;
}
//! @}

/*!
 * \brief A SharedEntity shares its data off another Entity.
 *
 * Since all entities need their separate data, they also create their separate buffers. Say, you've two cubes, they'll
 * have the same vertex data, but will be loaded twice. SharedEntity intends to optimize this by using the buffers from
 * another Entity whose data has already been loaded. The #source entity's buffers must have been created before
 * they can be used.
 * \warning This class is unstable and might be removed in future.
 */
class SharedEntity: virtual public Entity
{
public:
    Entity *source = nullptr;  //!< The source Entity whose data and buffers are used.

    /*!
     * \brief Create a SharedEntity from \a src.
     * \param src The Entity to share from.
     */
    SharedEntity(Entity &src);
    /*!
     * \brief Does nothing. Everything is done by #source's Entity#mergeData.
     */
    virtual void mergeData();
    /*!
     * \brief Does nothing. Copies the buffers created by #source's Entity#createBuffers.
     *
     * For this to work, the #source's buffer must have been create before this is called. This must either be done
     * manually, or the Entity tree must be arranged in such a way that the DFS traversal in Scene#prepare gets to the
     * #source before itself.
     */
    virtual void createBuffers();
};
}

#endif // ENTITY_H
