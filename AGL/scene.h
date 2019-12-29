#ifndef SCENE_H
#define SCENE_H

#include "entity.h"
#include<vector>
#include<GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace agl {
/*!
 * \brief Camera class for the scene
 *
 * Camera controls the positioning of the camera for the scene. It controls the rotation of the camera, up-vectors,
 * positioning, etc. This class handles the V part of the MVP matrix.
 */
class Camera
{
public:
    glm::mat4 view;  //!< The view matrix.
    // Cached values
    glm::vec3 _pos,  //!< The position of the camera.
              _lookAt, //!< The point the camera looks at.
              _up;  //!< The up vector for the camera.

    /*!
     * \brief Creates a camera.
     * \param pos The position of the camera.
     * \param lookAt The point the camera looks at.
     * \param up The up vector for the camera.
     */
    Camera(const glm::vec3 &pos=glm::vec3(3, 4, 5), const glm::vec3 &lookAt=glm::vec3(0), const glm::vec3 &up=glm::vec3(0, 1, 0));

    /*!
     * \name Camera setters
     * These methods are used to set the parameters of the camera, and the view matrix. They modify the internal #view
     * matrix.
     * @{
     */
    /*!
     * \brief Sets the position of the camera.
     * \param pos The position as a vector.
     */
    void setPos(const glm::vec3 &pos=glm::vec3(3, 4, 5));
    /*!
     * \brief Sets the position of the camera.
     * \param x The x position.
     * \param y The y position.
     * \param z The z position.
     */
    void setPos(float x=3, float y=4, float z=5);
    /*!
     * \brief Set the point the camera will look at.
     * \param target The point as a vector.
     *
     * This point will be the center of the scene.
     */
    void lookAt(const glm::vec3 &target=glm::vec3(0));
    /*!
     * \brief Set the point the camera will look at.
     * \param x The x position.
     * \param y The y position.
     * \param z The z position.
     */
    void lookAt(float x=0, float y=0, float z=0);
    /*!
     * \brief Set the up-vector.
     * \param up The up vector.
     */
    void setUp(const glm::vec3 &up=glm::vec3(0, 1, 0));
    /*!
     * \brief Set the up-vector.
     * \param x The x component of the vector.
     * \param y The y component of the vector.
     * \param z The z component of the vector.
     */
    void setUp(float x=0, float y=1, float z=0);
    //! @}

    /*!
     * \brief Set the view matrix.
     * \param mat The view matrix.
     *
     * This method can be used to set the view matrix manually, but it'll set #_pos, #_lookAt and #_up to \c nan.
     * Extracting these information from the matrix is not supported currently.
     */
    void setView(const glm::mat4 &mat);
    /*!
     * \brief Rotate the camera.
     * \param rad Angle to rotate by, in radians.
     * \param axis Axis of rotation.
     * \param around Rotate around center?
     *
     * Rotates the camera by \a rad radians around the \a axis. If \a around is \c true, the camera rotates while
     * keeping the #_lookAt at center, ie. the #_pos changes; the camera rotates around the scene. If \a around is \c
     * false, the camera rotates while keeping the #_pos at center, ie. the #_lookAt changes; the camera itself rotates.
     */
    void rotate(float rad=0, const glm::vec3 &axis=glm::vec3(0), bool around=true);
    /*!
     * \brief Rotate the camera with the [quaternion](https://en.wikipedia.org/wiki/Quaternions).
     * \param rot Quaternion for rotation.
     * \param around See #rotate.
     */
    void rotate(const glm::quat &rot=glm::quat(1, 0, 0, 0), bool around=true);
    /*!
     * \brief Rotate around the x-axis.
     * \param rad Angle to rotate by, in radians.
     * \param around See #rotate.
     * \param safe Rotations beyond 90 degree along the x-axis might cause the render to flip abruptly. Setting this to
     * \c true tries to prevent that. However, it isn't guaranteed that this will work.
     */
    void rotateX(float rad=0, bool around=true, bool safe=true);
    /*!
     * \brief Rotate around the y-axis.
     * \param rad Angle to rotate by, in radians.
     * \param around See #rotate.
     */
    void rotateY(float rad=0, bool around=true);
    /*!
     * \brief Rotate around the z-axis.
     * \param rad Angle to rotate by, in radians.
     * \param around See #rotate.
     */
    void rotateZ(float rad=0, bool around=true);
    /*!
     * \brief Translate (move, shift) the camera.
     * \param d Vector to translate by.
     */
    void translate(const glm::vec3 &d=glm::vec3(0));
    /*!
     * \brief Translate (move, shift) the camera.
     * \param dx Amount to translate along the x-axis.
     * \param dy Amount to translate along the y-axis.
     * \param dz Amount to translate along the z-axis.
     */
    void translate(float dx=0, float dy=0, float dz=0);

    /*!
     * \name View transformations
     * These methods applies transformations directly to the view matrix. It is advised that you do not use these
     * methods. These methods do not modify either of #_pos, #_lookAt or #_up. All of these are left handed, ie. these
     * are applied on the current transformed matrix.
     * @{
     */
    /*!
     * \brief Translate the view matrix.
     * \param d Vector to translate by.
     */
    void viewTranslate(const glm::vec3 &d=glm::vec3(0));
    /*!
     * \brief Translate the view matrix.
     * \param dx Amount to translate along the x-axis.
     * \param dy Amount to translate along the y-axis.
     * \param dz Amount to translate along the z-axis.
     */
    void viewTranslate(float dx=0, float dy=0, float dz=0);
    /*!
     * \brief Rotate the view matrix.
     * \param rad Angle to rotate by, in radians.
     * \param axis Axis of rotation.
     */
    void viewRotate(float rad=0, const glm::vec3 &axis=glm::vec3(0));
    /*!
     * \brief Scale the view matrix.
     * \param s Vector to scale by.
     */
    void viewScale(const glm::vec3 &s=glm::vec3(1));
    /*!
     * \brief Scale the view matrix.
     * \param sx Amount to scale along the x-axis.
     * \param sy Amount to scale along the y-axis.
     * \param sz Amount to scale along the z-axis.
     */
    void viewScale(float sx=1, float sy=1, float sz=1);
    /*!
     * \brief Transform the view matrix.
     * \param m Matrix to transform by.
     */
    void viewTransform(const glm::mat4 &m);
    //! @}
};


/*!
 * \brief Scene class, which holds everything.
 *
 * The Scene class holds the camera, all the entities that need to be displayed, the lights and much more. The Scene
 * creates a [GLFW](https://www.glfw.org/) window to display the rendered contents. The Scene can be controlled with
 * user inputs (default controls inbuilt). This class also contains the projection matrix, the P part of the MVP matrix.
 */
class Scene
{
public:
    int width,  //!< Width of the window.
        height;  //!< Height of the window.
    glm::mat4 projection;  //!< Projection matrix for the render.
    Camera camera;  //!< Camera for the scene.
    std::vector<BaseEntity*> children;  //!< Stores all the Entity and Light.
    GLFWwindow* window;  //!< The GLFW window for displaying everything.
    std::vector<Entity*> entities;  //!< Temporary store all the Entity before #render.
    std::vector<Light*> lights;  //!< Temporary store all the Light before #render.

    /*!
     * \brief Create a scene.
     * \param width Width of the window.
     * \param height Height of the window.
     * \param name Name for the window.
     *
     * This will create a scene, create a GLFW window, create a default perspective projection matrix and set some
     * default controllers. Although you can create as many scenes as you like, only one scene can be displayed at any
     * time currently.
     */
    Scene(int width=640, int height=480, const char *name="AGL");
    ~Scene();

    /*!
     * \brief Set a [perspective projection](https://en.wikipedia.org/wiki/3D_projection#Perspective_projection) matrix.
     * \param fovDeg Field of view for render.
     * \param aspectRatio Aspect ratio for render. If negative, automatically sets depending on the size of the window.
     * \param near Near for render, anything nearer to this will be clipped.
     * \param far Far for render, anything beyond this will be clipped.
     */
    void setPerspectiveProjection(float fovDeg=45, float aspectRatio=-1, float near=0.1, float far=100);
    /*!
     * \brief Set a [orthographic projection](https://en.wikipedia.org/wiki/Orthographic_projection) matrix.
     * \param left Left for render, this is the point aligned with the left side of the window.
     * \param right Right for render, this is the point aligned with the right side of the window.
     * \param bottom Bottom for render, this is the point aligned with the bottom of the window.
     * \param top Top for render, this is the point aligned with the top of the window.
     * \param near Near for render, anything nearer to this will be clipped.
     * \param far Far for render, anything beyond this will be clipped.
     */
    void setOrthographicProjection(float left=-1, float right=1, float bottom=-1, float top=1, float near=0.1, float far=100);
    /*!
     * \brief Set a new #camera for the scene.
     * \param pos The position of the camera.
     * \param lookAt The point the camera looks at.
     * \param up The up vector for the camera.
     */
    void setCamera(const glm::vec3 &pos, const glm::vec3 &lookAt, const glm::vec3 &up);
    /*!
     * \brief Set the projection matrix.
     * \param mat The matrix to set.
     */
    void setProjection(glm::mat4 &mat);
    /*!
     * \brief Set the view matrix (in the #camera).
     * \param mat The matrix to set.
     */
    void setView(glm::mat4 &mat);
    /*!
     * \brief Set controllers for the scene.
     * \param keyCallback Called on key press.
     * \param cursorPosCallback Called if the cursor is moved.
     * \param mouseButtonCallback Called if the mouse button is clicked.
     * \param scrollCallback Called if scrolled.
     * \param windowSizeCallback Called if the window is resized.
     *
     * This is used to set several callbacks for the scene. Several callbacks are already set when the scene is created.
     * This can be used to add more controls for the user. If any of the parameter is \c null, the old value, if one was
     * there, is retained.
     *
     * \sa defKeyCB defScrollCB defWindowSizeCB
     */
    void setController(GLFWkeyfun keyCallback=nullptr, GLFWcursorposfun cursorPosCallback=nullptr,
                       GLFWmousebuttonfun mouseButtonCallback=nullptr, GLFWscrollfun scrollCallback=nullptr,
                       GLFWwindowsizefun windowSizeCallback=nullptr);
    /*!
     * \brief Set the background color.
     * \param r Red.
     * \param g Green.
     * \param b Blue.
     * \param a Alpha.
     */
    void setBGcolor(float r=0, float g=0, float b=0, float a=0);
    /*!
     * \brief Add an Entity or Light to the scene.
     * \param e Entity or Light to add.
     */
    void add(BaseEntity &e);
    /*!
     * \brief Enable lighting calculations.
     * \param enable Enable?
     *
     * This enables (or disables) lights for the scene. This just sets the Material#lightsEnabled for all the Entity.
     * The shader for the Entity actually decides if lights should be used or not.
     */
    void enableLights(bool enable=true);
    /*!
     * \brief Prepare the scene before rendering.
     *
     * This method scans the Entity tree to access all the Entity and Light and creates the buffers and shaders for the
     * Entity. Call this method only once before any rendering loop.
     */
    void prepare();
    /*!
     * \brief Render the scene.
     * \return Returns \c false, if the #window should close, \c true otherwise.
     *
     * This method actually renders everything on the window. This loads the required buffers and shaders, sets the
     * parameters, etc. This method must be called each time in the rendering loop to update the scene, say for
     * animations. The return value of this method can be used as the condition for the render loop.
     */
    bool render();
    /*!
     * \brief Get the view-projection matrix.
     * \return The VP part of the MVP matrix.
     */
    glm::mat4 getMatVP();

private:
    glm::vec4 bgcolor;  //!< Background color for the scene.

    /*!
     * \brief Sets up the GLFW window.
     * \param name Name of the window.
     * \return An error code.
     */
    int setupGL(const char *name);
    /*!
     * \brief Separates the Entity and Light into #entities and #lights.
     * \param children The children tree.
     *
     * This method runs a recursive DFS on all the children to separate all the Entity and Light.
     */
    void getAllEntity(std::vector<BaseEntity*> &children);
};

/*!
 * \brief Default callback for key press.
 *
 * This moves and rotates the camera on key press. The movement is Minecraft like. Available keys are:
 *   - **W:** Moves the camera forward on the xz-plane.
 *   - **A:** Moves the camera left on the xz-plane.
 *   - **S:** Moves the camera backward on the xz-plane.
 *   - **D:** Moves the camera right on the xz-plane.
 *   - **Q:** Moves the camera down along the y-axis.
 *   - **E:** Moves the camera up along the y-axis.
 *   - **Arrow keys:** Rotates the camera around itself in the direction of the arrow.
 *   - **0:** Resets the camera position to (3, 4, 5) and looking at the origin.
 */
void defKeyCB(GLFWwindow* window, int key, int scancode, int action, int mods);
/*!
 * \brief Default scroll callback.
 *
 * This rotates the camera in the direction of the scroll. This works best with touch pads.
 */
void defScrollCB(GLFWwindow* window, double xoffset, double yoffset);
/*!
 * \brief Default window resize callback.
 *
 * Resizes the scene's Scene#width and Scene#height and sets a projection matrix with the appropriate aspect ratio,
 * a 45 degree FOV and a near and far value of 0.1 and 100 respectively.
 */
void defWindowSizeCB(GLFWwindow* window, int w, int h);
}

#endif // SCENE_H
